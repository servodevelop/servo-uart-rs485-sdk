#include "demo.h"
#include "stdio.h"
#include "user_uart.h"

#define ID 0 // 舵机的ID号

// 这些参数并非所有例程都会用到，根据具体接口的要求来设置
uint8_t statusCode;
float angle;
float nextAngle = 0;   // 舵机的目标角度
uint16_t power = 1000; // 舵机执行功率 单位mV 默认为0
uint16_t speed = 300;  // 舵机的转速 单位 °/s
uint16_t interval = 0; // 舵机旋转的周期
uint16_t t_acc;        // 加速时间
uint16_t t_dec;        // 减速时间
float angle_read;      // 读取的角度
uint8_t is_cw = 0;     // 舵机的旋转方向
uint16_t nTime = 3000; // 延时时间
uint16_t nCircle = 1;  // 舵机旋转的圈数
ServoData servodata_single[1];//读取一个舵机的数据

uint8_t sync_mode=1;     //自行更改数值设置模式
uint8_t sync_count=5;    //舵机数量
uint8_t async_mode=0;    //0：执行存储的命令     1：取消存储的命令


int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

/********************************************************
 * 测试通信检测指令，测试舵机是否在线
 ********************************************************/
void demo_ping()
{
    printf("\r\n");
    // Ping一下舵机
    printf("[INFO]ping servo %d \r\n", ID);
    statusCode = FSUS_Ping(&FSUS_usart1, ID);
    printf("[INFO]status code %d \r\n", statusCode);

    // 根据状态码做不同的处理
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        printf("[INFO]ping success, servo %d echo \r\n", ID);
    }
    else
    {
        printf("[ERROR]ping fail, servo %d not online \r\n", ID);
    }
    // 等待1000ms
    HAL_Delay(1000);
}

/********************************************************
 * 设置单个舵机的角度
 *  简易角度控制 + 当前角度查询
 *  带加减速的角度控制(指定周期) + 当前角度查询
 *  带加减速的角度控制(指定转速) + 当前角度查询
 * 串口2每隔一段时间打印一下舵机角度信息。
 ********************************************************/
void demo_setAngle_SingleServo()
{
    printf("GOTO: 135.0f\r\n");
    // 简易角度控制 + 当前角度查询
    angle = 135.0;
    interval = 2000;
    FSUS_SetServoAngle(&FSUS_usart1, ID, angle, interval, power);
    FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // 等待2s
    HAL_Delay(3000);

    // 带加减速的角度控制(指定周期) + 当前角度查询
    printf("GOTO+Interval: 0.0f\r\n");
    angle = 0.0f;
    interval = 1000;
    t_acc = 100;
    t_dec = 150;
    FSUS_SetServoAngleByInterval(&FSUS_usart1, ID, angle, interval, t_acc, t_dec, power);
    FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // 等待2s
    HAL_Delay(2000);

    // 带加减速的角度控制(指定转速) + 当前角度查询
    printf("GOTO+speed: -135.0f\r\n");
    angle = -135.0f;
    speed = 200.0f;
    t_acc = 100;
    t_dec = 150;
    FSUS_SetServoAngleByVelocity(&FSUS_usart1, ID, angle, speed, t_acc, t_dec, power);
    FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);
		
		HAL_Delay(2000);
}
/********************************************************
 * 设置多个舵机的角度
 * 演示如何使用简易角度控制指令来控制多个舵机。
 ********************************************************/
