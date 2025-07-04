/*
 * 串口总线舵机示例代码
 * ---------------------------------------------------
 * ---------------------------------------------------
 */

#include "fashion_star_uart_servo_examples.h"
#include "math.h"

// 使用串口1作为舵机控制的端口
// <接线说明>
// STM32F103 PA9(Tx)    <----> 总线伺服舵机转接板 Rx
// STM32F103 PA10(Rx)   <----> 总线伺服舵机转接板 Tx
// STM32F103 GND        <----> 总线伺服舵机转接板 GND
// STM32F103 V5         <----> 总线伺服舵机转接板 5V
// <注意事项>
// 使用前确保已设置usart.h里面的USART1_ENABLE为1
Usart_DataTypeDef* servo_usart = &usart1; 

// 使用串口2作为日志输出的端口
// <接线说明>
// STM32F103 PA2(Tx) <----> USB转TTL Rx
// STM32F103 PA3(Rx) <----> USB转TTL Tx
// STM32F103 GND     <----> USB转TTL GND
// STM32F103 V5      <----> USB转TTL 5V (可选)
Usart_DataTypeDef* logging_usart = &usart2;


// 重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    while((logging_usart->pUSARTx->SR&0X40)==0){}
    /* 发送一个字节数据到串口 */
    USART_SendData(logging_usart->pUSARTx, (uint8_t) ch);
    /* 等待发送完毕 */
    // while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);       
    return (ch);
}


/* 舵机通讯检测 */
void FSUSExample_PingServo(void)
{

	FSUS_STATUS status_code; // 状态码
	uint8_t servo_id = 0;	 // 舵机ID = 0

	printf("===Test Uart Servo Ping===r\n");
	while (1)
	{
		// 舵机通信检测
		status_code = FSUS_Ping(servo_usart, servo_id);
		if (status_code == FSUS_STATUS_SUCCESS)
		{
			printf("Servo Online \r\n");
		}
		else
		{
			printf("Servo Offline,Error Code=%d \r\n", status_code);
		}
		// 延时等待1s
		SysTick_DelayMs(1000);
	}
}

/* 控制单个舵机的角度 */
void FSUSExample_SetServoAngle(void)
{
	// 舵机控制相关的参数
	// 舵机的ID号
	uint8_t servo_id = 0;
	// 舵机的目标角度
	// 舵机角度在-180度到180度之间, 最小单位0.1
	float angle = 0;
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval;
	// 目标转速
	float velocity;
	// 加速时间
	uint16_t t_acc;
	// 减速时间
	uint16_t t_dec;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 读取的角度
	float angle_read;

	while (1)
	{
		printf("GOTO: 135.0f\r\n");
        // 简易角度控制 + 当前角度查询
        angle = 135.0;
        interval = 2000;
        FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待2.5s
        SysTick_DelayMs(2500);

        // 带加减速的角度控制(指定周期) + 当前角度查询
        printf("GOTO+Interval: 0.0f\r\n");
        angle = 0.0f;
        interval = 1000;
        t_acc = 100;
        t_dec = 150;
        FSUS_SetServoAngleByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power);
        FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待2s
        SysTick_DelayMs(2000);

        // 带加减速的角度控制(指定转速) + 当前角度查询
        printf("GOTO+Velocity: -135.0f\r\n");
        angle = -135.0f;
        velocity = 200.0f;
        t_acc = 100;
        t_dec = 150;
        FSUS_SetServoAngleByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power);
        FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);
				
				// 等待3s
        SysTick_DelayMs(3000);
	}
}

/* 控制多个舵机的角度 */
void FSUSExample_SetNServoAngle(void)
{
	//// 舵机控制相关的参数
	// 时间间隔ms
	// 可以尝试修改设置更小的时间间隔，例如500ms
	uint16_t interval = 2000;
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;

	while (1)
	{
		// 简易角度控制指令，控制0和1号舵机
        FSUS_SetServoAngle(servo_usart, 0, 135.0, interval, power);
        FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power);
				
        // 等待动作完成
        SysTick_DelayMs(interval);

        // 等待1s
        SysTick_DelayMs(1000);

        // 简易角度控制指令，控制0和1号舵机
        FSUS_SetServoAngle(servo_usart, 0, -135.0, interval, power);
        FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power);
        // 等待动作完成
        SysTick_DelayMs(interval);

        // 等待1s
        SysTick_DelayMs(1000);
	}
}

