/*
 * 读取舵机所有数据信息
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2021/06/12
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

using namespace std;
using namespace fsuservo;

// 创建协议对象
FSUS_Protocol protocol(SERVO_PORT_NAME, FSUS_DEFAULT_BAUDRATE);
// 创建一个舵机对象
FSUS_Servo servo0(SERVO_ID, &protocol);


int main(){

    while (1)
    {
    servo0.querymonitor(); // 读取舵机数据
	// 打印例程信息
	cout << "*********************Test Servo monitor**************" << endl;
    cout << "Servo ID: " << static_cast<int>(servo0.servomonitor.id) << endl;
    cout << "Voltage: " << servo0.servomonitor.voltage << " mV" << endl;
    cout << "Current: " << servo0.servomonitor.current << " mA" << endl;
    cout << "Power: " << servo0.servomonitor.power << " mW" << endl;
    cout << "Temperature: " << servo0.servomonitor.temperature << " °C" << endl;
    cout << "Status: " << static_cast<int>(servo0.servomonitor.status) << endl;
    cout << "Angle: " << servo0.servomonitor.angle << " °" << endl;
    cout << "Circle Count: " << static_cast<int>(servo0.servomonitor.circle_count) << endl;

    protocol.delay_ms(2000);
    }

}