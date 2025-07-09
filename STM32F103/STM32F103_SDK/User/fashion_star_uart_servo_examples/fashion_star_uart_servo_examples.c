/*
 * UART Servo Example Code
 * ---------------------------------------------------
 * ---------------------------------------------------
 */

#include "fashion_star_uart_servo_examples.h"
#include "math.h"

// Use UART1 as the servo control port
// <Wiring Instructions>
// STM32F103 PA9(Tx)    <----> Servo Transceiver Board Rx
// STM32F103 PA10(Rx)   <----> Servo Transceiver Board Tx
// STM32F103 GND        <----> Servo Transceiver Board GND
// STM32F103 V5         <----> Servo Transceiver Board 5V
// <Notes>
// Make sure USART1_ENABLE is set to 1 in usart.h before use
Usart_DataTypeDef* servo_usart = &usart1; 

// Use UART2 as the logging output port
// <Wiring Instructions>
// STM32F103 PA2(Tx) <----> USB-TTL Rx
// STM32F103 PA3(Rx) <----> USB-TTL Tx
// STM32F103 GND     <----> USB-TTL GND
// STM32F103 V5      <----> USB-TTL 5V (optional)
Usart_DataTypeDef* logging_usart = &usart2;


// Redirect C library function printf to UART, after redirection you can use printf function
int fputc(int ch, FILE *f)
{
    while((logging_usart->pUSARTx->SR&0X40)==0){}
    /* Send one byte of data to UART */
    USART_SendData(logging_usart->pUSARTx, (uint8_t) ch);
    /* Wait for transmission to complete */
    // while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);       
    return (ch);
}


/* Servo Communication Test */
void FSUSExample_PingServo(void)
{

	FSUS_STATUS status_code; // Status code
	uint8_t servo_id = 0;	 // Servo ID = 0

	printf("===Test Uart Servo Ping===r\n");
	while (1)
	{
		// Servo communication test
		status_code = FSUS_Ping(servo_usart, servo_id);
		if (status_code == FSUS_STATUS_SUCCESS)
		{
			printf("Servo Online \r\n");
		}
		else
		{
			printf("Servo Offline,Error Code=%d \r\n", status_code);
		}
		// Delay for 1s
		SysTick_DelayMs(1000);
	}
}

/* Control Single Servo Angle */
void FSUSExample_SetServoAngle(void)
{
	// Servo control related parameters
	// Servo ID number
	uint8_t servo_id = 0;
	// Target angle for the servo
	// Servo angle range is -180 to 180 degrees, minimum unit 0.1
	float angle = 0;
	// Time interval in ms
	// You can try setting a smaller time interval, e.g. 500ms
	uint16_t interval;
	// Target speed
	float velocity;
	// Acceleration time
	uint16_t t_acc;
	// Deceleration time
	uint16_t t_dec;
	// Servo operating power in mV, default is 0
	uint16_t power = 0;
	// Read angle
	float angle_read;

	while (1)
	{
		printf("GOTO: 135.0f\r\n");
        // Simple angle control + current angle query
        angle = 135.0;
        interval = 2000;
        FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // Wait for 2.5s
        SysTick_DelayMs(2500);

        // Angle control with acceleration/deceleration (specified period) + current angle query
        printf("GOTO+Interval: 0.0f\r\n");
        angle = 0.0f;
        interval = 1000;
        t_acc = 100;
        t_dec = 150;
        FSUS_SetServoAngleByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power);
        FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // Wait for 2s
        SysTick_DelayMs(2000);

        // Angle control with acceleration/deceleration (specified speed) + current angle query
        printf("GOTO+Velocity: -135.0f\r\n");
        angle = -135.0f;
        velocity = 200.0f;
        t_acc = 100;
        t_dec = 150;
        FSUS_SetServoAngleByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power);
        FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);
				
				// Wait for 3s
        SysTick_DelayMs(3000);
	}
}

