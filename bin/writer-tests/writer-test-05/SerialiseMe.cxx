#include "SerialiseMe.h"

#include "corda-utils/include/debug.h"
#include "corda-utils/include/types.h"

/******************************************************************************/

void
Recursive::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
    DBG (__FUNCTION__ << std::endl); // NOLINT

    sf_.write (m_a, "m_a", *this, blob_);
    sf_.write (m_b, "m_b", *this, blob_);
    //sf_.writeComposite (*m_c, "m_c", *this, blob_);
    sf_.write (m_c, "m_c", *this, blob_);
}

/******************************************************************************/

