#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <iostream>
#define  N 22
#include <grpcpp/grpcpp.h>
#include "snake.grpc.pb.h"
#include "snake_service.h"

using namespace std;
int gameover;
int xx, yy; // 随机出米
int x, y;
long start;
//=======================================
//类的实现与应用initialize
//=======================================

//下面定义贪吃蛇的坐标类
class snake_position
{
public:

    int x{}, y{};      //x表示行，y表示列

    snake_position() {};

    void initialize(int&);//坐标初始化


};

snake_position position[(N - 2) * (N - 2) + 1]; //定义贪吃蛇坐标类数组，有(N-2)*(N-2)个坐标

void snake_position::initialize(int& j)
{
    x = 1;

    y = j;
}


//下面定义贪吃蛇的棋盘图

class snake_map
{

private:

    char s[N][N]{};//定义贪吃蛇棋盘，包括墙壁。

    int grade{}, length;

    int gamespeed{}; //前进时间间隔

    char direction; // 初始情况下，向右运动

    int head, tail;

    int score;

    bool gameauto{};

public:

    snake_map(int h = 4, int t = 1, int l = 4, char d = 77, int s = 0) :length(l), direction(d), head(h), tail(t), score(s) {}

    void initialize();   //初始化函数

    void show_game();

    int updata_game();

    void setPoint();

    void getgrade();

    void display() const;

    [[nodiscard]] int getScore() const { return score; } //获取分数

    void setDirection(char d) { direction = d; } //设置方向

    [[nodiscard]] const char (*getMap() const)[N] {
        return s;
    }

    void setgrade(int g) { grade = g; } //设置等级

};

//定义初始化函数，将贪吃蛇的棋盘图进行初始化

void snake_map::initialize()
{
    int i, j;

    grade = 1; //设置等级为1

    for (i = 1; i <= 3; i++)

        s[1][i] = '*';

    s[1][4] = '#';

    for (i = 1; i <= N - 2; i++)

        for (j = 1; j <= N - 2; j++)

            s[i][j] = ' '; // 初始化贪吃蛇棋盘中间空白部分

    for (i = 0; i <= N - 1; i++)

        s[0][i] = s[N - 1][i] = '-'; //初始化贪吃蛇棋盘上下墙壁

    for (i = 1; i <= N - 2; i++)

        s[i][0] = s[i][N - 1] = '|'; //初始化贪吃蛇棋盘左右墙壁
}


//============================================
//输出贪吃蛇棋盘信息

void snake_map::show_game()

{

    system("cls"); // 清屏

    int i, j;

    cout << endl;

    for (i = 0; i < N; i++)
    {

        cout << '\t';

        for (j = 0; j < N; j++)

            cout << s[i][j] << ' '; // 输出贪吃蛇棋盘

        if (i == 2) cout << "\t等级：" << grade;

        if (i == 6) cout << "\t速度：" << gamespeed;

        if (i == 10) cout << "\t得分：" << score << "分";

        if (i == 14) cout << "\t暂停：按一下空格键";

        if (i == 18) cout << "\t继续：按两下空格键";

        cout << endl;
    }
}

//输入选择等级
void snake_map::getgrade()
{
    grade = 1;

    while (grade > 7 || grade < 1)
    {
        cout << "请输入数字1-7选择等级，输入其他数字无效" << endl;
        cin >> grade;
    }
    switch (grade)
    {
        case 1: gamespeed = 1000; gameauto = false; break;
        case 2: gamespeed = 800; gameauto = false; break;
        case 3: gamespeed = 600; gameauto = false; break;
        case 4: gamespeed = 400; gameauto = false; break;
        case 5: gamespeed = 200; gameauto = false; break;
        case 6: gamespeed = 100; gameauto = false; break;
        case 7: grade = 1; gamespeed = 1000; gameauto = true; break;
    }
}

//输出等级，得分情况以及称号

void snake_map::display() const
{
    cout << "\n\t\t\t\t等级：" << grade;
    cout << "\n\n\n\t\t\t\t速度：" << gamespeed;
    cout << "\n\n\n\t\t\t\t得分：" << score << "分";
}

