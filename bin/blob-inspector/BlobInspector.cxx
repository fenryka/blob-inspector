#include "BlobInspector.h"
#include "include/CordaBytes.h"

#include "amqp/src/assembler/CompositeFactoryInternal.h"
#include "corda-custom-types/include/java/security/PublicKeySerialiser.h"

#include <iostream>

/******************************************************************************/

BlobInspector::BlobInspector (amqp::CordaBytes & cb_)
    : m_blob { cb_ }
{
}

/******************************************************************************/

std::string
BlobInspector::dump() {
    amqp::internal::assembler::CompositeFactoryInternal cf;

    cf.installCustomSerialiser (std::make_shared<java::security::PublicKeySeraliser>());

    return m_blob.dumpContents (cf);
}

/******************************************************************************/
