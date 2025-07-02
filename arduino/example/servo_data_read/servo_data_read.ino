/*
 * 舵机数据读取实验
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/14
 **/
#include "FashionStar_UartServoProtocol.h" // 串口总线舵机通信协议
#include "FashionStar_UartServo.h"         // Fashion Star串口总线舵机的依赖

// 配置
#define SERVO_ID 1      // 舵机ID号
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
// HardwareSerial Serial2(PA3, PA2); //这里串口2不需要定义
HardwareSerial Serial3(PB11, PB10);
#define DEBUG_SERIAL Serial1
#define DEBUG_SERIAL_BAUDRATE (uint32_t)115200

#endif

FSUS_Protocol protocol(BAUDRATE);       // 协议
FSUS_Servo uservo(SERVO_ID, &protocol); // 创建舵机

// 读取数据
uint16_t voltage;     // 电压 mV
uint16_t current;     // 电流 mA
uint16_t power;       // 功率 mW
uint16_t temperature; // 温度 ADC
uint8_t status;       // 状态

void setup()
{

    protocol.init(); // 舵机通信协议初始化
    uservo.init();   // 串口总线舵机初始化
    // 打印例程信息
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE);
    DEBUG_SERIAL.println("Start To Test Servo Data Read \n"); // 打印日志

    // uservo.setAngle(-25.0,  1000, 200); // 设置舵机角度(限制功率)
}

void loop()
{
    // 读取电压数据
    voltage = uservo.queryVoltage();
    DEBUG_SERIAL.println("voltage: " + String((float)voltage, 1) + " mV");
    delay(100);
    // 读取电流数据
    current = uservo.queryCurrent();
    DEBUG_SERIAL.println("current: " + String((float)current, 1) + " mA");
    delay(100);
    // 读取功率数据
    power = uservo.queryPower();
    DEBUG_SERIAL.println("power: " + String((float)power, 1) + " mW");
    delay(100);
    // 读取温度数据，需要做ADC转℃
    temperature = uservo.queryTemperature();
    temperature = 1 / (log(temperature / (4096.0f - temperature)) / 3435.0f + 1 / (273.15 + 25)) - 273.15;
    DEBUG_SERIAL.println("temperature: " + String((float)temperature, 1) + " Celsius");

    // 读取工作状态数据
    /*
        BIT[0] - 执行指令置1，执行完成后清零。
        BIT[1] - 执行指令错误置1，在下次正确执行后清零。
        BIT[2] - 堵转错误置1，解除堵转后清零。
        BIT[3] - 电压过高置1，电压恢复正常后清零。
        BIT[4] - 电压过低置1，电压恢复正常后清零。
        BIT[5] - 电流错误置1，电流恢复正常后清零。
        BIT[6] - 功率错误置1，功率恢复正常后清零。
        BIT[7] - 温度错误置1，温度恢复正常后清零。
    */
    status = uservo.queryStatus();
    char binStr[9]; // 8位二进制字符串加上终止符
    for (int i = 7; i >= 0; i--) {
        binStr[7 - i] = (status & (1 << i)) ? '1' : '0';
    }
    binStr[8] = '\0'; // 字符串终止符
    DEBUG_SERIAL.print("WorkState: ");
    DEBUG_SERIAL.println(binStr);
    int bitValue = bitRead(status, 3);
    //判断电压错误标志是否触发
    if (bitValue)
    {
        DEBUG_SERIAL.println("voltage_high");
    }
    bitValue = bitRead(status, 4);
    if (bitValue)
    {
        DEBUG_SERIAL.println("voltage_low");
    }
    delay(100);

    delay(1000);
}
