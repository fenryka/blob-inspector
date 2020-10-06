#include "amqp/include/serializable/Serializable.h"
#include "amqp/include/assembler/SerialiserFactory.h"

#include "corda-utils/include/debug.h"

/******************************************************************************/

amqp::serializable::
Serializable::Serializable (
    std::string name_,
    std::string fingerprint_,
    std::string namespace_
) : m_name (std::move (name_))
  , m_fingerprint (std::move (fingerprint_))
  , m_namespace (std::move (namespace_)
) {

}

/******************************************************************************/

/**
 *
 * @param sf_ serialiser factory
 * @return
 */
uPtr<amqp::AMQPBlob>
amqp::serializable::
Serializable::serialise (
        const amqp::assembler::SerialiserFactory & sf_
) const {
    auto blob = sf_.blob();
    serialiseImpl (sf_, *blob);
    return blob->toBlob();
}

/******************************************************************************/

void
amqp::serializable::
Serializable::serialise (
    const amqp::assembler::SerialiserFactory & sf_,
    ModifiableAMQPBlob & blob_) const
{
    DBG (__FUNCTION__ << "::" << m_name << std::endl);
    serialiseImpl (sf_, blob_);
}

/******************************************************************************/

const std::string &
amqp::serializable::
Serializable::name() const {
    return m_name;
}

/******************************************************************************/

const std::string &
amqp::serializable::
Serializable::fingerprint() const {
    return m_fingerprint;
}

/******************************************************************************/

const std::string &
amqp::serializable::
Serializable::javaNamespace() const {
    return m_namespace;
}

/******************************************************************************/
