'''
总线伺服舵机
> Python SDK读取状态指令 Example <
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

'''打印舵机状态'''
# 读取温度
voltage = control.query_voltage(SERVO_ID)
print("Voltage: {:4.1f}V".format(voltage))
# 读取电流
current = control.query_current(SERVO_ID)
print("Current: {:4.1f}A".format(current))
# 读取功率
power = control.query_power(SERVO_ID)
print("Power:   {:4.1f}W".format(power))
# 读取温度
temp = control.query_temperature(SERVO_ID)
print("temp:    {:2.0f}°C".format(temp))
#  舵机工作状态标志位
#  BIT[0] - 执行指令置1，执行完成后清零。
#  BIT[1] - 执行指令错误置1，在下次正确执行后清零。
#  BIT[2] - 堵转错误置1，解除堵转后清零。
#  BIT[3] - 电压过高置1，电压恢复正常后清零。
#  BIT[4] - 电压过低置1，电压恢复正常后清零。
#  BIT[5] - 电流错误置1，电流恢复正常后清零。
#  BIT[6] - 功率错误置1，功率恢复正常后清零。
#  BIT[7] - 温度错误置1，温度恢复正常后清零。
status = control.query_status(SERVO_ID)
print("Status:  {:08b}".format(status))

