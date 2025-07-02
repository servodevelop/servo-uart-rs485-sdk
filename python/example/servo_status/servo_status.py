'''
总线伺服舵机
> Python SDK读取状态指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/23
--------------------------------------------------
'''
# 添加uservo.py的系统路径
import sys
import os
sys.path.append("../../src")
# sys.path.append(os.getcwd()+"\\src")

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

def log_servo_status():
    '''打印舵机状态'''
    # 读取温度
    voltage = uservo.query_voltage(SERVO_ID)
    # 读取电流
    current = uservo.query_current(SERVO_ID)
    # 读取功率
    power = uservo.query_power(SERVO_ID)
    # 读取温度
    temp = uservo.query_temperature(SERVO_ID)
    #  舵机工作状态标志位
	#  BIT[0] - 执行指令置1，执行完成后清零。
	#  BIT[1] - 执行指令错误置1，在下次正确执行后清零。
	#  BIT[2] - 堵转错误置1，解除堵转后清零。
	#  BIT[3] - 电压过高置1，电压恢复正常后清零。
	#  BIT[4] - 电压过低置1，电压恢复正常后清零。
	#  BIT[5] - 电流错误置1，电流恢复正常后清零。
	#  BIT[6] - 功率错误置1，功率恢复正常后清零。
	#  BIT[7] - 温度错误置1，温度恢复正常后清零。
    status = uservo.query_status(SERVO_ID)
    print("Voltage: {:4.1f}V; Current: {:4.1f}A; Power: {:4.1f}W; T: {:2.0f}; Status: {:08b}".format(voltage, current, power, temp,status), end='\r')

while True:
    uservo.set_servo_angle(SERVO_ID, 90)
    while not uservo.is_stop():
        log_servo_status()
        time.sleep(0.1)
    
    time.sleep(1)

    uservo.set_servo_angle(SERVO_ID, -90)
    while not uservo.is_stop():
        log_servo_status()
        time.sleep(0.1)

    time.sleep(1)