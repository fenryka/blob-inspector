#include "SerialiseMe.h"

#include "corda-utils/include/debug.h"

#include "amqp/include/assembler/SerialiserFactory.h"

/******************************************************************************/

void
Inner::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
    DBG (__FUNCTION__ << std::endl);

    sf_.write (m_a, "m_a", *this, blob_);
    sf_.write<const std::string &> (m_b, "m_b", *this, blob_);
    sf_.write (m_c, "m_c", *this, blob_);
    sf_.write (m_d, "m_d", *this, blob_);
    sf_.write (m_e, "m_e", *this, blob_);
}

/******************************************************************************/

void
Outer::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
    DBG (__FUNCTION__ << std::endl);
    DBG (javaTypeName<decltype(m_a)>() << std::endl);

//    sf_.writeComposite (m_a, "m_a", *this, blob_);
    sf_.write (m_a, "m_a", *this, blob_);
}

/******************************************************************************/
