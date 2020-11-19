#pragma once

/******************************************************************************/

#include "amqp/src/schema/described-types/Descriptor.h"

#include "corda-utils/include/types.h"

#include <list>
#include <string>
#include <iosfwd>

/******************************************************************************/

namespace amqp::internal::schema {

    /**
     *
     * A Corda AMQP Schema Field type has:
     *   - name      : String
     *   - type      : String
     *   - requires  : List<String>
     *   - default   : nullable String
     *   - label     : nullable String
     *   - mandatory : Boolean
     *   - multiple  : Boolean
     */
    class Field : public amqp::schema::ISchemaElement {
        public :
            friend std::ostream & operator << (std::ostream &, const Field &);

            static bool typeIsPrimitive (const std::string &);

            static uPtr<Field> make (
                std::string, std::string, std::list<std::string>,
                std::string, std::string, bool, bool);

            enum Type { composite_t, restricted_t, primitive_t, custom_t };

    private :
            std::string            m_name;
            std::string            m_type;
            std::list<std::string> m_requires;
            std::string            m_default;
            std::string            m_label;
            [[maybe_unused]] bool  m_mandatory;
            [[maybe_unused]] bool  m_multiple;

        protected :
            Field (std::string, std::string, std::list<std::string>,
               std::string, std::string, bool, bool);

        public :
            [[nodiscard]] const std::string & name() const;
            [[nodiscard]] const std::string & type() const;
            [[nodiscard]] const std::string & label() const;
            [[nodiscard]] const std::list<std::string> & requires() const;

            [[nodiscard]] virtual Type AMQPType() const = 0;
            [[nodiscard]] virtual const std::string & fieldType() const = 0;
            [[nodiscard]] virtual const std::string & resolvedType() const = 0;
    };

}

/******************************************************************************/

