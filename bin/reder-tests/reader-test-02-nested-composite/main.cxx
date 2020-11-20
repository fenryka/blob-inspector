#include <iostream>

#include "DeSerialiseMe.h"

#include "amqp/include/CordaBytes.h"
#include "amqp/include/AMQPBlob.h"

#include "amqp/src/assembler/SerialiserFactoryInternal.h"

/******************************************************************************/

int
main (int argc, char ** argv) {
    if (argc != 1) {
        return EXIT_FAILURE;
    }

    amqp::internal::assembler::SerialiserFactoryInternal sf;

    auto o = Outer();
    auto blob = o.serialise (sf);

    DBG ("\n\nDESERIALISE\n\n");

    Outer o2 = sf.deserialise<Outer> (*blob);


    return EXIT_SUCCESS;
}

/******************************************************************************/
