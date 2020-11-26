target_sources(${libName}
        PRIVATE
            AMQPTypeNotation.cxx
            Descriptors.cxx
            SchemaUtils.cxx
        PUBLIC
        SchemaUtils.h
            OrderedTypeNotations.h
            AMQPTypeNotation.h
)

INCLUDE_DIRECTORIES(descriptors)

ADD_SUBDIRECTORY (restricted-types)
ADD_SUBDIRECTORY (described-types)
ADD_SUBDIRECTORY (descriptors)