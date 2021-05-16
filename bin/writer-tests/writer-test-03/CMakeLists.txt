include_directories (${BLOB-INSPECTOR_SOURCE_DIR}/libs/amqp)

link_directories (${BLOB-INSPECTOR_BINARY_DIR}/libs/amqp)

set (writer-test-3-srcs
        main.cxx SerialiseMe.cxx SerialiseMe.h)


add_executable (writer-test-3 ${writer-test-3-srcs})

target_link_libraries (writer-test-3 amqp)

