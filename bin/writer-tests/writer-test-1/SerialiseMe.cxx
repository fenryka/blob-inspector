#include "SerialiseMe.h"

/******************************************************************************/

uPtr<amqp::serialiser::ISerialiser>
SerialiseMe::build (
        amqp::assembler::SerialiserFactory & factory_
) const {
    /*
     * auto builder = factory.obj()
     * builder.add ("x", getter)
     */

//    factory_.primSerialiser(m_val, "int")
//    factory_.primSerialiser(m_val2, "string")



}

uPtr<amqp::AMQPBlob>
SerialiseMe::serialiseImpl(
        const amqp::assembler::SerialiserFactory & sf_,
        uPtr<amqp::ModifiableAMQPBlob> blob_
) const {
    return uPtr<amqp::AMQPBlob>();
}

/******************************************************************************/
