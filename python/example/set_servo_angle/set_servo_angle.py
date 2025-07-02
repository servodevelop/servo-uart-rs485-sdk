'''
总线伺服舵机
> Python SDK角度设置 Example <
注意：以下包括单圈角度和多圈角度指令
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
SERVO_PORT_NAME =  'COM4'		# 舵机串口号
SERVO_BAUDRATE = 115200			# 舵机的波特率
SERVO_ID = 0					# 舵机的ID号
SERVO_HAS_MTURN_FUNC = False	# 舵机是否拥有多圈模式

# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
					 parity=serial.PARITY_NONE, stopbits=1,\
					 bytesize=8,timeout=0)
# 初始化舵机管理器
uservo = UartServoManager(uart, is_debug=True)

print("[单圈模式]设置舵机角度为90.0°")
uservo.set_servo_angle(SERVO_ID, 90.0, interval=0) # 设置舵机角度 极速模式
uservo.wait() # 等待舵机静止
print("-> {}".format(uservo.query_servo_angle(SERVO_ID)))

print("[单圈模式]设置舵机角度为-80.0°, 周期1000ms")
uservo.set_servo_angle(SERVO_ID, -80.0, interval=1000) # 设置舵机角度(指定周期 单位ms)
uservo.wait() # 等待舵机静止
print("-> {}".format(uservo.query_servo_angle(SERVO_ID)))

print("[单圈模式]设置舵机角度为70.0°, 设置转速为200 °/s, 加速时间100ms, 减速时间100ms")
uservo.set_servo_angle(SERVO_ID, 70.0, velocity=200.0, t_acc=100, t_dec=100) # 设置舵机角度(指定转速 单位°/s)
uservo.wait() # 等待舵机静止
print("-> {}".format(uservo.query_servo_angle(SERVO_ID)))


print("[单圈模式]设置舵机角度为-90.0°, 添加功率限制")
uservo.set_servo_angle(SERVO_ID, -90.0, power=400) # 设置舵机角度(指定功率 单位mW)
uservo.wait() # 等待舵机静止

#########################################################################################
if SERVO_HAS_MTURN_FUNC:
	print("[多圈模式]设置舵机角度为900.0°, 周期1000ms")
	uservo.set_servo_angle(SERVO_ID, 900.0, interval=1000, is_mturn=True) # 设置舵机角度(指定周期 单位ms)
	uservo.wait() # 等待舵机静止
	print("-> {}".format(uservo.query_servo_angle(SERVO_ID)))

	print("[多圈模式]设置舵机角度为-900.0°, 设置转速为200 °/s")
	uservo.set_servo_angle(SERVO_ID, -900.0, velocity=200.0, t_acc=100, t_dec=100, is_mturn=True) # 设置舵机角度(指定转速 单位°/s) dps: degree per second
	uservo.wait() # 等待舵机静止
	print("-> {}".format(uservo.query_servo_angle(SERVO_ID)))

	print("[多圈模式]设置舵机角度为-850.0°, 添加功率限制")
	uservo.set_servo_angle(SERVO_ID, -850.0, power=400, is_mturn=True) # 设置舵机角度(指定功率 单位mW)
	uservo.wait() # 等待舵机静止
	print("-> {}".format(uservo.query_servo_angle(SERVO_ID)))