/* 设置舵机角度(多圈模式) */
void FSUSExample_SetServoAngleMTurn(void)
{
	//// 舵机控制相关的参数
	// 舵机的ID号
	uint8_t servo_id = 0;
	// 舵机的目标角度
	// 舵机角度在-180度到180度之间, 最小单位0.1
	float angle;
	uint32_t interval; // 时间间隔ms
	float velocity;	   // 电机转速, 单位dps,°/s
	// 舵机执行功率 mV 默认为0
	uint16_t power = 0;
	// 加速时间(单位ms)
	uint16_t t_acc;
	// 减速时间
	uint16_t t_dec;
	// 读取的角度
	float angle_read;

	while (1)
	{
		printf("MTurn GOTO: 720.0f\r\n");
        //简易多圈角度控制 + 当前多圈角度查询
        angle = 720.0f;
        interval = 2000;
        FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待4s
        SysTick_DelayMs(4000);

			
        printf("MTurn GOTO: 0.0f\r\n");
        angle = 0.0;
        FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待3s
        SysTick_DelayMs(3000);


        //带加减速的多圈角度控制(指定周期) + 当前多圈角度查询
        printf("MTurn+Interval GOTO: -180.0f\r\n");
        angle = 180.0f; 
        interval = 1000;
        t_acc = 100;
        t_dec = 200;
        FSUS_SetServoAngleMTurnByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待2s
        SysTick_DelayMs(2000);

        //带加减速的多圈角度控制(指定转速) + 当前多圈角度查询
        printf("MTurn+Velocity GOTO: -180.0f\r\n");
        angle = -180.0f;
        velocity = 100.0f;
        t_acc = 100;
        t_dec = 200;
        FSUS_SetServoAngleMTurnByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待4s
        SysTick_DelayMs(4000);
	}
}

/* 舵机阻尼模式与角度回传 */
void FSUSExample_SetServoDamping(void)
{
	FSUS_STATUS status_code; // 请求包的状态码
	uint8_t servo_id = 0;	 // 连接在转接板上的总线伺服舵机ID号
	uint16_t power = 500;	 // 阻尼模式下的功率，功率越大阻力越大
	float angle = 0;		 // 舵机的角度

	// 设置舵机为阻尼模式
	FSUS_DampingMode(servo_usart, servo_id, power);
	while (1)
	{
		// 读取一下舵机的角度
		status_code = FSUS_QueryServoAngle(servo_usart, servo_id, &angle);

		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// 成功的读取到了舵机的角度
			printf("[INFO] servo id= %d ; angle = %f\r\n", servo_id, angle);
		}
		else
		{
			// 没有正确的读取到舵机的角度
			printf("\r\n[INFO] read servo %d angle, status code: %d \r\n", servo_id, status_code);
			printf("[ERROR]failed to read servo angle\r\n");
		}
		// 等待1000ms
		SysTick_DelayMs(500);
	}
}


/* 用户自定义数据-重置 */
void FSUSExample_ResetData(void)
{

	// 连接在转接板上的总线伺服舵机ID号
	uint8_t servo_id = 0;
	// 状态码
	FSUS_STATUS status_code;

	// 发送重置用户数据指令
	status_code = FSUS_ResetUserData(servo_usart, servo_id);
	printf("=====reset user data======\r\n status code: %d\r\n", status_code);
	if (status_code == FSUS_STATUS_SUCCESS)
	{
		printf("sucess\r\n");
	}
	else
	{
		printf("fail\r\n");
	}

	// 死循环
	while (1)
	{
	}
}

