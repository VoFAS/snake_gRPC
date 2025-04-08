import grpc
import snake_pb2
import snake_pb2_grpc
import time

def run():
    # 创建与服务器的连接
    with grpc.insecure_channel('localhost:50051') as channel:
        i = 10
        while i > 0:
            action_request = snake_pb2.GameAction(action="DOWN")

            # 创建动作请求（可以根据需要发送特定动作如"up"、"down"、"left"、"right"）

            # 调用SendState方法获取当前游戏状态
            stub = snake_pb2_grpc.SnakeGameStub(channel)
            response = stub.SendState(action_request)

            # 输出地图信息
            print("贪吃蛇地图:")
            for row in response.map:
                print(row)

            # 输出分数和游戏状态
            print(f"当前分数: {response.score}")
            print(f"游戏是否结束: {response.done}")

            time.sleep(0.5)  # 500毫秒的延迟
            i -= 1


if __name__ == '__main__':
    run()