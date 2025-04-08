#include "snake_service.h"
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Starting Snake Game gRPC Server..." << std::endl;
    RunServer();
    return 0;
}