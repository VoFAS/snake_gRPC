#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <memory>
#include <grpcpp/grpcpp.h>
#include "proto/snake.grpc.pb.h"

class SnakeGameServiceImpl final : public snake::SnakeGame::Service {
public:
    SnakeGameServiceImpl();

    grpc::Status SendState(grpc::ServerContext* context,
                           const snake::GameAction* request,
                           snake::GameState* response) override;

private:
    std::vector<std::string> map;
    std::vector<std::pair<int, int>> snake;
    std::pair<int, int> food;
    std::string direction;
    int score;
    bool game_over;
    const int width = 20;
    const int height = 20;

    void initGame();
    void updateGame(const std::string& action);
    void generateFood();
    bool checkCollision();
    void updateMap();
};

void RunServer();