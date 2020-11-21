# Make it OK to convert relatvie paths when adding interface sources to targets
CMAKE_POLICY (SET CMP0076 NEW)

set (libName corda-custom-types)

include_directories (include)
ADD_LIBRARY ( ${libName} SHARED "" )

SET_TARGET_PROPERTIES (${libName} PROPERTIES VERSION ${PROJECT_VERSION})
SET_TARGET_PROPERTIES (${libName} PROPERTIES SOVERSION 1)

#
# So on Linux we don't need to add these static libraries to this dynamic library
# target but on MacOS it appears we do
#
if(APPLE)
    target_link_libraries (${libName} proton-wrapper qpid-proton)
endif()

ADD_SUBDIRECTORY (include)
ADD_SUBDIRECTORY (src)
