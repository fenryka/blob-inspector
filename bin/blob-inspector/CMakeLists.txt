include_directories (${BLOB-INSPECTOR_SOURCE_DIR}/libs/corda-utils)
include_directories (${BLOB-INSPECTOR_SOURCE_DIR}/libs/proton-wrapper)
include_directories (${BLOB-INSPECTOR_SOURCE_DIR}/libs/amqp)
include_directories (${BLOB-INSPECTOR_SOURCE_DIR}/libs/corda-custom-types)

link_directories (${BLOB-INSPECTOR_BINARY_DIR}/libs/amqp)
link_directories (${BLOB-INSPECTOR_BINARY_DIR}/libs/proton-wrapper)
link_directories (${BLOB-INSPECTOR_BINARY_DIR}/libs/corda-custom-types)

set (blob-inspector-sources
        BlobInspector.cxx)


add_executable (blob-inspector main.cxx ${blob-inspector-sources})

target_link_libraries (blob-inspector amqp proton-wrapper qpid-proton corda-custom-types)

#
# Unit tests for the blob inspector. For this to work we also need to create
# a linkable library from the code here to link into our test.
#
add_library (blob-inspector-lib ${blob-inspector-sources} )
ADD_SUBDIRECTORY (test)
