#pragma once

/******************************************************************************/

#include <map>
#include <string>
#include <amqp/src/serialiser/serialisers/restricted-serialisers/ListSerialiser.h>
#include <amqp/src/serialiser/serialisers/reader/restricted-readers/ListReader.h>
#include <amqp/src/serialiser/serialisers/writer/Writer.h>

#include "amqp/src/serialiser/Serialiser.h"
#include "amqp/src/serialiser/serialisers/property-serialisers/IntPropertySerialiser.h"
#include "amqp/src/serialiser/serialisers/property-serialisers/DoublePropertySerialiser.h"
#include "amqp/src/serialiser/serialisers/property-serialisers/LongPropertySerialiser.h"
#include "amqp/src/serialiser/serialisers/property-serialisers/BoolPropertySerialiser.h"
#include "amqp/src/serialiser/serialisers/property-serialisers/StringPropertySerialiser.h"
#include "amqp/src/serialiser/serialisers/property-serialisers/FloatPropertySerialiser.h"
#include "amqp/src/serialiser/serialisers/property-serialisers/BinaryPropertySerialiser.h"

#include "amqp/src/serialiser/serialisers/reader/property-readers/IntPropertyReader.h"
#include "amqp/src/serialiser/serialisers/reader/property-readers/DoublePropertyReader.h"
#include "amqp/src/serialiser/serialisers/reader/property-readers/LongPropertyReader.h"
#include "amqp/src/serialiser/serialisers/reader/property-readers/BoolPropertyReader.h"
#include "amqp/src/serialiser/serialisers/reader/property-readers/StringPropertyReader.h"
#include "amqp/src/serialiser/serialisers/reader/property-readers/FloatPropertyReader.h"
#include "amqp/src/serialiser/serialisers/reader/property-readers/BinaryPropertyReader.h"

/******************************************************************************/

namespace amqp::internal::serialiser::serialisers {

    class SerialisersFactory {
        public :
            static std::shared_ptr<amqp::serialiser::ISerialiser>
            makePropertyReader (const std::string &type_) {
                if (type_ == "int") {
                    return std::make_shared<
                        IntPropertySerialiser<
                            reader::IntPropertyReader,
                            writer::Writer>> ();
                } else if (type_ == "long") {
                    return std::make_shared<
                        LongPropertySerialiser<
                            reader::LongPropertyReader,
                            writer::Writer>> ();
                } else if (type_ == "double") {
                    return std::make_shared<
                        DoublePropertySerialiser<
                            reader::DoublePropertyReader,
                            writer::Writer>> ();
                } else if (type_ == "boolean") {
                    return std::make_shared<
                        BoolPropertySerialiser<
                            reader::BoolPropertyReader,
                            writer::Writer>> ();
                } else if (type_ == "string") {
                    return std::make_shared<
                        StringPropertySerialiser<
                            reader::StringPropertyReader,
                            writer::Writer>> ();
                } else if (type_ == "float") {
                    return std::make_shared<
                        FloatPropertySerialiser<
                            reader::FloatPropertyReader,
                            writer::Writer>> ();
                } else if (type_ == "binary") {
                    return std::make_shared<
                        BinaryPropertySerialiser<
                            reader::BinaryPropertyReader,
                            writer::Writer>> ();
                } else {
                    throw std::runtime_error ("THIS IS BAD");
                }
            }

            static std::shared_ptr<amqp::serialiser::ISerialiser>
            makeList (const std::string & type_, std::weak_ptr<amqp::serialiser::ISerialiser> serialser_) {
                return std::make_shared<ListSerialiser<reader::ListReader, writer::Writer>> (type_, serialser_);
            }
    };

}

/******************************************************************************/
