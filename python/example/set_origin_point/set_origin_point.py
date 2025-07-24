'''
总线伺服舵机
> Python SDK设置舵机原点指令 Example <
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

angle = control.query_servo_angle(SERVO_ID)

print("当前舵机角度: {:4.1f} °".format(angle), end='\n')

control.disable_torque(SERVO_ID)

control.set_origin_point(SERVO_ID)

time.sleep(0.1)

angle = control.query_servo_angle(SERVO_ID)

print("设置新的原点后舵机角度: {:4.1f} °".format(angle), end='\n')

