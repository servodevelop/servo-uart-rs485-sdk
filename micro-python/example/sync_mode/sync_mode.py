'''
总线伺服舵机
> MicroPython SDK同步命令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/23
--------------------------------------------------
'''
import ustruct
from machine import UART
from uservo import UartServoManager
import time


# 舵机ID编号: [0, 1, 2, ..., srv_num-1]
# 扫描舵机个数
servo_num = 4
# 舵机ID
#servo_id = 0
# 舵机是否有多圈模式的功能
#servo_has_mturn_func = False

# 创建串口对象 使用串口2作为控制对象
# 波特率: 115200
# RX: gpio 16
# TX: gpio 17
uart = UART(2, baudrate=115200)
# 创建舵机管理器
uservo = UartServoManager(uart, srv_num=servo_num)

command_data_list1 = [
    ustruct.pack('<BhHH', 1, -400, 800, 10000),  # 同步命令角度模式控制
    ustruct.pack('<BhHH', 0, -400, 800, 10000),  #id0+度数-40 +时间 +功率
]
uservo.send_sync_angle(8, 2, command_data_list1)
time.sleep(2.02)


command_data_list2 = [
    ustruct.pack('<BhHHHH', 1, 0, 500, 100, 100, 10000), # 同步命令角度模式控制(基于加减速时间)
    ustruct.pack('<BhHHHH', 0, 0, 500, 100, 100, 10000), #id0+度数0+总时间+启动加速时间+运动减速时间+功率
]
uservo.send_sync_anglebyinterval(11, 2, command_data_list2)
time.sleep(2.02)


command_data_list4 = [
    ustruct.pack('<BhHHHH', 1, 400, 500, 100, 100, 10000),# 同步命令角度模式控制 (基于目标速度 )
    ustruct.pack('<BhHHHH', 0, 400, 500, 100, 100, 10000),# id0+度数40+时间+功率
]
uservo.send_sync_anglebyvelocity(12, 2, command_data_list4)
time.sleep(2.02)


command_data_list3 = [
    ustruct.pack('<BlLH', 1, 800, 1000, 10000),# 同步命令多圈角度模式控制
    ustruct.pack('<BlLH', 0, 800, 1000, 10000),# id0+度数80 +时间 +功率
]
uservo.send_sync_multiturnangle(13, 2, command_data_list3)
time.sleep(2.02)


command_data_list5 = [
    ustruct.pack('<BlLHHH', 1, 1200, 500, 100, 100, 10000),# 多圈角度模式控制 (基于加减速时间 )
    ustruct.pack('<BlLHHH', 0, 1200, 500, 100, 100, 10000),# id0+度数120+时间+启动加速时间+运动减速时间+功率
]
uservo.send_sync_multiturnanglebyinterval(14, 2, command_data_list5)
time.sleep(2.02)


command_data_list6 = [
    ustruct.pack('<BlHHHH', 1, 1600, 3000, 100, 100, 10000)  ,# 多圈角度模式控制(基于目标速度)
    ustruct.pack('<BlHHHH', 0, 1600 ,3000, 100, 100, 10000)  ,# id0+度数160+目标速度300dps+启动加速时间+运动减速时间+功率
]
uservo.send_sync_multiturnanglebyvelocity(15, 2, command_data_list6)