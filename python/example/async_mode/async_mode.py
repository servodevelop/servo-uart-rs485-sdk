'''
总线伺服舵机
> Python SDK异步指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2025/7/24
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


control.begin_async()   #  async begin
time.sleep(0.02)
control.set_servo_angle( SERVO_ID, angle = 20.0, interval=1000) 
time.sleep(2.02)
control.end_async(0)    #  async end   0:run  1:cancel