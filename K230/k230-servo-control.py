#导入串口模块
from machine import UART
from machine import FPIOA
import utime
from media.sensor import *
from media.display import *
from ybUtils.YbUart import YbUart
import math

from machine import Timer
import time


def timer_callback(timer):
    global ticks_ms
    ticks_ms += 1



def bytes_to_int(data, byteorder='little', signed=True):
    """
    MicroPython兼容的字节转整数函数（不使用关键字参数）
    """
    # 对于4字节数据（32位整数）
    if len(data) != 4:
        return 0

    # 转换为无符号整数
    if byteorder == 'little':
        # 小端序：低位在前
        val = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24)
    else:
        # 大端序：高位在前
        val = data[3] | (data[2] << 8) | (data[1] << 16) | (data[0] << 24)

    # 处理有符号整数
    if signed:
        # 32位有符号整数的最大值
        max_signed = 0x7FFFFFFF
        if val > max_signed:
            # 转换为负数（补码）
            val -= 0x100000000

    return val

class servo_control:

    def __init__(self):
        self.uart = UART(UART.UART1, 115200, bits=UART.EIGHTBITS, parity=UART.PARITY_NONE, stop=UART.STOPBITS_ONE)

        self.disable_servo(0xff)
        time.sleep(1)
        self.reset_servo_circle(0xff)
        time.sleep(1)


    def disable_servo(self,servo_id):
        START_BYTE1 = 0x12
        START_BYTE2 = 0x4C
        CMD_ID2 = 24  # 清除圈数命令
        # 构建数据包 (不含校验和)
        data_packet = bytearray()
        data_packet.append(START_BYTE1)
        data_packet.append(START_BYTE2)
        data_packet.append(CMD_ID2)
        data_packet.append(0x04)  # 数据长度
        data_packet.append(servo_id)  # 舵机ID
        data_packet.append(0x10)  # 舵机ID
        data_packet.append(0x00)  # 舵机ID
        data_packet.append(0x00)  # 舵机ID

        # 计算校验和(从START_BYTE1开始)
        checksum = sum(data_packet) & 0xFF
        data_packet.append(checksum)
        # 发送数据
        self.uart.write(data_packet)
    #        print("reset servo angle command:", [hex(x) for x in data_packet])




    def reset_servo_circle(self,servo_id):
        """
        通过串口发送舵机指令
        参数:
        uart: 串口对象
        servo_id: 舵机ID (0-255)

        """
        # 协议常量
        START_BYTE1 = 0x12
        START_BYTE2 = 0x4C
        CMD_ID2 = 17  # 清除圈数命令
        # 构建数据包 (不含校验和)
        data_packet = bytearray()
        data_packet.append(START_BYTE1)
        data_packet.append(START_BYTE2)
        data_packet.append(CMD_ID2)
        data_packet.append(0x01)  # 数据长度
        data_packet.append(servo_id)  # 舵机ID
        # 计算校验和(从START_BYTE1开始)
        checksum = sum(data_packet) & 0xFF
        data_packet.append(checksum)
        # 发送数据
        self.uart.write(data_packet)
