/*
 * 同步命令演示
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/05
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

uint8_t set_count=6;//舵机发送数量
uint8_t data_count=1;//舵机读取数量

int main(){
	// 打印例程信息
	cout << "SYNC SEND" << endl;

    servo0.Syncsend[0].id=0;
    servo0.Syncsend[0].angle=90;servo0.Syncsend[0].velocity=100.0;
    servo0.Syncsend[0].interval_single=1000;servo0.Syncsend[0].interval_multi=1000;
    servo0.Syncsend[0].t_acc=20;servo0.Syncsend[0].t_dec=20;servo0.Syncsend[0].power=500;
    servo0.Syncsend[1].id=1;
    servo0.Syncsend[1].angle=90;servo0.Syncsend[1].velocity=100.0;
    servo0.Syncsend[1].interval_single=1000;servo0.Syncsend[1].interval_multi=1000;
    servo0.Syncsend[1].t_acc=20;servo0.Syncsend[1].t_dec=20;servo0.Syncsend[1].power=500;
    servo0.Syncsend[2].id=2;
    servo0.Syncsend[2].angle=90;servo0.Syncsend[2].velocity=100.0;
    servo0.Syncsend[2].interval_single=1000;servo0.Syncsend[2].interval_multi=1000;
    servo0.Syncsend[2].t_acc=20;servo0.Syncsend[2].t_dec=20;servo0.Syncsend[2].power=500;
    servo0.Syncsend[3].id=3;
    servo0.Syncsend[3].angle=90;servo0.Syncsend[3].velocity=100.0;
    servo0.Syncsend[3].interval_single=1000;servo0.Syncsend[3].interval_multi=1000;
    servo0.Syncsend[3].t_acc=20;servo0.Syncsend[3].t_dec=20;servo0.Syncsend[3].power=500;
    servo0.Syncsend[4].id=4;
    servo0.Syncsend[4].angle=90;servo0.Syncsend[4].velocity=100.0;
    servo0.Syncsend[4].interval_single=1000;servo0.Syncsend[4].interval_multi=1000;
    servo0.Syncsend[4].t_acc=20;servo0.Syncsend[4].t_dec=20;servo0.Syncsend[4].power=500;

    servo0.setSyncRawAngle(set_count,servo0.Syncsend);//设置舵机同步的原始角度
    // servo0.setSyncRawAngleByInterval(count,servo0.Syncsend);//设置舵机同步的原始角度(指定周期)
    // servo0.setSyncRawAngleByVelocity(count,servo0.Syncsend);// 设定舵机同步的原始角度(指定转速)
    // servo0.setSyncRawAngleMTurn(count,servo0.Syncsend);// 设定舵机同步的原始角度(多圈)
    // servo0.setSyncRawAngleMTurnByInterval(count,servo0.Syncsend);// 设定舵机同步的原始角度(多圈+指定周期)
    // servo0.setSyncRawAngleMTurnByVelocity(count,servo0.Syncsend);// 设定舵机同步的原始角度(多圈+指定转速)
    protocol.delay_ms(2000);

    servo0.querySyncMonitor(data_count);//设定舵机同步的数据监控

    for (uint8_t i = 0; i < data_count; i++)
        {
            cout << "Test Servo ******************************************************" << endl;
            cout << "Servo ID[" << static_cast<int>(i) << "]: " << static_cast<int>(servo0.Syncmonitor[i].id) << endl;
            cout << "Voltage[" << static_cast<int>(i) << "]: " << servo0.Syncmonitor[i].voltage << " mV" << endl;
            cout << "Current[" << static_cast<int>(i) << "]: " << servo0.Syncmonitor[i].current << " mA" << endl;
            cout << "Power[" << static_cast<int>(i) << "]: " << servo0.Syncmonitor[i].power << " mW" << endl;
            cout << "Temperature[" << static_cast<int>(i) << "]: " << servo0.Syncmonitor[i].temperature << " °C" << endl;
            cout << "Status[" << static_cast<int>(i) << "]: " << static_cast<int>(servo0.Syncmonitor[i].status) << endl;
            cout << "Angle[" << static_cast<int>(i) << "]: " << servo0.Syncmonitor[i].angle << " °" << endl;
            cout << "Circle Count[" << static_cast<int>(i) << "]: " << static_cast<int>(servo0.Syncmonitor[i].circle_count) << endl;
        }


    protocol.delay_ms(2000);

}