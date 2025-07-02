/*
 * FashionStar总线伺服舵机 C++ SDK
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/05
 */
#ifndef _FS_UART_SERVO_H
#define _FS_UART_SERVO_H

#include <iostream>
#include <stdint.h>
#include <time.h>
#include <string>

#include "FashionStar_UartServoProtocol.h"

namespace fsuservo
{

#define FSUS_K_ANGLE_REAL2RAW 1
#define FSUS_B_ANGLE_REAL2RAW 0
#define FSUS_SERVO_SPEED 100.0 // 舵机角度的默认转速
#define FSUS_ANGLE_CTL_DEADBLOCK 1.5 // 舵机控制的死区
#define FSUS_WAIT_TIMEOUT_MS 60000 /s/ 等待的时间上限 ms

class DLL_EXPORT FSUS_Servo{
public:
    FSUS_Protocol *protocol; // 舵机串口通信协议

    //单个舵机数据监控的信息
    FSUS_ServoMonitor servomonitor;
    //同步命令参数结构体
    FSUS_Sync_T Syncsend[5];
    //同步命令舵机数据监控到的信息
    FSUS_ServoMonitor Syncmonitor[5];

    FSUS_SERVO_ID_T servoId; //舵机ID
    bool isOnline; //舵机是否在线
    bool isMTurn; // 舵机是否是多圈模式
    float kAngleReal2Raw; // 舵机标定数据-舵机角度与位置之间的比例系数
    float bAngleReal2Raw; // 舵机标定数据-舵机角度与位置转换过程中的偏移量

    FSUS_MODE_T sync_mode;// 舵机的同步模式

    FSUS_SERVO_ANGLE_T curAngle; // 真实的当前角度
    FSUS_SERVO_ANGLE_T targetAngle; // 真实的目标角度

    FSUS_SERVO_ANGLE_T curRawAngle;     // 当前的原始角度
    FSUS_SERVO_ANGLE_T targetRawAngle;  // 目标原始角度

    FSUS_SERVO_ANGLE_T angleMin; //舵机角度的最小值
    FSUS_SERVO_ANGLE_T angleMax; // 舵机角度最大值
    FSUS_SERVO_SPEED_T speed; // 舵机转速 单位dps °/s

