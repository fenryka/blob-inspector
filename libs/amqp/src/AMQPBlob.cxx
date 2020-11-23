#include <sstream>
#include <cassert>

#include "AMQPBlob.h"

#include "amqp/include/CordaBytes.h"
#include "amqp/include/assembler/CompositeFactory.h"

#include "amqp/src/schema/described-types/Envelope.h"
#include "amqp/src/schema/descriptors/Descriptors.h"
#include "amqp/src/schema/descriptors/AMQPDescriptorRegistory.h"
#include "amqp/src/assembler/CompositeFactoryInternal.h"

#include "proton-wrapper/include/proton_wrapper.h"


/******************************************************************************/

/**
 * Should only be being invoked by Modifiable Blob
 */
amqp::AMQPBlob::AMQPBlob()
    : m_data { pn_data (0) }
{

}

/******************************************************************************/

amqp::
AMQPBlob::AMQPBlob (amqp::CordaBytes & cb_)
    : m_data { pn_data (cb_.size()) }
{
    // returns how many bytes we processed which right now we don't care
    // about but I assume there is a case where it doesn't process the
    // entire file

    auto rtn = pn_data_decode(m_data, cb_.bytes(), cb_.size());
    assert (static_cast<size_t>(rtn) == cb_.size());
}

/******************************************************************************/

amqp::
AMQPBlob::AMQPBlob (pn_data_t * bytes_)
    : m_data (bytes_)
{

}

/******************************************************************************/

