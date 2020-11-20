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

DeSerialiseMe::DeSerialiseMe (
    const amqp::assembler::SerialiserFactory & sf_,
    const amqp::AMQPBlob & blob_
) : Serializable (javaTypeName<decltype(this)>(), "fingerprint123") {
    m_val = sf_.read<int>(blob_);
    DBG ("m_val = " << m_val << std::endl);
    m_val2 = sf_.read<int *>(blob_);
    DBG ("m_val2 = " << *m_val2 << std::endl);
}

/******************************************************************************/
