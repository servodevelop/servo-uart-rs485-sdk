/* 
 * 舵机数据监控
 * 提示: 拓展板上电之后, 记得按下Arduino的RESET按键
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/17
 */

#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h" // Fashion Star串口总线舵机的依赖


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


uint16_t Voltage;
uint16_t Current;
uint16_t Power;
uint16_t Temperature;
uint8_t Status;
float Angle;
uint8_t Turns;

/* 等待并报告当前的角度*/
void waitAndReport(){
    uservo.wait();          // 等待舵机旋转到目标角度
    DEBUG_SERIAL.println("Real Angle = " + String(uservo.curRawAngle, 1) + " Target Angle = "+String(uservo.targetRawAngle, 1));
    delay(2000); // 暂停2s
}

void setup(){
    protocol.init(); // 通信协议初始化
    uservo.init(); //舵机角度初始化
    // 打印例程信息
   DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE); // 初始化软串口的波特率
   DEBUG_SERIAL.println("Set Servo Angle");
}

void loop(){

ServoMonitorData data = uservo.ServoMonitor();

    // 打印舵机监控数据
    if (data.isValid) {
        DEBUG_SERIAL.println("Servo Monitor Data (Valid):");
        DEBUG_SERIAL.print("Servo ID: ");
        DEBUG_SERIAL.println(data.servoId);
        DEBUG_SERIAL.print("Voltage: ");
        DEBUG_SERIAL.println(data.voltage);
        DEBUG_SERIAL.print("Current: ");
        DEBUG_SERIAL.println(data.current);
        DEBUG_SERIAL.print("Power: ");
        DEBUG_SERIAL.println(data.power);
        DEBUG_SERIAL.print("Temperature: ");
        DEBUG_SERIAL.println(data.temperature);
        DEBUG_SERIAL.print("Status: ");
        DEBUG_SERIAL.println(data.status);
        DEBUG_SERIAL.print("Angle: ");
        DEBUG_SERIAL.println(data.angle);
        DEBUG_SERIAL.print("Turns: ");
        DEBUG_SERIAL.println(data.turns);
        delay(2000);
    } else {
        DEBUG_SERIAL.println("Failed to receive valid servo data.");
        delay(2000);
    }
}

