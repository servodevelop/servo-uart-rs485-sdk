/*
 * Fashion Star Bus Servo Driver Library
 * Version: v0.0.2
 * UpdateTime: 2024/07/17
 */
#include "fashion_star_uart_servo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Servo structure array for synchronous commands
FSUS_sync_servo SyncArray[20]; // Assuming you want to control 20 servos synchronously
ServoData servodata[20];//Assuming you want to read data from 20 servo motors


// Unified package processing
void FSUS_Package2RingBuffer(PackageTypeDef *pkg, RingBufferTypeDef *ringBuf) {
    uint8_t checksum;
    RingBuffer_WriteUShort(ringBuf, pkg->header);// Write frame header
    RingBuffer_WriteByte(ringBuf, pkg->cmdId);// Write command ID
    
    if (pkg->isSync || pkg->size > 255) {
        RingBuffer_WriteByte(ringBuf, 0xFF);
        RingBuffer_WriteUShort(ringBuf, pkg->size);// Write package length
    } else {
        RingBuffer_WriteByte(ringBuf, (uint8_t)pkg->size);// Write package length
    }
    
    RingBuffer_WriteByteArray(ringBuf, pkg->content, pkg->size);// Write content
    checksum = RingBuffer_GetChecksum(ringBuf);// Calculate checksum
    RingBuffer_WriteByte(ringBuf, checksum);// Write checksum
}

// Calculate package checksum
uint8_t FSUS_CalcChecksum(PackageTypeDef *pkg) {
    RingBufferTypeDef ringBuf;// Initialize ring buffer
    uint8_t pkgBuf[FSUS_PACK_RESPONSE_MAX_SIZE + 5];
    RingBuffer_Init(&ringBuf, sizeof(pkgBuf), pkgBuf);
	// Convert package to ring buffer
	// Checksum is automatically calculated during conversion
    FSUS_Package2RingBuffer(pkg, &ringBuf);
	// Get the element at the end of the ring buffer (checksum position)
    return RingBuffer_GetValueByIndex(&ringBuf, RingBuffer_GetByteUsed(&ringBuf)-1);
}

// Validate if the response package is valid (data package verification)
FSUS_STATUS FSUS_IsValidResponsePackage(PackageTypeDef *pkg) {
    if (pkg->header != FSUS_PACK_RESPONSE_HEADER)
        return FSUS_STATUS_WRONG_RESPONSE_HEADER;// Invalid frame header
    if (pkg->cmdId > FSUS_CMD_NUM)// Check if control command is valid, command range exceeded
        return FSUS_STATUS_UNKOWN_CMD_ID;
    if (pkg->size > (FSUS_PACK_RESPONSE_MAX_SIZE - 6))  // Adjust validation length
        return FSUS_STATUS_SIZE_TOO_BIG;
    if (FSUS_CalcChecksum(pkg) != pkg->checksum)// Checksum mismatch
        return FSUS_STATUS_CHECKSUM_ERROR;
    return FSUS_STATUS_SUCCESS;// Data valid
}

// Convert byte array to data frame
FSUS_STATUS FSUS_RingBuffer2Package(RingBufferTypeDef *ringBuf, PackageTypeDef *pkg){
    // Allocate memory
    pkg = (PackageTypeDef *)malloc(sizeof(PackageTypeDef));
    // Read frame header
    pkg->header = RingBuffer_ReadUShort(ringBuf);
    // Read command ID
    pkg->cmdId = RingBuffer_ReadByte(ringBuf);
    // Read package length
    pkg->size = RingBuffer_ReadByte(ringBuf);
    // Allocate memory for parameters
    // pkg->content = (uint8_t *)malloc(pkg->size);
    // Read content
    RingBuffer_ReadByteArray(ringBuf, pkg->content, pkg->size);
    // Read checksum
    pkg->checksum = RingBuffer_ReadByte(ringBuf);
    // Return whether the current data frame is a valid response frame
    return FSUS_IsValidResponsePackage(pkg);
}

// Send data package
void FSUS_SendPackage_Common(Usart_DataTypeDef *usart, uint8_t cmdId, uint16_t size, uint8_t *content, uint8_t isSync) {
    PackageTypeDef pkg = {
        .header = FSUS_PACK_REQUEST_HEADER,
        .cmdId = cmdId,
        .size = size,
        .isSync = isSync
    };
    memcpy(pkg.content, content, size);
    FSUS_Package2RingBuffer(&pkg, usart->sendBuf);// Put data into send buffer
    Usart_SendAll(usart);// Send data through serial port
}

