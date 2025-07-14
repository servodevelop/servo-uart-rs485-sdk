/*
 * Fashion Star Bus Servo Driver Library
 * Version: v0.0.2
 * UpdateTime: 2024/07/17
 */

#ifndef __FASHION_STAR_USART_SERVO_H
#define __FASHION_STAR_USART_SERVO_H

#include "math.h"
#include "usart.h"
#include "ring_buffer.h"
#include "sys_tick.h"

#define COUNTDOWN_MS 100

// FSUS communication protocol
// Note: FSUS is the abbreviation for Fashion Star Uart Servo

// Serial communication timeout setting
#define FSUS_TIMEOUT_MS 100
// The size of the servo user-defined data block in bytes
#define FSUS_SERVO_BATCH_DATA_SIZE 32
// The maximum length of the returned response data packet
#define FSUS_PACK_RESPONSE_MAX_SIZE 350
// In the communication system design of bus servo servos, Little Endian (low byte order / little-endian format) is used for byte order
// The STM32 system's numerical storage mode is Little Endian
// So for the value 0x4c12, when actually sending, the low byte comes first 0x12, 0x4c
#define FSUS_PACK_REQUEST_HEADER 0x4c12
#define FSUS_PACK_RESPONSE_HEADER 0x1c05

// FSUS control command data
// Note: All the following commands are for a single servo
#define FSUS_CMD_NUM 30
#define FSUS_CMD_PING 1                               // Servo communication detection
#define FSUS_CMD_RESET_USER_DATA 2                    // Reset user data
#define FSUS_CMD_READ_DATA 3                          // Read database for a single servo
#define FSUS_CMD_WRITE_DATA 4                         // Write data block for a single servo
#define FSUS_CMD_READ_BATCH_DATA 5                    // Batch read for a single servo (read all data of a single servo)
#define FSUS_CMD_WRITE_BATCH_DATA 6                   // Batch write for a single servo (write all data of a single servo)
#define FSUS_CMD_SPIN 7                               // Set wheel mode for a single servo
#define FSUS_CMD_ROTATE 8                             // Angle control mode (set servo angle))
#define FSUS_CMD_DAMPING 9                            // Damping mode
#define FSUS_CMD_READ_ANGLE 10                        // Read servo angle
#define FSUS_CMD_SET_SERVO_ANGLE_BY_INTERVAL 11       // Angle setting (specify cycle)
#define FSUS_CMD_SET_SERVO_ANGLE_BY_VELOCITY 12       // Angle setting (specify speed)
#define FSUS_CMD_SET_SERVO_ANGLE_MTURN 13             // Multi-turn angle setting
#define FSUS_CMD_SET_SERVO_ANGLE_MTURN_BY_INTERVAL 14 // Multi-turn angle setting (specify cycle)
#define FSUS_CMD_SET_SERVO_ANGLE_MTURN_BY_VELOCITY 15 // Multi-turn angle setting (specify speed)
#define FSUS_CMD_QUERY_SERVO_ANGLE_MTURN 16           // Query servo angle (multi-turn)
#define FSUS_CMD_RESERT_SERVO_ANGLE_MTURN 17          // Reset servo multi-turn angle
#define FSUS_CMD_BEGIN_ASYNC 18                       // Start asynchronous command
#define FSUS_CMD_END_ASYNC 19                      		// End asynchronous command
#define FSUS_CMD_SET_SERVO_ReadData 22                // Servo data monitoring
#define FSUS_CMD_SET_ORIGIN_POINT 23                  // Set zero point
#define FSUS_CMD_CONTROL_MODE_STOP 24                 // Control mode stop command
#define FSUS_CMD_SET_SERVO_SyncCommand 25             // Synchronous command

// FSUS status codes
#define FSUS_STATUS uint8_t
#define FSUS_STATUS_SUCCESS 0               // Set/read success
#define FSUS_STATUS_FAIL 1                  // Set/read failure
#define FSUS_STATUS_TIMEOUT 2               // Wait timeout
#define FSUS_STATUS_WRONG_RESPONSE_HEADER 3 // Wrong response header
#define FSUS_STATUS_UNKOWN_CMD_ID 4         // Unknown control command
#define FSUS_STATUS_SIZE_TOO_BIG 5          // Parameter size exceeds the limit in FSUS_PACK_RESPONSE_MAX_SIZE
#define FSUS_STATUS_CHECKSUM_ERROR 6        // Checksum error
#define FSUS_STATUS_ID_NOT_MATCH 7          // Requested servo ID does not match the feedback servo ID
#define FSUS_STATUS_ERRO 8                  // Synchronous mode setting error

// Static state judgment conditions
#define FSUS_ANGLE_DEADAREA 2.0f  // Motor angle dead zone
#define FSUS_WAIT_COUNT_MAX 10000 // Maximum number of times to wait for repeated angle queries