/* Control Multiple Servo Angles */
void FSUSExample_SetNServoAngle(void)
{
	//// Servo control related parameters
	// Time interval in ms
	// You can try setting a smaller time interval, e.g. 500ms
	uint16_t interval = 2000;
	// Servo operating power in mV, default is 0
	uint16_t power = 0;

	while (1)
	{
		// Simple angle control command, control servos 0 and 1
        FSUS_SetServoAngle(servo_usart, 0, 90.0, interval, power);
        FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power);
				
        // Wait for action to complete
        SysTick_DelayMs(interval);

        // Wait for 1s
        SysTick_DelayMs(1000);

        // Simple angle control command, control servos 0 and 1
        FSUS_SetServoAngle(servo_usart, 0, -90.0, interval, power);
        FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power);
        // Wait for action to complete
        SysTick_DelayMs(interval);

        // Wait for 1s
        SysTick_DelayMs(1000);
	}
}

/* Set Servo Angle (Multi-turn Mode) */
void FSUSExample_SetServoAngleMTurn(void)
{
	//// Servo control related parameters
	// Servo ID number
	uint8_t servo_id = 0;
	// Target angle for the servo
	// Servo angle range is -180 to 180 degrees, minimum unit 0.1
	float angle;
	uint32_t interval; // Time interval in ms
	float velocity;	   // Motor speed, in dps (degrees per second)
	// Servo operating power in mV, default is 0
	uint16_t power = 0;
	// Acceleration time (in ms)
	uint16_t t_acc;
	// Deceleration time
	uint16_t t_dec;
	// Read angle
	float angle_read;

	while (1)
	{
		printf("MTurn GOTO: 720.0f\r\n");
        // Simple multi-turn angle control + current multi-turn angle query
        angle = 720.0f;
        interval = 2000;
        FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // Wait for 4s
        SysTick_DelayMs(4000);

			
        printf("MTurn GOTO: 0.0f\r\n");
        angle = 0.0;
        FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // Wait for 3s
        SysTick_DelayMs(3000);


        // Multi-turn angle control with acceleration/deceleration (specified period) + current multi-turn angle query
        printf("MTurn+Interval GOTO: -180.0f\r\n");
        angle = 180.0f; 
        interval = 1000;
        t_acc = 100;
        t_dec = 200;
        FSUS_SetServoAngleMTurnByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // Wait for 2s
        SysTick_DelayMs(2000);

        // Multi-turn angle control with acceleration/deceleration (specified speed) + current multi-turn angle query
        printf("MTurn+Velocity GOTO: -180.0f\r\n");
        angle = -180.0f;
        velocity = 100.0f;
        t_acc = 100;
        t_dec = 200;
        FSUS_SetServoAngleMTurnByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // Wait for 4s
        SysTick_DelayMs(4000);
	}
}

/* Servo Damping Mode and Angle Feedback */
void FSUSExample_SetServoDamping(void)
{
	FSUS_STATUS status_code; // Status code for request packet
	uint8_t servo_id = 0;	 // Servo ID number connected to the transceiver board
	uint16_t power = 500;	 // Power in damping mode, higher power means more resistance
	float angle = 0;		 // Servo angle

	// Set servo to damping mode
	FSUS_DampingMode(servo_usart, servo_id, power);
	while (1)
	{
		// Read the servo angle
		status_code = FSUS_QueryServoAngle(servo_usart, servo_id, &angle);

		if (status_code == FSUS_STATUS_SUCCESS)
		{
			// Successfully read the servo angle
			printf("[INFO] servo id= %d ; angle = %f\r\n", servo_id, angle);
		}
		else
		{
			// Failed to read the servo angle
			printf("\r\n[INFO] read servo %d angle, status code: %d \r\n", servo_id, status_code);
			printf("[ERROR]failed to read servo angle\r\n");
		}
		// Wait for 1000ms
		SysTick_DelayMs(500);
	}
}