auto
amqp::
AMQPBlob::dumpSchema (schema::DumpTarget target_) const -> std::string {
    proton::is_described (m_data);

    std::stringstream ss;

    amqp::internal::AMQPDescriptorRegistory[PN_DESCRIBED]->read (
       m_data,
        ss,
       target_);

/**
 * I have zero idea why clan things *this* line is an infinite loop, but it
 * does and that's vexing so shut up the warning
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    return ss.str();
#pragma clang diagnostic pop
#pragma clang diagnostic pop
}

/******************************************************************************
 *
 * Helper functions for the pure data dumper. Doesn't seem much point
 * adding them to the classes interface or externalising them
 * beyond this modele
 *
 ******************************************************************************/

namespace {

    using IValPtr = uPtr<amqp::serialiser::reader::IValue>;
    using SingleString = amqp::internal::serialiser::reader::TypedSingle<std::string>;
    using SingleList = amqp::internal::serialiser::reader::TypedSingle<sList<IValPtr>>;
    using PairList = amqp::internal::serialiser::reader::TypedPair<sList<IValPtr>>;

    /*
     * Forward declarations
     */
    IValPtr dumpDescribed (pn_data_t *);
    sList<IValPtr> dumpList (pn_data_t *);
    sList<IValPtr> dumpMap (pn_data_t *);

    IValPtr
    dumpPrimitive (pn_data_t * data_) {
        switch (pn_data_type (data_)) {
            case PN_LONG :
                return std::make_unique<SingleString> (std::to_string (pn_data_get_long (data_)));
            case PN_ULONG :
                return std::make_unique<SingleString> (std::to_string (pn_data_get_ulong (data_)));
            case PN_STRING :
                return std::make_unique<SingleString> (proton::get_string (data_));
            case PN_INT :
                return std::make_unique<SingleString> (std::to_string (pn_data_get_int (data_)));
            case PN_UINT :
                return std::make_unique<SingleString> (std::to_string (pn_data_get_uint (data_)));
            case PN_FLOAT :
                return std::make_unique<SingleString> (std::to_string (pn_data_get_float (data_)));
            case PN_DOUBLE :
                return std::make_unique<SingleString> (std::to_string (pn_data_get_double (data_)));
            case PN_BOOL :
                return std::make_unique<SingleString> (pn_data_get_bool (data_) ? "true" : "false");
            case PN_SYMBOL :
                return std::make_unique<SingleString> ("<<SYMBOL>>");
            default :
                return std::make_unique<SingleString> (proton::typeToString(data_));

        }
    }


    sList<IValPtr>
    dumpList (pn_data_t * data_) {
        proton::attest_is_list (data_, __FILE__, __LINE__);
        proton::auto_list_enter ale (data_, true);

        sList<IValPtr> list;

        /*
         * Cope with the list being empty
         */
        if (pn_data_type(data_) == PN_INVALID) {
            list.emplace_back (
                    std::make_unique<SingleString>("<<<Empty List>>>"));

            return list;
        }

        /*
         * List isn't empty so iterate the contents
         */
        do {
            auto type = pn_data_type(data_);

            if (type == PN_LIST) {
                list.emplace_back (std::make_unique<SingleList> (dumpList(data_)));
            } else if (type == PN_MAP) {
                list.emplace_back (std::make_unique<SingleList> (dumpMap(data_)));
            } else if (type == PN_DESCRIBED) {
                list.emplace_back (dumpDescribed(data_));
            } else {
                list.emplace_back (dumpPrimitive(data_));
            }
        } while (pn_data_next(data_));

        return list;
    }


    sList<IValPtr>
    dumpMap (pn_data_t * data_) {
        proton::is_map (data_, __FILE__, __LINE__);
        sList<IValPtr> list;
        {
            proton::auto_map_enter am (data_, true);

            /*
             * Cope with an empty map
             */
            if (pn_data_type(data_) == PN_INVALID) {
                list.emplace_back (
                    std::make_unique<SingleString>("<<<Empty Map>>>"));

                return list;
            }

            DBG ("MAP IS NOT EMPTY" << std::endl); // NOLINT

            /*
             * Map isn't empty
             */
            for (int i {0} ; i < am.elements() ; i += 2) {
                IValPtr key, value;

                auto type = pn_data_type ((pn_data_t *)proton::auto_next (data_));
                DBG ("  " << pn_type_name(type) << " " << type << std::endl); // NOLINT
                switch (type) {
                    case PN_LIST : key = std::make_unique<SingleList>(dumpList (data_)); break;
                    case PN_MAP  : key = std::make_unique<SingleList>(dumpMap (data_)); break;
                    case PN_DESCRIBED : key = dumpDescribed (data_); break;
                    default : key = dumpPrimitive (data_);
                }

                type = pn_data_type ((pn_data_t *)proton::auto_next (data_));
                DBG ("  " << pn_type_name(type) << " " << type << std::endl); // NOLINT
                switch (type) {
                    case PN_LIST : value = std::make_unique<SingleList>(dumpList (data_)); break;
                    case PN_MAP  : value = std::make_unique<SingleList>(dumpMap (data_)); break;
                    case PN_DESCRIBED : value = dumpDescribed (data_); break;
                    default : value = dumpPrimitive (data_);
                }

                list.emplace_back (
                        std::make_unique<amqp::internal::serialiser::reader::ValuePair> (
                                std::move (key), std::move (value)
                        )
                );

                pn_data_next (data_);
            }
        }

        return list;
    }


    IValPtr
    dumpDescribed (pn_data_t * data_) {
        DBG (__FUNCTION__  << std::endl); // NOLINT
    //    proton::attest_is_described (data_, __FILE__, __LINE__);

        {
            proton::auto_enter p2(data_);
            auto fingerprint = proton::readAndNext<std::string>(data_, __FILE__, __LINE__);
            DBG ("  FINGERPRINT: " << fingerprint << std::endl); // NOLINT

            auto type = pn_data_type (data_);

            if (type == PN_LIST) {
                return std::make_unique<PairList> (fingerprint, dumpList (data_));
            } else if (type == PN_MAP) {
                return std::make_unique<PairList> (fingerprint, dumpMap (data_));
            } else {
                throw std::runtime_error ("Described data must be either a list or map");
            }
        }
    }
}

/******************************************************************************/

/**
 * Take a aCorda serialized blob and walk the data section of it dumping
 * the contents. Less usful than the schema dumper but a useful debugging
 * tool none the less
 */
auto
amqp::
AMQPBlob::dumpData() const -> std::string {
    IValPtr rtn;

    if (pn_data_is_described (m_data)) {
        // move to the actual blob entry in the tree - ideally we'd have
        // saved this on the Envelope but that's not easily doable as we
        // can't grab an actual copy of our data pointer
        proton::attest_is_described (m_data, __FILE__, __LINE__);
        {
            proton::auto_enter p (m_data);
            uint64_t key = proton::readAndNext<u_long>(m_data, __FILE__, __LINE__);
            assert (stripCorda (key) == amqp::internal::schema::descriptors::ENVELOPE);
        }

        proton::auto_enter p (m_data, true);
        proton::attest_is_list (m_data, __FILE__, __LINE__);

        // first element in the list is the data (second the schema, third the transforms)
        proton::auto_list_enter ale (m_data, true);
        proton::attest_is_described (m_data, __FILE__, __LINE__);
        rtn = dumpDescribed (m_data);
    }

    return rtn->dump();
}

/******************************************************************************/

/*
 * Read the contents of the blob out as a string matching data in the
 * payload to elements from the schema
 */
std::string
amqp::
AMQPBlob::dumpContents (amqp::CompositeFactory & cf_) const {
    std::unique_ptr<internal::schema::Envelope> envelope;

    if (pn_data_is_described (m_data)) {
        proton::auto_enter p (m_data);

        auto a = pn_data_get_ulong(m_data);

        envelope.reset (
            dynamic_cast<amqp::internal::schema::Envelope *> (
                internal::AMQPDescriptorRegistory[a]->build(m_data).release()));
    }

    cf_.process (envelope->schema());

    auto reader = cf_.byDescriptor (envelope->descriptor());
    assert (reader);

    {
        // move to the actual blob entry in the tree - ideally we'd have
        // saved this on the Envelope but that's not easily doable as we
        // can't grab an actual copy of our data pointer
        proton::auto_enter p (m_data, true);
        proton::attest_is_list (m_data, __FILE__, __LINE__);
        assert (pn_data_get_list (m_data) == 3);

        {
            proton::auto_enter p2 (m_data);

            std::stringstream ss;

            // We wrap our output like this to make sure it's valid JSON to
            // facilitate easy pretty printing
            ss << reader->dump ("{ Parsed", m_data, envelope->schema())->dump()
               << " }";

            return ss.str();
        }
    }
}

/******************************************************************************/

pn_data_t *
amqp::
AMQPBlob::data() const {
    return m_data;
}

/******************************************************************************/

void
amqp::
AMQPBlob::readyPayload () const {
    DBG (__FUNCTION__ << std::endl);

    proton::attest_is_described (m_data, __FILE__, __LINE__);

    proton::pn_data_enter (m_data);

    uint64_t key = proton::readAndNext<u_long>(m_data, __FILE__, __LINE__);
    assert (stripCorda (key) == amqp::internal::schema::descriptors::ENVELOPE);
    proton::attest_is_list (m_data, __FILE__, __LINE__);
    proton::pn_data_enter (m_data);
    proton::attest_is_described (m_data, __FILE__, __LINE__);
    {
        proton::auto_enter an (m_data);
        DBG (__FUNCTION__ << "::" << m_data << " " << describedToString (pn_data_get_ulong (m_data))
                          << std::endl); // NOLINT
    }

    /*
    DBG (__FUNCTION__ << " " << describedToString (pn_data_get_ulong (m_data)) << std::endl);

    proton::pn_data_enter (m_data);
    proton::attest_is_list (m_data, __FILE__, __LINE__);

    // first element in the list is the data (second the schema, third the transforms)
    proton::auto_list_enter ale (m_data, true);
    proton::attest_is_described (m_data, __FILE__, __LINE__);
     */
}

void
amqp::
AMQPBlob::startComposite () const {
    proton::attest_is_described (m_data, __FILE__, __LINE__);
    proton::pn_data_enter (m_data);
    uint64_t key = proton::readAndNext<u_long>(m_data, __FILE__, __LINE__);
    DBG (__FUNCTION__ << "::" << describedToString (key) <<  std::endl); // NOLINT
    proton::attest_is_list (m_data, __FILE__, __LINE__);
    proton::pn_data_enter (m_data);
    pn_data_next (m_data);
    DBG (__FUNCTION__ << "::" << m_data << std::endl);
}

void
amqp::
AMQPBlob::endComposite () {

}

void
amqp::
AMQPBlob::startRestricted (const amqp::serializable::RestrictedSerializable &) {

}

void
amqp::
AMQPBlob::endRestricted (const amqp::serializable::RestrictedSerializable &) {

}

/******************************************************************************/
