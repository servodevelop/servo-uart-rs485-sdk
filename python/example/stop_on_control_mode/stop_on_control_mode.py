'''
总线伺服舵机
> Python SDK停止指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/23
--------------------------------------------------
'''
# 添加uservo.py的系统路径
import sys
sys.path.append("../../src")
# 导入依赖
import time
import struct
import serial
from uservo import UartServoManager

# 参数配置
# 角度定义
SERVO_PORT_NAME =  'COM3'		# 舵机串口号 请根据实际串口进行修改
SERVO_BAUDRATE = 115200			# 舵机的波特率 请根据实际波特率进行修改


# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
					 parity=serial.PARITY_NONE, stopbits=1,\
					 bytesize=8,timeout=0)
# 初始化舵机管理器
uservo = UartServoManager(uart, is_debug=True)

uservo.stop_on_control_mode(servo_id=2, method=0x10, power=500)
uservo.stop_on_control_mode(servo_id=2, method=0x11, power=500)
uservo.stop_on_control_mode(servo_id=2, method=0x12, power=500)