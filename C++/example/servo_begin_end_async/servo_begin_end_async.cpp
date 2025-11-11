/*
 * 舵机存储一次命令的应用
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2025/01/05
 */

#include <iostream>

#include "CSerialPort/SerialPort.h"
#include "FashionStar/UServo/FashionStar_UartServoProtocol.h"
#include "FashionStar/UServo/FashionStar_UartServo.h"

// 总线伺服舵机配置
// 设置总线伺服Servo转接板的端口号
#ifdef _WIN32
#define SERVO_PORT_NAME "COM8" 	        // Windows下端口号名称 COM{}
#else
#define SERVO_PORT_NAME "/dev/ttyUSB0" 	// Linux下端口号名称 /dev/ttyUSB{}
#endif
#define SERVO_ID 0 				        // 舵机ID号
#define DAMPING_POWER 800 		// 阻尼模式的功率

using namespace std;
using namespace fsuservo;

// 创建协议对象
FSUS_Protocol protocol(SERVO_PORT_NAME, FSUS_DEFAULT_BAUDRATE);
// 创建一个舵机对象
FSUS_Servo servo0(SERVO_ID, &protocol);

// 轨迹规划参数定义
uint16_t interval;  // 运行周期 单位ms
uint16_t t_acc;     // 加速时间 单位ms
uint16_t t_dec;     // 减速时间 单位ms
float velocity;         // 目标转速 单位°/s

uint8_t endasync_mode = 0;//0执行存储命令，1取消存储命令

/* 等待并报告当前的角度*/
void waitAndReport(FSUS_Servo* servo){
	cout << "Real Angle = " << servo->curRawAngle << ", Target Angle = " << servo->targetRawAngle << endl;
    protocol.delay_ms(2000); // 暂停2s
}

int main(){
	// 打印例程信息
	cout << "Star Async" << endl;
    servo0.SetBeginAsync();

    cout << "---------First send---------" << endl;
	cout << "Set Angle = 90 deg" << endl;
    servo0.setRawAngle(90.0);  // 设置舵机的角度
    waitAndReport(&servo0);

    protocol.delay_ms(2000); // 暂停2s

    cout << "---------Second send---------" << endl;
    servo0.SetEndAsync(endasync_mode);//0执行存储命令，1取消存储命令

}
