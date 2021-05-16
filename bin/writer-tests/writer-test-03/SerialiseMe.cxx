#include "SerialiseMe.h"

/******************************************************************************/

void
DeSerialiseMe::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
    sf_.write (m_a, "m_a", *this, blob_);
    sf_.write<const std::string &> (m_b, "m_b", *this, blob_);
}

/******************************************************************************/