#        print("reset servo angle command:", [hex(x) for x in data_packet])

    def set_servo_angle(self,servo_id, target_angle, duration_ms, power=0):
        """
        通过串口发送舵机角度控制指令
        参数:
        uart: 串口对象
        servo_id: 舵机ID (0-255)
        target_angle: 目标角度(度)
        duration_ms: 运动时间(毫秒)
        power: 执行功率(默认0)
        """
        # 协议常量
        START_BYTE1 = 0x12
        START_BYTE2 = 0x4C
        CMD_ID = 0x0d  # 角度控制命令
        # 计算角度值(单位为0.1度)
        angle_value = int(target_angle * 10)
        # 转换为小端字节序
        angle_bytes = angle_value.to_bytes(4, 'little')
        duration_bytes = duration_ms.to_bytes(4, 'little')
        power_bytes = power.to_bytes(2, 'little')
        # 构建数据包 (不含校验和)
        data_packet = bytearray()
        data_packet.append(START_BYTE1)
        data_packet.append(START_BYTE2)
        data_packet.append(CMD_ID)
        data_packet.append(0x0B)  # 数据长度(11字节)
        data_packet.append(servo_id)  # 舵机ID
        data_packet.extend(angle_bytes)  # 角度值(4字节)
        data_packet.extend(duration_bytes)  # 时间(4字节)
        data_packet.extend(power_bytes)  # 功率(2字节)
        # 计算校验和(从START_BYTE1开始)
        checksum = sum(data_packet) & 0xFF
        data_packet.append(checksum)
        # 发送数据
        self.uart.write(data_packet)
    #    print("Sent servo angle command:", [hex(x) for x in data_packet])

    def query_servo_angle(self,servo_id):
        """
        通过串口发送舵机指令
        参数:
        uart: 串口对象
        servo_id: 舵机ID (0-255)
        """
        # 协议常量
        START_BYTE1 = 0x12
        START_BYTE2 = 0x4C
        CMD_ID1 = 16  # 角度控制命令
        # 构建数据包 (不含校验和)
        data_packet = bytearray()
        data_packet.append(START_BYTE1)
        data_packet.append(START_BYTE2)
        data_packet.append(CMD_ID1)
        data_packet.append(0x01)  # 数据长度
        data_packet.append(servo_id)  # 舵机ID
        # 计算校验和(从START_BYTE1开始)
        checksum = sum(data_packet) & 0xFF
        data_packet.append(checksum)
        # 发送数据
        self.uart.write(data_packet)
        start_time = ticks_ms
        data = b''
        flag = 0
        received = 0
        while ticks_ms - start_time < 80:
            received = self.uart.read(1)
#            print(flag,received)
            if flag == 0:
                if  received == b'\x05':
                    flag = 1
                else:
                    flag = 0
                    continue

            elif flag == 1:
                if  received == b'\x1c':
                    flag = 2
                else:
                    flag = 0
                    continue
            elif flag == 2:
                if len(data) >= 10:
#                    print("ok",data)
                    break
                if received is not None:
                   data += received
        angle = bytes_to_int(data[3:7], 'little', True)
        return angle/10



    def set_servo_damping(self,servo_id, power):
        """
        通过串口发送舵机指令
        参数:
        uart: 串口对象
        servo_id: 舵机ID (0-255)
        """
        # 协议常量
        START_BYTE1 = 0x12
        START_BYTE2 = 0x4C
        CMD_ID4 = 0x09  # 控制命令
        # 构建数据包 (不含校验和)
        power_bytes = power.to_bytes(2, 'little')
        data_packet = bytearray()
        data_packet.append(START_BYTE1)
        data_packet.append(START_BYTE2)
        data_packet.append(CMD_ID4)
        data_packet.append(0x03)  # 数据长度
        data_packet.append(servo_id)  # 舵机ID
        data_packet.append(power_bytes)  # 功率
        # 计算校验和(从START_BYTE1开始)
        checksum = sum(data_packet) & 0xFF
        data_packet.append(checksum)
        # 发送数据
        uart.write(data_packet)
        print("set servo damping command:", [hex(x) for x in data_packet])





if __name__ == "__main__":
    ticks_ms = 0

    # 实例化一个软定时器
    # 初始化定时器为周期模式，频率为 1000Hz
    tim = Timer(-1)
    tim.init(freq=1000, mode=Timer.PERIODIC, callback=timer_callback)
    # IO初始化
    fpioa = FPIOA()
    sensor = None
    fpioa.set_function(9, fpioa.UART1_TXD, ie=0, oe=1)
    fpioa.set_function(10, fpioa.UART1_RXD, ie=1, oe=0)
    # 串口控制类初始化
    control = servo_control()

    time.sleep_ms(50)
    #ID 目标角度 时间MS
    control.set_servo_angle(1,0,1000)
    control.set_servo_angle(0,0,1000)
    while(1):

       time.sleep_ms(50)
       print("0:",control.query_servo_angle(0))
       # 读取角度
       time.sleep_ms(50)
       print("1:",control.query_servo_angle(1))





