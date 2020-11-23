#pragma once

/******************************************************************************/

#include "amqp/src/serialiser/serialisers/PropertySerialiser.h"
#include "BinaryPropertySerialiserBase.h"

/******************************************************************************/

namespace amqp::internal::serialiser::serialisers {

    template <class Reader, class Writer>
    class BinaryPropertySerialiser
            : public PropertySerialiser<Reader, Writer>
            , public BinaryPropertySerialiserBase
    {
        public :
            [[nodiscard]] const std::string & name() const override {
                return m_name;
            }

            [[nodiscard]] const std::string & type() const override {
                return m_type;
            }
    };

}

/******************************************************************************/

