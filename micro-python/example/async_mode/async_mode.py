'''
总线伺服舵机
> MicroPython SDK异步指令 Example <
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


# 舵机个数
# 舵机ID编号: [0, 1, 2, ..., srv_num-1]
servo_num = 4
# 舵机ID
servo_id = 0
# 舵机是否有多圈模式的功能
servo_has_mturn_func = False

# 创建串口对象 使用串口2作为控制对象
# 波特率: 115200
# RX: gpio 16
# TX: gpio 17
uart = UART(2, baudrate=115200)
# 创建舵机管理器
uservo = UartServoManager(uart, srv_num=servo_num)

uservo.begin_async()   #  开始异步命令
time.sleep(0.02)
SERVO_ID  = 0
uservo.set_servo_angle( 0, angle = 20.0, interval=0, power=10000) 
time.sleep(2.02)
uservo.end_async(0)    #  是否取消异步命令    0:否； 1:是