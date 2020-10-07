#include "SerialiseMe.h"

#include "corda-utils/include/debug.h"

#include <cxxabi.h>

/******************************************************************************/

void
Recursive::serialiseImpl (
    const amqp::assembler::SerialiserFactory & sf_,
    amqp::ModifiableAMQPBlob & blob_
) const {
    DBG (__FUNCTION__ << std::endl);
    int status;
    std::cout << abi::__cxa_demangle (
        typeid(std::remove_pointer_t<decltype(m_c)>).name(),
        nullptr,
        nullptr, &status) << std::endl;

    sf_.writeInt (m_a, "m_a", *this, blob_);
    sf_.writeString (m_b, "m_b", *this, blob_);
    sf_.writeComposite (m_c, "m_c", *this, blob_);
}

/******************************************************************************/

