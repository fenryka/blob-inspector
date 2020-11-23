target_sources(${libName}
    PRIVATE
        CompositeFactoryInternal.cxx
        SerialiserFactoryInternal.cxx
    PUBLIC
        SerialiserFactoryInternal.h
        CompositeFactoryInternal.h
)