#pragma once

/******************************************************************************/

#include "Reader.h"

#include <any>
#include <vector>
#include <iostream>

/******************************************************************************/

namespace amqp::schema {
    class ISchema;
}

/******************************************************************************/

namespace amqp::internal::serialiser::reader {

    class CompositeReader : public Reader {
        public :
            CompositeReader() = default;
            ~CompositeReader() override = default;

            std::any read (pn_data_t *) const override;

            std::string readString (pn_data_t *) const override;

            uPtr<amqp::serialiser::reader::IValue> dump(
                const std::string &,
                pn_data_t *,
                const amqp::schema::ISchema &) const override;

            uPtr<amqp::serialiser::reader::IValue> dump(
                pn_data_t *,
                const amqp::schema::ISchema &) const override;

        private :
            sVec<uPtr<amqp::serialiser::reader::IValue>> _dump (
                pn_data_t *,
                const amqp::schema::ISchema &) const;
    };

}

/******************************************************************************/
