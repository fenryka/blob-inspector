#pragma once

/******************************************************************************/

#include <map>
#include <memory>
#include <string>
#include <iostream>

#include "amqp/include/schema/AMQPDescribed.h"
#include "amqp/include/schema/SchemaDumpTargets.h"

/******************************************************************************
 *
 * Forward type declarations
 *
 ******************************************************************************/

struct pn_data_t;

/******************************************************************************
 *
 * amqp::internal::AMQPDescribed
 *
 ******************************************************************************/

namespace amqp::internal::schema::descriptors {

    class AutoIndent {
        private :
            std::string indent;
        public :
            AutoIndent() : indent { "" } { }

            AutoIndent (const AutoIndent & ai_)
                : indent { ai_.indent + "  "}
            { }

            friend std::ostream &
            operator << (std::ostream & stream_, const AutoIndent & ai_);

            decltype (indent.length()) length() const { return indent.length(); }
        };
}

/******************************************************************************
 *
 * amqp::internal::AMQPDescriptor
 *
 ******************************************************************************/

namespace amqp::internal::schema::descriptors {

    class AMQPDescriptor {
        protected :
            std::string m_symbol;
            int32_t m_val;

        public :
            AMQPDescriptor()
                : m_symbol ("ERROR")
                , m_val (-1)
            { }

            AMQPDescriptor (std::string symbol_, int val_)
                : m_symbol (std::move (symbol_))
                , m_val (val_)
            { }

            virtual ~AMQPDescriptor() = default;

            const std::string & symbol() const;

            void validateAndNext (pn_data_t *) const;

            virtual std::unique_ptr<AMQPDescribed> build (pn_data_t *) const;

            void read (
                pn_data_t *,
                std::stringstream &,
                amqp::schema::DumpTarget) const;

            virtual void readRaw (
                pn_data_t *,
                std::stringstream &,
                const AutoIndent &) const;

            virtual void readAMQP (
                pn_data_t *,
                std::stringstream &,
                const AutoIndent &) const;

            virtual void readAvro (
                pn_data_t *,
                std::stringstream &,
                const AutoIndent &) const;
    };

}

/******************************************************************************/
