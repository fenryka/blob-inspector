#include <iostream>

#include "DeSerialiseMe.h"

#include "amqp/include/CordaBytes.h"
#include "amqp/include/AMQPBlob.h"

#include "amqp/src/assembler/SerialiserFactoryInternal.h"

/******************************************************************************/

int
main (int argc, char ** argv) {
    if (argc != 2) {
        std::cerr << "ERROR: usage ./reader-test-1 <<filename>>" << std::endl;
        return EXIT_FAILURE;
    }

    amqp::internal::assembler::SerialiserFactoryInternal sf;

    amqp::CordaBytes cb (argv[1]);
    amqp::AMQPBlob blob (cb);

    auto b = sf.deserialise<DeSerialiseMe>(blob);

    return EXIT_SUCCESS;
}

/******************************************************************************/
