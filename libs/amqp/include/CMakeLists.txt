target_sources (${libName}
        PUBLIC
            AMQPBlob.h
            AMQPHeader.h
            AMQPSectionId.h
            ModifiableAMQPBlob.h
        )

ADD_SUBDIRECTORY (serializable)
ADD_SUBDIRECTORY (test)
ADD_SUBDIRECTORY (serialiser)