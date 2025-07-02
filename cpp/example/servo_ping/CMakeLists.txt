cmake_minimum_required(VERSION 3.0)
# 设置工程名称, 设定版本
project(SERVO_PING VERSION 1.0)

# 动态链接库的安装路径
if(CMAKE_HOST_WIN32)
    # Windows
    # 注意事项：这里需要改成自己的Windows下的动态链接库的安装路径
    # 这里只是示例
    SET(CMAKE_INSTALL_PREFIX "D:/KyleSoftware/CPP_LIBRARIES/")
elseif(CMAKE_HOST_UNIX)
    # Linux
    # 注意事项: Linux下，CMake默认会将库安装到/usr/local, 需要注意的是，安装的时候需要sudo权限
    # sudo make install
    SET(CMAKE_INSTALL_PREFIX "/usr/local/")
endif()

# 添加依赖的.h文件路径(即hello.h所在的文件夹)
include_directories(${CMAKE_INSTALL_PREFIX}/include)
# # 添加link文件夹
link_directories(${CMAKE_INSTALL_PREFIX}/bin)

# 添加可执行程序hello_demo
add_executable(servo_ping servo_ping.cpp)
# 添加动态链接库的路径
target_link_libraries(servo_ping  fsuartservo cserialport)