//随机产生米
void snake_map::setPoint()
{
    srand(time(0));

    do
    {
        xx = rand() % (N - 2) + 1;
        yy = rand() % (N - 2) + 1;
    } while (s[xx][yy] != ' ');
    s[xx][yy] = '*';
}
char key;
int snake_map::updata_game()
{
    gameover = 1;
    key = direction;
    start = clock();
    while ((gameover = (clock() - start <= gamespeed)) && !_kbhit());
    //如果有键按下或时间超过自动前进时间间隔则终止循环

    if (gameover)
    {
        _getch();
        key = _getch();
    }
    if (key == ' ')
    {
        while (_getch() != ' ') {}
    }
    else
        direction = key;
    switch (direction)
    {
        case 72: x = position[head].x - 1; y = position[head].y; break; // 向上
        case 80: x = position[head].x + 1; y = position[head].y; break; // 向下
        case 75: x = position[head].x; y = position[head].y - 1; break; // 向左
        case 77: x = position[head].x; y = position[head].y + 1; // 向右
    }
    if (!(direction == 72 || direction == 80 || direction == 75 || direction == 77))   // 按键非方向键
        gameover = 0;
    else if (x == 0 || x == N - 1 || y == 0 || y == N - 1)   // 碰到墙壁
        gameover = 0;
    else if (s[x][y] != ' ' && !(x == xx && y == yy))    // 蛇头碰到蛇身
        gameover = 0;
    else if (x == xx && y == yy)
    { // 吃米，长度加1
        length++;
        if (length >= 8 && gameauto)
        {
            length -= 8;
            grade++;
            if (gamespeed >= 200)
                gamespeed -= 200; // 改变贪吃蛇前进速度
            else
                gamespeed = 100;

        }
        s[x][y] = '#';  //更新蛇头
        s[position[head].x][position[head].y] = '*'; //吃米后将原先蛇头变为蛇身
        head = (head + 1) % ((N - 2) * (N - 2));   //取蛇头坐标
        position[head].x = x;
        position[head].y = y;
        show_game();
        gameover = 1;
        score += grade * 20;  //加分
        setPoint();   //产生米
    }

    else
    { // 不吃米
        s[position[tail].x][position[tail].y] = ' ';//将蛇尾置空
        tail = (tail + 1) % ((N - 2) * (N - 2));//更新蛇尾坐标
        s[position[head].x][position[head].y] = '*';  //将蛇头更为蛇身
        head = (head + 1) % ((N - 2) * (N - 2));
        position[head].x = x;
        position[head].y = y;
        s[position[head].x][position[head].y] = '#'; //更新蛇头
        gameover = 1;
    }
    return gameover;
}

std::unique_ptr<snake::SnakeGame::Stub> stub_;

void InitGrpcClient() {
    auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    stub_ = snake::SnakeGame::NewStub(channel);
}

std::string SendStateToServer(const char map[N][N], int score) {
    snake::GameState request;
    for (int i = 0; i < N; ++i) {
        std::string row;
        for (int j = 0; j < N; ++j) {
            row += map[i][j];
        }
        request.add_map(row);
    }
    request.set_score(score);

    snake::GameAction response;
    grpc::ClientContext context;
    grpc::Status status = stub_->SendState(&context, request, &response);

    if (status.ok()) {
        return response.action();
    } else {
        std::cout << "Failed to send state: " << status.error_message() << std::endl;
        return "";
    }
}

//====================================
//主函数部分
//====================================
int main()
{
    InitGrpcClient();
    char ctn = 'y';
    int nodead;
    while (ctn == 'y')
    {
        system("cls"); // 清屏
        snake_map snake;
        snake.initialize();
        snake.getgrade(); //选择等级
        for (int i = 1; i <= 4; i++)
        {
            position[i].initialize(i);//初始化坐标
        }
        snake.setPoint();
        do
        {
            snake.show_game();
            std::string action = SendStateToServer(snake.getMap(), snake.getScore()); // 发送地图状态和当前分数，接收操作指令
            // 根据接收到的操作指令更新方向
            if (action == "UP") snake.setDirection(72);
            else if (action == "DOWN") snake.setDirection(80);
            else if (action == "LEFT") snake.setDirection(75);
            else if (action == "RIGHT") snake.setDirection(77);
            nodead = snake.updata_game();

        } while (nodead);
        system("cls"); //清屏
        cout << "\n\n\n\t\t\t\tGameover！\n\n" << endl;
        snake.display();//输出等级/得分情况
        cout << "\n\n\n\t\t    是否选择继续游戏？输入 y 继续，n 退出" << endl;
        cin >> ctn;
    }
    return 0;
}