/*读取舵机状态*/
void FSUSExample_ReadData(void)
{
	uint8_t servo_id = 0;	// 连接在转接板上的总线伺服舵机ID号
	FSUS_STATUS statusCode; // 状态码

	// 数据表里面的数据字节长度一般为1个字节/2个字节/4个字节
	// 查阅通信协议可知,舵机角度上限的数据类型是有符号短整型(UShort, 对应STM32里面的int16_t),长度为2个字节
	// 所以这里设置value的数据类型为int16_t
	int16_t value;
	uint8_t dataSize;
	// 传参数的时候, 要将value的指针强行转换为uint8_t

	// 读取电压
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_VOLTAGE, (uint8_t *)&value, &dataSize);

	printf("read ID: %d\r\n", servo_id);

	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, voltage: %d mV\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}

	// 读取电流
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_CURRENT, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, current: %d mA\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}

	// 读取功率
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_POWER, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, power: %d mW\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}
	// 读取温度
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_TEMPRATURE, (uint8_t *)&value, &dataSize);
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
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_SERVO_STATUS, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		// 舵机工作状态标志位
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

	// 死循环
	while (1)
	{
	}
}

/* 用户自定义数据-写入 */
void FSUSExample_WriteData(void)
{
	uint8_t servo_id = 0;	 // 连接在转接板上的总线伺服舵机ID号
	FSUS_STATUS status_code; // 状态码
	// 限位角度
	float angle_limit_high = 90.0;	   // 舵机角度上限 (默认值 180)
	float angle_limit_low = -90.0;	   // 舵机角度下限 (默认值-180)
	uint8_t angle_limit_switch = 0x01; // 0x01: 开启限位; 0x00: 关闭限位

	uint16_t value;

	// 写入舵机角度上限
	value = (int16_t)(angle_limit_high * 10); // 舵机角度上限 转换单位为0.1度
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_HIGH, (uint8_t *)&value, 2);
	printf("write angle limit high = %f, status code: %d\r\n", angle_limit_high, status_code);

	// 写入舵机角度下限制
	value = (int16_t)(angle_limit_low * 10); // 舵机角度下限 转换单位为0.1度
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, (uint8_t *)&value, 2);
	printf("write angle limit low = %f, status code: %d\r\n", angle_limit_low, status_code);

	// 打开舵机角度限位开关, 配置生效
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, &angle_limit_switch, 1);
	printf("enable angle limit mode, status code: %d\r\n", status_code);

	while (1)
	{
		// 控制舵机角度
		FSUS_SetServoAngle(servo_usart, servo_id, 90.0, 2000, 0);
		FSUS_SetServoAngle(servo_usart, servo_id, -90.0, 2000, 0);
	}
}

/* 同步命令控制 */
void FSUSExample_SYNC(void)
{
	/*同步命令模式选择
* 1：设置舵机的角度
* 2：设置舵机的角度(指定周期)
* 3：设置舵机的角度(指定转速)
* 4：设置舵机的角度(多圈模式)
* 5：设置舵机的角度(多圈模式, 指定周期) 
* 6：设置舵机的角度(多圈模式, 指定转速)
* 7：读取舵机的数据*/
uint8_t sync_mode=1;//同步命令模式

uint8_t sync_count=5;//舵机数量

	while(1)
	{
		SyncArray[0].angle=90;
		SyncArray[0].id=0;SyncArray[0].interval_single=300;SyncArray[0].interval_multi=1000;SyncArray[0].velocity=100;SyncArray[0].t_acc=100;SyncArray[0].t_dec=100;
		SyncArray[1].angle=-90;
		SyncArray[1].id=1;SyncArray[1].interval_single=300;SyncArray[1].interval_multi=1000;SyncArray[1].velocity=100;SyncArray[1].t_acc=100;SyncArray[1].t_dec=100;
		SyncArray[2].angle=90;
		SyncArray[2].id=2;SyncArray[2].interval_single=300;SyncArray[2].interval_multi=1000;SyncArray[2].velocity=100;SyncArray[2].t_acc=100;SyncArray[2].t_dec=100;
		SyncArray[3].angle=-90;
		SyncArray[3].id=3;SyncArray[3].interval_single=300;SyncArray[3].interval_multi=1000;SyncArray[3].velocity=100;SyncArray[3].t_acc=100;SyncArray[3].t_dec=100;
		SyncArray[4].angle=-90;
		SyncArray[4].id=4;SyncArray[4].interval_single=300;SyncArray[4].interval_multi=1000;SyncArray[4].velocity=100;SyncArray[4].t_acc=100;SyncArray[4].t_dec=100;
		//发送同步命令控制
		FSUS_SyncCommand(servo_usart,sync_count,sync_mode,SyncArray);
		SysTick_DelayMs(1000);
		//发送同步命令读取
		FSUS_SyncCommand(servo_usart,sync_count,7,SyncArray);
		SysTick_DelayMs(200);

		SyncArray[0].angle=45;SyncArray[0].interval_single=300;SyncArray[0].velocity=20;
		SyncArray[1].angle=-45;SyncArray[1].interval_single=300;SyncArray[1].velocity=20;
		SyncArray[2].angle=45;SyncArray[2].interval_single=300;SyncArray[2].velocity=20;
		SyncArray[3].angle=-45;SyncArray[3].interval_single=300;SyncArray[3].velocity=20;
		SyncArray[4].angle=-45;SyncArray[4].interval_single=300;SyncArray[4].velocity=20;
		//发送同步命令控制
		FSUS_SyncCommand(servo_usart,sync_count,sync_mode,SyncArray);
		SysTick_DelayMs(1000);
		//发送同步命令读取
		FSUS_SyncCommand(servo_usart,sync_count,7,SyncArray);
		SysTick_DelayMs(200);
	}
}

