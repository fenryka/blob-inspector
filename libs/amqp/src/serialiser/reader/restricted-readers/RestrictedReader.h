#pragma once

/******************************************************************************/

#include "Reader.h"

#include <any>
#include <vector>

#include "restricted-types/Restricted.h"

/******************************************************************************/

struct pn_data_t;

/******************************************************************************/

namespace amqp::internal::serialiser::reader {

    class RestrictedReader : public Reader {
        private :
            static const std::string m_name;
            const std::string m_type;

        public :
            explicit RestrictedReader (std::string);
            ~RestrictedReader() override = default;

            std::any read (pn_data_t *) const override ;

            std::string readString (pn_data_t *) const override;

            std::unique_ptr<amqp::serialiser::reader::IValue> dump(
                const std::string &,
                pn_data_t *,
                const amqp::schema::ISchema &) const override = 0;

            const std::string & name() const override;
            const std::string & type() const override;
    };

}

/******************************************************************************/


