#include "CompositeFactory.h"

#include <vector>
#include <sstream>
#include <functional>

#include <cassert>
#include "amqp/src/serialiser/serialisers/CompositeSerialiser.h"
#include "amqp/src/serialiser/serialisers/restricted-serialisers/ArraySerialiser.h"
#include "amqp/src/serialiser/serialisers/restricted-serialisers/MapSerialiser.h"
#include "amqp/src/serialiser/serialisers/restricted-serialisers/EnumSerialiser.h"
#include "amqp/src/serialiser/serialisers/restricted-serialisers/ListSerialiser.h"

#include "corda-utils/include/debug.h"

#include "serialiser/reader/IReader.h"

#include "amqp/src/serialiser/SerialiserFactoryInternal.h"

#include "amqp/src/serialiser/serialisers/writer/Writer.h"

#include "amqp/src/serialiser/serialisers/reader/CompositeReader.h"
#include "amqp/src/serialiser/serialisers/reader/restricted-readers/MapReader.h"
#include "amqp/src/serialiser/serialisers/reader/restricted-readers/ListReader.h"
#include "amqp/src/serialiser/serialisers/reader/restricted-readers/ArrayReader.h"
#include "amqp/src/serialiser/serialisers/reader/restricted-readers/EnumReader.h"

#include "amqp/src/schema/restricted-types/Map.h"
#include "amqp/src/schema/restricted-types/List.h"
#include "amqp/src/schema/restricted-types/Enum.h"
#include "amqp/src/schema/restricted-types/Array.h"

/******************************************************************************/

/**
 *
 * YES, this global is bad and should be injected into the framework. It
 * will be, right now this is a hack
 *
 */
static amqp::internal::serialiser::serialisers::SerialisersFactory g_sf;

/******************************************************************************/

namespace {

/**
 *
 */
    template<typename T>
    std::shared_ptr<T> &
    computeIfAbsent(
            spStrMap_t<T> &map_,
            const std::string &k_,
            std::function<std::shared_ptr<T>(void)> f_
    ) {
        auto it = map_.find(k_);

        if (it == map_.end()) {
            DBG ("ComputeIfAbsent \"" << k_ << "\" - missing" << std::endl); // NOLINT
            map_[k_] = std::move (f_());
            DBG ("                \"" << k_ << "\" - RTN: " << map_[k_]->name() << " : " << map_[k_]->type()
                                      << std::endl); // NOLINT
            assert (map_[k_]);
            assert (map_[k_] != nullptr);
            DBG (k_ << " =?= " << map_[k_]->type() << std::endl); // NOLINT
            assert (k_ == map_[k_]->type());

            return map_[k_];
        } else {
            DBG ("ComputeIfAbsent \"" << k_ << "\" - found it" << std::endl); // NOLINT
            DBG ("                \"" << k_ << "\" - RTN: " << map_[k_]->name() << std::endl); // NOLINT

            assert (it->second != nullptr);

            return it->second;
        }
    }

}

/******************************************************************************
 *
 *  CompositeFactory
 *
 ******************************************************************************/

/**
 *
 * Walk through the types in a Schema and produce readers for them.
 *
 * We are making the assumption that the contents of [schema_]
 * are strictly ordered by dependency so we can construct types
 * as we go without needing to provide look ahead for types
 * we haven't built yet.
 *
 */
