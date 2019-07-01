#pragma once

/******************************************************************************/

#include <set>
#include <map>
#include <iosfwd>

#include "types.h"
#include "Composite.h"
#include "Descriptor.h"

#include "amqp/AMQPDescribed.h"

/******************************************************************************/

namespace amqp::internal::schema {

    typedef std::function <bool(const uPtr<AMQPTypeNotation> &, const uPtr<AMQPTypeNotation> &)>  const SetSort;

}

/******************************************************************************/

namespace amqp::internal::schema {


}

/******************************************************************************/

namespace amqp::internal::schema {

    /*
     */
    class Schema : public AMQPDescribed {
        public :
            friend std::ostream & operator << (std::ostream &, const Schema &);

            typedef std::set<uPtr<AMQPTypeNotation>, SetSort> SchemaSet;
            typedef std::map<std::string, const std::reference_wrapper<const uPtr<AMQPTypeNotation>>> SchemaMap;

            static amqp::internal::schema::SetSort setSorter;

        private :
            SchemaSet m_types;
            SchemaMap m_descriptorToType;
            SchemaMap m_typeToDescriptor;

        public :
            Schema (SchemaSet types_);

            const SchemaSet & types() const;

            SchemaMap::const_iterator fromType (const std::string &) const;
            SchemaMap::const_iterator fromDescriptor (const std::string &) const;
            SchemaSet::const_iterator end() const;
            SchemaSet::const_iterator begin() const;
    };

}

/******************************************************************************/