/* Read-only data ID and usage instructions for servos (read-only)*/
#define FSUS_PARAM_VOLTAGE 1    // Voltage (unit mV)
#define FSUS_PARAM_CURRENT 2    // Current (unit mA)
#define FSUS_PARAM_POWER 3      // Power (unit mw)
#define FSUS_PARAM_TEMPRATURE 4 // Temperature (unit ADC)

/*
Servo working status
BIT[0] - Set to 1 when executing a command, cleared after completion.
BIT[1] - Set to 1 when command execution error, cleared after correct execution.
BIT[2] - Set to 1 when stall error, cleared after stall is resolved.
BIT[3] - Set to 1 when voltage is too high, cleared after voltage returns to normal.
BIT[4] - Set to 1 when voltage is too low, cleared after voltage returns to normal.
BIT[5] - Set to 1 when current error, cleared after current returns to normal.
BIT[6] - Set to 1 when power error, cleared after power returns to normal.
BIT[7] - Set to 1 when temperature error, cleared after temperature returns to normal.
*/
#define FSUS_PARAM_SERVO_STATUS 5 // Servo working status (byte length 1)

/* Data ID and usage instructions for user-defined parameters of servos (readable and writable)*/

/* This item has two functions at the same time
 * In wheel mode and angle control mode
 * 1. Whether the servo command can be interrupted interruptable?
 * 2. Whether feedback data is generated?
 * 0x00(default)
 *      Servo control commands can be interrupted, new commands overwrite old commands
 *      No feedback data
 * 0x01
 *      Servo control commands cannot be interrupted, new commands are added to the waiting queue
 *      The waiting queue length is 1, you need to maintain a queue in your program yourself
 *      When the new control command exceeds the buffer size, the newly added command is ignored
 *      After the command execution is completed, feedback data is sent
 */
#define FSUS_PARAM_RESPONSE_SWITCH 33
/*
 * Servo ID number, (byte length 1)
 * Range of values is 0-254
 * 255 is the broadcast address, which cannot be assigned to the servo. The broadcast address is used in the PING command.
 */
#define FSUS_PARAM_SERVO_ID 34
/*
 * Serial communication baud rate ID  (byte length 1)
 * Range of values [0x01,0x07] , default value0x05
 * 0x01-9600,
 * 0x02-19200,
 * 0x03-38400,
 * 0x04-57600,
 * 0x05-115200 (default baud rate),
 * 0x06-250000,
 * 0x07-500000,
 * The baud rate takes effect immediately upon setting
 */
#define FSUS_PARAM_BAUDRATE 36

/* Servo protection value related settings, the servo enters protection mode when exceeding the threshold */
/*
 * Servo stall protection mode  (byte length 1)
 * 0x00-Mode 1 Reduce power to stall power upper limit
 * 0x01-Mode 2 Release servo lock force
 */
#define FSUS_PARAM_STALL_PROTECT 37
/* Servo stall power upper limit, (unit mW) (byte length 2) */
#define FSUS_PARAM_STALL_POWER_LIMIT 38
/* Servo voltage lower limit (unit mV) (byte length 2) */
#define FSUS_PARAM_OVER_VOLT_LOW 39
/* Servo voltage upper limit (unit mV) (byte length 2) */
#define FSUS_PARAM_OVER_VOLT_HIGH 40
/* Servo temperature upper limit (unit Celsius) (byte length 2) */
#define FSUS_PARAM_OVER_TEMPERATURE 41
/* Servo power upper limit (unit mW) (byte length 2) */
#define FSUS_PARAM_OVER_POWER 42
/* Servo current upper limit (unit mA) (byte length 2) */
#define FSUS_PARAM_OVER_CURRENT 43
/*
 * Servo startup acceleration processing switch (byte length 1)
 * 0x00 Do not start acceleration processing (invalid setting)
 * 0x01 Enable acceleration processing(default value)
 *      Trapezoidal speed control of the servo, calculate acceleration a according to time t
 *      Accelerate in the first 1/4 of the stroke
 *      Maintain constant speed in the middle 1/2 of the stroke
 *      In the last 1/4 of the stroke
 */
#define FSUS_PARAM_ACCEL_SWITCH 44
/*
 * Servo power-on lock force switch (byte length 1)
 * 0x00 Power-on servo releases lock force(default value)
 * 0x11 Power-on brake
 */
#define FSUS_PARAM_POWER_ON_LOCK_SWITCH 46
/*
 * [Wheel mode] Wheel mode brake switch (byte length 1)
 * 0x00 When stopped, the servo releases lock force(default)
 * 0x01 When stopped, brake
 */
