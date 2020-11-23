set (EXE "blob-inspector-test")

set (blob-inspector-test-sources
        main.cxx
        blob-inspector-test.cxx
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_directories (${BLOB-INSPECTOR_BINARY_DIR}/bin/blob-inspector)
link_directories (${BLOB-INSPECTOR_BINARY_DIR}/libs/corda-custom-types)
include_directories (${BLOB-INSPECTOR_BINARY_DIR}/bin/blob-inspector)

add_executable (${EXE} ${blob-inspector-test-sources})

target_link_libraries (${EXE} gtest amqp blob-inspector-lib corda-custom-types)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if (LINUX)
    target_link_libraries (${EXE} pthread qpid-proton proton)
endif (LINUX)

if (APPLE)
    target_link_libraries (${EXE} pthread qpid-proton)
endif (APPLE)
