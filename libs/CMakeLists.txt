
include_directories(amqp)
include_directories(corda-utils)
include_directories(proton-wrapper)
include_directories(corda-custom-types)

ADD_SUBDIRECTORY (amqp)
ADD_SUBDIRECTORY (proton-wrapper)
ADD_SUBDIRECTORY (corda-utils)
ADD_SUBDIRECTORY (corda-custom-types)