void demo_setAngle_MultiServo()
{
    // 简易角度控制指令，控制0和1号舵机
    angle = 135.0;
    FSUS_SetServoAngle(&FSUS_usart1, 0, angle, interval, power);
    angle = 45.0;
    FSUS_SetServoAngle(&FSUS_usart1, 1, angle, interval, power);
    // 等待动作完成
    HAL_Delay(interval);

    // 等待2s
    HAL_Delay(2000);

    // 简易角度控制指令，控制0和1号舵机
    angle = -135.0;
    FSUS_SetServoAngle(&FSUS_usart1, 0, angle, interval, power);
    angle = -45.0;
    FSUS_SetServoAngle(&FSUS_usart1, 1, angle, interval, power);
    // 等待动作完成
    HAL_Delay(interval);

    // 等待2s
    HAL_Delay(2000);
}
/********************************************************
 * 例程演示了多圈角度控制以及查询实时多圈角度的API使用方法
 * 简易多圈角度控制+ 当前多圈角度查询
 * 带加减速的多圈角度控制(指定周期)+ 当前多圈角度查询
 * 带加减速的多圈角度控制(指定转速)+ 当前多圈角度查询
 ********************************************************/
void demo_setAngle_MultiCircle()
{
    printf("MTurn GOTO: 720.0f\r\n");
    // 简易多圈角度控制 + 当前多圈角度查询
    angle = 720.0f;
    interval = 2000;
    FSUS_SetServoAngleMTurn(&FSUS_usart1, ID, angle, interval, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // 等待2s
    HAL_Delay(2000);

    printf("MTurn GOTO: 0.0f\r\n");
    angle = 0.0;
    FSUS_SetServoAngleMTurn(&FSUS_usart1, ID, angle, interval, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // 等待2s
    HAL_Delay(2000);

    // 带加减速的多圈角度控制(指定周期) + 当前多圈角度查询
    printf("MTurn+Interval GOTO: -180.0f\r\n");
    angle = 180.0f;
    interval = 1000;
    t_acc = 100;
    t_dec = 200;
    FSUS_SetServoAngleMTurnByInterval(&FSUS_usart1, ID, angle, interval, t_acc, t_dec, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // 等待2s
    HAL_Delay(2000);

    // 带加减速的多圈角度控制(指定转速) + 当前多圈角度查询
    printf("MTurn+speed GOTO: -180.0f\r\n");
    angle = -180.0f;
    speed = 100.0f;
    t_acc = 100;
    t_dec = 200;
    FSUS_SetServoAngleByVelocity(&FSUS_usart1, ID, angle, speed, t_acc, t_dec, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // 等待2s
    HAL_Delay(2000);
}

/********************************************************
 * 设置舵机为阻尼模式，同时请求舵机的角度。在旋转舵机的情况下，每隔一段时间就更新一下舵机的角度。
 * 串口2每隔一段时间打印一下舵机角度信息。
 ********************************************************/
void demo_DampingModeAndQueryAngle()
{
    // 启用阻尼模式，给一次指令即可，重复发送会应用最新的指令。
    FSUS_DampingMode(&FSUS_usart1, ID, power);
    // 读取一下舵机的角度
    statusCode = FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle);

    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        // 成功的读取到了舵机的角度
        printf("[INFO] servo id= %d ; angle = %f\r\n", ID, angle);
    }
    else
    {
        // 没有正确的读取到舵机的角度
        printf("\r\n[INFO] read servo %d angle, status code: %d \r\n", ID, statusCode);
        printf("[ERROR]failed to read servo angle\r\n");
    }
    // 等待500ms
    HAL_Delay(500);
}

/********************************************************
 * 读取舵机的实时状态，并且给出了判断工作状态异常的示例
 * 电压
 * 电流
 * 功率
 * 温度
 * 工作状态标志位
    // 舵机工作状态标志位
    // BIT[0] - 执行指令置1，执行完成后清零。
    // BIT[1] - 执行指令错误置1，在下次正确执行后清零。
    // BIT[2] - 堵转错误置1，解除堵转后清零。
    // BIT[3] - 电压过高置1，电压恢复正常后清零。
    // BIT[4] - 电压过低置1，电压恢复正常后清零。
    // BIT[5] - 电流错误置1，电流恢复正常后清零。
    // BIT[6] - 功率错误置1，功率恢复正常后清零。
    // BIT[7] - 温度错误置1，温度恢复正常后清零。
 ********************************************************/
void demo_QueryServoStatus()
{
    // 读取用户自定义数据
    // 数据表里面的数据字节长度一般为1个字节/2个字节/4个字节
    // 查阅通信协议可知,舵机角度上限的数据类型是有符号短整型(UShort, 对应STM32里面的int16_t),长度为2个字节
    // 所以这里设置value的数据类型为int16_t
    int16_t value;
    uint8_t dataSize;
    // 传参数的时候, 要将value的指针强行转换为uint8_t

    // 读取电压
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_VOLTAGE, (uint8_t *)&value, &dataSize);

    printf("read ID: %d\r\n", ID);

    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        printf("read sucess, voltage: %d mV\r\n", value);
    }
    else
    {
        printf("fail\r\n");
    }

    // 读取电流
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_CURRENT, (uint8_t *)&value, &dataSize);
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        printf("read sucess, current: %d mA\r\n", value);
    }
    else
    {
        printf("fail\r\n");
    }

    // 读取功率
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_POWER, (uint8_t *)&value, &dataSize);
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        printf("read sucess, power: %d mW\r\n", value);
    }
    else
    {
        printf("fail\r\n");
    }
    // 读取温度
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_TEMPRATURE, (uint8_t *)&value, &dataSize);
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        double temperature, temp;
        temp = (double)value;
        temperature = 1 / (log(temp / (4096.0f - temp)) / 3435.0f + 1 / (273.15 + 25)) - 273.15;
        printf("read sucess, temperature: %f\r\n", temperature);
    }
    else
    {
        printf("fail\r\n");
    }
    // 读取工作状态
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_SERVO_STATUS, (uint8_t *)&value, &dataSize);
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        // 舵机工作状态
        // BIT[0] - 执行指令置1，执行完成后清零。
        // BIT[1] - 执行指令错误置1，在下次正确执行后清零。
        // BIT[2] - 堵转错误置1，解除堵转后清零。
        // BIT[3] - 电压过高置1，电压恢复正常后清零。
        // BIT[4] - 电压过低置1，电压恢复正常后清零。
        // BIT[5] - 电流错误置1，电流恢复正常后清零。
        // BIT[6] - 功率错误置1，功率恢复正常后清零。
        // BIT[7] - 温度错误置1，温度恢复正常后清零。

        if ((value >> 3) & 0x01)
            printf("read sucess, voltage too high\r\n");
        if ((value >> 4) & 0x01)
            printf("read sucess, voltage too low\r\n");
    }
    else
    {
        printf("fail\r\n");
    }
    printf("================================= \r\n");
    HAL_Delay(2000);
}
/********************************************************
* 设置舵机为同步命令，控制舵机按不同参数旋转，
	旋转舵机的情况下，每隔一段时间就读取舵机的数据。
 ********************************************************/
