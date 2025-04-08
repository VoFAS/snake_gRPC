cmake_minimum_required(VERSION 3.28)
project(snake)

set(CMAKE_CXX_STANDARD 17)

# Integrate vcpkg
set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Include directories for absl and other dependencies
include_directories("D:/vcpkg/vcpkg/installed/x64-windows/include")

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

add_executable(snake main.cpp
        snake_service.h
        snake_service.cpp
)
target_link_libraries(snake PRIVATE gRPC::grpc++ gRPC::grpc++_reflection gRPC::gpr gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)