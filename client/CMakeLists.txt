cmake_minimum_required(VERSION 3.0.0)
project(myproj VERSION 0.1.0)

# Find Protobuf installation
  # Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
find_program(_PROTOBUF_PROTOC protoc)
else()
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()


# Proto file
get_filename_component(service_proto "../rpc/echo/service.proto" ABSOLUTE)
get_filename_component(service_proto_path "${service_proto}" PATH)

# Generated sources
set(service_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/service.pb.cc")
set(service_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/service.pb.h")
set(service_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.cc")
set(service_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/service.grpc.pb.h")
add_custom_command(
      OUTPUT "${service_proto_srcs}" "${service_proto_hdrs}" "${service_grpc_srcs}" "${service_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${service_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${service_proto}"
      DEPENDS "${service_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# service_grpc_proto
add_library(service_grpc_proto
  ${service_grpc_srcs}
  ${service_grpc_hdrs}
  ${service_proto_srcs}
  ${service_proto_hdrs})
target_link_libraries(service_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})


# Targets service_client
foreach(_target
  service_client)
  add_executable(${_target}
    "${_target}.cc")
  target_link_libraries(${_target}
    service_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()