// Receive data package (unified processing)
FSUS_STATUS FSUS_RecvPackage(Usart_DataTypeDef *usart, PackageTypeDef *pkg) {
    // Simplified reception logic with dynamic length support
    uint16_t header = 0;

		// Start countdown
    SysTick_CountdownBegin(FSUS_TIMEOUT_MS);	
		// If not timed out
		while (!SysTick_CountdownIsTimeout()){
        if (RingBuffer_GetByteUsed(usart->recvBuf) < 5) continue;

			
        // Parse header
        header = RingBuffer_PeekUShort(usart->recvBuf, 0);
        if (header != FSUS_PACK_RESPONSE_HEADER) {
            RingBuffer_ReadByte(usart->recvBuf);
            continue;
        }

        // Extract length
        uint16_t size;
        if (RingBuffer_PeekByte(usart->recvBuf, 3) == 0xFF) {
            size = RingBuffer_PeekUShort(usart->recvBuf, 4);
            pkg->isSync = 1;
        } else {
            size = RingBuffer_PeekByte(usart->recvBuf, 3);
            pkg->isSync = 0;
        }

        // Check complete package
//        uint16_t totalLen = pkg->isSync ? 6 + size : 5 + size;
				uint16_t totalLen = 5 + size + (pkg->isSync ? 2 : 0);
        if (RingBuffer_GetByteUsed(usart->recvBuf) < totalLen) continue;

        // Parse complete package
        pkg->header = RingBuffer_ReadUShort(usart->recvBuf);  // header
        pkg->cmdId = RingBuffer_ReadByte(usart->recvBuf);
        if (pkg->isSync) {
            RingBuffer_ReadByte(usart->recvBuf);  // 0xFF
            pkg->size = RingBuffer_ReadUShort(usart->recvBuf);
        } else {
            pkg->size = RingBuffer_ReadByte(usart->recvBuf);
        }
        RingBuffer_ReadByteArray(usart->recvBuf, pkg->content, pkg->size);
        pkg->checksum = RingBuffer_ReadByte(usart->recvBuf);
        					
        return FSUS_IsValidResponsePackage(pkg);
    }
    return FSUS_STATUS_TIMEOUT;
}

FSUS_STATUS FSUS_sync_RecvPackage(Usart_DataTypeDef *usart, PackageTypeDef *pkg)
{
    pkg->status = 0;  // Initialize package status
    uint8_t bIdx = 0;  // Data index
    uint16_t header = 0;  // Frame header detection
    uint8_t byte;
    // Countdown starts
    SysTick_CountdownBegin(FSUS_TIMEOUT_MS);	
    // If not timeout
    while (!SysTick_CountdownIsTimeout()){
        // If receive buffer is empty, continue waiting
        if (RingBuffer_GetByteUsed(usart->recvBuf) == 0) {
            continue;
        }
        // Read one byte
        byte = RingBuffer_ReadByte(usart->recvBuf);
//        printf("Received byte: 0x%02X\n", byte);
        // Frame header synchronization: strictly match 0x05 0x1C
        if ((pkg->status & FSUS_RECV_FLAG_HEADER) == 0) {
            if (header == 0 && byte == 0x05) {
                header = byte;  // First byte of frame header
            } else if (header == 0x05 && byte == 0x1C) {
                pkg->header = 0x1C05;  // Frame header detection success
                pkg->status |= FSUS_RECV_FLAG_HEADER;
                header = 0;  // Reset frame header
            } else {
                header = 0;  // Frame header detection failed, reset
            }
            continue;
        }
        // Parse cmdId
        if ((pkg->status & FSUS_RECV_FLAG_CMD_ID) == 0) {
            pkg->cmdId = byte;
            if (pkg->cmdId != 22) {  // Check if cmdId is correct
                return FSUS_STATUS_UNKOWN_CMD_ID;
            }
            pkg->status |= FSUS_RECV_FLAG_CMD_ID;
            continue;
        }
        // Parse size
        if ((pkg->status & FSUS_RECV_FLAG_SIZE) == 0) {
            pkg->size = byte;
                        if (pkg->size != 16) {  // Each data group is 16 bytes
                return FSUS_STATUS_SIZE_TOO_BIG;
            }
            pkg->status |= FSUS_RECV_FLAG_SIZE;
            continue;
        }
        // Read content
        if ((pkg->status & FSUS_RECV_FLAG_CONTENT) == 0) {
            pkg->content[bIdx++] = byte;
            if (bIdx >= pkg->size) {
                pkg->status |= FSUS_RECV_FLAG_CONTENT;
            }
            continue;
        }
        // Checksum
        if ((pkg->status & FSUS_RECV_FLAG_CHECKSUM) == 0) {
            pkg->checksum = byte;
            if (FSUS_CalcChecksum(pkg) != pkg->checksum) {
                return FSUS_STATUS_CHECKSUM_ERROR;
            }
            return FSUS_STATUS_SUCCESS;
        }
    }
    return FSUS_STATUS_TIMEOUT;  // Timeout return
}

// Servo communication ping test
// Note: If no servo responds to this Ping command, it will timeout
FSUS_STATUS FSUS_Ping(Usart_DataTypeDef *usart, uint8_t servo_id){
    uint8_t statusCode; // Status code
    uint8_t ehcoServoId; // Echoed servo ID from PING
    // printf("[PING]Send Ping Package\r\n");
    // Send request package
    FSUS_SendPackage_Common(usart, FSUS_CMD_PING, 1, &servo_id,0);
    // Receive returned Ping
    PackageTypeDef pkg;
    statusCode = FSUS_RecvPackage(usart, &pkg);
    if(statusCode == FSUS_STATUS_SUCCESS){
        // Further check if the ID matches
        ehcoServoId = (uint8_t)pkg.content[0];
        if (ehcoServoId != servo_id){
            // Feedback that the echoed servo ID does not match
            return FSUS_STATUS_ID_NOT_MATCH;
        }
    }
    return statusCode;
}

