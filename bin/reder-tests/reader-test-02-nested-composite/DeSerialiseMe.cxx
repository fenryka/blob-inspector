#include "DeSerialiseMe.h"

/******************************************************************************/

Inner::Inner (
    const amqp::assembler::SerialiserFactory & sf_,
    const amqp::AMQPBlob & blob_
) : Serializable (
    javaTypeName<decltype(this)>(),
    "fingerprint123")
  , m_val { sf_.read<int> (blob_) }
{

}

void
Inner::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
//    sf_.write<int> (m_val, "m_val", *this, blob_);
}

/******************************************************************************/

Outer::Outer (
    const amqp::assembler::SerialiserFactory & sf_,
    const amqp::AMQPBlob & blob_
) : Serializable (
    javaTypeName<decltype(this)>(),
    "fingerprint456")
  , m_a (sf_.read<Inner> (blob_))
  , m_b (sf_.read<Inner *> (blob_))
{
}

/******************************************************************************/

void
Outer::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
    sf_.write (m_a, "m_a", *this, blob_);
    sf_.write (m_b, "m_b", *this, blob_);
}

/******************************************************************************/