/* User-defined Data - Reset */
void FSUSExample_ResetData(void)
{

	// Servo ID number connected to the transceiver board
	uint8_t servo_id = 0;
	// Status code
	FSUS_STATUS status_code;

	// Send reset user data command
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

	// Infinite loop
	while (1)
	{
	}
}

/* Read Servo Status */
void FSUSExample_ReadData(void)
{
	uint8_t servo_id = 0;	// Servo ID number connected to the transceiver board
	FSUS_STATUS statusCode; // Status code

	// Data bytes length in the data table is generally 1 byte/2 bytes/4 bytes
	// According to the communication protocol, the servo angle upper limit data type is signed short integer (UShort, corresponding to int16_t in STM32), length is 2 bytes
	// So set the value data type to int16_t here
	int16_t value;
	uint8_t dataSize;
	// When passing parameters, cast the pointer of value to uint8_t

	// Read voltage
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

	// Read current
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_CURRENT, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, current: %d mA\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}

	// Read power
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_POWER, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		printf("read sucess, power: %d mW\r\n", value);
	}
	else
	{
		printf("fail\r\n");
	}
	// Read temperature
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
	// Read working status
	statusCode = FSUS_ReadData(servo_usart, servo_id, FSUS_PARAM_SERVO_STATUS, (uint8_t *)&value, &dataSize);
	if (statusCode == FSUS_STATUS_SUCCESS)
	{
		// Servo working status flags
		// BIT[0] - Command execution flag, set to 1 when executing, cleared after completion.
		// BIT[1] - Command execution error flag, cleared after next correct execution.
		// BIT[2] - Stall error flag, cleared after stall is resolved.
		// BIT[3] - Overvoltage flag, cleared when voltage returns to normal.
		// BIT[4] - Undervoltage flag, cleared when voltage returns to normal.
		// BIT[5] - Current error flag, cleared when current returns to normal.
		// BIT[6] - Power error flag, cleared when power returns to normal.
		// BIT[7] - Temperature error flag, cleared when temperature returns to normal.

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

	// Infinite loop
	while (1)
	{
	}
}

/* User-defined Data - Write */
void FSUSExample_WriteData(void)
{
	uint8_t servo_id = 0;	 // Servo ID number connected to the transceiver board
	FSUS_STATUS status_code; // Status code
	// Angle limit
	float angle_limit_high = 90.0;	   // Servo angle upper limit (default 180)
	float angle_limit_low = -90.0;	   // Servo angle lower limit (default -180)
	uint8_t angle_limit_switch = 0x01; // 0x01: Enable limit; 0x00: Disable limit

	uint16_t value;

	// Write servo angle upper limit
	value = (int16_t)(angle_limit_high * 10); // Convert servo angle upper limit to 0.1 degree units
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_HIGH, (uint8_t *)&value, 2);
	printf("write angle limit high = %f, status code: %d\r\n", angle_limit_high, status_code);

	// Write servo angle lower limit
	value = (int16_t)(angle_limit_low * 10); // Convert servo angle lower limit to 0.1 degree units
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, (uint8_t *)&value, 2);
	printf("write angle limit low = %f, status code: %d\r\n", angle_limit_low, status_code);

	// Enable servo angle limit switch, configuration takes effect
	status_code = FSUS_WriteData(servo_usart, servo_id, FSUS_PARAM_ANGLE_LIMIT_LOW, &angle_limit_switch, 1);
	printf("enable angle limit mode, status code: %d\r\n", status_code);

	while (1)
	{
		// Control servo angle
		FSUS_SetServoAngle(servo_usart, servo_id, 90.0, 2000, 0);
		FSUS_SetServoAngle(servo_usart, servo_id, -90.0, 2000, 0);
	}
}

