#include "BinaryPropertyReader.h"

#include "proton-wrapper/include/proton_wrapper.h"

#include <sstream>

/******************************************************************************
 *
 * BinaryPropertyReader
 *
 ******************************************************************************/

std::any
amqp::internal::serialiser::reader::
BinaryPropertyReader::read (pn_data_t * data_) const {
    return std::any (proton::readAndNext<bool> (data_, __FILE__, __LINE__));
}

/******************************************************************************/

std::string
amqp::internal::serialiser::reader::
BinaryPropertyReader::readString (pn_data_t * data_) const {
    return std::to_string (proton::readAndNext<char> (data_, __FILE__, __LINE__));
}

/******************************************************************************/

uPtr<amqp::serialiser::reader::IValue>
amqp::internal::serialiser::reader::
BinaryPropertyReader::dump (
        const std::string & name_,
        pn_data_t * data_,
        const amqp::schema::ISchema & schema_) const
{
    auto val = proton::readAndNext<std::pair<size_t, char *>>(data_, __FILE__, __LINE__);

    std::stringstream ss;
    ss << "<<<binary: " << std::to_string (val.first) << " bytes>>>";

    return std::make_unique<TypedPair<std::string>> (
            name_,
            ss.str());
}

/******************************************************************************/

uPtr<amqp::serialiser::reader::IValue>
amqp::internal::serialiser::reader::
BinaryPropertyReader::dump (
        pn_data_t * data_,
        const amqp::schema::ISchema & schema_) const
{
    auto val = proton::readAndNext<std::pair<size_t, char *>>(data_, __FILE__, __LINE__);

    std::stringstream ss;
    ss << "<<<binary: " << std::to_string (val.first) << " bytes>>>";

    return std::make_unique<TypedSingle<std::string>> (
            ss.str());
}

/******************************************************************************/

