#pragma once

/******************************************************************************/

#include <memory>
#include "corda-utils/include/types.h"

#include "described-types/Descriptor.h"
#include "OrderedTypeNotations.h"

/******************************************************************************
 *
 * Forward class declarations
 *
 ******************************************************************************/

namespace amqp::internal::schema {

    class Restricted;
    class Composite;

}

/******************************************************************************
 *
 *
 *
 ******************************************************************************/

namespace amqp::internal::schema {

    class AMQPTypeNotation
        : public amqp::schema::ISchemaElement, public OrderedTypeNotation
    {
        public :
            friend std::ostream & operator << (
                    std::ostream &,
                    const AMQPTypeNotation &);

            enum Type { composite_t, restricted_t };

        private :
            std::string      m_name;
            uPtr<Descriptor> m_descriptor;

        public :
            AMQPTypeNotation (
                std::string name_,
                uPtr<Descriptor> descriptor_
            ) : m_name (std::move (name_))
              , m_descriptor (std::move (descriptor_))
            { }

            [[nodiscard]] const std::string & descriptor() const;
            [[nodiscard]] const std::string & name() const;
            [[nodiscard]] virtual Type type() const = 0;

            [[nodiscard]] virtual int dependsOnRHS (const Restricted &) const = 0;
            [[nodiscard]] virtual int dependsOnRHS (const Composite &) const = 0;
    };

}

/******************************************************************************/
