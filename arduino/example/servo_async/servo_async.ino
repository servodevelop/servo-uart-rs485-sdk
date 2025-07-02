/* 
 * 舵机异步命令
 * 提示: 拓展板上电之后, 记得按下Arduino的RESET按键
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/12/17
 */

#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h" // Fashion Star串口总线舵机的依赖


// 串口总线舵机配置参数
#define SERVO_ID 3 //舵机ID号
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

    float angle; 
    uservo.setRawAngle(0.0);    // 设置舵机的角度      
    uservo.BeginAsync();        //开始异步指令
    delay(1000);
    uservo.setRawAngle(90.0);   // 存入设置舵机角度
    /*支持存入命令：
     设置舵机原始角度
     设置舵机的原始角度(指定周期)
     设定舵机的原始角度(指定转速)
     设定舵机的原始角度(多圈)
     设定舵机的原始角度(多圈+指定周期)
     设定舵机的原始角度(多圈+指定转速)
    */
    delay(1000);
    uservo.EndAsync(0);         //结束异步，执行存入的指令
    delay(1000);

    uservo.BeginAsync();        //开始异步指令
    delay(1000);
    uservo.setRawAngle(180.0);   // 存入设置舵机角度
    delay(1000);
    uservo.EndAsync(1);         //结束异步，执行存入的指令
    delay(1000);

}
