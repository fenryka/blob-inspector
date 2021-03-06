#include "DeSerialiseMe.h"

/******************************************************************************/

void
DeSerialiseMe::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
    sf_.write<int> (m_val, "m_val", *this, blob_);
    sf_.write<int *> (m_val2, "m_val2", *this, blob_);
}

/******************************************************************************/
