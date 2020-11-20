#include "amqp/include/serializable/Serializable.h"
#include "amqp/include/assembler/SerialiserFactory.h"

#include "corda-utils/include/debug.h"

/******************************************************************************/

amqp::serializable::
Serializable::Serializable (
    std::string name_,
    std::string fingerprint_
) : m_name (std::move (name_))
  , m_fingerprint (std::move (fingerprint_)
) {

}

/******************************************************************************/

namespace {

    struct AutoComposite {
        const amqp::serializable::Serializable & m_s;
        amqp::ModifiableAMQPBlob & m_b;

        AutoComposite (decltype(m_s) s_, decltype(m_b) b_) : m_s (s_), m_b (b_) {
            amqp::assembler::SerialiserFactory::startComposite (m_s, m_b);
        }

        ~AutoComposite() {
            amqp::assembler::SerialiserFactory::endComposite (m_s, m_b);
        }

    };

};

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
    {
        AutoComposite ac (*this, *blob);
        serialiseImpl (sf_, *blob);
    }
    return blob->toBlob();
}

/******************************************************************************/

void
amqp::serializable::
Serializable::deserialise (
    const amqp::AMQPBlob & blob_
) {
    DBG (__FUNCTION__ << std::endl); // NOLINT
    blob_.startComposite();
    DBG (__FUNCTION__ << "::DOME" << std::endl); // NOLINT
}

/******************************************************************************/

void
amqp::serializable::
Serializable::serialise (
    const amqp::assembler::SerialiserFactory & sf_,
    ModifiableAMQPBlob & blob_) const
{
    DBG (__FUNCTION__ << "::" << m_name << std::endl); // NOLINT
    AutoComposite ac (*this, blob_);
    serialiseImpl (sf_, blob_);
}

/******************************************************************************/

void
amqp::serializable::
Serializable::startComposite (
    const amqp::assembler::SerialiserFactory & sf_,
    const AMQPBlob & blob_
) {

}

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
