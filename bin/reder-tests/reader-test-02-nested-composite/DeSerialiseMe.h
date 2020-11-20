#pragma once

#include "include/serializable/Serializable.h"

#include "amqp/include/AMQPBlob.h"
#include "amqp/include/assembler/SerialiserFactory.h"

/******************************************************************************/

class Inner : public amqp::serializable::Serializable {
    private :
        int m_val;

        /**
         * Overriding this lets us actually specify how to serialise
         * this class
         */
        void serialiseImpl(
            const amqp::assembler::SerialiserFactory &,
            amqp::ModifiableAMQPBlob &) const override;

    public :
        explicit Inner (int val_)
            : Serializable (javaTypeName<decltype(this)>(), "fingerprint123")
            , m_val (val_)
        { }

        Inner (
            const amqp::assembler::SerialiserFactory &,
            const amqp::AMQPBlob &);
};

/******************************************************************************/

class Outer : public amqp::serializable::Serializable {
    private :
        Inner   m_a;
        Inner * m_b;

        void serialiseImpl(
            const amqp::assembler::SerialiserFactory &,
            amqp::ModifiableAMQPBlob &) const override;

    public :
        explicit Outer ()
            : Serializable (javaTypeName<decltype(this)>(), "fingerprint456")
            , m_a (Inner (1))
            , m_b (new Inner (2))
        { }

        Outer (
            const amqp::assembler::SerialiserFactory &,
            const amqp::AMQPBlob &);
};

/******************************************************************************/
