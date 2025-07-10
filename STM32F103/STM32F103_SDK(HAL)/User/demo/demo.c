#include "demo.h"
#include "stdio.h"
#include "user_uart.h"

#define ID 0 // Servo ID number

// These parameters are not used in all examples, set them according to the requirements of specific interfaces
uint8_t statusCode;
float angle;
float nextAngle = 0;   // Servo target angle
uint16_t power = 1000; // Servo execution power in mV, default is 0
uint16_t speed = 300;  // Servo rotation speed in °/s
uint16_t interval = 0; // Servo rotation period
uint16_t t_acc;        // Acceleration time
uint16_t t_dec;        // Deceleration time
float angle_read;      // Read angle
uint8_t is_cw = 0;     // Servo rotation direction
uint16_t nTime = 3000; // Delay time
uint16_t nCircle = 1;  // Servo rotation circles
ServoData servodata_single[1];// Data for reading one servo

uint8_t sync_mode=1;     // Change the value to set the mode
uint8_t sync_count=5;    // Number of servos
uint8_t async_mode=0;    // 0: Execute stored commands, 1: Cancel stored commands


int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xffff);
    return ch;
}

/********************************************************
 * Test the communication detection command to check if the servo is online
 ********************************************************/
void demo_ping()
{
    printf("\r\n");
    // Ping the servo
    printf("[INFO]ping servo %d \r\n", ID);
    statusCode = FSUS_Ping(&FSUS_usart1, ID);
    printf("[INFO]status code %d \r\n", statusCode);

    // Handle different status codes
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        printf("[INFO]ping success, servo %d echo \r\n", ID);
    }
    else
    {
        printf("[ERROR]ping fail, servo %d not online \r\n", ID);
    }
    // Wait for 1000ms
    HAL_Delay(1000);
}

/********************************************************
 * Set the angle of a single servo
 *  Simple angle control + current angle query
 *  Angle control with acceleration and deceleration (specified period) + current angle query
 *  Angle control with acceleration and deceleration (specified speed) + current angle query
 *  UART2 prints the servo angle information at regular intervals.
 ********************************************************/
