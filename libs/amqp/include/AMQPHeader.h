#pragma once

/******************************************************************************/

#include <array>

/******************************************************************************/

namespace amqp {

    /**
     * The 8th byte is used to store weather the stream is compressed or 
     * not
     */
    const std::array<char, 7> AMQP_HEADER {
        { 'c', 'o', 'r', 'd', 'a', 1, 0 }
    };

}

/******************************************************************************/

