#include "snake_service.h"
#include <grpcpp/grpcpp.h>
#include <random>
#include <algorithm>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using snake::GameState;
using snake::GameAction;
using snake::SnakeGame;

SnakeGameServiceImpl::SnakeGameServiceImpl() {
    initGame();
}

void SnakeGameServiceImpl::initGame() {
    // 初始化地图
    map.resize(height, std::string(width, '.'));

    // 初始化蛇
    snake.clear();
    snake.push_back({width / 2, height / 2});

    // 初始化方向
    direction = "RIGHT";

    // 初始化分数
    score = 0;

    // 游戏状态
    game_over = false;

    // 生成食物
    generateFood();

    // 更新地图
    updateMap();
}

void SnakeGameServiceImpl::generateFood() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);

    int x, y;
    bool valid = false;

    while (!valid) {
        x = disX(gen);
        y = disY(gen);
        valid = true;

        // 检查生成的食物是否与蛇身重叠
        for (const auto& segment : snake) {
            if (segment.first == x && segment.second == y) {
                valid = false;
                break;
            }
        }
    }

    food = {x, y};
}

bool SnakeGameServiceImpl::checkCollision() {
    // 获取蛇头位置
    int x = snake[0].first;
    int y = snake[0].second;

    // 检查是否撞墙
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return true;
    }

    // 检查是否撞到自己
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].first == x && snake[i].second == y) {
            return true;
        }
    }

    return false;
}

void SnakeGameServiceImpl::updateMap() {
    // 清空地图
    for (auto& row : map) {
        std::fill(row.begin(), row.end(), '.');
    }

    // 绘制蛇
    for (size_t i = 0; i < snake.size(); ++i) {
        int x = snake[i].first;
        int y = snake[i].second;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            map[y][x] = (i == 0) ? 'H' : 'S';
        }
    }

    // 绘制食物
    map[food.second][food.first] = 'F';
}

void SnakeGameServiceImpl::updateGame(const std::string& action) {
    if (game_over) return;

    // 根据动作更新方向
    if (action == "UP" && direction != "DOWN") {
        direction = "UP";
    } else if (action == "DOWN" && direction != "UP") {
        direction = "DOWN";
    } else if (action == "LEFT" && direction != "RIGHT") {
        direction = "LEFT";
    } else if (action == "RIGHT" && direction != "LEFT") {
        direction = "RIGHT";
    }

    // 根据方向移动蛇头
    int headX = snake[0].first;
    int headY = snake[0].second;

    if (direction == "UP") {
        headY--;
    } else if (direction == "DOWN") {
        headY++;
    } else if (direction == "LEFT") {
        headX--;
    } else if (direction == "RIGHT") {
        headX++;
    }

    // 更新蛇头位置
    snake.insert(snake.begin(), {headX, headY});

    // 检查是否吃到食物
    if (headX == food.first && headY == food.second) {
        score++;
        generateFood();
    } else {
        // 如果没有吃到食物，删除尾部
        snake.pop_back();
    }

    // 检查碰撞
    game_over = checkCollision();

    // 更新地图
    updateMap();
}

grpc::Status SnakeGameServiceImpl::SendState(grpc::ServerContext* context,
                                             const snake::GameAction* request,
                                             snake::GameState* response) {
    updateGame(request->action());

    for (const auto& row : map) {
        response->add_map(row);
    }
    response->set_score(score);
    response->set_done(game_over);

    const std::string& action = request->action();
    std::cout << "action: " << action << std::endl;
    updateGame(action);

    if (response->done()) {
        // 记录分数或其他需要保留的信息
        int final_score = response->score();
        std::cout << "game_end, final_score: " << final_score << std::endl;

        // 重置游戏状态，开始新游戏
        initGame(); // 调用初始化游戏的方法

        // 将初始化后的游戏状态填充到响应中
        response->clear_map();
        for (const auto& row : map) {
            response->add_map(row);
        }
        response->set_score(0);
        response->set_done(false);

        std::cout << "game_start" << std::endl;
    }

    return Status::OK;
}

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    SnakeGameServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}