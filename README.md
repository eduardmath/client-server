# Client-server
I performed all the actions in windows.

We will need installed:
- Git
- Cmake
- Visual Studio 2015 Update 3 or greater ***with the English language pack***
## Table of Contents
- [Client-server](#Client-server)
- [Installing of vcpkg]()
  - [Installing of gRPC]()
  - [Installing of protobuf]()
- [Generation of source files using proto file]()
- [Creating a project using CMake]()
- [Compiling the code and testing the server and client executables]()
## Installing of vcpkg]
Vcpkg helps you manage C and C++ libraries on Windows, Linux and MacOS.  
I installed `vcpkg` in `C:/dev`:

Go to `C:/dev`
```
> git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat
```
### Installing of gRPC
```
> .\vcpkg\vcpkg install grpc:x64-windows
```
### Installing of protobuf
```
> .\vcpkg\vcpkg install protobuf protobuf:x64-windows
```
Installed packages are located in `<vcpkg_installed_path>\packages`.


In order to make the packages available system-wide, you need to issue the command:
```
> .\vcpkg\vcpkg integrate install
```
## Generation of source files using proto file
```
client-server  
|--proto  
|  |--helloworld.proto  
|--src  
|  |--server.cpp  
|  |--client.cpp  
|--CMakeLists.txt  
```

Now open our `client-server/proto` folder and run the following commands from there:
```
> protoc -I=. --cpp_out=. helloworld.proto
```
or
```
> <vcpkg_installed_path>\packages\protobuf_x64-windows\bin\protoc.exe -I=. --cpp_out=. helloworld.proto
```
This will generate the cpp file required for the message structures.
```
> protoc -I=. --grpc_out=. --plugin=protoc-gen-grpc="<vcpkg_installed_path>\packages\grpc_x64-windows\tools\grpc\grpc_cpp_plugin.exe" helloworld.proto
```
or
```
> <vcpkg_installed_path>\packages\protobuf_x64-windows\bin\protoc.exe -I=. --grpc_out=. --plugin=protoc-gen-grpc="<vcpkg_installed_path>\packages\grpc_x64-windows\tools\grpc\grpc_cpp_plugin.exe" helloworld.proto
```
This command will generate the cpp file required for the service interfaces.
## Creating a project using CMake
Now go to our `project/build` folder and issue the following command to generate a project using CMake:
```
> cmake ../ -DCMAKE_TOOLCHAIN_FILE=<vcpkg_install_path>/scripts/buildsystems/vcpkg.cmake
```
## Compiling the code and testing the server and client executables
Now once the project is created, we can easily compile it from the console from the build directory using the command:
```
> cmake --build .
```
Open the `build/debug` folder and type the following command for launch server:
```
> server.exe
```
Open another console, go to the `build/Debug` folder and run client.exe:
```
> client.exe
```
Now send the binary code to the server and get answer.
