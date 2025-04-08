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
int xx, yy; // �������
int x, y;
long start;
//=======================================
//���ʵ����Ӧ��initialize
//=======================================

//���涨��̰���ߵ�������
class snake_position
{
public:

    int x{}, y{};      //x��ʾ�У�y��ʾ��

    snake_position() {};

    void initialize(int&);//�����ʼ��


};

snake_position position[(N - 2) * (N - 2) + 1]; //����̰�������������飬��(N-2)*(N-2)������

void snake_position::initialize(int& j)
{
    x = 1;

    y = j;
}


//���涨��̰���ߵ�����ͼ

class snake_map
{

private:

    char s[N][N]{};//����̰�������̣�����ǽ�ڡ�

    int grade{}, length;

    int gamespeed{}; //ǰ��ʱ����

    char direction; // ��ʼ����£������˶�

    int head, tail;

    int score;

    bool gameauto{};

public:

    snake_map(int h = 4, int t = 1, int l = 4, char d = 77, int s = 0) :length(l), direction(d), head(h), tail(t), score(s) {}

    void initialize();   //��ʼ������

    void show_game();

    int updata_game();

    void setPoint();

    void getgrade();

    void display() const;

    [[nodiscard]] int getScore() const { return score; } //��ȡ����

    void setDirection(char d) { direction = d; } //���÷���

    [[nodiscard]] const char (*getMap() const)[N] {
        return s;
    }

    void setgrade(int g) { grade = g; } //���õȼ�

};

//�����ʼ����������̰���ߵ�����ͼ���г�ʼ��

void snake_map::initialize()
{
    int i, j;

    grade = 1; //���õȼ�Ϊ1

    for (i = 1; i <= 3; i++)

        s[1][i] = '*';

    s[1][4] = '#';

    for (i = 1; i <= N - 2; i++)

        for (j = 1; j <= N - 2; j++)

            s[i][j] = ' '; // ��ʼ��̰���������м�հײ���

    for (i = 0; i <= N - 1; i++)

        s[0][i] = s[N - 1][i] = '-'; //��ʼ��̰������������ǽ��

    for (i = 1; i <= N - 2; i++)

        s[i][0] = s[i][N - 1] = '|'; //��ʼ��̰������������ǽ��
}


//============================================
//���̰����������Ϣ

void snake_map::show_game()

{

    system("cls"); // ����

    int i, j;

    cout << endl;

    for (i = 0; i < N; i++)
    {

        cout << '\t';

        for (j = 0; j < N; j++)

            cout << s[i][j] << ' '; // ���̰��������

        if (i == 2) cout << "\t�ȼ���" << grade;

        if (i == 6) cout << "\t�ٶȣ�" << gamespeed;

        if (i == 10) cout << "\t�÷֣�" << score << "��";

        if (i == 14) cout << "\t��ͣ����һ�¿ո��";

        if (i == 18) cout << "\t�����������¿ո��";

        cout << endl;
    }
}

//����ѡ��ȼ�
void snake_map::getgrade()
{
    grade = 1;

    while (grade > 7 || grade < 1)
    {
        cout << "����������1-7ѡ��ȼ�����������������Ч" << endl;
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

//����ȼ����÷�����Լ��ƺ�

void snake_map::display() const
{
    cout << "\n\t\t\t\t�ȼ���" << grade;
    cout << "\n\n\n\t\t\t\t�ٶȣ�" << gamespeed;
    cout << "\n\n\n\t\t\t\t�÷֣�" << score << "��";
}

//���������
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
    //����м����»�ʱ�䳬���Զ�ǰ��ʱ��������ֹѭ��

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
        case 72: x = position[head].x - 1; y = position[head].y; break; // ����
        case 80: x = position[head].x + 1; y = position[head].y; break; // ����
        case 75: x = position[head].x; y = position[head].y - 1; break; // ����
        case 77: x = position[head].x; y = position[head].y + 1; // ����
    }
    if (!(direction == 72 || direction == 80 || direction == 75 || direction == 77))   // �����Ƿ����
        gameover = 0;
    else if (x == 0 || x == N - 1 || y == 0 || y == N - 1)   // ����ǽ��
        gameover = 0;
    else if (s[x][y] != ' ' && !(x == xx && y == yy))    // ��ͷ��������
        gameover = 0;
    else if (x == xx && y == yy)
    { // ���ף����ȼ�1
        length++;
        if (length >= 8 && gameauto)
        {
            length -= 8;
            grade++;
            if (gamespeed >= 200)
                gamespeed -= 200; // �ı�̰����ǰ���ٶ�
            else
                gamespeed = 100;

        }
        s[x][y] = '#';  //������ͷ
        s[position[head].x][position[head].y] = '*'; //���׺�ԭ����ͷ��Ϊ����
        head = (head + 1) % ((N - 2) * (N - 2));   //ȡ��ͷ����
        position[head].x = x;
        position[head].y = y;
        show_game();
        gameover = 1;
        score += grade * 20;  //�ӷ�
        setPoint();   //������
    }

    else
    { // ������
        s[position[tail].x][position[tail].y] = ' ';//����β�ÿ�
        tail = (tail + 1) % ((N - 2) * (N - 2));//������β����
        s[position[head].x][position[head].y] = '*';  //����ͷ��Ϊ����
        head = (head + 1) % ((N - 2) * (N - 2));
        position[head].x = x;
        position[head].y = y;
        s[position[head].x][position[head].y] = '#'; //������ͷ
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
//����������
//====================================
int main()
{
    InitGrpcClient();
    char ctn = 'y';
    int nodead;
    while (ctn == 'y')
    {
        system("cls"); // ����
        snake_map snake;
        snake.initialize();
        snake.getgrade(); //ѡ��ȼ�
        for (int i = 1; i <= 4; i++)
        {
            position[i].initialize(i);//��ʼ������
        }
        snake.setPoint();
        do
        {
            snake.show_game();
            std::string action = SendStateToServer(snake.getMap(), snake.getScore()); // ���͵�ͼ״̬�͵�ǰ���������ղ���ָ��
            // ���ݽ��յ��Ĳ���ָ����·���
            if (action == "UP") snake.setDirection(72);
            else if (action == "DOWN") snake.setDirection(80);
            else if (action == "LEFT") snake.setDirection(75);
            else if (action == "RIGHT") snake.setDirection(77);
            nodead = snake.updata_game();

        } while (nodead);
        system("cls"); //����
        cout << "\n\n\n\t\t\t\tGameover��\n\n" << endl;
        snake.display();//����ȼ�/�÷����
        cout << "\n\n\n\t\t    �Ƿ�ѡ�������Ϸ������ y ������n �˳�" << endl;
        cin >> ctn;
    }
    return 0;
}