    // 构造函数
    FSUS_Servo(uint8_t servoId, FSUS_Protocol *protocol);
    void init(); // 初始化
    void init(uint8_t servoId, FSUS_Protocol *protocol);
    //舵机通讯检测
    bool ping();
    // 舵机标定
    void calibration(FSUS_SERVO_ANGLE_T rawA, FSUS_SERVO_ANGLE_T realA, FSUS_SERVO_ANGLE_T rawB, FSUS_SERVO_ANGLE_T realB);
    void calibration(float kAngleReal2Raw, float bAngleReal2Raw);
    // 真实角度转化为原始角度
    FSUS_SERVO_ANGLE_T angleReal2Raw(FSUS_SERVO_ANGLE_T realAngle);
    // 原始角度转换为真实角度
    FSUS_SERVO_ANGLE_T angleRaw2Real(FSUS_SERVO_ANGLE_T rawAngle);
    // 查询舵机的角度
    FSUS_SERVO_ANGLE_T queryAngle();
    // 查询舵机原始的角度
    FSUS_SERVO_ANGLE_T queryRawAngle();
    // 查询舵机的原始角度(多圈)
    FSUS_SERVO_ANGLE_T queryRawAngleMTurn();
    // 范围是否合法
    bool isAngleLegal(FSUS_SERVO_ANGLE_T candiAngle);
    // 设置舵机的角度范围
    void setAngleRange(FSUS_SERVO_ANGLE_T angleMin, FSUS_SERVO_ANGLE_T angleMax);
    // 设置舵机的平均转速
    void setSpeed(FSUS_SERVO_SPEED_T speed);
    // 设置舵机角度
    void setAngle(FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T interval, FSUS_POWER_T power);
    void setAngle(FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T interval);
    void setAngle(FSUS_SERVO_ANGLE_T angle);
    // 设置舵机原始角度
    void setRawAngle(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T interval, FSUS_POWER_T power);
    void setRawAngle(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T interval);
    void setRawAngle(FSUS_SERVO_ANGLE_T rawAngle);
    // 设置舵机的原始角度(指定周期)
    void setRawAngleByInterval(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T interval, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 设定舵机的原始角度(指定转速)
    void setRawAngleByVelocity(FSUS_SERVO_ANGLE_T rawAngle, FSUS_SERVO_SPEED_T velocity, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 设定舵机的原始角度(多圈)
    void setRawAngleMTurn(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T_MTURN interval, FSUS_POWER_T power);
    void setRawAngleMTurn(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T_MTURN interval);
    void setRawAngleMTurn(FSUS_SERVO_ANGLE_T rawAngle);
    // 设定舵机的原始角度(多圈+指定周期)
    void setRawAngleMTurnByInterval(FSUS_SERVO_ANGLE_T rawAngle, FSUS_INTERVAL_T_MTURN interval, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 设定舵机的原始角度(多圈+指定转速)
    void setRawAngleMTurnByVelocity(FSUS_SERVO_ANGLE_T rawAngle, FSUS_SERVO_SPEED_T velocity, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power);
    // 查询舵机的电压(单位mV)
    uint16_t queryVoltage();
    // 查询舵机的电流(单位mA)
    uint16_t queryCurrent();
    // 查询舵机的功率(单位mW)
    uint16_t queryPower();
    // 查询舵机的温度(ADC值)
    uint16_t queryTemperature();
    // 查询舵机的工作状态
    uint8_t queryStatus();
    // 设置阻尼模式
    void setDamping(FSUS_POWER_T power);
    /* 设置舵机为阻尼模式, 默认功率为500mW*/
    void setDamping();
    // 舵机是否在旋转
    bool isStop();
    /*设置原点*/
    void SetOriginPoint();
    /*重设多圈圈数*/
    void ResetMultiTurnAngle();
    /*开启异步*/
    void SetBeginAsync();
    /*停止异步*/
    void SetEndAsync(uint8_t mode);
    /*舵机数据监控读取舵机信息*/
    uint16_t querymonitor();
    /*控制模式停止指令*/
    void SetStopOnControlMode(uint8_t mode, FSUS_POWER_T power);

    /* 设置舵机同步的原始角度 */
    void setSyncRawAngle(FSUS_SERVO_COUNT_T servo_count,FSUS_Sync_T servoSync[]);
    // 设置舵机同步的原始角度(指定周期)
    void setSyncRawAngleByInterval(FSUS_SERVO_COUNT_T servo_count,FSUS_Sync_T servoSync[]);
    // 设定舵机同步的原始角度(指定转速)
    void setSyncRawAngleByVelocity(FSUS_SERVO_COUNT_T servo_count,FSUS_Sync_T servoSync[]);
    // 设定舵机同步的原始角度(多圈)
    void setSyncRawAngleMTurn(FSUS_SERVO_COUNT_T servo_count,FSUS_Sync_T servoSync[]);
    // 设定舵机同步的原始角度(多圈+指定周期)
    void setSyncRawAngleMTurnByInterval(FSUS_SERVO_COUNT_T servo_count,FSUS_Sync_T servoSync[]);
    // 设定舵机同步的原始角度(多圈+指定转速)
    void setSyncRawAngleMTurnByVelocity(FSUS_SERVO_COUNT_T servo_count,FSUS_Sync_T servoSync[]);
    //设定舵机同步的数据监控
    uint16_t querySyncMonitor(FSUS_SERVO_COUNT_T servo_count);

private:

};// NOTE：类的末尾要加;

}

#endif