/* 数据监控读取舵机参数 */
void FSUSExample_MONTIOR(void)
{
	/*数据监控的数据
* id：舵机的id号
* voltage：舵机的电压
* current：舵机的电流
* power：舵机的执行功率
* temperature：舵机的温度 
* status：舵机的状态
* angle：舵机的角度
* circle_count：舵机的转动圈数*/
ServoData servodata_single[1];//读取一个舵机的数据

//要读取的舵机id号
uint8_t servo_id=0;
	
	FSUS_DampingMode(servo_usart,servo_id,500);
	while(1)
	{
			FSUS_ServoMonitor(servo_usart,servo_id,servodata_single);
			printf("read ID: %d\r\n", servodata_single[0].id);
			printf("read sucess, voltage: %d mV\r\n", servodata_single[0].voltage);
			printf("read sucess, current: %d mA\r\n", servodata_single[0].current);
			printf("read sucess, power: %d mW\r\n", servodata_single[0].power);
			printf("read sucess, temperature: %d \r\n", servodata_single[0].temperature);
			if ((servodata_single[0].status >> 3) & 0x01)
			printf("read sucess, voltage too high\r\n");
			if ((servodata_single[0].status >> 4) & 0x01)
			printf("read sucess, voltage too low\r\n");
			printf("read sucess, angle: %f\r\n", servodata_single[0].angle);
			printf("read sucess, circle_count: %d\r\n", servodata_single[0].circle_count);
			SysTick_DelayMs(1000);
	}
}

/* 控制模式停止状态 */
void FSUSExample_StopOnControlMode(void)
{
//0-停止后卸力(失锁)
//1-停止后保持锁力
//2-停止后进入阻尼状态
uint8_t stopcolmode=2;
	
float	angle = 135.0;// 舵机的目标角度
uint16_t interval = 1000;// 时间间隔ms
uint16_t	power = 500;// 舵机执行功率
uint8_t servo_id=0;// 舵机的ID号

  FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power);
	SysTick_DelayMs(1000);
	
	//停止后进入对应状态
	FSUS_StopOnControlMode(servo_usart, servo_id, stopcolmode, power);
	SysTick_DelayMs(1000);
}

/* 开启停止异步 */
void FSUSExample_BEGIN_ENDASYNC(void)
{
	// 舵机的ID号
	uint8_t servo_id = 0;
	// 舵机的目标角度
	// 舵机角度在-180度到180度之间, 最小单位0.1
	float angle;
	uint32_t interval; // 时间间隔ms
	uint16_t power = 0;// 舵机执行功率 mV 默认为0
	float angle_read;// 读取的角度
	uint8_t async_mode=0; //0:执行存储的命令  1:取消存储的命令


		//开启异步命令
		FSUS_BeginAsync(servo_usart);
	
		printf("GOTO: 135.0f\r\n");
    // 简易角度控制 + 当前角度查询
    angle = 135.0;
    interval = 2000;
    FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power);
    FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);
		
		printf("*******************\n");
		
	//发送上面的命令是不会动的，只是存储了命令，之后任意时刻发送停止命令将执行这个命令or取消存储
	//等待5秒
		SysTick_DelayMs(5000);
		
		//停止异步命令
		FSUS_EndAsync(servo_usart,async_mode);
}


