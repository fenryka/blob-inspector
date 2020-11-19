#include "CustomField.h"

/******************************************************************************/

const std::string
amqp::internal::schema::CustomField::m_fieldType { // NOLINT
    "custom"
};

/******************************************************************************/

amqp::internal::schema::
CustomField::CustomField (
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
}

/******************************************************************************/

amqp::internal::schema::Field::Type
amqp::internal::schema::
CustomField::AMQPType() const {
    return custom_t;
}

/******************************************************************************/

const std::string &
amqp::internal::schema::
CustomField::fieldType() const {
    return m_fieldType;
}

/******************************************************************************/

const std::string &
amqp::internal::schema::
CustomField::resolvedType() const {
    return requires().front();
}

/******************************************************************************/
