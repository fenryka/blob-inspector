#pragma once

/******************************************************************************/

#include "amqp/include/serialiser/ISerialiser.h"
#include "corda-utils/include/types.h"

/******************************************************************************/

namespace amqp::schema {

    class ISchema;

}

/******************************************************************************/

namespace amqp {

    class CompositeFactory {
        public :
            CompositeFactory() = default;

            virtual ~CompositeFactory() = default;

            virtual void process (const schema::ISchema &) = 0;

            virtual sPtr<serialiser::ISerialiser> byType (
                    const std::string &) = 0;

            virtual sPtr<serialiser::ISerialiser> byDescriptor (
                    const std::string &) = 0;

            virtual void installCustomSerialiser (sPtr<amqp::serialiser::ISerialiser>) = 0;
    };

}

/******************************************************************************/

