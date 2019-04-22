#include "PropertyReader.h"

#include <map>
#include <string>
#include <iostream>
#include <functional>

#include <proton/codec.h>

#include "proton/proton_wrapper.h"

/******************************************************************************/

namespace {

    std::map<std::string, std::shared_ptr<amqp::PropertyReader>(*)() > propertyMap = {
        {
            "int", []() -> std::shared_ptr<amqp::PropertyReader> {
                return std::make_shared<amqp::IntPropertyReader> (
                        amqp::IntPropertyReader());
            }
        },
        {
            "string", []() -> std::shared_ptr<amqp::PropertyReader> {
                return std::make_shared<amqp::StringPropertyReader> (
                        amqp::StringPropertyReader());
            }
        },
        {
            "boolean", []() -> std::shared_ptr<amqp::PropertyReader> {
                return std::make_shared<amqp::BoolPropertyReader> (
                        amqp::BoolPropertyReader());
            }
        },
        {
            "long", []() -> std::shared_ptr<amqp::PropertyReader> {
                return std::make_shared<amqp::LongPropertyReader> (
                        amqp::LongPropertyReader());
            }
        },
        {
            "double", []() -> std::shared_ptr<amqp::PropertyReader> {
                return std::make_shared<amqp::DoublePropertyReader> (
                        amqp::DoublePropertyReader());
            }
        }
    };

}

/******************************************************************************/

const std::string amqp::StringPropertyReader::m_name { // NOLINT
    "String Reader"
};

const std::string amqp::IntPropertyReader::m_name { // NOLINT
    "Int Reader"
};

const std::string amqp::BoolPropertyReader::m_name { // NOLINT
    "Bool Reader"
};

const std::string amqp::LongPropertyReader::m_name { // NOLINT
    "Long Reader"
};

const std::string amqp::DoublePropertyReader::m_name { // NOLINT
    "Double Reader"
};

/******************************************************************************/

/**
 * Static factory method
 */
std::shared_ptr<amqp::PropertyReader>
amqp::
PropertyReader::make (const FieldPtr & field_) {
    return propertyMap[field_->type()]();
}

/******************************************************************************
 *
 * StringPropertyReader
 *
 ******************************************************************************/

std::any
amqp::
StringPropertyReader::read (pn_data_t * data_) const {
    return std::any ("hello");
}

/******************************************************************************/

std::string
amqp::
StringPropertyReader::readString (pn_data_t * data_) const {
    return proton::readAndNext<std::string> (data_);
}

/******************************************************************************/

std::unique_ptr<amqp::Pair>
amqp::
StringPropertyReader::dump (
    const std::string & name_,
    pn_data_t * data_,
    const std::unique_ptr<internal::schema::Schema> & schema_) const
{
    return std::make_unique<TypedPair<std::string>> (
            name_,
            "\"" + proton::readAndNext<std::string> (data_) + "\"");
}

/******************************************************************************
 *
 * IntPropertyReader
 *
 ******************************************************************************/

std::any
amqp::
IntPropertyReader::read (pn_data_t * data_) const {
    return std::any (1);
}

/******************************************************************************/

std::string
amqp::
IntPropertyReader::readString (pn_data_t * data_) const {
    return std::to_string (proton::readAndNext<int> (data_));
}

/******************************************************************************/

std::unique_ptr<amqp::Pair>
amqp::
IntPropertyReader::dump (
    const std::string & name_,
    pn_data_t * data_,
    const std::unique_ptr<internal::schema::Schema> & schema_) const
{
    return std::make_unique<TypedPair<std::string>> (
            name_,
            std::to_string (proton::readAndNext<int> (data_)));
}

/******************************************************************************
 *
 * BoolPropertyReader
 *
 ******************************************************************************/

std::any
amqp::
BoolPropertyReader::read (pn_data_t * data_) const {
    return std::any (true);
}

/******************************************************************************/

std::string
amqp::
BoolPropertyReader::readString (pn_data_t * data_) const {
    return std::to_string (proton::readAndNext<bool> (data_));
}

/******************************************************************************/

std::unique_ptr<amqp::Pair>
amqp::
BoolPropertyReader::dump (
    const std::string & name_,
    pn_data_t * data_,
    const std::unique_ptr<internal::schema::Schema> & schema_) const
{
    return std::make_unique<TypedPair<std::string>> (
            name_,
            std::to_string (proton::readAndNext<bool> (data_)));
}

/******************************************************************************
 *
 * LongPropertyReader
 *
 ******************************************************************************/

std::any
amqp::
LongPropertyReader::read (pn_data_t * data_) const {
    return std::any (10L);
}

/******************************************************************************/

std::string
amqp::
LongPropertyReader::readString (pn_data_t * data_) const {
    return std::to_string (proton::readAndNext<long> (data_));
}

/******************************************************************************/

std::unique_ptr<amqp::Pair>
amqp::
LongPropertyReader::dump (
    const std::string & name_,
    pn_data_t * data_,
    const std::unique_ptr<internal::schema::Schema> & schema_) const
{
    return std::make_unique<TypedPair<std::string>> (
            name_,
            std::to_string (proton::readAndNext<long> (data_)));
}

/******************************************************************************
 *
 * DoublePropertyReader
 *
 ******************************************************************************/

std::any
amqp::
DoublePropertyReader::read (pn_data_t * data_) const {
    return std::any (10.0);
}

/******************************************************************************/

std::string
amqp::
DoublePropertyReader::readString (pn_data_t * data_) const {
    return std::to_string (proton::readAndNext<double> (data_));
}

/******************************************************************************/

std::unique_ptr<amqp::Pair>
amqp::
DoublePropertyReader::dump (
    const std::string & name_,
    pn_data_t * data_,
    const std::unique_ptr<internal::schema::Schema> & schema_) const
{
    return std::make_unique<TypedPair<std::string>> (
            name_,
            std::to_string (proton::readAndNext<double> (data_)));
}

/******************************************************************************/

