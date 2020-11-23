#pragma once

#include "ISerialiser.h"

namespace amqp::serialiser {

    template<typename T>
    class CustomSerializer : public ISerialiser {
        public :
            ~CustomSerializer() override = default;

            [[nodiscard]] const std::string & name() const override = 0;
            [[nodiscard]] const std::string & type() const override = 0;

            std::any read (pn_data_t *) const override = 0;
            std::string readString (pn_data_t *) const override = 0;

            std::unique_ptr<amqp::serialiser::reader::IValue> dump (
                const std::string &,
                pn_data_t *,
                const schema::ISchema &) const override = 0;

            std::unique_ptr<amqp::serialiser::reader::IValue> dump (
                pn_data_t *,
                const schema::ISchema &) const override = 0;

            /*
             * Redeclare the IWriter interface
             */
            void write (std::any, pn_data_t *) const override = 0;
    };

}