/* Synchronous Command Control */
void FSUSExample_SYNC(void)
{
	/* Synchronous command mode selection
* 1: Set servo angle
* 2: Set servo angle (specified period)
* 3: Set servo angle (specified speed)
* 4: Set servo angle (multi-turn mode)
* 5: Set servo angle (multi-turn mode, specified period) 
* 6: Set servo angle (multi-turn mode, specified speed)
* 7: Read servo data*/
uint8_t sync_mode=1;// Synchronous command mode

uint8_t sync_count=5;// Number of servos

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
		// Send synchronous command control
		FSUS_SyncCommand(servo_usart,sync_count,sync_mode,SyncArray);
		SysTick_DelayMs(1000);
		// Send synchronous command read
		FSUS_SyncCommand(servo_usart,sync_count,7,SyncArray);
		SysTick_DelayMs(200);

		SyncArray[0].angle=45;SyncArray[0].interval_single=300;SyncArray[0].velocity=20;
		SyncArray[1].angle=-45;SyncArray[1].interval_single=300;SyncArray[1].velocity=20;
		SyncArray[2].angle=45;SyncArray[2].interval_single=300;SyncArray[2].velocity=20;
		SyncArray[3].angle=-45;SyncArray[3].interval_single=300;SyncArray[3].velocity=20;
		SyncArray[4].angle=-45;SyncArray[4].interval_single=300;SyncArray[4].velocity=20;
		// Send synchronous command control
		FSUS_SyncCommand(servo_usart,sync_count,sync_mode,SyncArray);
		SysTick_DelayMs(1000);
		// Send synchronous command read
		FSUS_SyncCommand(servo_usart,sync_count,7,SyncArray);
		SysTick_DelayMs(200);
	}
}

/* Data Monitoring - Read Servo Parameters */
void FSUSExample_MONTIOR(void)
{
	/* Data monitoring data
* id: Servo ID number
* voltage: Servo voltage
* current: Servo current
* power: Servo operating power
* temperature: Servo temperature 
* status: Servo status
* angle: Servo angle
* circle_count: Servo rotation count*/
ServoData servodata_single[1];// Read data for one servo

// Servo ID number to read
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

/* Control Mode Stop State */
void FSUSExample_StopOnControlMode(void)
{
//0-Release after stop (unlock)
//1-Maintain lock after stop
//2-Enter damping state after stop
uint8_t stopcolmode=2;
	
float	angle = 135.0;// Servo target angle
uint16_t interval = 1000;// Time interval in ms
uint16_t	power = 500;// Servo operating power
uint8_t servo_id=0;// Servo ID number

  FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power);
	SysTick_DelayMs(1000);
	
	// Enter corresponding state after stop
	FSUS_StopOnControlMode(servo_usart, servo_id, stopcolmode, power);
	SysTick_DelayMs(1000);
}

/* Enable Stop Asynchronous */
void FSUSExample_BEGIN_ENDASYNC(void)
{
	// Servo ID number
	uint8_t servo_id = 0;
	// Servo target angle
	// Servo angle range is -180 to 180 degrees, minimum unit 0.1
	float angle;
	uint32_t interval; // Time interval in ms
	uint16_t power = 0;// Servo operating power in mV, default is 0
	float angle_read;// Read angle
	uint8_t async_mode=0; //0: Execute stored command  1: Cancel stored command


		// Enable asynchronous command
		FSUS_BeginAsync(servo_usart);
	
		printf("GOTO: 135.0f\r\n");
    // Simple angle control + current angle query
    angle = 135.0;
    interval = 2000;
    FSUS_SetServoAngle(servo_usart, servo_id, angle, interval, power);
    FSUS_QueryServoAngle(servo_usart, servo_id, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);
		
		printf("*******************\n");
		
	// The above command will not move the servo, it just stores the command, which can be executed or canceled at any time by sending a stop command
	// Wait for 5 seconds
		SysTick_DelayMs(5000);
		
		// Stop asynchronous command
		FSUS_EndAsync(servo_usart,async_mode);
}
