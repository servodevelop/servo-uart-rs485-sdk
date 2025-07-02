'''
总线伺服舵机
> Python SDK设置舵机原点指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/20
--------------------------------------------------
'''
# 添加uservo.py的系统路径
import os
import sys
sys.path.append(os.getcwd()+"\\src")
# sys.path.append("../../src")
# 导入依赖
import time
import serial
from uservo import UartServoManager

# 参数配置
# 角度定义
SERVO_PORT_NAME =  'COM3' # 舵机串口号
SERVO_BAUDRATE = 115200 # 舵机的波特率
SERVO_ID = 0  # 舵机的ID号

# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
					 parity=serial.PARITY_NONE, stopbits=1,\
					 bytesize=8,timeout=0)
# 初始化舵机管理器
uservo = UartServoManager(uart)

angle = uservo.query_servo_angle(SERVO_ID)

print("当前舵机角度: {:4.1f} °".format(angle), end='\n')

uservo.disable_torque(SERVO_ID)

uservo.set_origin_point(SERVO_ID)

time.sleep(1)

angle = uservo.query_servo_angle(SERVO_ID)

print("设置新的原点后舵机角度: {:4.1f} °".format(angle), end='\n')