void
amqp::internal::assembler::
CompositeFactory::process (const amqp::schema::ISchema & schema_) {
    DBG ("process schema" << std::endl); // NOLINT

    for (const auto & i : dynamic_cast<const schema::Schema &>(schema_)) {
        for (const auto & j : i) {
            process (*j);
            m_serialisersByDescriptor[j->descriptor()] = m_serialisersByType[j->name()];
        }
    }
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::process (
    const amqp::internal::schema::AMQPTypeNotation & schema_)
{
    DBG ("process::" << schema_.name() << std::endl); // NOLINT

    return computeIfAbsent<amqp::serialiser::ISerialiser> (
        m_serialisersByType,
        schema_.name(),
        [& schema_, this] () -> std::shared_ptr<amqp::serialiser::ISerialiser> {
            switch (schema_.type()) {
                case schema::AMQPTypeNotation::composite_t : {
                    return processComposite (schema_);
                }
                case schema::AMQPTypeNotation::restricted_t : {
                    return processRestricted (schema_);
                }
            }
        });
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::processComposite (
        const amqp::internal::schema::AMQPTypeNotation & type_
) {
    DBG ("processComposite - " << type_.name() << std::endl); // NOLINT

    const auto & fields = dynamic_cast<const schema::Composite &> (
            type_).fields();

    auto rtn = std::make_shared<serialiser::serialisers::CompositeSerialiser<
        serialiser::reader::CompositeReader,
        serialiser::writer::Writer>
    > (type_.name());

    std::vector<std::weak_ptr<amqp::serialiser::ISerialiser>> readers;
    readers.reserve (fields.size());

    for (const auto & field : fields) {
        DBG ("  Field: " << field->name() << ": \"" << field->type()
            << "\" {" << field->resolvedType() << "} "
            << field->fieldType() << std::endl); // NOLINT

        decltype (m_serialisersByType)::mapped_type serialiser;

        switch (field->AMQPType()) {
            case schema::Field::Type::primitive_t : {
                DBG ("    Field " << field->name() << "[" << field->type() << "] is primitive" << std::endl);
                serialiser = computeIfAbsent<amqp::serialiser::ISerialiser> (
                        m_serialisersByType,
                        field->resolvedType(),
                        [&field]() -> sPtr<amqp::serialiser::ISerialiser> {
                            return g_sf.makePropertyReader (field->type());
                        });
                break;
            }
            case schema::Field::Type::custom_t : {
                break;
            }
            default : {
                if (field->resolvedType() == type_.name()) {
                    // Special case where a composite type has a property that
                    // is a reference to that type itself.
                    DBG ("IT ME!!!!" << std::endl);
                    serialiser = rtn;

                } else {
                    // Insertion sorting ensures any type we depend on will have
                    // already been created and thus exist in the map
                    serialiser = m_serialisersByType[field->resolvedType()];
                }
            }
        }

        if (!serialiser) {
            std::stringstream ss;
            ss << __FILE__ << "::"
                << __FUNCTION__ << "::"
                << __LINE__ << ":: "
                << "Can't find " << field->resolvedType();

            throw std::runtime_error (ss.str());
        }
        readers.emplace_back (serialiser);
        assert (readers.back().lock());
    }

    rtn->install (readers);
    return rtn;
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::processEnum (
    const amqp::internal::schema::Enum & enum_
) {
    DBG ("Processing Enum - " << enum_.name() << std::endl); // NOLINT

    return std::make_shared<serialiser::serialisers::EnumSerialiser<
            serialiser::reader::EnumReader,
            serialiser::writer::Writer>
    > (enum_.name(),
       enum_.makeChoices());
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::fetchReaderForRestricted (const std::string & type_) {
    decltype(m_serialisersByType)::mapped_type rtn;

    DBG ("fetchReaderForRestricted - " << type_ << std::endl); // NOLINT

    if (schema::Field::typeIsPrimitive(type_)) {
        DBG ("It's primitive" << std::endl); // NOLINT
        rtn = computeIfAbsent<amqp::serialiser::ISerialiser> (
                m_serialisersByType,
                type_,
                [& type_]() -> sPtr<amqp::serialiser::ISerialiser> {
                    return g_sf.makePropertyReader (type_);
                });
    } else {
        rtn = m_serialisersByType[type_];
    }

    if (!rtn) {
        throw std::runtime_error ("Missing type in map");
    }

    return rtn;
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::processMap (
    const amqp::internal::schema::Map & map_
) {
    DBG ("Processing Map - "
        << map_.mapOf().first.get() << " "
        << map_.mapOf().second.get() << std::endl); // NOLINT

    const auto types = map_.mapOf();

    return std::make_shared<serialiser::serialisers::MapSerialiser<
            serialiser::reader::MapReader,
            serialiser::writer::Writer>
    > (map_.name(),
       fetchReaderForRestricted (types.first),
       fetchReaderForRestricted (types.second));
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::processList (
    const amqp::internal::schema::List & list_
) {
    DBG ("Processing List - " << list_.listOf() << std::endl); // NOLINT

    return std::make_shared<serialiser::serialisers::ListSerialiser<
            serialiser::reader::ListReader,
            serialiser::writer::Writer>
     > (list_.name(),
        fetchReaderForRestricted (list_.listOf()));
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::processArray (
        const amqp::internal::schema::Array & array_
) {
    DBG ("Processing Array - " << array_.name() << " " << array_.arrayOf() << std::endl); // NOLINT

    return std::make_shared<serialiser::serialisers::ArraySerialiser<
            serialiser::reader::ArrayReader,
            serialiser::writer::Writer>
     > (array_.name(),
        fetchReaderForRestricted (array_.arrayOf()));
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::processRestricted (
        const amqp::internal::schema::AMQPTypeNotation & type_)
{
    DBG ("processRestricted - " << type_.name() << std::endl); // NOLINT
    const auto & restricted = dynamic_cast<const schema::Restricted &> (
            type_);

    switch (restricted.restrictedType()) {
        case schema::Restricted::RestrictedTypes::list_t : {
            return processList (
                dynamic_cast<const schema::List &> (restricted));
        }
        case schema::Restricted::RestrictedTypes::enum_t : {
            return processEnum (
                dynamic_cast<const schema::Enum &> (restricted));
        }
        case schema::Restricted::RestrictedTypes::map_t : {
            return processMap (
                dynamic_cast<const schema::Map &> (restricted));
        }
        case schema::Restricted::RestrictedTypes::array_t : {
            DBG ("  array_t" << std::endl); // NOLINT
            return processArray (
                dynamic_cast<const schema::Array &> (restricted));
        }
    }

    DBG ("  ProcessRestricted: Returning nullptr"); // NOLINT
    return nullptr;
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::byType (const std::string & type_) {
    auto it = m_serialisersByType.find (type_);

    return (it == m_serialisersByType.end()) ? nullptr : it->second;
}

/******************************************************************************/

sPtr<amqp::serialiser::ISerialiser>
amqp::internal::assembler::
CompositeFactory::byDescriptor (const std::string & descriptor_) {
    auto it = m_serialisersByDescriptor.find (descriptor_);

    return (it == m_serialisersByDescriptor.end()) ? nullptr : it->second;
}

/******************************************************************************/
