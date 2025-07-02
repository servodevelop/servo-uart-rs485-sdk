/*
 * Fashion Star总线伺服舵机通信协议
 * - 用户自定义表地址
 * - 通信协议结构体
 * - 数据帧的收发与校验
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/05
*/
#ifndef _FS_UART_SERVO_protocol_H
#define _FS_UART_SERVO_protocol_H

#include <iostream>
#include <stdint.h>
#include <time.h>
#include <string>
#include <queue>

#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"



// 延时函数定义
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <math.h>
#endif


namespace fsuservo
{

#define FSUS_DEFAULT_BAUDRATE  itas109::BaudRate115200

// FSUS通信协议
// 注: FSUS是Fashion Star Uart Servo的缩写

// 串口通讯超时设置
#define FSUS_TIMEOUT_MS 100
// 舵机用户自定义数据块的大小 单位Byte
#define FSUS_SERVO_BATCH_DATA_SIZE 32
// 封包的长度
#define PACKAGE_BUFFER_SIZE 250
// 返回的响应数据包最长的长度
#define FSUS_PACK_REQUEST_MAX_SIZE 500
#define FSUS_PACK_RESPONSE_MAX_SIZE 500
// 在串口舵机的通信系统设计里, 使用的字节序为Little Endian(低字节序/小端格式)
// STM32系统的数值存储模式就是Little Endian
// 所以0x4c12 这个数值, 在实际发送的时候低位会在前面 0x12, 0x4c
#define FSUS_PACK_REQUEST_HEADER 0x4c12
#define FSUS_PACK_RESPONSE_HEADER 0x1c05

// FSUS控制指令数据
// 注: 一下所有的指令都是针对单个舵机的
#define FSUS_CMD_NUM 30
#define FSUS_CMD_PING 1                         // 舵机通讯检测
#define FSUS_CMD_RESET_USER_DATA 2              // 重置用户数据
#define FSUS_CMD_READ_DATA 3                    // 单个舵机 读取数据库
#define FSUS_CMD_WRITE_DATA 4                   // 单个舵机 写入数据块
#define FSUS_CMD_READ_BATCH_DATA 5              // 单个舵机 批次读取(读取一个舵机所有的数据)
#define FSUS_CMD_WRITE_BATCH_DATA 6             // 单个舵机 批次写入(写入一个舵机所有的数据)
#define FSUS_CMD_WHEEL 7                        // 单个舵机 设置轮式模式
#define FSUS_CMD_SET_ANGLE 8                    // 角度控制模式(设置舵机的角度))
#define FSUS_CMD_DAMPING 9                      // 阻尼模式
#define FSUS_CMD_QUERY_ANGLE 10                 // 舵机角度读取
#define FSUS_CMD_SET_ANGLE_BY_INTERVAL 11       // 设定角度(指定周期)
#define FSUS_CMD_SET_ANGLE_BY_VELOCITY 12       // 设定转速(指定转速)
#define FSUS_CMD_SET_ANGLE_MTURN 13             // 设定角度(多圈)
#define FSUS_CMD_SET_ANGLE_MTURN_BY_INTERVAL 14 // 设定角度(多圈+指定周期)
#define FSUS_CMD_SET_ANGLE_MTURN_BY_VELOCITY 15 // 设定角度(多圈+指定转速)
#define FSUS_CMD_QUERY_ANGLE_MTURN 16           // 查询角度多圈
#define FSUS_CMD_RESET_MTURN_ANGLE 17           // 复位多圈角度
#define FSUS_CMD_BEGIN_ASYNC 18                 // 开始异步命令
#define FSUS_CMD_END_ASYNC 19                   // 结束异步命令
#define FSUS_CMD_SET_SERVO_ReadData 22          // 舵机数据监控
#define FSUS_CMD_SET_ORIGIN_POINT 23            // 设置零点
#define FSUS_CMD_CONTROL_MODE_STOP 24           // 控制模式停止指令
#define FSUS_CMD_SET_SERVO_SyncCommand 25       // 同步命令

// FSUS状态码
#define FSUS_STATUS uint8_t
#define FSUS_STATUS_SUCCESS 0               // 设置/读取成功
#define FSUS_STATUS_FAIL 1                  // 设置/读取失败
#define FSUS_STATUS_WRONG_RESPONSE_HEADER 3 // 响应头不对
#define FSUS_STATUS_UNKOWN_CMD_ID 4         // 未知的控制指令
#define FSUS_STATUS_SIZE_TOO_BIG 5          // 参数的size大于FSUS_PACK_RESPONSE_MAX_SIZE里面的限制
#define FSUS_STATUS_CHECKSUM_ERROR 6        // 校验和错误
#define FSUS_STATUS_ID_NOT_MATCH 7          // 请求的舵机ID跟反馈回来的舵机ID不匹配
#define FSUS_STATUS_TIMEOUT 8               // 等待超时
#define FSUS_STATUS_ERRO 9                  // 设置同步模式错误

//同步命令的模式选择
#define MODE_SET_SERVO_ANGLE 1                     //设置舵机的角度
#define MODE_SET_SERVO_ANGLE_BY_INTERVAL 2         //设置舵机的角度(指定周期)
#define MODE_SET_SERVO_ANGLE_BY_VELOCITY 3         //设置舵机的角度(指定转速)
#define MODE_SET_SERVO_ANGLE_MTURN 4               //设置舵机的角度(多圈模式)
#define MODE_SET_SERVO_ANGLE_MTURN_BY_INTERVAL 5   //设置舵机的角度(多圈模式, 指定周期)
#define MODE_SET_SERVO_ANGLE_MTURN_BY_VELOCITY 6   //设置舵机的角度(多圈模式, 指定转速)
#define MODE_Query_SERVO_Monitor 7                 //读取舵机的数据*/

/* 用户只读区域 */

/**
 * 电机的电压 (字节长度2, 单位mV)
 */
#define FSUS_PARAM_VOLTAGE 1
/**
 * 电机的电流 (字节长度2, 单位mA)
 */
#define FSUS_PARAM_CURRENT 2
/**
 * 电机的功率 (字节长度2, 单位mW)
 */
#define FSUS_PARAM_POWER 3
/**
 * 电机的温度 (字节长度2, ADC值)
 */
#define FSUS_PARAM_TEMPRATURE 4
// 舵机工作状态标志位
// BIT[0] - 执行指令置1，执行完成后清零。
// BIT[1] - 执行指令错误置1，在下次正确执行后清零。
// BIT[2] - 堵转错误置1，解除堵转后清零。
// BIT[3] - 电压过高置1，电压恢复正常后清零。
// BIT[4] - 电压过低置1，电压恢复正常后清零。
// BIT[5] - 电流错误置1，电流恢复正常后清零。
// BIT[6] - 功率错误置1，功率恢复正常后清零。
// BIT[7] - 温度错误置1，温度恢复正常后清零。
#define FSUS_PARAM_STATUS 5
/* 舵机用户自定义参数的数据ID及使用说明 (可度也可写)*/

/* 此项设置同时具备两个功能
 * 在轮式模式与角度控制模式下
 * 1. 舵机指令是否可以中断 interruptable?
 * 2. 是否产生反馈数据?
 * 0x00(默认)
 *      舵机控制指令执行可以被中断, 新的指令覆盖旧的指令
 *      无反馈数据
 * 0x01
 *      舵机控制指令不可以被中断, 新的指令添加到等候队列里面
 *      等候队列的长度是1, 需要自己在程序里面维护一个队列
 *      当新的控制指令超出了缓冲区的大小之后, 新添加的指令被忽略
 *      指令执行结束之后发送反馈数据
 */
#define FSUS_PARAM_RESPONSE_SWITCH 33
/*
 * 舵机的ID号, (字节长度 1)
 * 取值范围是 0-254
 * 255号为广播地址，不能赋值给舵机 广播地址在PING指令中使用。
 */
#define FSUS_PARAM_SERVO_ID 34
/*
 * 串口通讯的波特率ID  (字节长度 1)
 * 取值范围 [0x01,0x07] , 默认值0x05
 * 0x01-9600,
 * 0x02-19200,
 * 0x03-38400,
 * 0x04-57600,
 * 0x05-115200 (默认波特率),
 * 0x06-250000,
 * 0x07-500000,
 * 波特率设置时即生效
 */
#define FSUS_PARAM_BAUDRATE 36

/* 舵机保护值相关设置, 超过阈值舵机就进入保护模式 */
/*
 * 舵机堵转保护模式  (字节长度 1)
 * 0x00-模式1 降功率到堵转功率上限
 * 0x01-模式2 释放舵机锁力
 */
#define FSUS_PARAM_STALL_PROTECT 37
/* 舵机堵转功率上限, (单位mW) (字节长度 2) */
#define FSUS_PARAM_STALL_POWER_LIMIT 38
/* 舵机电压下限 (单位mV) (字节长度 2) */
#define FSUS_PARAM_OVER_VOLT_LOW 39
/* 舵机电压上限 (单位mV) (字节长度 2) */
#define FSUS_PARAM_OVER_VOLT_HIGH 40
/* 舵机温度上限 (单位 摄氏度) (字节长度 2) */
#define FSUS_PARAM_OVER_TEMPERATURE 41
/* 舵机功率上限 (单位mW) (字节长度 2) */
#define FSUS_PARAM_OVER_POWER 42
/* 舵机电流上限 (单位mA) (字节长度 2) */
#define FSUS_PARAM_OVER_CURRENT 43
/*
 * 舵机启动加速度处理开关 (字节长度 1)
 * 0x00 不启动加速度处理 (无效设置)
 * 0x01 启用加速度处理(默认值)
 *      舵机梯形速度控制,根据时间t推算加速度a
 *      行程前1/4 加速
 *      行程中间1/2保持匀速
 *      行程后1/4
 */
#define FSUS_PARAM_ACCEL_SWITCH 44
/*
 * 舵机上电锁力开关 (字节长度 1)
 * 0x00 上电舵机释放锁力(默认值)
 * 0x11 上电时刹车
 */
#define FSUS_PARAM_POWER_ON_LOCK_SWITCH 46
/*
 * [轮式模式] 轮式模式刹车开关 (字节长度 1)
 * 0x00 停止时舵机释放锁力(默认)
 * 0x01 停止时刹车
 */
#define FSUS_PARAM_WHEEL_MODE_BRAKE_SWITCH 47
/*
 * [角度模式] 角度限制开关 (字节长度 1)
 * 0x00 关闭角度限制
 * 0x01 开启角度限制
 * 注: 只有角度限制模式开启之后, 角度上限下限才有效
 */
#define FSUS_PARAM_ANGLE_LIMIT_SWITCH 48
/*
 * [角度模式] 舵机上电首次角度设置缓慢执行 (字节长度 1)
 * 0x00 关闭
 * 0x01 开启
 * 开启后更安全
 * 缓慢旋转的时间周期即为下方的“舵机上电启动时间”
 */
#define FSUS_PARAM_SOFT_START_SWITCH 49
/*
 * [角度模式] 舵机上电启动时间 (单位ms)(字节长度 2)
 * 默认值: 0x0bb8
 */
#define FSUS_PARAM_SOFT_START_TIME 50
/*
 * [角度模式] 舵机角度上限 (单位0.1度)(字节长度 2)
 */
#define FSUS_PARAM_ANGLE_LIMIT_HIGH 51
/*
 * [角度模式] 舵机角度下限 (单位0.1度)(字节长度 2)
 */
#define FSUS_PARAM_ANGLE_LIMIT_LOW 52
/*
 * [角度模式] 舵机中位角度偏移 (单位0.1度)(字节长度 2)
 */
#define FSUS_PARAM_ANGLE_MID_OFFSET 53

// 帧头接收完成的标志位
#define FSUS_RECV_FLAG_HEADER 0x01
// 控制指令接收完成的标志位
#define FSUS_RECV_FLAG_CMD_ID 0x02
// 内容长度接收完成的标志位
#define FSUS_RECV_FLAG_SIZE 0x04
// 内容接收完成的标志位
#define FSUS_RECV_FLAG_CONTENT 0x08
// 校验和接收的标志位
#define FSUS_RECV_FLAG_CHECKSUM 0x10


// 串口舵机用户自定义设置
// 是否开启响应模式
#define FSUS_IS_RESPONSE_ON 0

// 舵机角度范围
#define FSUS_SERVO_ANGLE_MIN -180.0 // -135.0
#define FSUS_SERVO_ANGLE_MAX 180.0  // 135.0

typedef unsigned char FSUS_SERVO_ID_T;       //舵机ID的格式
typedef unsigned char FSUS_SERVO_COUNT_T;    //舵机的数量
typedef uint8_t FSUS_MODE_T;    //舵机的数量
typedef char *FSUS_SERVO_NAME_T;             //舵机的名称
typedef uint16_t FSUS_INTERVAL_T;            //时间的格式
typedef uint32_t FSUS_INTERVAL_T_MTURN;      //时间的格式(多圈)
typedef uint16_t FSUS_SERVO_POSITION_T;      //舵机位置采样数值格式(2Bit)
typedef int32_t FSUS_SERVO_POSITION_T_MTURN; //舵机位置采样数值格式(多圈)(4Bit)
typedef float FSUS_SERVO_ANGLE_T;            //舵机角度的格式 [-135°, 135°]
typedef float FSUS_SERVO_SPEED_T;            //舵机转速的格式
typedef uint16_t FSUS_POWER_T;               //功率的格式
typedef uint16_t FSUS_HEADER_T;              //帧头
typedef unsigned char FSUS_PACKAGE_SIZE_T;   //Package的长度格式
// typedef unsigned char FSUS_CHECKSUM_T; // 校验和的数据类型
typedef uint8_t FSUS_CHECKSUM_T; // 校验和的数据类型

// 请求数据帧的结构体
typedef struct
{
    uint16_t header;                           // 请求头
    uint8_t cmdId;                             // 指令ID号
    uint16_t content_size;                      // 包的长度
    uint8_t content[FSUS_PACK_RESPONSE_MAX_SIZE]; // 包的内容
    FSUS_CHECKSUM_T checksum;                  // 校验和
    // 接收
    FSUS_STATUS recv_status;                          // Package的状态码
    uint8_t recv_cnt;                                 // 接收字符的计数
    uint8_t recv_buffer[FSUS_PACK_RESPONSE_MAX_SIZE]; // 测试用
} FSUS_PACKAGE_T;

// 舵机的数据监控结构体
typedef struct {
    uint8_t id;
    int16_t voltage;
    int16_t current;
    int16_t power;
    int16_t temperature;
    uint16_t status;
    float angle;
    int16_t circle_count;
} FSUS_ServoMonitor;
/*同步命令的舵机设置参数结构体*/
typedef struct
{
    uint8_t id;
	float angle;
	float velocity;
	uint16_t interval_single;
	uint32_t interval_multi;
	uint16_t t_acc;
	uint16_t t_dec;
	uint16_t power;
}FSUS_Sync_T;


// 串口通信舵机通信协议
class DLL_EXPORT FSUS_Protocol
{
public:
    //请求数据缓冲区
    FSUS_PACKAGE_T requestPack;
    //发送数据的缓冲区
    FSUS_PACKAGE_T responsePack;

