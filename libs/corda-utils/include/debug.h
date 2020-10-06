#pragma once

/******************************************************************************/

#define AMQP_DEBUG 0

/******************************************************************************/

#include <iostream>

/******************************************************************************/

#if defined AMQP_DEBUG && AMQP_DEBUG >= 1
    #include <iostream>

    #define DBG(X) std::cout << "[" << __FILE__ << "::" << __LINE__ << "] " << X
#else
    #define DBG(X)
#endif

/******************************************************************************/
