'''
总线伺服舵机
> Python SDK监控指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2026/05/14
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo
# import uservo


SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200
SERVO_ID = 0  # servo id
REAL_TIME = False   # you can try "True" and power-off

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)

while True:
    servo_info = control.query_servo_monitor(servo_id=SERVO_ID, realtime=REAL_TIME)
    print(servo_info)
    time.sleep(0.5)