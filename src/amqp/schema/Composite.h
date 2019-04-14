#pragma once

/******************************************************************************/

#include <list>
#include <vector>
#include <iosfwd>
#include <string>

#include "Field.h"
#include "Descriptor.h"

#include "amqp/AMQPDescribed.h"


/******************************************************************************/

namespace amqp {
namespace internal {
namespace schema {

    /*
     * A Corda AMQP Scehma Composite type has:
     *
     * val name: String,
     * val label: String?,
     * val provides: List<String>,
     * val descriptor: Descriptor,
     * val fields: List<Field>
     */
    class Composite : public AMQPDescribed {
        public :
            friend std::ostream & operator << (std::ostream &, const Composite&);

        private :
            std::string m_name;

            // could be null in the stream... not sure that information is
            // worth preserving beyond an empty string here.
            std::string m_label;

            // interfaces the class implements... again since we can't 
            // use Karen to dynamically construct a class
            // we don't know about knowing the interfaces (java concept)
            // that this class mpleemnted isn't al that useful but we'll
            // at least preserve the list
            std::list<std::string> m_provides;

            std::unique_ptr<Descriptor> m_descriptor;

            /**
             * The properties of the Class
             */
            std::vector<std::unique_ptr<Field>> m_fields;

        public :
            Composite (
                const std::string & name_,
                const std::string & label_,
                const std::list<std::string> & provides_,
                std::unique_ptr<Descriptor> & descriptor_,
                std::vector<std::unique_ptr<Field>> & fields_
            ) : m_name (name_)
              , m_label (label_)
              , m_provides (provides_)
              , m_descriptor (std::move (descriptor_))
              , m_fields (std::move (fields_))
            { }

            const std::string & name() const { return m_name; }

            const std::vector<std::unique_ptr<Field>> & fields() const {
                return m_fields;
            }

            const std::string & descriptor() const { return m_descriptor->name(); }
    };

}
}
}

/******************************************************************************/
