cmake_minimum_required(VERSION 3.5)

# 设置库的名字
set(LIB_NAME libcserialport)

# 苹果MAC专属
if(APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(FOUNDATION_LIBRARY Foundation)
endif()


# static libarary
# add_compile_options(-static)

# 导入依赖
include_directories(../include)

# 设置源码
set(COMMON_SOURCES ../src/SerialPort.cpp ../src/SerialPortBase.cpp ../src/SerialPortInfo.cpp ../src/SerialPortInfoBase.cpp)

# OS_ABOUT_SOURCES：与操作系统相关的源码
if (CMAKE_HOST_WIN32)
	# Windows相关源码
	set(OS_ABOUT_SOURCES ../src/SerialPortInfoWinBase.cpp ../src/SerialPortWinBase.cpp)
	if (BUILD_SHARED_LIBS)
	    list(APPEND OS_ABOUT_SOURCES version.rc)
	endif()
elseif (CMAKE_HOST_UNIX)
	# Linux下的源码
	set(OS_ABOUT_SOURCES ../src/SerialPortInfoUnixBase.cpp ../src/SerialPortUnixBase.cpp)
endif ()

# 添加动态链接库
add_library( ${LIB_NAME} ${COMMON_SOURCES} ${OS_ABOUT_SOURCES})

# remove prefix
set_target_properties( ${LIB_NAME} PROPERTIES PREFIX "")

# 链接静态库文件
if (CMAKE_HOST_WIN32)
	# for function availableFriendlyPorts
	target_link_libraries( ${LIB_NAME} setupapi )
	#target_link_libraries( ${LIB_NAME} libgcc.a)
	#target_link_libraries( ${LIB_NAME} libstdc++.a)
	#target_link_libraries( ${LIB_NAME} libpthread.a)
elseif(APPLE)
    target_link_libraries( ${LIB_NAME} ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY})
elseif (CMAKE_HOST_UNIX)
	target_link_libraries( ${LIB_NAME} pthread )
endif()

# 将项目源码里面.h文件 安装到include里面
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/CSerialPort DESTINATION include FILES_MATCHING PATTERN "*.h")

# 安装
install(TARGETS libcserialport
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib # 动态链接库，安装到lib
		ARCHIVE DESTINATION lib
		)

install(FILES ${PROJECT_SOURCE_DIR}/cserialport-config.cmake.in DESTINATION lib/cmake/CSerialPort RENAME cserialport-config.cmake)