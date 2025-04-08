//
// Created by VoFAS on 25-4-8.
//

#ifndef UNTITLED_SNAKE_SERVICE_H
#define UNTITLED_SNAKE_SERVICE_H

#endif //UNTITLED_SNAKE_SERVICE_H

#include <grpcpp/grpcpp.h>
#include "proto/snake.grpc.pb.h"
#include "proto/snake.pb.h"

class SnakeGameServiceImpl final : public snake::SnakeGame::Service {
public:
    grpc::Status SendState(grpc::ServerContext* context, const snake::GameState* request, snake::GameAction* response) override {
        // 处理地图状态和当前分数，返回操作指令
        response->set_action("UP"); // 示例操作指令
        return grpc::Status::OK;
    }
};
