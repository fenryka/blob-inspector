
#include "include/java/security/PublicKeySerialiser.h"

#include <string>
#include <proton/codec.h>

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
    const std::string &,
    pn_data_t *,
    const amqp::schema::ISchema &
) const  {

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