#define FSUS_PARAM_WHEEL_MODE_BRAKE_SWITCH 47
/*
 * [Angle mode] Angle limit switch (byte length 1)
 * 0x00 Disable angle limit
 * 0x01 Enable angle limit
 * Note: Only when the angle limit mode is enabled, the angle upper and lower limits are valid
 */
#define FSUS_PARAM_ANGLE_LIMIT_SWITCH 48
/*
 * [Angle mode] Slow execution of initial angle setting when servo is powered on (byte length 1)
 * 0x00 Disable
 * 0x01 Enable
 * Enabling is safer
 * The time period for slow rotation is the "servo power-on startup time" below
 */
#define FSUS_PARAM_SOFT_START_SWITCH 49
/*
 * [Angle mode] Servo power-on startup time (unit ms)(byte length 2)
 * Default value: 0x0bb8
 */
#define FSUS_PARAM_SOFT_START_TIME 50
/*
 * [Angle mode] Servo angle upper limit (unit 0.1 degree)(byte length 2)
 */
#define FSUS_PARAM_ANGLE_LIMIT_HIGH 51
/*
 * [Angle mode] Servo angle lower limit (unit 0.1 degree)(byte length 2)
 */
#define FSUS_PARAM_ANGLE_LIMIT_LOW 52
/*
 * [Angle mode] Servo mid-position angle offset (unit 0.1 degree)(byte length 2)
 */
#define FSUS_PARAM_ANGLE_MID_OFFSET 53

// Header received completion flag
#define FSUS_RECV_FLAG_HEADER 0x01
// Command ID received completion flag
#define FSUS_RECV_FLAG_CMD_ID 0x02
// Content length received completion flag
#define FSUS_RECV_FLAG_SIZE 0x04
// Content received completion flag
#define FSUS_RECV_FLAG_CONTENT 0x08
// Checksum received flag
#define FSUS_RECV_FLAG_CHECKSUM 0x10

// Bus servo servo user-defined settings
// Whether to enable response mode
#define FSUS_IS_RESPONSE_ON 0

// Data frame structure (unified structure)
typedef struct {
    uint16_t header;
    uint8_t cmdId;
    uint16_t size;  // Unified use of 16-bit length
    uint8_t content[FSUS_PACK_RESPONSE_MAX_SIZE];
    uint8_t checksum;
    uint8_t status;
    uint8_t isSync;  // New synchronization flag
} PackageTypeDef;

// Function declarations
void FSUS_Package2RingBuffer(PackageTypeDef *pkg, RingBufferTypeDef *ringBuf);
// Calculate checksum
uint8_t FSUS_CalcChecksum(PackageTypeDef *pkg);
// Determine if it is a valid request header
FSUS_STATUS FSUS_IsValidResponsePackage(PackageTypeDef *pkg);
// Byte array to data frame
FSUS_STATUS FSUS_RingBuffer2Package(RingBufferTypeDef *ringBuf, PackageTypeDef *pkg);
// Construct send data frame
void FSUS_SendPackage_Common(Usart_DataTypeDef *usart, uint8_t cmdId, uint16_t size, uint8_t *content, uint8_t isSync);
// Receive data frame (dynamically allocate memory during reception)
FSUS_STATUS FSUS_RecvPackage(Usart_DataTypeDef *usart, PackageTypeDef *pkg);
FSUS_STATUS FSUS_sync_RecvPackage(Usart_DataTypeDef *usart, PackageTypeDef *pkg);

// Servo communication detection
// Note: If no servo responds to this Ping command, it will time out
FSUS_STATUS FSUS_Ping(Usart_DataTypeDef *usart, uint8_t servo_id);

// Reset the user data of the servo
FSUS_STATUS FSUS_ResetUserData(Usart_DataTypeDef *usart, uint8_t servo_id);

// Read data
FSUS_STATUS FSUS_ReadData(Usart_DataTypeDef *usart, uint8_t servo_id, uint8_t address, uint8_t *value, uint8_t *size);

// Write data
FSUS_STATUS FSUS_WriteData(Usart_DataTypeDef *usart, uint8_t servo_id, uint8_t address, uint8_t *value, uint8_t size);

// Set the angle of the servo
// @angle Unit degree
// @interval Unit ms
// @power Servo execution power Unit mW
//        If power=0 or greater than the protection value
FSUS_STATUS FSUS_SetServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, uint16_t interval, uint16_t power);

/* Set the angle of the servo (specify cycle) */
FSUS_STATUS FSUS_SetServoAngleByInterval(Usart_DataTypeDef *usart, uint8_t servo_id,
                                         float angle, uint16_t interval, uint16_t t_acc,
                                         uint16_t t_dec, uint16_t power);

