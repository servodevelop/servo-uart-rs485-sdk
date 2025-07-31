'''
总线伺服舵机
> Python SDK同步指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2025/07/24
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo
import struct

SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200 

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)


command_data_list1 = [
    struct.pack('<BhHH', 1, -400, 500, 10000),  # 同步命令角度模式控制
    struct.pack('<BhHH', 2, -400 ,500, 10000),  #id2+度数-40 +时间 +功率
]
control.send_sync_angle(8, 2, command_data_list1)
time.sleep(2.02)

command_data_list2 = [
    struct.pack('<BhHHHH', 1, 400, 500, 100, 100, 10000) , # 同步命令角度模式控制(基於加减速時間)
    struct.pack('<BhHHHH', 2, 400, 500, 100, 100, 10000) , #id2+度数40+总时间+启动加速时间+运动减速时间+功率
]
control.send_sync_anglebyinterval(11, 2, command_data_list2)
time.sleep(2.02)


command_data_list4 = [
    struct.pack('<BhHHHH', 1, 600, 500, 100, 100, 10000), # 同步命令角度模式控制 (基於速率的運動控制 )
    struct.pack('<BhHHHH', 2, 600, 500, 100, 100, 10000), # id2+度数60+时间+功率
]
control.send_sync_anglebyvelocity(12, 2, command_data_list4)
time.sleep(2.02)


command_data_list3 = [
    struct.pack('<BlLH', 1, 800, 1500, 10000)  ,# 同步命令多圈角度模式控制
    struct.pack('<BlLH', 2, 800 ,1500, 10000)  ,# id2+度数80 +时间 +功率
]
control.send_sync_multiturnangle(13, 2, command_data_list3)
time.sleep(2.02)



command_data_list5 = [
    struct.pack('<BlLHHH', 1, 10010, 1000, 100,100,10000)  ,# 多圈角度模式控制 (基於加減速時段的運動控制 )
    struct.pack('<BlLHHH', 2, 10010 ,1000,100,100, 10000)  ,# id2+度数1001+总时间+启动加速时间+运动减速时间+功率
]
control.send_sync_multiturnanglebyinterval(14, 2, command_data_list5)
time.sleep(2.02)



command_data_list6 = [
    struct.pack('<BlHHHH', 1, 12010, 3000,100 ,100,10000)  ,# 多圈角度模式控制(基於速率的運動控制)
    struct.pack('<BlHHHH', 2, 12010 ,3000, 100, 100, 10000)  ,# id2+度数1201+目标速度300dps+启动加速时间+运动减速时间+功率
]
control.send_sync_multiturnanglebyvelocity(15, 2, command_data_list6)