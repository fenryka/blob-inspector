#include "AMQPDescriptors.h"

#include <string>
#include <iostream>
#include <proton/types.h>
#include <proton/codec.h>

#include "Field.h"
#include "Schema.h"
#include "Envelope.h"
#include "Composite.h"

#include "proton/proton_wrapper.h"
#include "AMQPDescriptorRegistory.h"

/******************************************************************************
 *
 *
 *
 ******************************************************************************/

namespace {

    /**
     * Look up a described type by its ID in the AMQPDescriptorRegistory and
     * return the coresponding schema type. Specialised below to avoid
     * the cast and re-owning of the unigue pointer when we're happy
     * with a simple std::unique_ptr<AMQPDescribed>
     */
    template<class T>
    std::unique_ptr<T>
    dispatchDescribed (pn_data_t * data_) {
        proton::is_described(data_);
        proton::auto_enter p (data_);
        proton::is_ulong(data_);

        auto id = pn_data_get_ulong(data_);

        return std::unique_ptr<T> (
              static_cast<T *>(amqp::AMQPDescriptorRegistory[id]->build(data_).release()));

    }

}

/******************************************************************************/

void
amqp::internal::
AMQPDescriptor::validateAndNext (pn_data_t * const data_) const {
    if (pn_data_type(data_) != PN_ULONG) {
        throw std::runtime_error ("Bad type for a descriptor");
    }

    if (pn_data_get_ulong(data_) != (m_val | amqp::internal::DESCRIPTOR_TOP_32BITS)) {
        throw std::runtime_error ("Invalud Type");
    }

    pn_data_next(data_);
/*
    if (pn_data_type(data_) != PN_LIST) {
        throw std::runtime_error ("Envelope should be list");
    }
    */
}

/******************************************************************************/

namespace {
    void
    consumeBlob (pn_data_t * data_) {
        proton::is_described(data_);
        proton::auto_enter p (data_);
        std::cout << data_ << std::endl;
        proton::is_symbol (data_);
        pn_data_next (data_);
        proton::is_list (data_);
        {
            proton::auto_list_enter p (data_);
            while (pn_data_next(data_)) {
                if (pn_data_is_described (data_)) {
                    consumeBlob (data_);
                }
                else {
                    std::cout << data_ << std::endl;
                }
            }
        }
    }
}

/******************************************************************************
 *
 * amqp::internal::EnvelopeDescriptor
 *
 ******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
EnvelopeDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    proton::auto_enter p (data_);

    /*
     * The actual blob... if this was java we would use the type symbols
     * in the blob to look up serialisers in the cache... but we don't 
     * have any so we are actually going to need to use the schema
     * which we parse *after* this to be able to read any data!
     */
    consumeBlob(data_);

    pn_data_next(data_);

    /*
     * The scehama
     */
    auto schema_ = dispatchDescribed<schema::Schema> (data_);

    pn_data_next(data_);

    /*
     * The transforms schema
     */
    // Skip for now
    // dispatchDescribed (data_);

    return std::make_unique<schema::Envelope> (schema::Envelope (schema_));
}

/******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
SchemaDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    std::map<std::string, std::unique_ptr<schema::Composite>> types;

    /*
     * The Schema is stored as a list of listf of described objects
     */
    {
        proton::auto_list_enter p (data_);

        while (pn_data_next(data_)) {
            proton::auto_list_enter p (data_);
            while (pn_data_next(data_)) {
                auto type = dispatchDescribed<schema::Composite>(data_);
                types[type->name()] = std::move(type);
            }
        }
    }

    return std::make_unique<schema::Schema> (schema::Schema (types));
}

/******************************************************************************
 *
 * amqp::internal::ObjectDescriptor
 *
 ******************************************************************************/

/**
 * 
 */
std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
ObjectDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    proton::auto_enter p (data_);

    auto symbol = proton::get_symbol (data_);

    return std::make_unique<schema::Descriptor> (
        schema::Descriptor (std::string (symbol.start, symbol.size)));
}

/******************************************************************************
 *
 * amqp::internal::FieldDescriptor
 *
 ******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
FieldDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    proton::auto_enter p (data_);

    /* name: String */
    auto name = proton::get_string(data_);

    pn_data_next(data_);

    /* type: String */
    auto type = proton::get_string(data_);

    pn_data_next(data_);

    /* requires: List<String> */
    std::list<std::string> requires;
    {
        proton::auto_list_enter p (data_);
        while (pn_data_next(data_)) {
            requires.push_back (proton::get_string(data_));
        }
    }

    pn_data_next(data_);

    /* default: String? */
    auto def = proton::get_string(data_, true);

    pn_data_next(data_);

    /* label: String? */
    auto label = proton::get_string(data_, true);

    pn_data_next(data_);

    /* mandatory: Boolean - copes with the Kotlin concept of nullability.
       If something is mandatory then it cannot be null */
    auto mandatory = proton::get_boolean(data_);

    pn_data_next(data_);

    /* multiple: Boolean */
    auto multiple = proton::get_boolean(data_);

    return std::make_unique<schema::Field> (name, type, requires, def, label,
            mandatory, multiple);
}

/******************************************************************************
 *
 * amqp::internal::CompositeDescriptor
 *
 ******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
CompositeDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    proton::auto_enter p (data_);

    /* Class Name - String */
    auto name = proton::get_string(data_);

    pn_data_next(data_);

    /* Label Name - Nullable String */
    auto label = proton::get_string(data_, true);

    pn_data_next(data_);

    /* provides: List<String> */
    std::list<std::string> provides;
    {
        proton::auto_list_enter p (data_);
        while (pn_data_next(data_)) {
            provides.push_back (proton::get_string (data_));
        }
    }

    pn_data_next(data_);

    /* descriptor: Descriptor */
    auto descriptor = dispatchDescribed<schema::Descriptor>(data_);

    pn_data_next(data_);

    /* fields: List<Described>*/
    std::list<std::unique_ptr<schema::Field>> fields;
    {
        proton::auto_list_enter p (data_);
        while (pn_data_next(data_)) {
            fields.push_back (dispatchDescribed<schema::Field>(data_));
        }
    }

    return std::make_unique<schema::Composite> (
        schema::Composite (name, label, provides, descriptor, fields));
}

/******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
RestrictedDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    std::cout << "RESTRICTED " << data_ << std::endl;

    return std::unique_ptr<amqp::internal::AMQPDescribed> (nullptr);
}

/******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
ChoiceDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    std::cout << "CHOICE " << data_ << std::endl;

    return std::unique_ptr<amqp::internal::AMQPDescribed> (nullptr);
}

/******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
ReferencedObjectDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    std::cout << "REFERENCED OBJECT " << data_ << std::endl;

    return std::unique_ptr<amqp::internal::AMQPDescribed> (nullptr);
}

/******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
TransformSchemaDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    std::cout << "TRANSFORM SCHEMA " << data_ << std::endl;

    return std::unique_ptr<amqp::internal::AMQPDescribed> (nullptr);
}

/******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
TransformElementDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    std::cout << "TRANFORM ELEMENT " << data_ << std::endl;

    return std::unique_ptr<amqp::internal::AMQPDescribed> (nullptr);
}

/******************************************************************************/

std::unique_ptr<amqp::internal::AMQPDescribed>
amqp::internal::
TransformElementKeyDescriptor::build(pn_data_t * data_) const {
    validateAndNext(data_);

    std::cout << "TRANSFORM ELEMENT KEY" << data_ << std::endl;

    return std::unique_ptr<amqp::internal::AMQPDescribed> (nullptr);
}

/******************************************************************************/
