'''
总线伺服舵机
> Python SDK读取指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2025/07/24
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo
import struct

SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200 
SERVO_ID = 0  # servo id

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)

# 内存表读取
# 注: 因为每个数据位数据格式各不相同
# 因此读取得到的是字节流
voltage_bytes = control.read_data(SERVO_ID, control.ADDRESS_VOLTAGE)
# 数据解析
# 电压的数据格式为uint16_t,单位: mV
# 关于struct的用法，请参阅官方手册: https://docs.python.org/3/library/struct.html
voltage = struct.unpack('<H', voltage_bytes)

print("总线电压 {} mV".format(voltage))