void demo_SYNC(void)
{
		SyncArray[0].angle=90;
		SyncArray[0].id=0;SyncArray[0].interval_single=100;SyncArray[0].interval_multi=1000;SyncArray[0].velocity=100;SyncArray[0].t_acc=20;SyncArray[0].t_dec=20;
		SyncArray[1].angle=-90;
		SyncArray[1].id=1;SyncArray[1].interval_single=100;SyncArray[1].interval_multi=1000;SyncArray[1].velocity=100;SyncArray[1].t_acc=20;SyncArray[1].t_dec=20;
		SyncArray[2].angle=90;
		SyncArray[2].id=2;SyncArray[2].interval_single=100;SyncArray[2].interval_multi=1000;SyncArray[2].velocity=100;SyncArray[2].t_acc=20;SyncArray[2].t_dec=20;
		SyncArray[3].angle=-90;
		SyncArray[3].id=3;SyncArray[3].interval_single=100;SyncArray[3].interval_multi=1000;SyncArray[3].velocity=100;SyncArray[3].t_acc=20;SyncArray[3].t_dec=20;
		SyncArray[4].angle=-90;
		SyncArray[4].id=4;SyncArray[4].interval_single=100;SyncArray[4].interval_multi=1000;SyncArray[4].velocity=100;SyncArray[4].t_acc=20;SyncArray[4].t_dec=20;
		//发送同步命令控制
		FSUS_SyncCommand(&FSUS_usart1,sync_count,sync_mode,SyncArray);
		HAL_Delay(1000);
		//发送同步命令读取
		FSUS_SyncCommand(&FSUS_usart1,sync_count,7,SyncArray);
		HAL_Delay(200);

		SyncArray[0].angle=45;SyncArray[0].interval_single=0;SyncArray[0].velocity=20;
		SyncArray[1].angle=-45;SyncArray[1].interval_single=0;SyncArray[1].velocity=20;
		SyncArray[2].angle=45;SyncArray[2].interval_single=0;SyncArray[2].velocity=20;
		SyncArray[3].angle=-45;SyncArray[3].interval_single=0;SyncArray[3].velocity=20;
		SyncArray[4].angle=-45;SyncArray[4].interval_single=0;SyncArray[4].velocity=20;
		//发送同步命令控制
		FSUS_SyncCommand(&FSUS_usart1,sync_count,sync_mode,SyncArray);
		HAL_Delay(1000);
		//发送同步命令读取
		FSUS_SyncCommand(&FSUS_usart1,sync_count,7,SyncArray);
		HAL_Delay(200);
}

