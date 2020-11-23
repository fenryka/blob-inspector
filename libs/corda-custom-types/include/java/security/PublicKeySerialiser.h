#pragma once

#include "PublicKey.h"
#include "amqp/include/serialiser/CustomSerializer.h"

namespace amqp::schema {

    class ISchema;

}

struct pn_data_t;

namespace java::security {

    class PublicKeySeraliser : public amqp::serialiser::CustomSerializer<PublicKey> {
        private :
            static const std::string m_name;
            static const std::string m_type;

        public :
            PublicKeySeraliser() : amqp::serialiser::CustomSerializer<PublicKey>() { }
            ~PublicKeySeraliser() override = default;

            [[nodiscard]] const std::string & name() const override;
            [[nodiscard]] const std::string & type() const override;

            std::any read (pn_data_t *) const override;
            std::string readString (pn_data_t *) const override;

            std::unique_ptr<amqp::serialiser::reader::IValue> dump (
                const std::string &,
                pn_data_t *,
                const amqp::schema::ISchema &) const override;

            std::unique_ptr<amqp::serialiser::reader::IValue> dump (
                pn_data_t *,
                const amqp::schema::ISchema &) const override;

            /*
             * Redeclare the IWriter interface
             */
            void write (std::any, pn_data_t *) const override;
    };

}