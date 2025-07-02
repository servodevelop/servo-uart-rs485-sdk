# 设定动态链接库的存储路径
# 生成在build/lib文件夹下 （非必须)
set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

# 库的名称
set(LIB_NAME fsuartservo)
# 库的源码
set(LIB_SRC ./FashionStar_UartServoProtocol.cpp ./FashionStar_UartServo.cpp)

add_library(${LIB_NAME} SHARED ${LIB_SRC})
target_link_libraries(${LIB_NAME} cserialport)

# 安装动态链接库
install(
	TARGETS  ${LIB_NAME}
	# 安装LIBRARY 动态链接库
	LIBRARY DESTINATION lib
)

# 将项目源码里面.h文件 安装到include里面
INSTALL(FILES ../include/FashionStar_UartServoProtocol.h DESTINATION include/FashionStar/UServo)
INSTALL(FILES ../include/FashionStar_UartServo.h DESTINATION include/FashionStar/UServo)
