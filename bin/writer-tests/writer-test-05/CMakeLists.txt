include_directories (${BLOB-INSPECTOR_SOURCE_DIR}/libs/amqp)

link_directories (${BLOB-INSPECTOR_BINARY_DIR}/libs/amqp)

set (writer-test-5-srcs
        main.cxx SerialiseMe.cxx SerialiseMe.h)


add_executable (writer-test-5 ${writer-test-5-srcs})

target_link_libraries (writer-test-5 amqp)

