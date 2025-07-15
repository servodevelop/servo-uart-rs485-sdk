#!/usr/bin/env python
# coding:utf-8


import rclpy
from rclpy.node import Node

import struct
import math
import fashionstar_uart_sdk as uservo
from std_msgs.msg import Float32MultiArray
import time
import serial

SERVO_DRIVER_NODE = "servo_driver_node" 
SERVO_ANGLE_PUBLISHER = "servo_angle_topic"

SERVO_PORT_NAME = "/dev/ttyUSB0"  
SERVO_BAUDRATE = 115200

ID_list = [0]               #SERVO id
ANGLE_list=[50.0]           # degree
INTERVAL_list = [1000]      # ms

# # if servo num >1
# ID_list = [0,1]
# ANGLE_list=[50.0,70.0]
# INTERVAL_list = [1000,2000]

class uservo_sdk:
    def __init__(self,PORT_NAME=None,PORT_BAUDRATE=None):
        try:
            self.uart = serial.Serial(
                port=PORT_NAME,
                baudrate=PORT_BAUDRATE,
                parity=serial.PARITY_NONE,
                stopbits=1,
                bytesize=8,
                timeout=0,
            )
        except serial.SerialException as e:
            raise

        try:
            self.uservo = uservo.UartServoManager(self.uart)
        except Exception as e:
            raise

        time.sleep(0.01)
        self.uservo.reset_multi_turn_angle(0xff)
        time.sleep(0.1)

    def query_servo_angle(self,id_list):
        self.uservo.send_sync_servo_monitor(id_list)

        self.current_angle = [0 for i in ID_list]
        for i in id_list:
            self.current_angle[i] = self.uservo.servos[i].angle_monitor
        return self.current_angle


    def control_sero_angle(self,id_list,angle_list,interval_list):
        # angle_list [10.0,20.0]        #   degree
        # interval_list  [1000,2000]   #   ms
        command_data_list1 = [
        struct.pack('<BhHH', id_list[i], int(angle_list[i]*10), interval_list[i], 0)for i in id_list]
        self.uservo.send_sync_angle(8, len(id_list), command_data_list1)


class servo_contorl(Node):

    def __init__(self):
        super().__init__(SERVO_DRIVER_NODE)

        try:
            self.Servo = uservo_sdk(SERVO_PORT_NAME,SERVO_BAUDRATE)
            self.control = uservo.UartServoManager(self.Servo.uart)
        except ValueError as e:
            raise

        self.publisher = self.create_publisher(
            Float32MultiArray, SERVO_ANGLE_PUBLISHER,10
        )

        ## once
        # print(self.control.ping(0))                                #ping servo ,only use in code init
        # self.control_servo()                                       #send target angle

        
        ## loop
        self.timer = self.create_timer(0.1, self.timer_callback)     #timer_period_sec > 0.05



    def node_close(self):
        pass


    def publish_angle(self):
        result = self.Servo.query_servo_angle(ID_list)
        # print(f"{result}")                                        #<---debug
        msg = Float32MultiArray()
        msg.data = result
        self.publisher.publish(msg)


    def control_servo(self):
        self.Servo.control_sero_angle(id_list = ID_list,angle_list = ANGLE_list ,interval_list = INTERVAL_list)


    def timer_callback(self):
        self.publish_angle()



def main(args=None):
    rclpy.init(args=args)
    try:
        servo_driver_node = servo_contorl()
    except Exception as e:
        print(e)
        return

    try:
        rclpy.spin(servo_driver_node)
    except KeyboardInterrupt:
        pass
    finally:
        servo_driver_node.node_close()



if __name__ == "__main__":
    main()