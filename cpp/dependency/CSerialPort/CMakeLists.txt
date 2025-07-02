cmake_minimum_required(VERSION 3.0)

# 工程名: CSerialPort
# 指定编程语言为c++
project(CSerialPort LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# 构建共享库
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# 系统信息
message(STATUS "CSerialPort CMake Info")
message(STATUS "=======================================================")
message(STATUS "    Operation System :  ${CMAKE_SYSTEM}")
message(STATUS "    CPU Architecture : ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "    Build Type : ${CMAKE_BUILD_TYPE}${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "    Shared Library  : ${BUILD_SHARED_LIBS}")
message(STATUS "    Build Examples  : ${CSERIALPORT_BUILD_EXAMPLES}")
message(STATUS "    Build Test  : ${CSERIALPORT_BUILD_TEST}")
message(STATUS "=======================================================")

# libcserialport
add_subdirectory(lib)