void demo_setAngle_SingleServo()
{
    printf("GOTO: 135.0f\r\n");
    // Simple angle control + current angle query
    angle = 135.0;
    interval = 2000;
    FSUS_SetServoAngle(&FSUS_usart1, ID, angle, interval, power);
    FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // Wait for 2s
    HAL_Delay(3000);

    // Angle control with acceleration and deceleration (specified period) + current angle query
    printf("GOTO+Interval: 0.0f\r\n");
    angle = 0.0f;
    interval = 1000;
    t_acc = 100;
    t_dec = 150;
    FSUS_SetServoAngleByInterval(&FSUS_usart1, ID, angle, interval, t_acc, t_dec, power);
    FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // Wait for 2s
    HAL_Delay(2000);

    // Angle control with acceleration and deceleration (specified speed) + current angle query
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
 * Set the angles of multiple servos
 * Demonstrate how to use simple angle control commands to control multiple servos.
 ********************************************************/
void demo_setAngle_MultiServo()
{
    // Simple angle control command, control servos 0 and 1
    angle = 135.0;
    FSUS_SetServoAngle(&FSUS_usart1, 0, angle, interval, power);
    angle = 45.0;
    FSUS_SetServoAngle(&FSUS_usart1, 1, angle, interval, power);
    // Wait for the action to complete
    HAL_Delay(interval);

    // Wait for 2s
    HAL_Delay(2000);

    // Simple angle control command, control servos 0 and 1
    angle = -135.0;
    FSUS_SetServoAngle(&FSUS_usart1, 0, angle, interval, power);
    angle = -45.0;
    FSUS_SetServoAngle(&FSUS_usart1, 1, angle, interval, power);
    // Wait for the action to complete
    HAL_Delay(interval);

    // Wait for 2s
    HAL_Delay(2000);
}
/********************************************************
 * This example demonstrates the API usage for multi-turn angle control and querying real-time multi-turn angles
 * Simple multi-turn angle control + current multi-turn angle query
 * Angle control with acceleration and deceleration (specified period) + current multi-turn angle query
 * Angle control with acceleration and deceleration (specified speed) + current multi-turn angle query
 ********************************************************/
void demo_setAngle_MultiCircle()
{
    printf("MTurn GOTO: 720.0f\r\n");
    // Simple multi-turn angle control + current multi-turn angle query
    angle = 720.0f;
    interval = 2000;
    FSUS_SetServoAngleMTurn(&FSUS_usart1, ID, angle, interval, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // Wait for 2s
    HAL_Delay(2000);

    printf("MTurn GOTO: 0.0f\r\n");
    angle = 0.0;
    FSUS_SetServoAngleMTurn(&FSUS_usart1, ID, angle, interval, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // Wait for 2s
    HAL_Delay(2000);

    // Angle control with acceleration and deceleration (specified period) + current multi-turn angle query
    printf("MTurn+Interval GOTO: -180.0f\r\n");
    angle = 180.0f;
    interval = 1000;
    t_acc = 100;
    t_dec = 200;
    FSUS_SetServoAngleMTurnByInterval(&FSUS_usart1, ID, angle, interval, t_acc, t_dec, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // Wait for 2s
    HAL_Delay(2000);

    // Angle control with acceleration and deceleration (specified speed) + current multi-turn angle query
    printf("MTurn+speed GOTO: -180.0f\r\n");
    angle = -180.0f;
    speed = 100.0f;
    t_acc = 100;
    t_dec = 200;
    FSUS_SetServoAngleByVelocity(&FSUS_usart1, ID, angle, speed, t_acc, t_dec, power);
    FSUS_QueryServoAngleMTurn(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);

    // Wait for 2s
    HAL_Delay(2000);
}

/********************************************************
 * Set the servo to damping mode and request the servo angle. Update the servo angle at regular intervals while rotating the servo.
 * UART2 prints the servo angle information at regular intervals.
 ********************************************************/
void demo_DampingModeAndQueryAngle()
{
    // Enable damping mode, a single command is enough, sending it repeatedly will apply the latest command.
    FSUS_DampingMode(&FSUS_usart1, ID, power);
    // Read the servo angle
    statusCode = FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle);

    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        // Successfully read the servo angle
        printf("[INFO] servo id= %d ; angle = %f\r\n", ID, angle);
    }
    else
    {
        // Failed to correctly read the servo angle
        printf("\r\n[INFO] read servo %d angle, status code: %d \r\n", ID, statusCode);
        printf("[ERROR]failed to read servo angle\r\n");
    }
    // Wait for 500ms
    HAL_Delay(500);
}

/********************************************************
 * Read the real-time status of the servo and provide an example of judging abnormal working status
 * Voltage
 * Current
 * Power
 * Temperature
 * Working status flags
    // Servo working status flags
    // BIT[0] - Set to 1 when executing a command, cleared after completion.
    // BIT[1] - Set to 1 when command execution error, cleared after correct execution.
    // BIT[2] - Set to 1 when stall error, cleared after stall is resolved.
    // BIT[3] - Set to 1 when voltage is too high, cleared after voltage returns to normal.
    // BIT[4] - Set to 1 when voltage is too low, cleared after voltage returns to normal.
    // BIT[5] - Set to 1 when current error, cleared after current returns to normal.
    // BIT[6] - Set to 1 when power error, cleared after power returns to normal.
    // BIT[7] - Set to 1 when temperature error, cleared after temperature returns to normal.
 ********************************************************/
void demo_QueryServoStatus()
{
    // Read user-defined data
    // The data byte length in the data table is generally 1 byte/2 bytes/4 bytes
    // According to the communication protocol, the servo angle upper limit data type is a signed short integer (UShort, corresponding to int16_t in STM32), with a length of 2 bytes
    // So set the data type of value to int16_t
    int16_t value;
    uint8_t dataSize;
    // When passing parameters, forcibly cast the pointer of value to uint8_t

    // Read voltage
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

    // Read current
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_CURRENT, (uint8_t *)&value, &dataSize);
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        printf("read sucess, current: %d mA\r\n", value);
    }
    else
    {
        printf("fail\r\n");
    }

    // Read power
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_POWER, (uint8_t *)&value, &dataSize);
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        printf("read sucess, power: %d mW\r\n", value);
    }
    else
    {
        printf("fail\r\n");
    }
    // Read temperature
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
    // Read working status
    statusCode = FSUS_ReadData(&FSUS_usart1, ID, FSUS_PARAM_SERVO_STATUS, (uint8_t *)&value, &dataSize);
    if (statusCode == FSUS_STATUS_SUCCESS)
    {
        // Servo working status
        // BIT[0] - Set to 1 when executing a command, cleared after completion.
        // BIT[1] - Set to 1 when command execution error, cleared after correct execution.
        // BIT[2] - Set to 1 when stall error, cleared after stall is resolved.
        // BIT[3] - Set to 1 when voltage is too high, cleared after voltage returns to normal.
        // BIT[4] - Set to 1 when voltage is too low, cleared after voltage returns to normal.
        // BIT[5] - Set to 1 when current error, cleared after current returns to normal.
        // BIT[6] - Set to 1 when power error, cleared after power returns to normal.
        // BIT[7] - Set to 1 when temperature error, cleared after temperature returns to normal.

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
* Set the servo to synchronous command, control the servo to rotate with different parameters,
	While rotating the servo, read the servo data at regular intervals.
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
		// Send synchronous command to control
		FSUS_SyncCommand(&FSUS_usart1,sync_count,sync_mode,SyncArray);
		HAL_Delay(1000);
		// Send synchronous command to read
		FSUS_SyncCommand(&FSUS_usart1,sync_count,7,SyncArray);
		HAL_Delay(200);

		SyncArray[0].angle=45;SyncArray[0].interval_single=0;SyncArray[0].velocity=20;
		SyncArray[1].angle=-45;SyncArray[1].interval_single=0;SyncArray[1].velocity=20;
		SyncArray[2].angle=45;SyncArray[2].interval_single=0;SyncArray[2].velocity=20;
		SyncArray[3].angle=-45;SyncArray[3].interval_single=0;SyncArray[3].velocity=20;
		SyncArray[4].angle=-45;SyncArray[4].interval_single=0;SyncArray[4].velocity=20;
		// Send synchronous command to control
		FSUS_SyncCommand(&FSUS_usart1,sync_count,sync_mode,SyncArray);
		HAL_Delay(1000);
		// Send synchronous command to read
		FSUS_SyncCommand(&FSUS_usart1,sync_count,7,SyncArray);
		HAL_Delay(200);
}