// Reset servo user data
FSUS_STATUS FSUS_ResetUserData(Usart_DataTypeDef *usart, uint8_t servo_id){
    const uint8_t size = 1;
    FSUS_STATUS statusCode;
    // Send request package
    FSUS_SendPackage_Common(usart, FSUS_CMD_RESET_USER_DATA, size, &servo_id,0);
    // Receive reset result
    PackageTypeDef pkg;
    statusCode = FSUS_RecvPackage(usart, &pkg);
    if (statusCode == FSUS_STATUS_SUCCESS){
        // Successfully received feedback data
        // Read result from feedback data
        uint8_t result = (uint8_t)pkg.content[1];
        if (result == 1){
            return FSUS_STATUS_SUCCESS;
        }else{
            return FSUS_STATUS_FAIL;
        }
    }
    return statusCode;
}

// Reset servo user data
FSUS_STATUS FSUS_ReadData(Usart_DataTypeDef *usart, uint8_t servo_id,  uint8_t address, uint8_t *value, uint8_t *size){
    FSUS_STATUS statusCode;
    // Construct content buffer
    uint8_t buffer[2] = {servo_id, address};
    // Send request package
    FSUS_SendPackage_Common(usart, FSUS_CMD_READ_DATA, 2, buffer,0);
    // Receive returned information
    PackageTypeDef pkg;
    statusCode = FSUS_RecvPackage(usart, &pkg);
    if (statusCode == FSUS_STATUS_SUCCESS){
        // Successfully received feedback data
        // Read the number of bytes in the data
        *size = pkg.size - 2; // The length of content minus the lengths of servo_id and address
        // Data copy
        for (int i=0; i<*size; i++){
            value[i] = pkg.content[i+2];
        }
    }
    return statusCode;
}

// Write data to servo
FSUS_STATUS FSUS_WriteData(Usart_DataTypeDef *usart, uint8_t servo_id, uint8_t address, uint8_t *value, uint8_t size){
    FSUS_STATUS statusCode;
    // Construct content buffer
    uint8_t buffer[size+2]; // servo ID + Address + number of data bytes
    buffer[0] = servo_id;
    buffer[1] = address;
    // Copy data
    for (int i=0; i<size; i++){
        buffer[i+2] = value[i];
    }
    // Send request package
    FSUS_SendPackage_Common(usart, FSUS_CMD_WRITE_DATA, size+2, buffer,0);
    // Receive returned information
    PackageTypeDef pkg;
    statusCode = FSUS_RecvPackage(usart, &pkg);
    if (statusCode == FSUS_STATUS_SUCCESS){
        uint8_t result = pkg.content[2];
        if(result == 1){
            statusCode = FSUS_STATUS_SUCCESS;
        }else{
            statusCode = FSUS_STATUS_FAIL;
        }
    }
    return statusCode;
}


// Set servo angle
// @angle Unit: degrees
// @interval Unit: ms
// @power Servo execution power Unit: mW
//        If power=0 or exceeds protection value
FSUS_STATUS FSUS_SetServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, uint16_t interval, uint16_t power){
    // Create ring buffer queue
    const uint8_t size = 7;
    uint8_t buffer[size+1];
    RingBufferTypeDef ringBuf;
    RingBuffer_Init(&ringBuf, size, buffer);    
    // Value constraint
    if(angle > 180.0f){
        angle = 180.0f;
    }else if(angle < -180.0f){
        angle = -180.0f;
    }
    // Construct content
    RingBuffer_WriteByte(&ringBuf, servo_id);
    RingBuffer_WriteShort(&ringBuf, (int16_t)(10*angle));
    RingBuffer_WriteUShort(&ringBuf, interval);
    RingBuffer_WriteUShort(&ringBuf, power);
    // Send request package
    // Note: Since we're using a ring queue, head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_ROTATE, size, buffer+1,0);
    
    return FSUS_STATUS_SUCCESS;
}

/* Set servo angle (with specified period) */
FSUS_STATUS FSUS_SetServoAngleByInterval(Usart_DataTypeDef *usart, uint8_t servo_id, 
                float angle, uint16_t interval, uint16_t t_acc, 
                uint16_t t_dec, uint16_t  power){
    // Create ring buffer queue
    const uint8_t size = 11;
    uint8_t buffer[size+1];
    RingBufferTypeDef ringBuf;
    RingBuffer_Init(&ringBuf, size, buffer);    
    // Value constraint
    if(angle > 180.0f){
        angle = 180.0f;
    }else if(angle < -180.0f){
        angle = -180.0f;
    }
    if (t_acc < 20){
        t_acc = 20;
    }
    if (t_dec < 20){
        t_dec = 20;
    }
    
    // Protocol packing
    RingBuffer_WriteByte(&ringBuf, servo_id);
    RingBuffer_WriteShort(&ringBuf, (int16_t)(10*angle));
    RingBuffer_WriteUShort(&ringBuf, interval);
    RingBuffer_WriteUShort(&ringBuf, t_acc);
    RingBuffer_WriteUShort(&ringBuf, t_dec);
    RingBuffer_WriteUShort(&ringBuf, power);
    
    // Send request package
    // Note: Since we're using a ring queue, head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_ANGLE_BY_INTERVAL, size, buffer+1,0);
    
    return FSUS_STATUS_SUCCESS;
}

