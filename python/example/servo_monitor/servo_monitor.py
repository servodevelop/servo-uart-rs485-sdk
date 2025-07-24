'''
总线伺服舵机
> Python SDK监控指令 Example <
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

servo_info = control.query_servo_monitor(servo_id=0)
print("舵机 电压: {:.2f} V".format( servo_info["voltage"]/1000))         #单个参数

print("舵机 角度: {:.2f} °".format( servo_info["angle"] )) 
print("舵机电压: {:.2f}V, 电流: {:.2f}A, 功率: {:.2f}W, 温度: {:.2f}°C, 状态: {}, 角度: {:.2f}°, 圈数: {:.0f}"
          .format( servo_info["voltage"] / 1000, servo_info["current"] / 1000, servo_info["power"] / 1000, servo_info["temp"], servo_info["status"], servo_info["angle"], servo_info["turn"])) #多个参数