    // 构造体
    FSUS_Protocol(std::string port_name, itas109::BaudRate baudrate);

    // 获取当前的时间戳，单位ms
    uint32_t millis();
    // 延时ms
    void delay_ms(uint32_t microsecond);
    // 通过串口发送一个字节
    void write(uint8_t value);
    // 串口当前是否有数据读入，同时也充当了更新串口接收数据的功能
    int available();
    // 读取单个字节
    bool read(uint8_t *value);
    // 读入所有字节
    int readAll(uint8_t *buffer);

    // FSUS_Protocol();
    // void init(); // 资源初始化
    // void init(uint32_t baudrate);
    // void init(String, uint32_t baudrate);
    //总线伺服舵机底层的通信协议相关的函数
    //加工并发送请求数据包
    void sendPack();
    // 清空缓冲区
    void emptyCache();
    // 初始化响应数据包的数据结构
    void initResponsePack();
    //接收响应包
    FSUS_STATUS recvPack();
    //计算CRC校验码
    FSUS_CHECKSUM_T calcPackChecksum(const FSUS_PACKAGE_T *package);
    // 获取请求内容的尺寸
    FSUS_PACKAGE_SIZE_T getPackSize(const FSUS_PACKAGE_T *package);
    // 发送PING的请求包
    void sendPing(FSUS_SERVO_ID_T servoId);
    // 接收PING的响应包
    FSUS_STATUS recvPing(FSUS_SERVO_ID_T *servoId, bool *isOnline);
    // 发送旋转的请求包
    void sendSetAngle(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T interval, FSUS_POWER_T power);
    // 发送旋转的请求包(指定周期)
    void sendSetAngleByInterval(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T interval, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 发送旋转的请求包(指定转速)
    void sendSetAngleByVelocity(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_SERVO_SPEED_T velocity, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 发送舵机角度查询指令
    void sendQueryAngle(FSUS_SERVO_ID_T servoId);
    // 接收角度查询指令
    FSUS_STATUS recvQueryAngle(FSUS_SERVO_ID_T *servoId, FSUS_SERVO_ANGLE_T *angle);
    // 发送旋转的请求包(多圈)
    void sendSetAngleMTurn(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T_MTURN interval, FSUS_POWER_T power);
    // 发送旋转的请求包(多圈+指定周期)
    void sendSetAngleMTurnByInterval(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T_MTURN interval,
                                        FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 发送旋转的请求包(多圈+指定转速)
    void sendSetAngleMTurnByVelocity(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_SERVO_SPEED_T velocity,
                                        FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 发送舵机角度查询指令
    void sendQueryAngleMTurn(FSUS_SERVO_ID_T servoId);
    // 接收角度查询指令(多圈模式)
    FSUS_STATUS recvQueryAngleMTurn(FSUS_SERVO_ID_T *servoId, FSUS_SERVO_ANGLE_T *angle);
    // 发送阻尼模式
    void sendDamping(FSUS_SERVO_ID_T servoId, FSUS_POWER_T power);
    // 发送重置用户数据
    void sendResetUserData(FSUS_SERVO_ID_T servoId);
    // 接收重置用户数据
    FSUS_STATUS recvResetUserData(FSUS_SERVO_ID_T *servoId, bool *result);
    // 发送数据读取指令
    void sendReadData(FSUS_SERVO_ID_T servoId, uint8_t address);
    // 接收数据读取指令
    FSUS_STATUS recvReadData(FSUS_SERVO_ID_T *servoId, uint8_t *address, uint8_t *contentLen, uint8_t *content);
    // 发送数据写入指令
    void sendWriteData(FSUS_SERVO_ID_T servoId, uint8_t address, uint8_t contentLen, uint8_t *content);
    // 接收数据写入指令
    FSUS_STATUS recvWriteData(FSUS_SERVO_ID_T *servoId, uint8_t *address, bool *result);
    // 发送设置原点的请求包
    void sendSetOriginPoint(FSUS_SERVO_ID_T servoId);
    // 发送清除多圈圈数请求包
    void sendResetMultiTurnAngle(FSUS_SERVO_ID_T servoId);
    // 发送舵机开启异步指令（可以存储一次命令）
    void sendServoBeginAsync();
    // 发送舵机停止异步指令
    void sendServoEndAsync(FSUS_MODE_T mode);
    // 发送舵机数据监控查询指令
    void sendServoMonitor(FSUS_SERVO_ID_T servoId);
    // 接收舵机数据监控查询指令
    FSUS_STATUS recvQueryServoMonitor(FSUS_SERVO_ID_T *servoId, FSUS_ServoMonitor *servodata);
    // 发送舵机控制模式停止指令
    void sendStopOnControlMode(FSUS_SERVO_ID_T servoId, FSUS_MODE_T mode, FSUS_POWER_T power);
    // 发送舵机同步控制命令
    void sendSyncCommand(FSUS_SERVO_COUNT_T servo_count,FSUS_MODE_T mode, FSUS_Sync_T Syncsend[]);
    // 接收舵机同步命令下每个舵机的数据
    FSUS_STATUS recvQuerySyncCommand(FSUS_SERVO_COUNT_T servo_count, FSUS_ServoMonitor Syncmonitor[]);

    std::vector<FSUS_Sync_T> Syncsend; // 使用 std::vector 替代灵活数组
    std::vector<FSUS_ServoMonitor> Syncmonitor; // 使用 std::vector 替代灵活数组
private:
    std::string port_name;      // 串口名称
    itas109::BaudRate baudrate; //串口通信的波特率
    // HardwareSerial *serial; //串口
    // Stream *serial;
    itas109::CSerialPort serial;

    std::queue<uint8_t> recv_queue;    // 接收队列
    uint8_t tmp_recv_buffer[1024];      // 临时接收缓冲区

};

}

#endif
