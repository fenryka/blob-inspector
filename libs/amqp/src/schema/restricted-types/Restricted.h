#pragma once

/******************************************************************************/

#include <list>
#include <vector>
#include <iosfwd>
#include <string>

#include "amqp/src/schema/described-types/Choice.h"
#include "amqp/src/schema/described-types/Descriptor.h"
#include "amqp/src/schema/AMQPTypeNotation.h"

/******************************************************************************
 *
 * Forward class declarations
 *
 ******************************************************************************/

namespace amqp::internal::schema {

    class Composite;
    class OrderedTypeNotation;

    class Map;
    class Enum;
    class List;
    class Array;

}

/******************************************************************************
 *
 * class Restricted
 *
 ******************************************************************************/

namespace amqp::internal::schema {

    class Restricted : public AMQPTypeNotation {
        public :
            friend std::ostream & operator << (std::ostream &, const Restricted&);

            enum RestrictedTypes { list_t, map_t, enum_t, array_t };

        private :
            // could be null in the stream... not sure that information is
            // worth preserving beyond an empty string here.
            std::string m_label;

            /**
             * Which Java interfaces the type implemented when serialised within
             * the JVM. Not really useful for C++ but we're keepign it for
             * the sense of completeness
             */
            std::vector<std::string> m_provides;

            /**
             * Is it a map or list
             */
            RestrictedTypes m_source;

        protected :
            /**
             * keep main constructor private to force use of the named constructor
             */
            Restricted (
                std::unique_ptr<Descriptor> descriptor_,
                std::string,
                std::string,
                std::vector<std::string>,
                RestrictedTypes);

            [[nodiscard]] virtual int dependsOnMap (const Map &) const = 0;
            [[nodiscard]] virtual int dependsOnList (const List &) const = 0;
            [[nodiscard]] virtual int dependsOnArray (const Array &) const = 0;
            [[nodiscard]] virtual int dependsOnEnum (const Enum &) const = 0;

        public :
            static std::unique_ptr<Restricted> make(
                    std::unique_ptr<Descriptor>,
                    std::string,
                    std::string,
                    std::vector<std::string>,
                    std::string,
                    std::vector<uPtr<Choice>>);

            [[nodiscard]] Restricted (Restricted&) = delete;

            [[nodiscard]] Type type() const override;

            [[nodiscard]] RestrictedTypes restrictedType() const;

            /**
             * @return an iterator over the types the restricted class represents.
             * In the case of a list, the element this is a list of, in the
             * case of a map the key and value types etc.
             */
            [[nodiscard]] virtual std::vector<std::string>::const_iterator begin() const = 0;
            [[nodiscard]] virtual std::vector<std::string>::const_iterator end() const = 0;

            [[nodiscard]] int dependsOn (const OrderedTypeNotation &) const override;
            [[nodiscard]] int dependsOnRHS (const Restricted &) const override;

            [[nodiscard]] int dependsOnRHS (const Composite &) const override = 0;

            [[nodiscard]] const decltype (m_provides) & provides() const { return m_provides; }
            [[nodiscard]] const decltype (m_label) & label() const { return m_label; }
            [[nodiscard]] const decltype (m_source) & source() const { return m_source; }
    };


    std::ostream & operator << (std::ostream &, const Restricted::RestrictedTypes &);
}


/******************************************************************************/

