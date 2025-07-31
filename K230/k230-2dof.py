'''
实验名称：UART（串口通信）
作者：01Studio
实验平台：01Studio CanMV K230
说明：通过编程实现串口通信，跟电脑串口助手实现数据收发。
'''

#导入串口模块
from machine import UART
from machine import FPIOA
import time
from media.sensor import *
from media.display import *
from ybUtils.YbUart import YbUart
fpioa = FPIOA()
sensor_id = 2
sensor = None
# UART1代码
#fpioa.set_function(3,FPIOA.UART1_TXD)
#fpioa.set_function(4,FPIOA.UART1_RXD)

#uart=UART(UART.UART1,115200, bits=UART.EIGHTBITS, parity=UART.PARITY_NONE, stop=UART.STOPBITS_ONE) #设置串口号1和波特率

top_id=1
button_id=0

# UART2代码
#fpioa.set_function(9,FPIOA.UART1_TXD)
#fpioa.set_function(10,FPIOA.UART1_RXD)
#uart=UART(UART.UART1,115200) #设置串口号2和波特率
fpioa.set_function(9, fpioa.UART1_TXD, ie=0, oe=1)
fpioa.set_function(10, fpioa.UART1_RXD, ie=1, oe=0)
uart = UART(UART.UART1, 115200, bits=UART.EIGHTBITS, parity=UART.PARITY_NONE, stop=UART.STOPBITS_ONE)
#uart = YbUart(baudrate=115200)
#uart = YbUart(baudrate=115200)
class PIDController:
    def __init__(self, kp=1.0, ki=0.0, kd=0.0, dt=1.0, integral_limit=None):
        self.kp = kp  # 比例系数
        self.ki = ki / dt  # 积分系数，除以dt是为了离散化
        self.kd = kd / dt  # 微分系数，除以dt是为了离散化
        self.dt = dt  # 时间间隔
        self.prev_error = 0
        self.integral = 0
        self.integral_limit = integral_limit

    def update(self, setpoint, measured_value):
        error = setpoint - measured_value

        # 计算P, I, D部分
        p = self.kp * error
        i = self.ki * self.integral
        if self.integral_limit is not None:
            # 积分饱和防止积分溢出
            self.integral = max(min(self.integral + error * self.dt, self.integral_limit), -self.integral_limit)
        else:
            self.integral += error * self.dt

        derivative = (error - self.prev_error) / self.dt
        d = self.kd * derivative

        # 更新误差用于下一次迭代
        self.prev_error = error

        # 计算PID输出
        output = p + i + d

        return output

def set_servo_angle(servo_id, target_angle, duration_ms, power=0):
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
    CMD_ID = 0x0D  # 角度控制命令

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
    uart.write(data_packet)
#    print("Sent servo command:", [hex(x) for x in data_packet])

#set_servo_angle(servo_id=0, target_angle=0, duration_ms=100)

width_ = 640
height_ = 480
try:
    # 构造一个具有默认配置的摄像头对象
    sensor = Sensor(id=sensor_id)
    # 重置摄像头sensor
    sensor.reset()


    # 无需进行镜像翻转
    # 设置水平镜像
    # sensor.set_hmirror(False)
    # 设置垂直翻转
    # sensor.set_vflip(False)

    # 设置通道0的输出尺寸为显示分辨率
    sensor.set_framesize(width=width_,height=height_) #设置帧大小800x480,LCD专用,默认通道0
    # 设置通道0的输出像素格式为RGB565
    sensor.set_pixformat(Sensor.RGB565) #设置输出图像格式，默认通道0
    Display.init(Display.VIRT, sensor.width(), sensor.height())
   # Display.init(Display.ST7701, to_ide=True) #通过01Studio 3.5寸mipi显示屏显示图像
    # 初始化媒体管理器
    MediaManager.init()
    # 启动传感器
    sensor.run()


    # 指定颜色阈值
    # 格式：[min_L, max_L, min_A, max_A, min_B, max_B]
    color_threshold = [(12, 80, 20, 58, -21, 59)]

#    Step_Motor_Alternate_Ctl(100, 1, 100, 1)

    pid_y = PIDController(kp=0.55 ,ki=0.0, kd=0.004, dt=0.1)
    pid_x = PIDController(kp=0.55, ki=0.0, kd=0.004, dt=0.1)


    #Step_T_Motor_Contrl(200,100,1)
    while True:
        os.exitpoint()
        # 捕获通道0的图像
        img = sensor.snapshot(chn=CAM_CHN_ID_0)
        #color_threshold 是要寻找的颜色的阈值，area_threshold 表示过滤掉小于此面积的色块。
        blobs = img.find_blobs(color_threshold,area_threshold = 2000,merge=True)
        # 如果检测到颜色块
        if blobs:
            # 遍历每个检测到的颜色块
            for blob in blobs:
                # 绘制颜色块的外接矩形
                # blob[0:4] 表示颜色块的矩形框 [x, y, w, h]，
                img.draw_rectangle(blob[0:4])
                # 在颜色块的中心绘制一个十字
                # blob[5] 和 blob[6] 分别是颜色块的中心坐标 (cx, cy)
                img.draw_cross(blob[5], blob[6],color=(0,255,0),thickness=2,size=5)
                # 在控制台输出颜色块的中心坐标
                Blob_centerX=blob[5]
                Blob_centerY=blob[6]

            out_X=-pid_x.update( width_/2, Blob_centerX)
            out_Y=pid_y.update( height_/2, Blob_centerY)
            set_servo_angle(servo_id=top_id, target_angle=-out_Y, duration_ms=1000)
            set_servo_angle(servo_id=button_id, target_angle=out_X, duration_ms=1000)
#            if abs((out_X)) < 5:
#               out_X=0
#            Step_Simul_Ctl((int)(out_X), (int)(out_Y))
            print(out_X,out_Y)

        # 显示捕获的图像，中心对齐，居中显示
#        Display.show_image(img,x=int((DISPLAY_WIDTH - 320) / 2), y=int((DISPLAY_HEIGHT - 240) / 2))
        Display.show_image(img)

except KeyboardInterrupt as e:
    print("用户停止: ", e)
except BaseException as e:
    print(f"异常: {e}")
finally:
    # 停止传感器运行
    if isinstance(sensor, Sensor):
        sensor.stop()
    # 反初始化显示模块
    Display.deinit()
    os.exitpoint(os.EXITPOINT_ENABLE_SLEEP)
    time.sleep_ms(100)
    # 释放媒体缓冲区
    MediaManager.deinit()