/* Set servo angle (with specified velocity) */
FSUS_STATUS FSUS_SetServoAngleByVelocity(Usart_DataTypeDef *usart, uint8_t servo_id, \
                float angle, float velocity, uint16_t t_acc, \
                uint16_t t_dec, uint16_t  power){
    // Create ring buffer queue
    const uint8_t size = 11;
    uint8_t buffer[size+1];
    RingBufferTypeDef ringBuf;
    RingBuffer_Init(&ringBuf, size, buffer);    
    
    // Value constraint
    if(angle > 180.0f){
        angle = 180.0f;
    }else if(angle < -180.0f){
        angle = -180.0f;
    }
    if(velocity < 1.0f){
        velocity = 1.0f;
    }else if(velocity > 750.0f){
        velocity = 750.0f;
    }
    if(t_acc < 20){
        t_acc = 20;
    }
    if(t_dec < 20){
        t_dec = 20;
    }
    
    // Protocol packing
    RingBuffer_WriteByte(&ringBuf, servo_id);
    RingBuffer_WriteShort(&ringBuf, (int16_t)(10.0f*angle));
    RingBuffer_WriteUShort(&ringBuf, (uint16_t)(10.0f*velocity));
    RingBuffer_WriteUShort(&ringBuf, t_acc);
    RingBuffer_WriteUShort(&ringBuf, t_dec);
    RingBuffer_WriteUShort(&ringBuf, power);
    
    // Send request package
    // Note: Since we're using a ring queue, head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_ANGLE_BY_VELOCITY, size, buffer+1,0);
    
    return FSUS_STATUS_SUCCESS;
}

/* Query the angle information of a single servo angle unit degree */
FSUS_STATUS FSUS_QueryServoAngle(Usart_DataTypeDef *usart, uint8_t servo_id, float *angle){
    const uint8_t size = 1; // Request package content length
    uint8_t echoServoId;
    int16_t echoAngle;
    
    // Send servo angle request package
    FSUS_SendPackage_Common(usart, FSUS_CMD_READ_ANGLE, size, &servo_id,0);
    // Receive returned package
    PackageTypeDef pkg;
    uint8_t statusCode = FSUS_RecvPackage(usart, &pkg);
    if (statusCode == FSUS_STATUS_SUCCESS){
        // Successfully received servo angle readback data
        echoServoId = (uint8_t)pkg.content[0];
        // Check if servo ID matches
        if (echoServoId != servo_id){
            // Feedback: received servo ID does not match
            return FSUS_STATUS_ID_NOT_MATCH;
        }

        // Extract servo angle
        echoAngle = (int16_t)(pkg.content[1] | (pkg.content[2] << 8));
        *angle = (float)(echoAngle / 10.0);
    }
    return statusCode;
}

// Set servo angle (multi-turn mode)
// @angle Unit: 0.01 degrees (range: -368640.0f to 368640.0f)
// @interval Unit: us (microseconds, range: 0 to 4096000)
// @power Servo execution power Unit: mW
//        If power=0 or exceeds protection value
FSUS_STATUS FSUS_SetServoAngleMTurn(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, 
    uint32_t interval, uint16_t power){
    // Create ring buffer queue
    const uint8_t size = 11;
    uint8_t buffer[size+1];
    RingBufferTypeDef ringBuf;
    RingBuffer_Init(&ringBuf, size, buffer); 
    // Value constraint            
    if(angle > 368640.0f){
        angle = 368640.0f;
    }else if(angle < -368640.0f){
        angle = -368640.0f;
    }
    if(interval > 4096000){
        interval = 4096000;  // Fixed bug: should modify interval instead of angle
    }
    // Protocol packing
    RingBuffer_WriteByte(&ringBuf, servo_id);
    RingBuffer_WriteLong(&ringBuf, (int32_t)(10*angle));
    RingBuffer_WriteULong(&ringBuf, interval);
    RingBuffer_WriteShort(&ringBuf, power);
    
    // Send request package
    // Note: Since we're using a ring queue, head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_ANGLE_MTURN, size, buffer+1,0);
    return FSUS_STATUS_SUCCESS;
}

