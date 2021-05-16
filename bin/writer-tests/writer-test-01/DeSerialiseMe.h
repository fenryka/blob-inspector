#pragma once

#include "include/serializable/Serializable.h"

#include "amqp/include/AMQPBlob.h"
#include "amqp/include/assembler/SerialiserFactory.h"

/******************************************************************************/

class DeSerialiseMe : public amqp::serializable::Serializable {
    private :
        int   m_val;
        int * m_val2;

        /**
         * Overriding this lets us actually specify how to serialise
         * this class
         */
        void serialiseImpl(
            const amqp::assembler::SerialiserFactory &,
            amqp::ModifiableAMQPBlob &) const override;

    public :
        explicit DeSerialiseMe (int val_)
            : Serializable (javaTypeName<decltype(this)>(), "fingerprint123")
            , m_val (val_)
            , m_val2 { new int (val_) }
        { }

        [[maybe_unused]] [[nodiscard]] int val() const {
            return m_val;
        }
};

/******************************************************************************/
