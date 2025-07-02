'''
总线伺服舵机
> MicroPython SDK同步命令 同步监控 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/23
--------------------------------------------------
'''
import ustruct
from machine import UART
from uservo import UartServoManager
import time


# 舵机ID编号: [0, 1, 2, ..., srv_num-1]
# 扫描舵机个数
servo_num = 4
# 舵机ID
#servo_id = 0
# 舵机是否有多圈模式的功能
#servo_has_mturn_func = False

# 创建串口对象 使用串口2作为控制对象
# 波特率: 115200
# RX: gpio 16
# TX: gpio 17
uart = UART(2, baudrate=115200)
# 创建舵机管理器
uservo = UartServoManager(uart, srv_num=servo_num)
servo_ids = [1,2,3]  # 想要同步读取的舵机ID列表
servo_monitor_data = uservo.send_sync_servo_monitor(servo_ids)
for servo_id, info in servo_monitor_data.items():
    if info:
        print("舵机ID: {}, 电压: {:.2f} V, 电流: {:.2f} A, 功率: {:.2f} W, 温度: {:.2f} °C, 状态: {}, 角度: {:.2f} °, 圈数: {:.0f}"
              .format(servo_id, info.voltage / 1000, info.current / 1000, info.power / 1000, info.temp, info.status, info.angle, info.turn))