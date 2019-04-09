#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstddef>

#include <proton/types.h>
#include <proton/codec.h>
#include <sys/stat.h>

#include "proton/proton_wrapper.h"

#include "amqp/AMQPHeader.h"
#include "amqp/AMQPSectionId.h"
#include "amqp/descriptors/AMQPDescriptorRegistory.h"

#include "amqp/schema/Envelope.h"

/******************************************************************************/

void
data_and_stop(std::ifstream & f_, size_t sz) {
    char * blob = new char[sz];
    memset (blob, 0, sz);
    f_.read(blob, sz);

    pn_data_t * d = pn_data(sz);

    // returns how many bytes we processed which right now we don't care
    // about but I assume there is a case where it doesn't process the
    // entire file
    auto rtn = pn_data_decode (d, blob, sz);

    if (pn_data_is_described(d)) {
        proton::auto_enter p (d);

        auto a = pn_data_get_ulong(d);

        auto envelope = std::unique_ptr<amqp::internal::schema::Envelope> (
            static_cast<amqp::internal::schema::Envelope *> (
                amqp::AMQPDescriptorRegistory[a]->build(d).release()));

        std::cout << std::endl << "Types in schema: " << std::endl
            << *envelope << std::endl;
    }
}

/******************************************************************************/

int
main (int argc, char **argv) {
    std::cout << "Inspecting = " << argv[1] << std::endl;
    struct stat results;

    if (stat(argv[1], &results) != 0) {
        return EXIT_FAILURE;
    }

    std::ifstream f (argv[1], std::ios::in | std::ios::binary);
    std::array<char, 7> header;
    f.read(header.data(), 7);

    if (header != amqp::AMQP_HEADER) {
        std::cerr << "Bad Header in blob" << std::endl;
        return EXIT_FAILURE;
    }

    amqp::amqp_section_id_t encoding;
    f.read((char *)&encoding, 1);

    if (encoding == amqp::DATA_AND_STOP) {
        data_and_stop(f, results.st_size - 8);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/******************************************************************************/

