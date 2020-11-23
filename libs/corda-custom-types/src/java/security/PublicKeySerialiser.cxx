
#include "include/java/security/PublicKeySerialiser.h"

#include <string>
#include <proton/codec.h>

#include "proton-wrapper/include/proton_wrapper.h"
#include "amqp/include/serialiser/reader/IReader.h"
#include "amqp/src/serialiser/serialisers/reader/Reader.h"

const std::string
java::security::PublicKeySeraliser::m_name {
   "Public Key serialiser"
};

const std::string
java::security::PublicKeySeraliser::m_type {
    "java.security.PublicKey"
};

const std::string &
java::security::
PublicKeySeraliser::name() const {
    return m_name;
}

const std::string &
java::security::
PublicKeySeraliser::type() const  {
    return m_type;
}

std::any
java::security::
PublicKeySeraliser::read (pn_data_t *) const  {

}

std::string
java::security::
PublicKeySeraliser::readString (pn_data_t *) const  {

}

std::unique_ptr<amqp::serialiser::reader::IValue>
java::security::
PublicKeySeraliser::dump (
    const std::string & name_,
    pn_data_t * data_,
    const amqp::schema::ISchema &
) const  {
    DBG (__FUNCTION__ << std::endl);
    proton::attest_is_binary (data_, __FILE__, __LINE__);
    auto  val = proton::readAndNext<char *> (data_, __FILE__, __LINE__);

    return std::make_unique<amqp::internal::serialiser::reader::TypedPair<std::string>> (
        name_,
        "<<<binary>>>");
}

std::unique_ptr<amqp::serialiser::reader::IValue>
java::security::
PublicKeySeraliser::dump (
    pn_data_t *,
    const amqp::schema::ISchema &
) const  {

}

/*
 * Redeclare the IWriter interface
 */
void
java::security::
PublicKeySeraliser::write (std::any, pn_data_t *) const  {

}
