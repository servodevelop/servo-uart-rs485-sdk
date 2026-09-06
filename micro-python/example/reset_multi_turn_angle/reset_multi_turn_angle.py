'''
总线伺服舵机
> MicroPython SDK圈数重置指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/20
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
servo_id = 0
# 舵机是否有多圈模式的功能
#servo_has_mturn_func = False

# 创建串口对象 使用串口2作为控制对象
# 波特率: 115200
# RX: gpio 16
# TX: gpio 17
uart = UART(2, baudrate=115200)
# 创建舵机管理器
uservo = UartServoManager(uart, srv_num=servo_num)

angle = uservo.query_servo_angle(servo_id)

print("当前舵机角度: {:4.1f} °".format(angle), end='\n')

uservo.disable_torque(servo_id)

uservo.reset_multi_turn_angle(servo_id)

time.sleep(1)

angle = uservo.query_servo_angle(servo_id)

print("圈数重置后舵机角度: {:4.1f}°".format(angle), end='\n')