#pragma once

/******************************************************************************/

#include "amqp/src/serialiser/serialisers/PropertySerialiser.h"
#include "DoublePropertySerialiserBase.h"

/******************************************************************************/

namespace amqp::internal::serialiser::serialisers {

    template <class Reader, class Writer>
    class DoublePropertySerialiser
            : public PropertySerialiser<Reader, Writer>
            , DoublePropertySerialiserBase
    {
        public :
            [[nodiscard]] const std::string & name() const override { return m_name; }
            [[nodiscard]] const std::string & type() const override { return m_type; }
    };

}

/******************************************************************************/
