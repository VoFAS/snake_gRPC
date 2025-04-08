//
// Created by VoFAS on 25-4-8.
//
#include "snake_service.h"
#include "proto/snake.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using snake::GameState;
using snake::GameAction;
using snake::SnakeGame;


void RunServer() {
    std::string server_address("0.0.0.0:50051");
    SnakeGameServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(reinterpret_cast<grpc::Service *>(&service));
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}