/* Setting the servo angle (multiturn mode, specified period) */
FSUS_STATUS FSUS_SetServoAngleMTurnByInterval(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, 
        uint32_t interval,  uint16_t t_acc,  uint16_t t_dec, uint16_t power){
    // Create ring buffer queue
    const uint8_t size = 15;
    uint8_t buffer[size+1];
    RingBufferTypeDef ringBuf;
    RingBuffer_Init(&ringBuf, size, buffer);    
    
    // Value constraint            
    if(angle > 368640.0f){
        angle = 368640.0f;
    }else if(angle < -368640.0f){
        angle = -368640.0f;
    }
    if(interval > 4096000){
        interval = 4096000;  // Fixed bug: should modify interval instead of angle
    }
    if(t_acc < 20){
        t_acc = 20;
    }
    if(t_dec < 20){
        t_dec = 20;
    }
    // Protocol packing
    RingBuffer_WriteByte(&ringBuf, servo_id);
    RingBuffer_WriteLong(&ringBuf, (int32_t)(10*angle));
    RingBuffer_WriteULong(&ringBuf, interval);
    RingBuffer_WriteUShort(&ringBuf, t_acc);
    RingBuffer_WriteUShort(&ringBuf, t_dec);
    RingBuffer_WriteShort(&ringBuf, power);
    
    // Send request package
    // Note: Since we're using a ring queue, head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_ANGLE_MTURN_BY_INTERVAL, size, buffer+1,0);
    
    return FSUS_STATUS_SUCCESS;
}

/* Setting the servo angle (multi-turn mode, specified rotation speed) */
FSUS_STATUS FSUS_SetServoAngleMTurnByVelocity(Usart_DataTypeDef *usart, uint8_t servo_id, float angle, \
			float velocity, uint16_t t_acc,  uint16_t t_dec, uint16_t power){
	// Create ring buffer queue
	const uint8_t size = 13;
	uint8_t buffer[size+1];
	RingBufferTypeDef ringBuf;
	RingBuffer_Init(&ringBuf, size, buffer);
	// Value constraint
	if(angle > 368640.0f){
		angle = 368640.0f;
	}else if(angle < -368640.0f){
		angle = -368640.0f;
	}
	if(velocity < 1.0f){
		velocity = 1.0f;
	}else if(velocity > 750.0f){
		velocity = 750.0f;
	}
	if(t_acc < 20){
		t_acc = 20;
	}
	if(t_dec < 20){
		t_dec = 20;
	}
	// Protocol packing
	RingBuffer_WriteByte(&ringBuf, servo_id);
	RingBuffer_WriteLong(&ringBuf, (int32_t)(10.0f*angle));
	RingBuffer_WriteUShort(&ringBuf, (uint16_t)(10.0f*velocity));
	RingBuffer_WriteUShort(&ringBuf, t_acc);
	RingBuffer_WriteUShort(&ringBuf, t_dec);
	RingBuffer_WriteShort(&ringBuf, power);
	
	// Send request package
    // Note: Since we're using a ring queue, head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_ANGLE_MTURN_BY_VELOCITY, size, buffer+1,0);
	
	return FSUS_STATUS_SUCCESS;
}

/* Query the angle of the servo (multi-turn mode) */
FSUS_STATUS FSUS_QueryServoAngleMTurn(Usart_DataTypeDef *usart, uint8_t servo_id, float *angle){
    // Create ring buffer queue
    const uint8_t size = 1; // Length of the request packet content
    uint8_t ehcoServoId;
    int32_t echoAngle;
    
    // Send servo angle request packet
    FSUS_SendPackage_Common(usart, FSUS_CMD_QUERY_SERVO_ANGLE_MTURN, size, &servo_id,0);
    // Receive returned package
    PackageTypeDef pkg;
    uint8_t statusCode = FSUS_RecvPackage(usart, &pkg);
    if (statusCode == FSUS_STATUS_SUCCESS){
        // Successfully received servo angle readback data
        ehcoServoId = (uint8_t)pkg.content[0];
        // Check if servo ID matches
        if (ehcoServoId != servo_id){
            // Feedback: received servo ID does not match
            return FSUS_STATUS_ID_NOT_MATCH;
        }
        
        // Extract servo angle
        echoAngle = (int32_t)(pkg.content[1] | (pkg.content[2] << 8) |  (pkg.content[3] << 16) | (pkg.content[4] << 24));
        *angle = (float)(echoAngle / 10.0);
    }
    return statusCode;
}


/* Servo Damping Mode */
FSUS_STATUS FSUS_DampingMode(Usart_DataTypeDef *usart, uint8_t servo_id, uint16_t power){
    // Create ring buffer queue
    const uint8_t size = 3; // Length of the request packet content
    uint8_t buffer[size+1]; // Content buffer for the request packet
    RingBufferTypeDef ringBuf;
    
    // Initialize the ring buffer
    RingBuffer_Init(&ringBuf, size, buffer);
    
    // Construct the request packet content
    RingBuffer_WriteByte(&ringBuf, servo_id);
    RingBuffer_WriteUShort(&ringBuf, power);
    
    // Send the request packet
    // Note: Since we're using a ring buffer, the head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_DAMPING, size, buffer+1,0);
    
    return FSUS_STATUS_SUCCESS;
}

