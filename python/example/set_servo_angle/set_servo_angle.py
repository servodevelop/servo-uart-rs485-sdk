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
import time
import serial
import fashionstar_uart_sdk as uservo


SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200 
SERVO_ID = 0  # servo id

is_mturn = 0

uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)


if is_mturn:
	print("[多圈模式]设置舵机角度为900.0°, 周期1000ms")
	control.set_servo_angle(SERVO_ID, 900.0, interval=10000, is_mturn=True) # 设置舵机角度(指定周期 单位ms)
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[多圈模式]设置舵机角度为-900.0°, 设置转速为200 °/s")
	control.set_servo_angle(SERVO_ID, -900.0, velocity=200.0, t_acc=100, t_dec=100, is_mturn=True) # 设置舵机角度(指定转速 单位°/s) dps: degree per second
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[多圈模式]设置舵机角度为-850.0°, 添加功率限制")
	control.set_servo_angle(SERVO_ID, -850.0, power=400, is_mturn=True) # 设置舵机角度(指定功率 单位mW)
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))


else:

	print("[单圈模式]设置舵机角度为90.0°")
	control.set_servo_angle(SERVO_ID, 90.0, interval=1000) 
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[单圈模式]设置舵机角度为-80.0°, 周期1000ms")
	control.set_servo_angle(SERVO_ID, -80.0, interval=1000) # 设置舵机角度(指定周期 单位ms)
	time.sleep(1)

	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[单圈模式]设置舵机角度为70.0°, 设置转速为200 °/s, 加速时间100ms, 减速时间100ms")
	control.set_servo_angle(SERVO_ID, 70.0, velocity=200.0, t_acc=100, t_dec=100) # 设置舵机角度(指定转速 单位°/s)
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))


	print("[单圈模式]设置舵机角度为-90.0°, 添加功率限制")
	control.set_servo_angle(SERVO_ID, -90.0, power=400) # 设置舵机角度(指定功率 单位mW)
