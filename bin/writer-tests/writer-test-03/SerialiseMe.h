#pragma once

#include "include/serializable/Serializable.h"

#include "amqp/include/AMQPBlob.h"
#include "amqp/include/assembler/SerialiserFactory.h"

/******************************************************************************/

class DeSerialiseMe : public amqp::serializable::Serializable {
    private :
        std::string m_a;
        std::string m_b;

        /**
         * Overriding this lets us actually specify how to serialise
         * this class
         */
        void serialiseImpl(
            const amqp::assembler::SerialiserFactory &,
            amqp::ModifiableAMQPBlob &) const override;

    public :
        explicit DeSerialiseMe (std::string a_, std::string b_)
            : Serializable (javaTypeName<decltype(this)>(), "fingerprint123")
            , m_a (std::move (a_))
            , m_b (std::move (b_))
        { }

        [[maybe_unused]] [[nodiscard]] const std::string & a() const {
            return m_a;
        }

        [[maybe_unused]] [[nodiscard]] const std::string & b() const {
            return m_b;
        }
};

/******************************************************************************/
