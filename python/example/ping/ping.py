'''
总线伺服舵机
> Python SDK舵机通讯检测 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2025/07/24
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo


SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200 
SERVO_ID = 0  # servo id

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)

while True:
    
	# Ping
	is_online = control.ping(SERVO_ID)
	print("servo ID={} is online: {}".format(SERVO_ID, is_online))
	time.sleep(0.1)
