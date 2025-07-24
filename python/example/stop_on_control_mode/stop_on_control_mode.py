'''
总线伺服舵机
> Python SDK停止指令 Example <
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
SERVO_BAUDRATE = 1000000 
SERVO_ID = 0  # servo id

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)


control.stop_on_control_mode(SERVO_ID, method=0x10)
# control.stop_on_control_mode(SERVO_ID, method=0x11, power=500)
# control.stop_on_control_mode(SERVO_ID, method=0x12, power=500)