#pragma once

#include "amqp/src/ModifiableAMQPBlobImpl.h"
#include "amqp/src/serialiser/serialisers/reader/CompositeReader.h"
#include "amqp/src/serialiser/serialisers/writer/Writer.h"
#include "amqp/include/assembler/SerialiserFactory.h"
#include "amqp/src/serialiser/Serialiser.h"
#include "amqp/src/serialiser/serialisers/CompositeSerialiser.h"

/******************************************************************************/

namespace amqp::internal::assembler {

    using Serializable = amqp::serializable::Serializable;

    class SerialiserFactoryInternal : public amqp::assembler::SerialiserFactory {
        public :
            [[nodiscard]] uPtr<ModifiableAMQPBlob> blob() const override;
    };

}

/******************************************************************************/
