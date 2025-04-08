<h2>贪吃蛇（gRPC跨语言测试）</h2>

<h4>环境</h4>

平台windows11。<br>

使用vcpkg进行包管理，需要安装`grpc_x64-windows`，`protobuf_x64-windows`。<br>

使用gRPC实现c++到python的通信，其中c++实现贪吃蛇，作为服务端，向客户端发送状态并接收和应用动作，python实现客户端，接受当前状态并返回动作，后续可在此基础上实现DRL。
<br>

<h4>编译指令</h4>

``` 
mkdir build && cd build && cmake -G "Visual Studio 17 2022" ../ -DCMAKE_TOOLCHAIN_FILE=D:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake 
```

<h4>运行结果</h4>

服务端结果：
<br>
![server](/photo/server.png)
<br>

用户端结果：
<br>
![client](/photo/client.png)
<br>
