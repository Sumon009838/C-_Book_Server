cmake_minimum_required(VERSION 3.10)

# Project name
project(BookServer)

# C++ version
set(CMAKE_CXX_STANDARD 17)
include(/home/admin1/C++/github.com/Sumon009838/Book_Server_C++/common.cmake)
# Set the path to your proto files
set(PROTO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# Proto files
set(PROTO_FILES "${PROTO_SRC_DIR}/book.proto")

# Output files for the generated sources
set(PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/book.pb.cc")
set(PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/book.pb.h")
set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/book.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/book.grpc.pb.h")
message(STATUS "here: ${PROTO_SRC_DIR} ")
# Add the generated files as sources for the project
add_custom_command(
        OUTPUT "${PROTO_SRCS}" "${PROTO_HDRS}" "${GRPC_SRCS}" "${GRPC_HDRS}"
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I ${PROTO_SRC_DIR}    # Specify the directory of the proto file
        --plugin=protoc-gen-grpc="/home/admin1/.local/bin/grpc_cpp_plugin"
        "${PROTO_FILES}"
        DEPENDS "${PROTO_FILES}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        COMMENT "Running gRPC and Protobuf code generation on ${PROTO_FILES}"
)

# Create a static library for the generated files
add_library(book_proto ${PROTO_SRCS} ${GRPC_SRCS} ${PROTO_HDRS} ${GRPC_HDRS} )
target_link_libraries(book_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
# Include directories
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${GRPC_INCLUDE_DIRS})

# Define your executable (main.cpp)
add_executable(server main.cpp)

# Link the necessary libraries
target_link_libraries(server
        book_proto
        gRPC::grpc++
        ${PROTOBUF_LIBRARIES}
        pthread)
