#include <iostream>

#include "SerialiseMe.h"

#include "amqp/include/CordaBytes.h"

#include "amqp/src/assembler/SerialiserFactoryInternal.h"

/******************************************************************************/

int
main (int argc, char ** argv) {
    if (argc != 2) {
        std::cerr << "ERROR: usage ./writer-test-5 <<filename>>" << std::endl;
        return EXIT_FAILURE;
    }

    Recursive c (500, "siz hundred", nullptr);
    Recursive b (300, "four hundred", &c);
    Recursive a (100, "two hundred", &b);

    amqp::internal::assembler::SerialiserFactoryInternal sf;

    auto blob = a.serialise (sf);

    amqp::CordaBytes cb (*blob);
    cb.toFile (std::string (argv[1]));

    return EXIT_SUCCESS;
}

/******************************************************************************/
