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
        // �����ͼ״̬�͵�ǰ���������ز���ָ��
        response->set_action("UP"); // ʾ������ָ��
        return grpc::Status::OK;
    }
};
