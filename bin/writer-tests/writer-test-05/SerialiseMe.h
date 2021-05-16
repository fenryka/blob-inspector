#pragma once

#include "include/serializable/Serializable.h"

#include "amqp/include/AMQPBlob.h"
#include "amqp/include/assembler/SerialiserFactory.h"

/******************************************************************************/

class Recursive: public amqp::serializable::Serializable {
    private :
        int m_a;
        std::string m_b;
        Recursive * m_c;

        /**
         * Overriding this lets us actually specify how to serialise
         * this class
         */
        void serialiseImpl(
            const amqp::assembler::SerialiserFactory &,
            amqp::ModifiableAMQPBlob &) const override;

    public :
        explicit Recursive (int a_, std::string b_, Recursive * c_)
            : Serializable (javaTypeName<decltype(this)>(),"fingerprint123")
            , m_a (a_)
            , m_b (std::move (b_))
            , m_c (c_)
        { }
};

/******************************************************************************/

