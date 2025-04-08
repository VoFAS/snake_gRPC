<h2>贪吃蛇（gRPC跨语言测试）</h2>

平台windows11<br>

使用vcpkg进行包管理，需要安装`grpc_x64-windows`，`protobuf_x64-windows`<br>

编译指令:

``` 
mkdir build && cd build && cmake -G "Visual Studio 17 2022" ../ -DCMAKE_TOOLCHAIN_FILE=D:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake 
```
