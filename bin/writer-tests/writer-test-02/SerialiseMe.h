#pragma once

#include "include/serializable/Serializable.h"

#include "amqp/include/AMQPBlob.h"
#include "amqp/include/assembler/SerialiserFactory.h"

/******************************************************************************/

class DeSerialiseMe : public amqp::serializable::Serializable {
    private :
        int m_a;
        int m_b;
        int m_c;

    /**
     * Overriding this lets us actually specify how to serialise
     * this class
     */
        void serialiseImpl(
            const amqp::assembler::SerialiserFactory &,
            amqp::ModifiableAMQPBlob &) const override;

    public :
        explicit DeSerialiseMe (int a_, int b_, int c_)
            : Serializable (javaTypeName<decltype(this)>(), "fingerprint123")
            , m_a (a_)
            , m_b (b_)
            , m_c (c_)
        { }

        [[maybe_unused]] [[nodiscard]] int a() const {
            return m_a;
        }

        [[maybe_unused]] [[nodiscard]] int b() const {
            return m_b;
        }

        [[maybe_unused]] [[nodiscard]] int c() const {
            return m_c;
        }
};

/******************************************************************************/
