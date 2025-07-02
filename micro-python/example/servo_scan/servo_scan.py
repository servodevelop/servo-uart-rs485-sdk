'''
总线伺服舵机
> MicroPython SDK扫描舵机指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
FashionStar Uart舵机
 * 网站：https://fashionrobo.com/
 * 更新时间: 2023/03/13
--------------------------------------------------
'''
from machine import UART
from uservo import UartServoManager
import ustruct

# 舵机个数
# 舵机ID编号: [0, 1, 2, ..., srv_num-1]
servo_num = 10

# 创建串口对象 使用串口2作为控制对象
# 波特率: 115200
# RX: gpio 16
# TX: gpio 17
uart = UART(2, baudrate=115200)
# 创建舵机管理器
uservo = UartServoManager(uart, srv_num=servo_num)

# 舵机扫描
print("开始进行舵机扫描")
uservo.scan_servo(srv_num=servo_num)
servo_list = list(uservo.servos.keys())
print("舵机扫描结束, 舵机列表: {}".format(servo_list))

