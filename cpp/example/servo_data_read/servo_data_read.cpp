/*
 * 读取舵机的状态
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/05
 **/
#include <iostream>
#include <string>

#include "CSerialPort/SerialPort.h"
#include "FashionStar/UServo/FashionStar_UartServoProtocol.h"
#include "FashionStar/UServo/FashionStar_UartServo.h"

// 总线伺服舵机配置
// 设置总线伺服Servo转接板的端口号
#ifdef _WIN32
#define SERVO_PORT_NAME "COM8" 	        // Windows下端口号名称 COM{}
#else
#define SERVO_PORT_NAME "/dev/ttyAMA0" 	// Linux下端口号名称 /dev/ttyUSB{}
#endif
#define SERVO_ID 0 				        // 舵机ID号

using namespace std;
using namespace fsuservo;

// 创建协议对象
FSUS_Protocol protocol(SERVO_PORT_NAME, FSUS_DEFAULT_BAUDRATE);
// 创建一个舵机对象
FSUS_Servo servo0(SERVO_ID, &protocol);


int main(){
	// 打印例程信息
	cout << "Servo Data Read" << endl;
	// 设置舵机角度(限制功率)
	// servo0.setAngle(0.0,  1000, 800);

	while(true){
		// 信息查询
		uint16_t voltage = servo0.queryVoltage();
		uint16_t current = servo0.queryCurrent();
		uint16_t power = servo0.queryPower();
		uint16_t temperature = servo0.queryTemperature();
		uint8_t status = servo0.queryStatus();

		// 打印状态信息
		cout << "voltage(mV): " << voltage << endl;
		cout << "current(mA): " << current << endl;
		cout << "power(mW): " << power << endl;
		cout << "temperature(ADC): " << temperature << endl;
        // 舵机工作状态标志位
        // BIT[0] - 执行指令置1，执行完成后清零。
        // BIT[1] - 执行指令错误置1，在下次正确执行后清零。
        // BIT[2] - 堵转错误置1，解除堵转后清零。
        // BIT[3] - 电压过高置1，电压恢复正常后清零。
        // BIT[4] - 电压过低置1，电压恢复正常后清零。
        // BIT[5] - 电流错误置1，电流恢复正常后清零。
        // BIT[6] - 功率错误置1，功率恢复正常后清零。
        // BIT[7] - 温度错误置1，温度恢复正常后清零。
		cout << "voltage high:"<<((status >> 3) & 0x01)<<endl;
		cout << "voltage low:"<<((status >> 4) & 0x01)<<endl;
		// 延时1s
    	protocol.delay_ms(1000);
	}
}