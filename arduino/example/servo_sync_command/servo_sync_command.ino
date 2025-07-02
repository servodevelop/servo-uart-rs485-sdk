/* 
 * 同步控制、读取数据
 * 提示: 拓展板上电之后, 记得按下Arduino的RESET按键
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/30
 */

#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h" // Fashion Star总线伺服舵机的依赖

// 串口总线舵机配置参数
#define SERVO_ID 0 //舵机ID号
#define BAUDRATE 115200 // 波特率

// 调试串口的配置
#if defined(ARDUINO_AVR_UNO)
    #include <SoftwareSerial.h>
    #define SOFT_SERIAL_RX 6 
    #define SOFT_SERIAL_TX 7
    SoftwareSerial softSerial(SOFT_SERIAL_RX, SOFT_SERIAL_TX); // 创建软串口
    #define DEBUG_SERIAL softSerial
    #define DEBUG_SERIAL_BAUDRATE 115200
    
#elif defined(ARDUINO_AVR_MEGA2560)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200
    
#elif defined(ARDUINO_ARCH_ESP32)
    #define DEBUG_SERIAL Serial
    #define DEBUG_SERIAL_BAUDRATE 115200
    
#elif defined(ARDUINO_ARCH_STM32)
    #include <HardwareSerial.h>
    //                      RX    TX
    HardwareSerial Serial1(PA10, PA9);
    //HardwareSerial Serial2(PA3, PA2); //这里串口2不需要定义
    HardwareSerial Serial3(PB11, PB10);
    #define DEBUG_SERIAL Serial1
    #define DEBUG_SERIAL_BAUDRATE (uint32_t)115200
    
#endif 

FSUS_Protocol protocol(BAUDRATE); //协议
FSUS_Servo uservo(SERVO_ID, &protocol); // 创建舵机

FSUS_sync_servo servoSyncArray[18]; // 

void setup(){
    protocol.init(); // 通信协议初始化
    uservo.init(); //舵机角度初始化
    // 打印例程信息
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE); // 初始化软串口的波特率
}
int mode;
int count;
ServoMonitorData servodata[1];
void loop(){
    mode = 1;
    /*  mode 1:单圈角度控制
        mode 2:单圈角度模式-指定时间
        mode 3:单圈角度模式-指定速度
        mode 4:多圈角度模式
        mode 5:多圈角度模式-指定时间
        mode 5:多圈角度模式-指定速度
        注意：Arduino UNO板子的RAM只有2k，目前只支持12个舵机同步
    */
    count = 12;
 for (int i = 0; i < count; i++) {
    servoSyncArray[i].servoId = i;
    servoSyncArray[i].angle = 90;
    servoSyncArray[i].interval = 1000;
    servoSyncArray[i].interval_multiturn = 1000;
    servoSyncArray[i].velocity = 360;
    servoSyncArray[i].t_acc = 100;
    servoSyncArray[i].t_dec = 100;
    servoSyncArray[i].power = 0;
}
    uservo.SyncCommand(count,mode, servoSyncArray);
    delay(2000);

    uservo.SyncMonitorCommand(count, servoSyncArray,servodata);
    delay(2000);
        for (int i = 0; i < count; i++) {
        DEBUG_SERIAL.println("id:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].servoId);

        DEBUG_SERIAL.println("voltage:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].voltage);

        DEBUG_SERIAL.println("current:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].current);

        DEBUG_SERIAL.println("power:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].power);

        DEBUG_SERIAL.println("temperature:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].temperature);

        DEBUG_SERIAL.println("status:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].status);

        DEBUG_SERIAL.println("angle:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].angle);

        DEBUG_SERIAL.println("turns:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].turns);
        delay(1000);
    }
    delay(2000);

    mode = 1;
    count = 12;
 for (int i = 0; i < count; i++) {
    servoSyncArray[i].servoId = i;
    servoSyncArray[i].angle = 0;
    servoSyncArray[i].interval = 1000;
    servoSyncArray[i].interval_multiturn = 1000;
    servoSyncArray[i].velocity = 360;
    servoSyncArray[i].t_acc = 100;
    servoSyncArray[i].t_dec = 100;
    servoSyncArray[i].power = 0;
}
    uservo.SyncCommand(count,mode, servoSyncArray);
    delay(2000);
    uservo.SyncMonitorCommand(count,servoSyncArray,servodata);
    delay(2000);
        for (int i = 0; i <= count; i++) {
        DEBUG_SERIAL.println("id:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].servoId);

        DEBUG_SERIAL.println("voltage:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].voltage);

        DEBUG_SERIAL.println("current:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].current);

        DEBUG_SERIAL.println("power:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].power);

        DEBUG_SERIAL.println("temperature:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].temperature);

        DEBUG_SERIAL.println("status:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].status);

        DEBUG_SERIAL.println("angle:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].angle);

        DEBUG_SERIAL.println("turns:");  // 打印每个 syncmonitorData
        DEBUG_SERIAL.println(servodata[i].turns);
        delay(1000);
    }
    delay(2000);

}