/* Servo Reset Multi-turn Angle Turns */
FSUS_STATUS FSUS_ServoAngleReset(Usart_DataTypeDef *usart, uint8_t servo_id){
	uint8_t statusCode;

	FSUS_SendPackage_Common(usart, FSUS_CMD_RESERT_SERVO_ANGLE_MTURN, 1, &servo_id,0);

	return statusCode;
}

/* Zero point setting */
FSUS_STATUS FSUS_SetOriginPoint(Usart_DataTypeDef *usart, uint8_t servo_id)
{
	uint8_t statusCode; 

	FSUS_SendPackage_Common(usart, FSUS_CMD_SET_ORIGIN_POINT, 1, &servo_id,0);

	return statusCode;
}

/* Servo start asynchronous command */
FSUS_STATUS FSUS_BeginAsync(Usart_DataTypeDef *usart)
{
    // Create ring buffer queue
    const uint8_t size = 0; // Length of the request packet content
    uint8_t buffer[size+1]; // Content buffer for the request packet
    RingBufferTypeDef ringBuf;
    
    // Initialize the ring buffer
    RingBuffer_Init(&ringBuf, size, buffer);
    
    // Construct the request packet content
    RingBuffer_WriteByte(&ringBuf, 0);
    
    // Send the request packet
    // Note: Since we're using a ring buffer, the head is reserved, so the pointer needs to be moved back one byte
    FSUS_SendPackage_Common(usart, FSUS_CMD_BEGIN_ASYNC, size, buffer+1,0);
    
    return FSUS_STATUS_SUCCESS;
}

/* Servo End Asynchronous Command */
FSUS_STATUS FSUS_EndAsync(Usart_DataTypeDef *usart,uint8_t mode)
{
/*mode
	0:Execute the stored command 
    1:Cancel the stored command
*/
	const uint8_t size = 1;  // Length of the request packet content
    uint8_t buffer[size+1]; // Content buffer for the request packet
    RingBufferTypeDef ringBuf;

	 // Initialize the ring buffer
    RingBuffer_Init(&ringBuf, size, buffer); 

	 // Construct the request packet content
	RingBuffer_WriteByte(&ringBuf, mode);
	// Send the request packet
    // Note: Since we're using a ring buffer, the head is reserved, so the pointer needs to be moved back one byte
	FSUS_SendPackage_Common(usart, FSUS_CMD_END_ASYNC, size, buffer+1,0);
	return FSUS_STATUS_SUCCESS;
}

/* Servo Individual Data Monitoring*/
FSUS_STATUS FSUS_ServoMonitor(Usart_DataTypeDef *usart, uint8_t servo_id, ServoData servodata[]) {

    // Create ring buffer queue
    const uint8_t size = 1;
    uint8_t buffer[size+1];
    RingBufferTypeDef ringBuf;
    RingBuffer_Init(&ringBuf, size, buffer);	
    double temp; // Temperature data conversion
    RingBuffer_WriteByte(&ringBuf, servo_id);	
    FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_ReadData,(uint8_t)size, buffer + 1,0);

    PackageTypeDef pkg;
    FSUS_STATUS status = FSUS_RecvPackage(usart, &pkg);;

    if (status != FSUS_STATUS_SUCCESS) {
        return status;  // If reception fails, return error status
    }

    // Parse current packet content
    servodata[0].id = pkg.content[0];
    servodata[0].voltage = (int16_t)((pkg.content[2] << 8) | pkg.content[1]);
    servodata[0].current = (int16_t)((pkg.content[4] << 8) | pkg.content[3]);
    servodata[0].power = (int16_t)((pkg.content[6] << 8) | pkg.content[5]);
    servodata[0].temperature = (int16_t)((pkg.content[8] << 8) | pkg.content[7]);
    temp = (double)servodata[0].temperature;
    servodata[0].temperature = 1 / (log(temp / (4096.0f - temp)) / 3435.0f + 1 / (273.15 + 25)) - 273.15;
    servodata[0].status = pkg.content[9];
    servodata[0].angle = (int32_t)((pkg.content[13] << 24) | (pkg.content[12] << 16) | (pkg.content[11] << 8) | pkg.content[10]);
    servodata[0].angle = servodata[0].angle/10.0f;
    servodata[0].circle_count = (int16_t)((pkg.content[15] << 8) | pkg.content[14]);

    // Reset pkg status to receive next set of data
    pkg.status = 0;

    return FSUS_STATUS_SUCCESS;
}


/* Servo Control Mode Stop Command */ 
//mode command stop form 
//0-Unloading force after stopping (lost lock) 
//1-Holding force after stopping 
//2-Damping state after stopping
FSUS_STATUS FSUS_StopOnControlMode(Usart_DataTypeDef *usart, uint8_t servo_id, uint8_t mode, uint16_t power) {

	const uint8_t size = 4;
	uint8_t buffer[size+1];
	RingBufferTypeDef ringBuf;
	RingBuffer_Init(&ringBuf, size, buffer);	
	
		RingBuffer_WriteByte(&ringBuf, servo_id);	
		mode = mode | 0x10;
		RingBuffer_WriteByte(&ringBuf, mode);	
		RingBuffer_WriteShort(&ringBuf, power);

		FSUS_SendPackage_Common(usart, FSUS_CMD_CONTROL_MODE_STOP,(uint8_t)size, buffer + 1,0);
	
    return FSUS_STATUS_SUCCESS;
}

