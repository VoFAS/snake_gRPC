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
    // ��ʼ����ͼ
    map.resize(height, std::string(width, '.'));

    // ��ʼ����
    snake.clear();
    snake.push_back({width / 2, height / 2});

    // ��ʼ������
    direction = "RIGHT";

    // ��ʼ������
    score = 0;

    // ��Ϸ״̬
    game_over = false;

    // ����ʳ��
    generateFood();

    // ���µ�ͼ
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

        // ������ɵ�ʳ���Ƿ��������ص�
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
    // ��ȡ��ͷλ��
    int x = snake[0].first;
    int y = snake[0].second;

    // ����Ƿ�ײǽ
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return true;
    }

    // ����Ƿ�ײ���Լ�
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i].first == x && snake[i].second == y) {
            return true;
        }
    }

    return false;
}

void SnakeGameServiceImpl::updateMap() {
    // ��յ�ͼ
    for (auto& row : map) {
        std::fill(row.begin(), row.end(), '.');
    }

    // ������
    for (size_t i = 0; i < snake.size(); ++i) {
        int x = snake[i].first;
        int y = snake[i].second;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            map[y][x] = (i == 0) ? 'H' : 'S';
        }
    }

    // ����ʳ��
    map[food.second][food.first] = 'F';
}

void SnakeGameServiceImpl::updateGame(const std::string& action) {
    if (game_over) return;

    // ���ݶ������·���
    if (action == "UP" && direction != "DOWN") {
        direction = "UP";
    } else if (action == "DOWN" && direction != "UP") {
        direction = "DOWN";
    } else if (action == "LEFT" && direction != "RIGHT") {
        direction = "LEFT";
    } else if (action == "RIGHT" && direction != "LEFT") {
        direction = "RIGHT";
    }

    // ���ݷ����ƶ���ͷ
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

    // ������ͷλ��
    snake.insert(snake.begin(), {headX, headY});

    // ����Ƿ�Ե�ʳ��
    if (headX == food.first && headY == food.second) {
        score++;
        generateFood();
    } else {
        // ���û�гԵ�ʳ�ɾ��β��
        snake.pop_back();
    }

    // �����ײ
    game_over = checkCollision();

    // ���µ�ͼ
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
        // ��¼������������Ҫ��������Ϣ
        int final_score = response->score();
        std::cout << "game_end, final_score: " << final_score << std::endl;

        // ������Ϸ״̬����ʼ����Ϸ
        initGame(); // ���ó�ʼ����Ϸ�ķ���

        // ����ʼ�������Ϸ״̬��䵽��Ӧ��
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