/* Set the angle of the servo (specify speed) */
FSUS_STATUS FSUS_SetServoAngleByVelocity(Usart_DataTypeDef *usart, uint8_t servo_id,
                                         float angle, float velocity, uint16_t t_acc,
                                         uint16_t t_dec, uint16_t power);

/* Query the angle information of a single servo angle Unit degree */
FSUS_STATUS FSUS_QueryServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float *angle);

/* Set the angle of the servo (multi-turn mode) */
FSUS_STATUS FSUS_SetServoAngleMTurn(Usart_DataTypeDef *usart, uint8_t servo_id, float angle,
                                    uint32_t interval, uint16_t power);

/* Set the angle of the servo (multi-turn mode, specify cycle) */
FSUS_STATUS FSUS_SetServoAngleMTurnByInterval(Usart_DataTypeDef *usart, uint8_t servo_id, float angle,
                                              uint32_t interval, uint16_t t_acc, uint16_t t_dec, uint16_t power);

/* Set the angle of the servo (multi-turn mode, specify speed) */
FSUS_STATUS FSUS_SetServoAngleMTurnByVelocity(Usart_DataTypeDef *usart, uint8_t servo_id, float angle,
                                              float velocity, uint16_t t_acc, uint16_t t_dec, uint16_t power);

/* Query the angle of the servo (multi-turn mode) */
FSUS_STATUS FSUS_QueryServoAngleMTurn(Usart_DataTypeDef *usart, uint8_t servo_id, float *angle);

/* Servo damping mode */
FSUS_STATUS FSUS_DampingMode(Usart_DataTypeDef *usart, uint8_t servo_id, uint16_t power);

/* Reset multi-turn servo angle*/
FSUS_STATUS FSUS_ServoAngleReset(Usart_DataTypeDef *usart, uint8_t servo_id);

/* Zero point setting Only applicable to brushless magnetic encoder servos*/
FSUS_STATUS FSUS_SetOriginPoint(Usart_DataTypeDef *usart, uint8_t servo_id);

/* Servo start asynchronous command*/
FSUS_STATUS FSUS_BeginAsync(Usart_DataTypeDef *usart);

/* Servo end asynchronous command*/
FSUS_STATUS FSUS_EndAsync(Usart_DataTypeDef *usart,uint8_t mode);

// Servo data structure
typedef struct {
    uint8_t id;
    int16_t voltage;
    int16_t current;
    int16_t power;
    int16_t temperature;
    uint8_t status;
    float angle;
    int16_t circle_count;
} ServoData;
/* Single servo data monitoring*/
FSUS_STATUS FSUS_ServoMonitor(Usart_DataTypeDef *usart, uint8_t servo_id, ServoData servodata[]);
/* Servo control mode stop command*/
FSUS_STATUS FSUS_StopOnControlMode(Usart_DataTypeDef *usart, uint8_t servo_id, uint8_t mode, uint16_t power);

/* Synchronous command mode selection
* 1: Set the angle of the servo
* 2: Set the angle of the servo (specify cycle)
* 3: Set the angle of the servo (specify speed)
* 4: Set the angle of the servo (multi-turn mode)
* 5: Set the angle of the servo (multi-turn mode, specify cycle) 
* 6: Set the angle of the servo (multi-turn mode, specify speed)
* 7: Read servo data*/
typedef enum {
    MODE_SET_SERVO_ANGLE = 1,
    MODE_SET_SERVO_ANGLE_BY_INTERVAL = 2,
		MODE_SET_SERVO_ANGLE_BY_VELOCITY = 3,
    MODE_SET_SERVO_ANGLE_MTURN = 4,
    MODE_SET_SERVO_ANGLE_MTURN_BY_INTERVAL = 5,
    MODE_SET_SERVO_ANGLE_MTURN_BY_VELOCITY = 6,
		MODE_Query_SERVO_Monitor = 7
} ServoMode;

/* Synchronous command servo setting parameter structure*/
typedef struct
{
	uint8_t id;
	float angle;
	float velocity;
	uint16_t interval_single;
	uint32_t interval_multi;
	uint16_t t_acc; 
	uint16_t t_dec;
	uint16_t power;
}FSUS_sync_servo;

extern FSUS_sync_servo SyncArray[20]; // Assume you want to control 20 servos synchronously
extern ServoData servodata[20];//Assume you want to read data from 20 servo servos

/* Synchronous command servo data parsing function*/
FSUS_STATUS FSUS_SyncServoMonitor(Usart_DataTypeDef *usart, uint8_t servo_count, ServoData servodata[]);
/* Synchronous command mode selection control function*/
FSUS_STATUS FSUS_SyncCommand(Usart_DataTypeDef *usart, uint8_t servo_count, uint8_t ServoMode, FSUS_sync_servo servoSync[]);

#endif
