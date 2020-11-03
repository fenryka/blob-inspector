#pragma once

/******************************************************************************/

#include <string>

/******************************************************************************/

namespace amqp::internal::serialiser::serialisers {

    /**
     * Allows us to define our static consts outside a header file since
     * [DoublePropertySerialiser] is a templated class
     */
    class DoublePropertySerialiserBase {
        public:
            static const std::string m_name;
            static const std::string m_type;
    };

}

/******************************************************************************/
