# 总线伺服舵机SDK使用手册（ROS2）

## 1.测试环境

No LSB modules are available.

Distributor ID: Ubuntu

Description:    Ubuntu 22.04.5 LTS

Release:        22.04

Codename:       jammy

ROS2:           humble



## 2.安装依赖

1. 自行安装ROS2 humble相关环境。

2. 安装python sdk。

   ```sh
   pip install fashionstar-uart-sdk
   
   
   pip install pyserial
   ```

   





## 3.运行 servo_driver 节点

进入ROS2文件夹，

```sh
colcon build

source install/setup.sh
```

然后运行driver节点。

```
ros2 run servo_driver driver
```





## 4.设置节点功能

进入到ROS2/src/servo_driver文件夹，打开文件 **servo_driver.py** 。根据需要，修改代码使用。在修改好代码后，重新运行节点。



### 4.1.修改串口端口和波特率

```python
#修改串口接口
SERVO_PORT_NAME = "/dev/ttyUSB0"  
#修改波特率 
SERVO_BAUDRATE = 115200				
```



### 4.2.读取角度

默认情况下，例程运行读取角度例程。会以10hz持续读取并发布 `servo_angle_topic`

舵机读取可设置的参数如下。

```python
#读取的id列表，默认情况下，舵机出厂设置为0号，可通过上位机设置其他id号。
ID_list = [0]               #SERVO id
#修改好id（0，1，2）后，作如下修改可以读取其他id舵机的角度。
ID_list = [0,1，2]		   #SERVO id
```





### 4.3.设置角度

通过设置参数，可以修改舵机的运行角度、时间间隔。

```python
#单个舵机时
ID_list = [0]               		#SERVO id
ANGLE_list=[50.0]           		# degree
INTERVAL_list = [1000]      		# ms
self.control_servo()				#send target angle
#多个舵机时
ID_list = [0,1]
ANGLE_list=[50.0,70.0]
INTERVAL_list = [1000,2000]
```



## 5.其他舵机功能

舵机的完整功能及协议请参考该文档。

[总线伺服舵机通信协议 - Fashion Star Wiki](https://wiki.fashionrobo.com/uartbasic/uart-protocol/)



### 5.1.如何使用

使用python SDK给出的接口，可以自由在ROS2节点上使用。

[总线伺服舵机SDK使用手册（Python） - Fashion Star Wiki](https://wiki.fashionrobo.com/uart-python-sdk/python/)



例如，想要使用ping指令。

```python
#ping 0 号舵机
self.control.ping(0)
```

或者，使用阻尼模式。

```python
#设置0号舵机进入阻尼模式，功率1000
self.contrl.set_damping(0,1000)
```

更多功能，请参考。

[总线伺服舵机SDK使用手册（Python） - Fashion Star Wiki](https://wiki.fashionrobo.com/uart-python-sdk/python/)