/* Synchronization Command Selection Mode Control Functions*/ 
// @servo_count Servo Sync Count 
// @servomode Synchronization Command Mode Selection 
// @FSUS_servo_sync servoSync[] Array of Servo Parameter Structures
FSUS_STATUS FSUS_SyncCommand(Usart_DataTypeDef *usart, uint8_t servo_count, uint8_t ServoMode, FSUS_sync_servo servoSync[]) {

		uint8_t buffer[3 + servo_count * 15]; //Maximum possible buffer size
		uint16_t size;
		RingBufferTypeDef ringBuf;

//		ServoData *servo_data = malloc(servo_count * sizeof(ServoData));

    switch (ServoMode) {
        case MODE_SET_SERVO_ANGLE:
            /* Synchronization command to set servo angle (one-turn mode)*/
						size = 3 + servo_count * 7;
            RingBuffer_Init(&ringBuf, size, buffer);
						RingBuffer_WriteByte(&ringBuf, 8);
            RingBuffer_WriteByte(&ringBuf, 7);
            RingBuffer_WriteByte(&ringBuf, servo_count);
            for (int i = 0; i < servo_count; i++) {
                RingBuffer_WriteByte(&ringBuf, servoSync[i].id);
                if (servoSync[i].angle > 180.0f) {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * 180.0f));
                } else if (servoSync[i].angle < -180.0f) {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * -180.0f));
                } else {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * servoSync[i].angle));
                }
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].interval_single);
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].power);
            }
            break;
				case MODE_SET_SERVO_ANGLE_BY_INTERVAL:
            /* Synchronization command sets the servo angle (one-turn mode, specified period) */
						size = 3 + servo_count * 11;
            RingBuffer_Init(&ringBuf, size, buffer);
            RingBuffer_WriteByte(&ringBuf, 11);
            RingBuffer_WriteByte(&ringBuf, 11);
            RingBuffer_WriteByte(&ringBuf, servo_count);
            for (int i = 0; i < servo_count; i++) {
                RingBuffer_WriteByte(&ringBuf, servoSync[i].id);
                if (servoSync[i].angle > 180.0f) {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * 180.0f));
                } else if (servoSync[i].angle < -180.0f) {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * -180.0f));
                } else {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * servoSync[i].angle));
                }
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].interval_single);
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_acc);
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_dec);
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].power);
            }
            break;
				case MODE_SET_SERVO_ANGLE_BY_VELOCITY:
            /* Synchronization command sets the servo angle (one-turn mode, specified RPM) */
						size = 3 + servo_count * 11;
            RingBuffer_Init(&ringBuf, size, buffer);
            RingBuffer_WriteByte(&ringBuf,12);
						RingBuffer_WriteByte(&ringBuf,11);
						RingBuffer_WriteByte(&ringBuf,servo_count);
	
						for(int i=0; i<servo_count; i++){
						RingBuffer_WriteByte(&ringBuf, servoSync[i].id);
						if (servoSync[i].angle > 180.0f) {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * 180.0f));
                } else if (servoSync[i].angle < -180.0f) {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * -180.0f));
                } else {
                    RingBuffer_WriteShort(&ringBuf, (int16_t)(10 * servoSync[i].angle));
                }
						RingBuffer_WriteUShort(&ringBuf, (uint16_t)(10.0f*servoSync[i].velocity));
						RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_acc);
            RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_dec);
            RingBuffer_WriteUShort(&ringBuf, servoSync[i].power);
            }
            break;
        case MODE_SET_SERVO_ANGLE_MTURN:
            /* Synchronization command to set servo angle (multi-turn mode) */
						size = 3 + servo_count * 11;
            RingBuffer_Init(&ringBuf, size, buffer);
            RingBuffer_WriteByte(&ringBuf, 13);
            RingBuffer_WriteByte(&ringBuf, 11);
            RingBuffer_WriteByte(&ringBuf, servo_count);
            for (int i = 0; i < servo_count; i++) {
                RingBuffer_WriteByte(&ringBuf, servoSync[i].id);
                if (servoSync[i].angle > 368640.0f) {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * 368640.0f));
                } else if (servoSync[i].angle < -368640.0f) {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * -368640.0f));
                } else {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * servoSync[i].angle));
                }
                RingBuffer_WriteULong(&ringBuf, servoSync[i].interval_multi);
                RingBuffer_WriteShort(&ringBuf, servoSync[i].power);
            }
            break;
        case MODE_SET_SERVO_ANGLE_MTURN_BY_INTERVAL:
            /* Synchronization command sets the servo angle (multi-turn mode, specified period) */
						size = 3 + servo_count * 15;
            RingBuffer_Init(&ringBuf, size, buffer);
            RingBuffer_WriteByte(&ringBuf, 14);
            RingBuffer_WriteByte(&ringBuf, 15);
            RingBuffer_WriteByte(&ringBuf, servo_count);
            for (int i = 0; i < servo_count; i++) {
                RingBuffer_WriteByte(&ringBuf, servoSync[i].id);
                if (servoSync[i].angle > 368640.0f) {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * 368640.0f));
                } else if (servoSync[i].angle < -368640.0f) {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * -368640.0f));
                } else {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * servoSync[i].angle));
                }
                RingBuffer_WriteULong(&ringBuf, servoSync[i].interval_multi);
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_acc);
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_dec);
                RingBuffer_WriteShort(&ringBuf, servoSync[i].power);
            }
            break;
        case MODE_SET_SERVO_ANGLE_MTURN_BY_VELOCITY:
            /* Synchronization command sets servo angle (multi-turn mode, specified RPM) */
						size = 3 + servo_count * 13;
            RingBuffer_Init(&ringBuf, size, buffer);
            RingBuffer_WriteByte(&ringBuf, 15);
            RingBuffer_WriteByte(&ringBuf, 13);
            RingBuffer_WriteByte(&ringBuf, servo_count);
            for (int i = 0; i < servo_count; i++) {
                RingBuffer_WriteByte(&ringBuf, servoSync[i].id);
                if (servoSync[i].angle > 368640.0f) {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * 368640.0f));
                } else if (servoSync[i].angle < -368640.0f) {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * -368640.0f));
                } else {
                    RingBuffer_WriteLong(&ringBuf, (int32_t)(10 * servoSync[i].angle));
                }
                RingBuffer_WriteUShort(&ringBuf, (uint16_t)(10.0f * servoSync[i].velocity));
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_acc);
                RingBuffer_WriteUShort(&ringBuf, servoSync[i].t_dec);
                RingBuffer_WriteShort(&ringBuf, servoSync[i].power);
            }
            break;
				case MODE_Query_SERVO_Monitor:
            /* Synchronization command to read data from servos */
						size = 3 + servo_count;
						RingBuffer_Init(&ringBuf, size, buffer);
						RingBuffer_WriteByte(&ringBuf, 22);
						RingBuffer_WriteByte(&ringBuf, 1);
						RingBuffer_WriteByte(&ringBuf, servo_count);
						for (int i = 0; i < servo_count; i++)RingBuffer_WriteByte(&ringBuf, servoSync[i].id);		
            break;
						
        default:
            return FSUS_STATUS_ERRO; // Invalid patterns
    }

    // Send the request packet
		if(size<=255)    FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_SyncCommand,(uint8_t)size, buffer + 1,0);
		else     FSUS_SendPackage_Common(usart, FSUS_CMD_SET_SERVO_SyncCommand,size, buffer + 1,1);

		if(ServoMode==7)FSUS_SyncServoMonitor(usart, servo_count, servodata);

    return FSUS_STATUS_SUCCESS;
}	
/* Synchronization command servo data parsing function */
FSUS_STATUS FSUS_SyncServoMonitor(Usart_DataTypeDef *usart, uint8_t servo_count, ServoData servodata[]) {
    PackageTypeDef pkg;
    FSUS_STATUS status;
		double temp;//Temperature Data Conversion
		uint8_t packet_count = 0;
		
		while (packet_count < servo_count) {
        // Call the synchronization function to receive a packet
        status = FSUS_sync_RecvPackage(usart, &pkg);

        if (status != FSUS_STATUS_SUCCESS) {
            return status;  // If reception fails, return an error status
				}
				// Parses the current packet contents
        servodata[packet_count].id = pkg.content[0];
        servodata[packet_count].voltage = (int16_t)((pkg.content[2] << 8) | pkg.content[1]);
        servodata[packet_count].current = (int16_t)((pkg.content[4] << 8) | pkg.content[3]);
        servodata[packet_count].power = (int16_t)((pkg.content[6] << 8) | pkg.content[5]);
        servodata[packet_count].temperature = (int16_t)((pkg.content[8] << 8) | pkg.content[7]);
				temp = (double)servodata[packet_count].temperature;
				servodata[packet_count].temperature = 1 / (log(temp / (4096.0f - temp)) / 3435.0f + 1 / (273.15 + 25)) - 273.15;
        servodata[packet_count].status = pkg.content[9];
        servodata[packet_count].angle = (int32_t)((pkg.content[13] << 24) | (pkg.content[12] << 16) | (pkg.content[11] << 8) | pkg.content[10]);
				servodata[packet_count].angle = servodata[packet_count].angle/10.0f;
        servodata[packet_count].circle_count = (int16_t)((pkg.content[15] << 8) | pkg.content[14]);

				// Complete parsing a set of packets, plus one.
				packet_count++;
				
				// Reset the pkg state to receive the next set of data
        pkg.status = 0;

			}
		RingBuffer_Reset(usart->recvBuf);
    return FSUS_STATUS_SUCCESS;
}