/********************************************************
 * Set the servo to data monitoring, read the servo data every two seconds.
 * 
 ********************************************************/
void demo_MONTIOR(void)
{
	// Read and print every two seconds
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
 * After controlling the servo to rotate to an angle, enter the control mode stop state
 * 
 ********************************************************/
void demo_StopOnControlMode(void)
{
//0-Release force after stopping (unlock)
//1-Maintain lock force after stopping
//2-Enter damping state after stopping
uint8_t stopcolmode=2;
	
	angle = 135.0;
  interval = 1000;
	power = 500;
  FSUS_SetServoAngle(&FSUS_usart1, ID, angle, interval, power);
	HAL_Delay(2000);
	
	// Enter damping state after stopping
	FSUS_StopOnControlMode(&FSUS_usart1,ID,stopcolmode,power);
	HAL_Delay(1000);
}

/********************************************************
 * Enable the servo to enter asynchronous mode, send an angle command once, and wait 5 seconds to send again
 * Only one command can be stored, if needed, enable asynchronous mode again and stop asynchronous mode after execution
 ********************************************************/
void demo_Begin_EndAsync(void)
{
		// Enable asynchronous command
		FSUS_BeginAsync(&FSUS_usart1);
	
		printf("GOTO: 135.0f\r\n");
    // Simple angle control + current angle query
    angle = 135.0;
    interval = 2000;
    FSUS_SetServoAngle(&FSUS_usart1, ID, angle, interval, power);
		FSUS_QueryServoAngle(&FSUS_usart1, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);
	
		printf("*******************\n");
		
		// Sending the above command once will not move, it only stores the command
		// Wait for 5 seconds and then send the stop asynchronous command
		HAL_Delay(5000);
		
		// Stop asynchronous command
		FSUS_EndAsync(&FSUS_usart1,async_mode);
}
