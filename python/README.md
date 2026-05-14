# 总线伺服舵机SDK使用手册（Python）



## 1.文档信息

### 1.1 文档名称
- 总线伺服舵机SDK使用手册（Python）

### 1.2 适用对象
- 使用 Python 语言进行总线伺服舵机控制的开发者
- 需要通过串口总线与总线伺服舵机通信的主控开发者

### 1.3 适用范围
- 基于[总线伺服舵机通信协议](https://wiki.fashionrobo.com/uartbasic/uart-protocol/)的Python API函数，适用于所有总线伺服舵机型号。
- > 注：有些指令仅支持特定规格舵机

### 1.4 配套资料
#### 上位机软件
上位机软件可以调试总线伺服舵机，测试总线伺服舵机的功能。

- 上位机软件：[FashionStar UART总线伺服舵机上位机软件](https://fashionrobo.com/downloadcenter)
- 使用说明：[总线伺服舵机上位机软件使用说明](https://wiki.fashionrobo.com/uartbasic/uart-servo-software/)

#### SDK
本文例程、API下载。

- Python_SDK下载链接：[SDK for Python](https://fashionrobo.com/downloadcenter/)

### 1.5 图例


![img](https://wiki.fashionrobo.com/uart-python-sdk/images/u45-slide-01.png)



![img](https://wiki.fashionrobo.com/uart-python-sdk/images/1.4.png)

## 2.概述

本文档用于说明总线伺服舵机 Python SDK 的使用方法，核心内容包括：

- 总线伺服舵机管理器的创建方式
- 串口通信初始化方法
- 常用控制命令、状态查询命令及扩展命令
- 典型使用示例
- 主控侧开发注意事项与建议

## 3.通讯约定

### 3.1 接线说明

1. 安装USB转TTL模块的驱动程序。
2. 将TTL/USB调试转换板UC-01与控制器、总线伺服舵机以及电源连接。

### 3.2 文件组织与引用方式

总线伺服舵机的文件路径`fashionstar-uart-servo-python/src/uservo.py`， 使用的时候可以将`uservo.py` 拷贝至你当前工程文件夹里面。 

或者使用的时候，将`uservo.py`所在的文件夹添加到系统路径里面， 相对路径/绝对路径都可以。

```python
# 添加uservo.py的系统路径
import sys
sys.path.append("../../src")
```

### 3.3 依赖导入

然后使用的过程中一般需要导入如下这两个依赖

```python
# PySerial 负责串口总线通信
import serial
# UartServoManager 总线伺服舵机管理器
from uservo import UartServoManager
```

### 3.4 串口对象初始化

接下来要创建串口对象，指定相关的参数

```python
# 参数配置
# 角度定义
SERVO_PORT_NAME =  'COM7' # 舵机串口号
SERVO_BAUDRATE = 115200 # 舵机的波特率
SERVO_ID = 0  # 舵机的ID号

# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
                     parity=serial.PARITY_NONE, stopbits=1,\
                     bytesize=8,timeout=0)
```

### 3.5 创建总线伺服舵机管理器

创建舵机管理器，将串口对象传入到构造器`UartServoManager`里面.

```python
# 初始化舵机管理器
uservo = UartServoManager(uart)
```

---

## 4.命令总览

| 分类 | API / 命令 | 功能说明 | 备注 |
| --- | --- | --- | --- |
| 通信检测 | `ping` | 舵机在线检测 | - |
| 阻尼模式 | `set_damping` | 设置舵机为阻尼模式 | - |
| 角度查询 | `query_servo_angle` | 查询舵机角度 | 单圈/多圈返回取决于控制模式 |
| 角度控制 | `set_servo_angle` | 角度控制 | 支持单圈/多圈、周期/速度/功率等模式 |
| 多圈维护 | `reset_multi_turn_angle` | 清除多圈圈数 | 须在失锁状态下使用 |
| 轮转模式 | `wheel_stop`、`set_wheel_norm`、`set_wheel_turn`、`set_wheel_time` | 轮转控制 | 316版本以及后续版本已弃用 |
| 用户数据 | `reset_user_data`、`read_data`、`write_data` | 用户自定义参数恢复、读取、写入 | 建议优先使用上位机修改 |
| 系统状态 | `query_voltage`、`query_current`、`query_power`、`query_temperature`、`query_status` | 查询电压、电流、功率、温度与状态字 | 温度返回为ADC值 |
| 失锁控制 | `disable_torque` | 使舵机进入失锁状态 | 失锁后仍会响应指令 |
| 原点设置 | `set_origin_point` | 设置舵机原点 | 仅适用于无刷磁编码舵机V311及之后版本，且须在失锁状态下使用 |
| 同步命令 | `send_sync_angle`、`send_sync_anglebyinterval`、`send_sync_anglebyvelocity`、`send_sync_multiturnangle`、`send_sync_multiturnanglebyinterval`、`send_sync_multiturnanglebyvelocity`、`send_sync_servo_monitor` | 多舵机同步控制与同步监控 | 仅适用于无刷磁编码舵机V316及之后版本 |
| 异步命令 | `begin_async`、`end_async` | 缓存并批量触发角度指令 | 仅适用于无刷磁编码舵机V316及之后版本 |
| 数据监控 | `query_servo_monitor` | 获取单个舵机监控数据 | 仅适用于无刷磁编码舵机V316及之后版本 |
| 停止模式 | `stop_on_control_mode` | 控制停止后的保持/卸力/阻尼状态 | 仅适用于无刷磁编码舵机V316及之后版本 |

### 4.1 同步命令支持的数据包编号

| 数据包编号 | 数据包命名 | 功能 |
| :--------- | :--------- | :--- |
| 8 | MoveOnAngleMode(Rotate) | 角度模式 |
| 11 | MoveOnAngleModeExByInterval | 角度模式(基于加减速时间) |
| 12 | MoveOnAngleModeExByVelocity | 角度模式(基于目标速度) |
| 13 | MoveOnMultiTurnAngleMode(Rotate) | 多圈角度控制 |
| 14 | MoveOnMultiTurnAngleModeExByInterval | 多圈角度控制(基于加减速时间) |
| 15 | MoveOnMultiTurnAngleModeExByVelocity | 多圈角度控制(基于目标速度) |
| 22 | ServoMonitor | 舵机数据监控 |

---

## 5.命令定义

### 5.1 通信检测
#### API-`ping`

调用舵机的`ping()` 函数用于舵机的通信检测, 判断舵机是否在线。

**函数原型**

```
def ping(self, servo_id):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |

**输出参数**

| Param     | Description  | Type | Remark |
| --------- | ------------ | ---- | ------ |
| is_online | 舵机是否在线 | bool |        |


### 5.2 阻尼模式
#### API-`set_damping`

设置舵机为阻尼模式。

**函数原型**

```
def set_damping(self, servo_id, power=0):
```

**输入参数**

| Param    | Description | Type  | Default | Remark |
| -------- | ----------- | ----- | ------- | ------ |
| servo_id | 舵机ID      | int   |         |        |
| power    | 舵机功率    | float |         | 单位mW |

**输出参数**

- 无


### 5.3 角度查询
#### API-`query_servo_angle`

**函数原型**

```
def query_servo_angle(self, servo_id, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param | Description  | Type  | Remark                                                       |
| ----- | ------------ | ----- | ------------------------------------------------------------ |
| angle | 舵机角度单圈 | float | 单圈角度范围[-180°，180°]<br />`realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |

#### API-`query_servo_mturn_angle`

**函数原型**

```
def query_servo_mturn_angle(self, servo_id, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param | Description  | Type  | Remark                                                       |
| ----- | ------------ | ----- | ------------------------------------------------------------ |
| angle | 舵机角度多圈 | float | 多圈角度范围[-368,640.0° , 368,640.0°]<br />`realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |


### 5.4 角度控制
#### API-`set_servo_angle`

设置舵机角度，这个API包含了6种舵机角度控制模式，通过传入不同的参数调用不同的指令。

**函数原型**

```
def set_servo_angle(self, servo_id:int, angle:float, is_mturn:bool=False, interval:float=None, velocity:float=None, t_acc:int=20, t_dec:int=20,  power:int=0, mean_dps:float=100.0):
```

**输入参数**

| Param    | Description                          | Type  | Default | Remark                                                       |
| -------- | ------------------------------------ | ----- | ------- | ------------------------------------------------------------ |
| servo_id | 舵机ID                               | int   |         |                                                              |
| angle    | 目标角度                             | bool  | False   | 单圈控制范围[-180°，180°]<br />多圈控制范围[-368,640.0° , 368,640.0°] |
| is_mturn | 是否是多圈模式                       | bool  | False   |                                                              |
| interval | 中间间隔                             | float | None    | 单位ms                                                       |
| velocity | 舵机的目标转速                       | float | None    | 单位dps                                                      |
| t_acc    | 加速时间，启动时加速段的时间         | int   | 20      | 单位ms                                                       |
| t_dec    | 减速时间，运动到接近目标的减速段时间 | int   | 20      | 单位ms                                                       |
| power    | 功率限制                             | int   | 0       | 单位mW                                                       |
| mean_dps | 平均转速                             | float | 100.0   | 单位dps，用于估计interval                                    |

**输出参数**

- 无


### 5.5 多圈圈数清除
该API用于清除多圈圈数，须在失锁状态下使用。

**函数原型**

```
def reset_multi_turn_angle(self, servo_id:int):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |

**输出参数**

- 无


### 5.6 轮转模式
#### API-`wheel_stop`

轮转模式停止转动。

**函数原型**

```
def wheel_stop(self, servo_id):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |

**输出参数**

- 无

#### API-`set_wheel_norm`

设置轮转普通模式，转速单位: °/s

**函数原型**

```
def set_wheel_norm(self, servo_id, is_cw=True, mean_dps=None)
```

**输入参数**

| Param    | Description  | Type  | Default | Remark                          |
| -------- | ------------ | ----- | ------- | ------------------------------- |
| servo_id | 舵机ID       | int   |         |                                 |
| is_cw    | 是否是顺时针 | bool  | True    | `True`: 顺时针，`False`: 逆时针 |
| mean_dps | 平均转速     | float | None    | 单位dps                         |

**输出参数**

- 无

#### API-`set_wheel_turn`

轮转模式，让舵机旋转特定的圈数。

**函数原型**

```
def set_wheel_turn(self, servo_id, turn=1, is_cw=True, mean_dps=None, is_wait=True):
```

**输入参数**

| Param    | Description      | Type  | Default | Remark                          |
| -------- | ---------------- | ----- | ------- | ------------------------------- |
| servo_id | 舵机ID           | int   |         |                                 |
| turn     | 目标要旋转的圈数 | int   | 1       |                                 |
| is_cw    | 是否是顺时针     | bool  | True    | `True`: 顺时针，`False`: 逆时针 |
| mean_dps | 平均转速         | float | None    | 单位dps                         |
| is_wait  | 是否是阻塞式等待 | bool  | True    |                                 |

**输出参数**

- 无

#### API-`set_wheel_time`

轮转模式，旋转特定的时间。

**函数原型**

```
def set_wheel_time(self, servo_id, interval=1000, is_cw=True, mean_dps=None, is_wait=True):
```

**输入参数**

| Param    | Description      | Type  | Default | Remark                          |
| -------- | ---------------- | ----- | ------- | ------------------------------- |
| servo_id | 舵机ID           | int   |         |                                 |
| interval | 目标要旋转的时间 | int   | 100     | 单位ms                          |
| is_cw    | 是否是顺时针     | bool  | True    | `True`: 顺时针，`False`: 逆时针 |
| mean_dps | 平均转速         | float | None    | 单位dps                         |
| is_wait  | 是否是阻塞式等待 | bool  | True    |                                 |

**输出参数**

- 无


### 5.7 用户自定义参数
 **注意事项：**  如有修改用户自定义参数的需要，可以在上位机进行，更加方便，直观。  



#### API-`reset_user_data`

重置用户数据表, 恢复默认值。

**函数原型**

```
def reset_user_data(self, servo_id):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |

**输出参数**

- 无

#### API-`read_data`

读取数据。

**函数原型**

```
def read_data(self, servo_id, address, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| address  | 内存表      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param   | Description        | Type  | Remark                                                       |
| ------- | ------------------ | ----- | ------------------------------------------------------------ |
| content | 数值的二进制数据流 | bytes | `realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |

#### API-`write_data`

写入数据。

**函数原型**

```
def write_data(self, servo_id, address, content, realtime=False):
```

**输入参数**

| Param    | Description        | Type  | Default | Remark |
| -------- | ------------------ | ----- | ------- | ------ |
| servo_id | 舵机ID             | int   |         |        |
| address  | 内存表             | int   |         |        |
| content  | 数值的二进制数据流 | bytes |         |        |
| realtime | 实时反馈           | bool  | False   |        |

**输出参数**

| Param              | Description  | Type | Remark                                 |
| ------------------ | ------------ | ---- | -------------------------------------- |
| data_write_success | 是否写入成功 | bool | `realtime=True`时，舵机掉电返回`False` |

- `data_write_success`:是否写入成功


### 5.8 系统状态查询
#### API-`query_voltage`

查询当前的电压

**函数原型**

```
def query_voltage(self, servo_id, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param   | Description | Type  | Remark                                                       |
| ------- | ----------- | ----- | ------------------------------------------------------------ |
| voltage | 电压        | float | 单位V<br />`realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |

#### API-`query_current`

查询当前的电流

**函数原型**

```
def query_current(self, servo_id, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param   | Description | Type  | Remark                                                       |
| ------- | ----------- | ----- | ------------------------------------------------------------ |
| current | 舵机电流    | float | 单位A<br />`realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |

#### API-`query_power`

查询当前的功率

**函数原型**

```
def query_power(self, servo_id, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param | Description | Type  | Remark                                                       |
| ----- | ----------- | ----- | ------------------------------------------------------------ |
| power | 舵机功率    | float | 单位W<br />`realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |

#### API-`query_temperature`

查询舵机当前的温度

**函数原型**

```
def query_temperature(self, servo_id, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param       | Description | Type  | Remark                                                       |
| ----------- | ----------- | ----- | ------------------------------------------------------------ |
| temperature | 温度，ADC值 | float | 单位摄氏度<br />`realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |

#### API-`query_status`

查询舵机当前的工作状态

**函数原型**

```
def query_status(self, servo_id, realtime=False):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param  | Description       | Type | Remark                                                       |
| ------ | ----------------- | ---- | ------------------------------------------------------------ |
| status | 8位工作状态标志位 | int  | 舵机工作状态标志位<br/>BIT[0] - 执行指令置1，执行完成后清零。<br/>BIT[1] - 执行指令错误置1，在下次正确执行后清零。<br/>BIT[2] - 堵转错误置1，解除堵转后清零。<br/>BIT[3] - 电压过高置1，电压恢复正常后清零。<br/>BIT[4] - 电压过低置1，电压恢复正常后清零。<br/>BIT[5] - 电流错误置1，电流恢复正常后清零。<br/>BIT[6] - 功率错误置1，功率恢复正常后清零。<br/>BIT[7] - 温度错误置1，温度恢复正常后清零。<br />`realtime=True`时，舵机掉电返回`None`，否则返回掉电前一时刻数据 |


#### 温度ADC值转换表

***注：ADC转换为摄氏度值公式\***

![image-20210421141916211](https://wiki.fashionrobo.com/uart-python-sdk/images/ZfqjeCzH3tWD2gw.png)

| 温度(℃) | ADC  | 温度(℃) | ADC  | 温度(℃) | ADC  |
| ------- | ---- | ------- | ---- | ------- | ---- |
| 50      | 1191 | 60      | 941  | 70      | 741  |
| 51      | 1164 | 61      | 918  | 71      | 723  |
| 52      | 1137 | 62      | 897  | 72      | 706  |
| 53      | 1110 | 63      | 876  | 73      | 689  |
| 54      | 1085 | 64      | 855  | 74      | 673  |
| 55      | 1059 | 65      | 835  | 75      | 657  |
| 56      | 1034 | 66      | 815  | 76      | 642  |
| 57      | 1010 | 67      | 796  | 77      | 627  |
| 58      | 986  | 68      | 777  | 78      | 612  |
| 59      | 963  | 69      | 759  | 79      | 598  |

- 以上为50-79℃ 温度/ADC参照表

### 5.9 失锁控制
 **注意事项：**  失锁状态下，舵机仍会响应指令。  



#### API-`disable_torque`

**函数原型**

```
def disable_torque(self, servo_id:int):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |

**输出参数**

- 无


### 5.10 原点设置
> 注：**仅适用于无刷磁编码舵机V311及之后的版本**，该API用于设置原点，须在失锁状态下使用。

#### API-`set_origin_point`

**函数原型**

```
def set_origin_point(self, servo_id:int):
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |

**输出参数**

- 无


### 5.11 同步命令
> 注：**仅适用于无刷磁编码舵机V316及之后的版本**

**支持的命令编号**

| 数据包编号 | 数据包命名                           | 功能                         |
| :--------- | :----------------------------------- | :--------------------------- |
| 8          | MoveOnAngleMode(Rotate)              | 角度模式                     |
| 11         | MoveOnAngleModeExByInterval          | 角度模式(基于加减速时间)     |
| 12         | MoveOnAngleModeExByVelocity          | 角度模式(基于目标速度)       |
| 13         | MoveOnMultiTurnAngleMode(Rotate)     | 多圈角度控制                 |
| 14         | MoveOnMultiTurnAngleModeExByInterval | 多圈角度控制(基于加减速时间) |
| 15         | MoveOnMultiTurnAngleModeExByVelocity | 多圈角度控制(基于目标速度)   |
| 22         | ServoMonitor                         | 舵机数据监控                 |

#### API-`send_sync_angle`

**函数原型**

```python
def send_sync_angle(self, command_id, servo_num, command_data_list):
```

**输入参数**

| Param             | Description  | Type | Default | Remark |
| ----------------- | ------------ | ---- | ------- | ------ |
| command_id        | 命令编号     | int  |         |        |
| servo_num         | 舵机个数     | int  |         |        |
| command_data_list | 命令数据列表 |      |         |        |

**输出参数**

- 无

#### API-`send_sync_anglebyinterval`

**函数原型**

```python
def send_sync_anglebyinterval(self, command_id, servo_num, command_data_list):
```

**输入参数**

| Param             | Description  | Type | Default | Remark |
| ----------------- | ------------ | ---- | ------- | ------ |
| command_id        | 命令编号     | int  |         |        |
| servo_num         | 舵机个数     | int  |         |        |
| command_data_list | 命令数据列表 |      |         |        |

**输出参数**

**输出参数**

- 无

#### API-`send_sync_anglebyvelocity`

**函数原型**

```python
def send_sync_anglebyvelocity(self, command_id, servo_num, command_data_list):
```

**输入参数**

**

| Param             | Description  | Type | Default | Remark |
| ----------------- | ------------ | ---- | ------- | ------ |
| command_id        | 命令编号     | int  |         |        |
| servo_num         | 舵机个数     | int  |         |        |
| command_data_list | 命令数据列表 |      |         |        |

**输出参数**

- 无

#### API-`send_sync_multiturnangle`

**函数原型**

```python
def send_sync_multiturnangle(self, command_id, servo_num, command_data_list):
```

**输入参数**

| Param             | Description  | Type | Default | Remark |
| ----------------- | ------------ | ---- | ------- | ------ |
| command_id        | 命令编号     | int  |         |        |
| servo_num         | 舵机个数     | int  |         |        |
| command_data_list | 命令数据列表 |      |         |        |

**输出参数**

- 无

#### API-`send_sync_multiturnanglebyinterval`

**函数原型**

```python
def send_sync_multiturnanglebyinterval(self, command_id, servo_num, command_data_list):
```

**输入参数**

| Param             | Description  | Type | Default | Remark |
| ----------------- | ------------ | ---- | ------- | ------ |
| command_id        | 命令编号     | int  |         |        |
| servo_num         | 舵机个数     | int  |         |        |
| command_data_list | 命令数据列表 |      |         |        |

**输出参数**

- 无

#### API-`send_sync_multiturnanglebyvelocity`

**函数原型**

```c
uservo.set_origin_point(self, servo_id:int):
```

**输入参数**

| Param      | Description | Type | Default | Remark |
| ---------- | ----------- | ---- | ------- | ------ |
| command_id | 命令编号    | int  |         |        |
| servo_num  | 舵机个数    | int  |         |        |

**输出参数**

- 无

#### API-`send_sync_servo_monitor`

**函数原型**

```python
def send_sync_servo_monitor(self, servo_ids, realtime=False):
```

**输入参数**

| Param      | Description | Type | Default | Remark |
| ---------- | ----------- | ---- | ------- | ------ |
| command_id | 命令编号    | int  |         |        |
| servo_num  | 舵机个数    | int  |         |        |
| realtime   | 实时反馈    | bool | False   |        |

**输出参数**

| Param       | Description      | Type | Remark                                                       |
| ----------- | ---------------- | ---- | ------------------------------------------------------------ |
| servos_info | 舵机各项信息集合 | set  | key:servo_ID,value:UartServoInfo<br /><br />`realtime=True`时，舵机掉电各项数据都返回`None`，否则各项数据返回掉电前一时刻数据 |


### 5.12 异步命令
> 注：**仅适用于无刷磁编码舵机V316及之后的版本**

#### API-`begin_async`

开始异步指令，对下一个接收到的指令进行缓存，仅支持角度指令。

**函数原型**

```python
def begin_async(self):
```

**输入参数**

- 无

**输出参数**

- 无



#### API-`end_async`

结束异步指令，立即执行缓存指令。若参数 cancel 不为0，則清除缓存缓存。

**函数原型**

```python
def end_async(self,cancel=0):
```

**输入参数**

| Param  | Description | Type | Default | Remark |
| ------ | ----------- | ---- | ------- | ------ |
| cancel | 是否取消    | int  | 0       |        |

**输出参数**

- 无


### 5.13 数据监控
> 注：**仅适用于无刷磁编码舵机V316及之后的版本**

#### API-`query_servo_monitor`

获取舵机数据。

**函数原型**

```python
def query_servo_monitor(self, servo_id, realtime=False): 
```

**输入参数**

| Param    | Description | Type | Default | Remark |
| -------- | ----------- | ---- | ------- | ------ |
| servo_id | 舵机ID      | int  |         |        |
| realtime | 实时反馈    | bool | False   |        |

**输出参数**

| Param       | Description      | Type          | Remark                                                       |
| ----------- | ---------------- | ------------- | ------------------------------------------------------------ |
| servos_info | 舵机各项信息集合 | UartServoInfo | `realtime=True`时，舵机掉电各项数据都返回`None`，否则各项数据返回掉电前一时刻数据 |


### 5.14 控制模式停止指令
> 注：**仅适用于无刷磁编码舵机V316及之后的版本**

#### API-`stop_on_control_mode`

使舵机停止后保持不同状态模式。

**函数原型**

```python
def stop_on_control_mode(self,servo_id, method, power):
```

**输入参数**



| Param    | Description  | Type  | Default      | Remark                                      |
| -------- | ------------ | ----- | ------------ | ------------------------------------------- |
| servo_id | 舵机ID       | int   |              |                                             |
| method   | 停止后的模式 | int   | 0x11保持锁力 | 0x10 卸力  ；0x11保持锁力；0x12进入阻尼状态 |
| power    | 保持的功率   | float | 0            | 单位mW                                      |

**输出参数**

- 无


---

## 6.典型使用示例

### 6.1 舵机通信检测
#### 示例
`example/ping`

```
'''
总线伺服舵机
> Python SDK舵机通讯检测 Example <
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

while True:
    
	# Ping
	is_online = control.ping(SERVO_ID)
	print("servo ID={} is online: {}".format(SERVO_ID, is_online))
	time.sleep(0.1)

```


### 6.2 舵机阻尼模式
#### 示例
`example/damping`

```
'''
总线伺服舵机
> Python SDK阻尼设置指令 Example <
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

power = 500 				# damping power(mv)
control.set_damping(SERVO_ID, power)
```


### 6.3 舵机角度查询
#### 示例
设置舵机为阻尼模式，转动舵机 1s打印一下当前的角度。

`example/query_servo_angle

```
'''
总线伺服舵机
> Python SDK舵机角度查询 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2026/05/14
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo
# import uservo


SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200 
SERVO_ID = 0  # servo id

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)

# query servo angle
while True:
    angle = control.query_servo_angle(SERVO_ID)
    print("current angle: {:4.1f} °".format(angle))
    angle = control.query_servo_mturn_angle(SERVO_ID)
    print("current angle: {:4.1f} °".format(angle))
    time.sleep(0.1)
```


### 6.4 设置舵机角度
#### 示例
`example/set_servo_angle`

```
'''
总线伺服舵机
> Python SDK角度设置 Example <
注意：以下包括单圈角度和多圈角度指令
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/23
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo


SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200 
SERVO_ID = 0  # servo id

is_mturn = 0

uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)


if is_mturn:
	print("[多圈模式]设置舵机角度为900.0°, 周期1000ms")
	control.set_servo_angle(SERVO_ID, 900.0, interval=10000, is_mturn=True) # 设置舵机角度(指定周期 单位ms)
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[多圈模式]设置舵机角度为-900.0°, 设置转速为200 °/s")
	control.set_servo_angle(SERVO_ID, -900.0, velocity=200.0, t_acc=100, t_dec=100, is_mturn=True) # 设置舵机角度(指定转速 单位°/s) dps: degree per second
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[多圈模式]设置舵机角度为-850.0°, 添加功率限制")
	control.set_servo_angle(SERVO_ID, -850.0, power=400, is_mturn=True) # 设置舵机角度(指定功率 单位mW)
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))


else:

	print("[单圈模式]设置舵机角度为90.0°")
	control.set_servo_angle(SERVO_ID, 90.0, interval=1000) 
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[单圈模式]设置舵机角度为-80.0°, 周期1000ms")
	control.set_servo_angle(SERVO_ID, -80.0, interval=1000) # 设置舵机角度(指定周期 单位ms)
	time.sleep(1)

	print("-> {}".format(control.query_servo_angle(SERVO_ID)))

	print("[单圈模式]设置舵机角度为70.0°, 设置转速为200 °/s, 加速时间100ms, 减速时间100ms")
	control.set_servo_angle(SERVO_ID, 70.0, velocity=200.0, t_acc=100, t_dec=100) # 设置舵机角度(指定转速 单位°/s)
	time.sleep(1)
	print("-> {}".format(control.query_servo_angle(SERVO_ID)))


	print("[单圈模式]设置舵机角度为-90.0°, 添加功率限制")
	control.set_servo_angle(SERVO_ID, -90.0, power=400) # 设置舵机角度(指定功率 单位mW)

```


### 6.5 轮转模式
#### 示例
`example/wheel`

```
'''
伺服总线舵机
> Python SDK 舵机轮转模式测试 <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2023/03/13
--------------------------------------------------
'''
# 添加uservo.py的系统路径
import sys
sys.path.append("../../src")
# 导入依赖
import time
import serial
from uservo import UartServoManager

# 参数配置
# 角度定义
SERVO_PORT_NAME =  'COM7' # 舵机串口号
SERVO_BAUDRATE = 115200 # 舵机的波特率
SERVO_ID = 0  # 舵机的ID号

# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
                     parity=serial.PARITY_NONE, stopbits=1,\
                     bytesize=8,timeout=0)
# 初始化舵机管理器
uservo = UartServoManager(uart)

print("测试常规模式")

# 设置舵机为轮转普通模式
# 旋转方向(is_cw) : 顺时针
# 角速度(mean_dps) : 单位°/s
uservo.set_wheel_norm(SERVO_ID, is_cw=True, mean_dps=200.0)

# 延时5s然后关闭
time.sleep(5.0)

# 停止
uservo.wheel_stop(SERVO_ID)

time.sleep(1)

# 定圈模式
print("测试定圈模式")
uservo.set_wheel_turn(SERVO_ID, turn=5, is_cw=False, mean_dps=200.0)

# 定时模式
print("测试定时模式")
uservo.set_wheel_time(SERVO_ID, interval=5000, is_cw=True, mean_dps=200.0)
```


### 6.6 用户自定义参数修改
#### 示例：重置用户数据表

`example/reset_user_data`

```
'''
伺服总线舵机
> 内存表数据重置 <

注意事项: 重置内存表这个指令比较特殊, 舵机ID也会被重置为0
因此测试该指令的时候, 最好只接一颗舵机。
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2023/03/13
--------------------------------------------------
'''
# 添加uservo.py的系统路径
import sys
sys.path.append("../../src")
# 导入依赖
import time
import struct
import serial
from uservo import UartServoManager

# 参数配置
# 角度定义
SERVO_PORT_NAME =  'COM7' # 舵机串口号
SERVO_BAUDRATE = 115200 # 舵机的波特率
SERVO_ID = 0  # 舵机的ID号

# 数据表定义
ADDRESS_SOFTSTART = 49 # 上电缓启动地址位
SOFTSTART_OPEN = 1 # 上电缓启动-开启
SOFTSTART_CLOSE = 0 # 上电缓启动-关闭

# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
                     parity=serial.PARITY_NONE, stopbits=1,\
                     bytesize=8,timeout=0)
# 初始化舵机管理器
uservo = UartServoManager(uart, is_debug=True)
# 重置用户数据
uservo.reset_user_data(SERVO_ID)


# 舵机扫描
print("开始进行舵机扫描")
uservo.scan_servo()
servo_list = list(uservo.servos.keys())
print("舵机扫描结束, 舵机列表: {}".format(servo_list))

if SERVO_ID not in servo_list:
    print("指定的SERVO_ID无效, 请修改舵机ID列表")
    exit(-1)

print("重置舵机内存表: 舵机ID = {}".format(SERVO_ID))
uservo.reset_user_data(SERVO_ID)

print("重新进行舵机扫描")
uservo.scan_servo()
servo_list = list(uservo.servos.keys())
print("舵机扫描结束, 舵机列表: {}".format(servo_list))
```

#### 示例：读取内存表

`example/read_data`

```

'''
伺服总线舵机
> 内存表数据读取 <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2023/03/13
--------------------------------------------------
'''
# 添加uservo.py的系统路径
import sys
sys.path.append("../../src")
# 导入依赖
import time
import struct
import serial
from uservo import UartServoManager

# 参数配置
# 角度定义
SERVO_PORT_NAME =  'COM7' # 舵机串口号
SERVO_BAUDRATE = 115200 # 舵机的波特率
SERVO_ID = 0  # 舵机的ID号
# 数据表定义
ADDRESS_VOLTAGE = 1 # 总线电压值的地址

# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
                     parity=serial.PARITY_NONE, stopbits=1,\
                     bytesize=8,timeout=0)
# 初始化舵机管理器
uservo = UartServoManager(uart)

# 内存表读取
# 注: 因为每个数据位数据格式各不相同
# 因此读取得到的是字节流
voltage_bytes = uservo.read_data(SERVO_ID, ADDRESS_VOLTAGE)
# 数据解析
# 电压的数据格式为uint16_t,单位: mV
# 关于struct的用法，请参阅官方手册: https://docs.python.org/3/library/struct.html
voltage = struct.unpack('<H', voltage_bytes)

print("总线电压 {} mV".format(voltage))
```

#### 示例：写入内存表

`example/write_data`

```
'''
伺服总线舵机
> 内存表数据写入 <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2023/03/13
--------------------------------------------------
'''
# 添加uservo.py的系统路径
import sys
sys.path.append("../../src")
# 导入依赖
import time
import struct
import serial
from uservo import UartServoManager

# 参数配置
# 角度定义
SERVO_PORT_NAME =  'COM7' # 舵机串口号
SERVO_BAUDRATE = 115200 # 舵机的波特率
SERVO_ID = 0  # 舵机的ID号

# 数据表定义
ADDRESS_SOFTSTART = 49 # 上电缓启动地址位
SOFTSTART_OPEN = 1 # 上电缓启动-开启
SOFTSTART_CLOSE = 0 # 上电缓启动-关闭

# 初始化串口
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,\
                     parity=serial.PARITY_NONE, stopbits=1,\
                     bytesize=8,timeout=0)
# 初始化舵机管理器
uservo = UartServoManager(uart)

# 内存表写入
# 注: 在写入之前，需要查阅手册确保该数据位可写
# 缓启动数据类型 uint8_t, 首先构造数据位
softstart_bytes = struct.pack('<B', SOFTSTART_OPEN)
# 将数据写入内存表
ret = uservo.write_data(SERVO_ID, ADDRESS_SOFTSTART, softstart_bytes)
# 打印日志
print("缓启动数据写入是否成功: {}".format(ret))
```


### 6.7 系统状态查询
#### 示例
`example/servo_status`

```python
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


```


### 6.8 原点设置
#### 示例
`example/set_origin_point`

```python
'''
总线伺服舵机
> Python SDK设置舵机原点指令 Example <
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

angle = control.query_servo_angle(SERVO_ID)

print("当前舵机角度: {:4.1f} °".format(angle), end='\n')

control.disable_torque(SERVO_ID)

control.set_origin_point(SERVO_ID)

time.sleep(0.1)

angle = control.query_servo_angle(SERVO_ID)

print("设置新的原点后舵机角度: {:4.1f} °".format(angle), end='\n')


```


### 6.9 同步命令
#### 示例
`example/sync_mode`

```python
'''
总线伺服舵机
> Python SDK同步指令 Example <
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

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)


command_data_list1 = [
    struct.pack('<BhHH', 1, -400, 500, 10000),  # 同步命令角度模式控制
    struct.pack('<BhHH', 2, -400 ,500, 10000),  #id2+度数-40 +时间 +功率
]
control.send_sync_angle(8, 2, command_data_list1)
time.sleep(2.02)

command_data_list2 = [
    struct.pack('<BhHHHH', 1, 400, 500, 100, 100, 10000) , # 同步命令角度模式控制(基於加减速時間)
    struct.pack('<BhHHHH', 2, 400, 500, 100, 100, 10000) , #id2+度数40+总时间+启动加速时间+运动减速时间+功率
]
control.send_sync_anglebyinterval(11, 2, command_data_list2)
time.sleep(2.02)


command_data_list4 = [
    struct.pack('<BhHHHH', 1, 600, 500, 100, 100, 10000), # 同步命令角度模式控制 (基於速率的運動控制 )
    struct.pack('<BhHHHH', 2, 600, 500, 100, 100, 10000), # id2+度数60+时间+功率
]
control.send_sync_anglebyvelocity(12, 2, command_data_list4)
time.sleep(2.02)


command_data_list3 = [
    struct.pack('<BlLH', 1, 800, 1500, 10000)  ,# 同步命令多圈角度模式控制
    struct.pack('<BlLH', 2, 800 ,1500, 10000)  ,# id2+度数80 +时间 +功率
]
control.send_sync_multiturnangle(13, 2, command_data_list3)
time.sleep(2.02)



command_data_list5 = [
    struct.pack('<BlLHHH', 1, 10010, 1000, 100,100,10000)  ,# 多圈角度模式控制 (基於加減速時段的運動控制 )
    struct.pack('<BlLHHH', 2, 10010 ,1000,100,100, 10000)  ,# id2+度数1001+总时间+启动加速时间+运动减速时间+功率
]
control.send_sync_multiturnanglebyinterval(14, 2, command_data_list5)
time.sleep(2.02)



command_data_list6 = [
    struct.pack('<BlHHHH', 1, 12010, 3000,100 ,100,10000)  ,# 多圈角度模式控制(基於速率的運動控制)
    struct.pack('<BlHHHH', 2, 12010 ,3000, 100, 100, 10000)  ,# id2+度数1201+目标速度300dps+启动加速时间+运动减速时间+功率
]
control.send_sync_multiturnanglebyvelocity(15, 2, command_data_list6)
```

`example/sync_monitor`

```python
'''
总线伺服舵机
> Python SDK同步命令-同步监控 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2026/05/14
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo


SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200 
REAL_TIME = False   # you can try "True" and power-off
# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)

servo_ids = [0,1,2,3,4,5,6]  # 想要同步读取的舵机ID列表

# 获取舵机信息
while True:
    data = control.send_sync_servo_monitor(servo_ids,realtime=REAL_TIME)
    for id in servo_ids:
        print(str(data[id]))
    time.sleep(0.5)

```


### 6.10 异步命令
#### 示例
`example/async_mode`

```python
'''
总线伺服舵机
> Python SDK异步指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2025/7/24
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


control.begin_async()   #  async begin
time.sleep(0.02)
control.set_servo_angle( SERVO_ID, angle = 20.0, interval=1000) 
time.sleep(2.02)
control.end_async(0)    #  async end   0:run  1:cancel
```


### 6.11 数据监控
#### 示例
`example/servo_monitor`

```python
'''
总线伺服舵机
> Python SDK监控指令 Example <
--------------------------------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2026/05/14
--------------------------------------------------
'''
import time
import serial
import fashionstar_uart_sdk as uservo
# import uservo


SERVO_PORT_NAME =  '/dev/ttyUSB0' 
SERVO_BAUDRATE = 115200
SERVO_ID = 0  # servo id
REAL_TIME = False   # you can try "True" and power-off

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)

while True:
    servo_info = control.query_servo_monitor(servo_id=SERVO_ID, realtime=REAL_TIME)
    print(servo_info)
    time.sleep(0.5)
```


### 6.12 控制模式停止指令
#### 示例
`example/stop_on_control_mode`

```python
'''
总线伺服舵机
> Python SDK停止指令 Example <
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
# SERVO_BAUDRATE = 1000000 
SERVO_ID = 0  # servo id

# uart init
uart = serial.Serial(port=SERVO_PORT_NAME, baudrate=SERVO_BAUDRATE,parity=serial.PARITY_NONE, stopbits=1,bytesize=8,timeout=0)
control = uservo.UartServoManager(uart)


# control.stop_on_control_mode(SERVO_ID, method=0x10, power=0)
control.stop_on_control_mode(SERVO_ID, method=0x11, power=500)
# control.stop_on_control_mode(SERVO_ID, method=0x12, power=500)
```


---

## 7.主控开发建议

### 7.1 初始化与通信
- 主控侧应先确认串口号、波特率、供电和接线正确，再创建`serial.Serial`对象与`UartServoManager`对象。
- 建议先使用`ping`进行在线检测，再进入控制流程。
- 串口初始化参数应与舵机实际配置保持一致。

### 7.2 角度控制与模式使用
- 使用`set_servo_angle`时，应明确当前是单圈模式还是多圈模式。
- 查询角度时，返回结果取决于上次控制命令对应的模式；如需显式指定，可在查询前设定`uservo.servos[servo_id].is_mturn`。
- 多圈相关操作前，应确认舵机具备对应功能，并注意目标角度范围。

### 7.3 状态维护与安全操作
- `reset_multi_turn_angle`须在失锁状态下使用。
- `set_origin_point`仅适用于无刷磁编码舵机V311及之后版本，且须在失锁状态下使用。
- 舵机失锁后仍会响应指令，主控程序应避免误发送运动指令。

### 7.4 参数读写与配置管理
- 如有修改用户自定义参数的需要，可以在上位机进行，更加方便，直观。
- 使用`write_data`前，应先确认目标地址位是否支持写入。
- `reset_user_data`测试时，最好只连接一颗舵机；原文特别说明该指令执行后舵机ID也会被重置为0。

### 7.5 版本兼容性
- 轮转模式在316版本以及后续版本已弃用。
- 同步命令、异步命令、数据监控、控制模式停止指令仅适用于无刷磁编码舵机V316及之后的版本。
- 原点设置仅适用于无刷磁编码舵机V311及之后的版本。

### 7.6 示例迁移建议
- 将文档中的串口号、波特率、舵机ID、功率、速度、时间等参数统一抽取为工程配置项，便于主控程序维护。
- 对批量控制、多舵机同步控制及监控类指令，建议先在单舵机环境下验证，再切换到多舵机场景。
- 对涉及状态查询的循环示例，建议在实际项目中增加超时、异常处理与日志记录机制。
