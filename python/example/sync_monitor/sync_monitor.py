'''
总线伺服舵机
> Python SDK同步命令-同步监控 Example <
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
# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)




servo_ids = [0,1,2,3,4,5,6]  # 想要同步读取的舵机ID列表
control.send_sync_servo_monitor(servo_ids)
for id in servo_ids:
    print(f"id {id} angle: {control.servos[id].angle_monitor} \
        current:{control.servos[id].current} \
        power:{control.servos[id].power} \
        voltage:{control.servos[id].voltage} \
        temp:{control.servos[id].temp} \
        status:{control.servos[id].status}")

