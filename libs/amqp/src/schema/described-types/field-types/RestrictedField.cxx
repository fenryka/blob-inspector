#include <include/debug.h>
#include "src/schema/SchemaUtils.h"
#include "RestrictedField.h"

/******************************************************************************/

const std::string
amqp::internal::schema::RestrictedField::m_fieldType { // NOLINT
    "restricted"
};

/******************************************************************************/

amqp::internal::schema::
RestrictedField::RestrictedField (
    std::string name_,
    std::string type_,
    std::list<std::string> requires_,
    std::string default_,
    std::string label_,
    bool mandatory_,
    bool multiple_
) : Field (
    std::move (name_),
    std::move (type_),
    std::move (requires_),
    std::move (default_),
    std::move (label_),
    mandatory_,
    multiple_)
{
    DBG ("RestrictedField - " << name() << "::" << type() << std::endl);

    for (const auto & r : requires()) {
        schema::types::nestedGenericTypes (r, m_subTypes);
    }
}

/******************************************************************************/

amqp::internal::schema::Field::Type
amqp::internal::schema::
RestrictedField::AMQPType() const {
    return restricted_t;
}

/******************************************************************************/

const std::string &
amqp::internal::schema::
RestrictedField::fieldType() const {
    return m_fieldType;
}

/******************************************************************************/

const std::string &
amqp::internal::schema::
RestrictedField::resolvedType() const {
    return requires().front();
}

/******************************************************************************/

const sVec<std::string> &
amqp::internal::schema::
RestrictedField::subTypes() const {
    return m_subTypes;
}

/******************************************************************************/
