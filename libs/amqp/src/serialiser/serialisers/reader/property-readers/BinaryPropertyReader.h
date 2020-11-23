#pragma once

/******************************************************************************/

#include "PropertyReader.h"

#include <string>

/******************************************************************************/

namespace amqp::internal::serialiser::reader {

    class BinaryPropertyReader : public PropertyReader {
        public :
            std::string readString (pn_data_t *) const override;

            std::any read (pn_data_t *) const override;

            uPtr<amqp::serialiser::reader::IValue> dump(
                const std::string &,
                pn_data_t *,
                const amqp::schema::ISchema &
            ) const override;

            uPtr<amqp::serialiser::reader::IValue> dump(
                pn_data_t *,
                const amqp::schema::ISchema &
            ) const override;
    };

}

/******************************************************************************/
