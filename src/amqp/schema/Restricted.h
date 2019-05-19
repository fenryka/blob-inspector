#pragma once

/******************************************************************************/

#include <list>
#include <vector>
#include <iosfwd>
#include <string>

#include "Field.h"
#include "Descriptor.h"
#include "AMQPTypeNotation.h"

#include "amqp/AMQPDescribed.h"

/******************************************************************************
 *
 * Forward class declarations
 *
 ******************************************************************************/

namespace amqp::internal::schema {

    class Composite;

}

/******************************************************************************/

namespace amqp::internal::schema {

    class Restricted : public AMQPTypeNotation {
        public :
            friend std::ostream & operator << (std::ostream &, const Restricted&);

            enum RestrictedTypes { List, Map };

        private :
            // could be null in the stream... not sure that information is
            // worth preserving beyond an empty string here.
            std::string m_label;

            std::vector<std::string> m_provides;

            std::string m_sourceStr;

            RestrictedTypes m_source;
        public :
            Restricted (
                std::unique_ptr<Descriptor> & descriptor_,
                const std::string &,
                const std::string &,
                const std::vector<std::string> &,
                const std::string &);

            Restricted(Restricted&) = delete;

            Type type() const override;

            RestrictedTypes restrictedType() const;

            bool lt (const uPtr<AMQPTypeNotation> &) const override;
            bool gte (const Restricted *) const override;
            bool gte (const class Composite *) const override;

            decltype(m_provides)::const_iterator begin() const { return m_provides.begin(); }
            decltype(m_provides)::const_iterator end() const { return m_provides.end(); }
    };


    std::ostream & operator << (std::ostream &, const Restricted::RestrictedTypes &);
}


/******************************************************************************/