/********************************************************
 * 设置舵机为数据监控，每隔两秒就读取舵机的数据。
 * 
 ********************************************************/
void demo_MONTIOR(void)
{
	//每两秒读取一次并打印
	FSUS_ServoMonitor(&FSUS_usart1,ID,servodata_single);
	printf("read ID: %d\r\n", servodata_single[0].id);
	printf("read sucess, voltage: %d mV\r\n", servodata_single[0].voltage);
	printf("read sucess, current: %d mA\r\n", servodata_single[0].current);
	printf("read sucess, power: %d mW\r\n", servodata_single[0].power);
	printf("read sucess, temperature: %d \r\n", servodata_single[0].temperature);
	if ((servodata_single[0].status >> 3) & 0x01)
	printf("read sucess, voltage too high\r\n");
	if ((servodata_single[0].status >> 4) & 0x01)
	printf("read sucess, voltage too low\r\n");
	printf("read sucess, angle: %.1f\r\n", servodata_single[0].angle);
	printf("read sucess, circle_count: %d\r\n", servodata_single[0].circle_count);
	HAL_Delay(1000);
}
/********************************************************
 * 控制舵机转动角度后，进入控制模式停止状态
 * 
 ********************************************************/
void demo_StopOnControlMode(void)
{
//0-停止后卸力(失锁)
//1-停止后保持锁力
//2-停止后进入阻尼状态
uint8_t stopcolmode=2;
	
	angle = 135.0;
  interval = 1000;
	power = 500;
  FSUS_SetServoAngle(&FSUS_usart1, ID, angle, interval, power);
	HAL_Delay(2000);
	
	//停止后进入阻尼状态
	FSUS_StopOnControlMode(&FSUS_usart1,ID,stopcolmode,power);
	HAL_Delay(1000);
}

/********************************************************
 * 设置舵机开启异步模式，发送一次角度命令后，等待5秒再次发送
 * 只能存储一次，如果需要请再次开启异步，执行完停止异步
 ********************************************************/
void demo_Begin_EndAsync(void)
{
		//开启异步命令
		FSUS_BeginAsync(&FSUS_usart1);
	
		printf("GOTO: 135.0f\r\n");
    // 简易角度控制 + 当前角度查询
    angle = 135.0;
    interval = 2000;
    FSUS_SetServoAngle(&FSUS_usart1, ID, angle, interval, power);
		FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);
	
		printf("*******************\n");
		
		//发送一次上面的命令是不会动的，只是存储了命令
		//等待5秒之后发送停止异步命令
		HAL_Delay(5000);
		
		//停止异步命令
		FSUS_EndAsync(&FSUS_usart1,async_mode);
}
