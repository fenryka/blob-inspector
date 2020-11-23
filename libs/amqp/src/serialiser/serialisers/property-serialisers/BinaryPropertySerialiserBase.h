#pragma once

/******************************************************************************/

#include <string>

/******************************************************************************/

namespace amqp::internal::serialiser::serialisers {

    /**
     * Allows us to define our static consts outside a header file since
     * [BinaryPropertySerialiser] is a templated class
     */
    class BinaryPropertySerialiserBase {
        public :
            static const std::string m_name;
            static const std::string m_type;
    };

}

/******************************************************************************/
