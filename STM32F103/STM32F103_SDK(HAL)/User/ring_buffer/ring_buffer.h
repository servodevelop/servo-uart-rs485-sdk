#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H


#include "main.h"


// ring-queue structure
typedef struct
{  
    uint8_t *buffer; // buffer
    uint16_t head; // team leader pointer
    uint16_t tail; // pointer at the end of the queue
    uint16_t size; // Dimensions of the ring queue
} RingBufferTypeDef;

// Creating a Queue
void RingBuffer_Init(RingBufferTypeDef* ringBuf, uint16_t capacity, uint8_t *buffer);
// Queue Reset
void RingBuffer_Reset(RingBufferTypeDef *ringBuf);
// Destruction queue
void RingBuffer_Destory(RingBufferTypeDef *ringBuf);
// Get the capacity of the queue
uint16_t RingBuffer_GetCapacity(RingBufferTypeDef *ringBuf);
// Get the number of bytes already stored in the ring queue
uint16_t RingBuffer_GetByteUsed(RingBufferTypeDef *ringBuf);
// Get the remaining free bytes in the queue
uint16_t RingBuffer_GetByteFree(RingBufferTypeDef *ringBuf);
// Whether the queue is empty
uint8_t RingBuffer_IsEmpty(RingBufferTypeDef *ringBuf);
// Whether the queue is full
uint8_t RingBuffer_IsFull(RingBufferTypeDef *ringBuf);
// Read the ith element according to the index number
uint8_t RingBuffer_GetValueByIndex(RingBufferTypeDef *ringBuf, uint16_t index);
// Popup team header element
uint8_t RingBuffer_Pop(RingBufferTypeDef *ringBuf);
// Read a single byte
uint8_t RingBuffer_ReadByte(RingBufferTypeDef *ringBuf);
// Read byte array
void RingBuffer_ReadByteArray(RingBufferTypeDef *ringBuf, uint8_t* dest, uint16_t size);
// Read signed Short integer (two bytes)
int16_t RingBuffer_ReadShort(RingBufferTypeDef *ringBuf);
// Read unsigned Short integer (two bytes)
uint16_t RingBuffer_ReadUShort(RingBufferTypeDef *ringBuf);
// Reads an integer of signed Long type (four bytes)
int32_t RingBuffer_ReadLong(RingBufferTypeDef *ringBuf);
// Reads an integer of unsigned Long type (four bytes)
uint32_t RingBuffer_ReadULong(RingBufferTypeDef *ringBuf);
// Read floating point numbers (four bytes)
float RingBuffer_ReadFloat(RingBufferTypeDef *ringBuf);
// Write to the last element of the queue
void RingBuffer_Push(RingBufferTypeDef *ringBuf, uint8_t value);
// Write single byte
void RingBuffer_WriteByte(RingBufferTypeDef *ringBuf, uint8_t value);
// Write to byte array
void RingBuffer_WriteByteArray(RingBufferTypeDef *ringBuf, uint8_t* src, uint16_t size);
// Write signed Short integer (two bytes)
void RingBuffer_WriteShort(RingBufferTypeDef *ringBuf, int16_t value);
// Write unsigned Short integer (two bytes)
void RingBuffer_WriteUShort(RingBufferTypeDef *ringBuf, uint16_t value);
// Write integer of signed Long type (four bytes)
void RingBuffer_WriteLong(RingBufferTypeDef *ringBuf, int32_t value);
// Write an integer of unsigned Long type (four bytes)
void RingBuffer_WriteULong(RingBufferTypeDef *ringBuf, uint32_t value);
// Write floating point number (four bytes)
void RingBuffer_WriteFloat(RingBufferTypeDef *ringBuf, float value);
// Calculate all own checksums of the buffer
uint8_t RingBuffer_GetChecksum(RingBufferTypeDef *ringBuf);

// ring_buffer.c
uint8_t RingBuffer_PeekByte(RingBufferTypeDef *ringBuf, uint16_t index);
uint16_t RingBuffer_PeekUShort(RingBufferTypeDef *ringBuf, uint16_t index);

// Synchronized Command Length Write Support
void RingBuffer_WriteUShortWithFlag(RingBufferTypeDef *ringBuf, uint16_t value);
// Enhanced read logic to resolve synchronization lengths
uint16_t RingBuffer_ReadUShortWithFlag(RingBufferTypeDef *ringBuf, uint8_t *isSync);
#endif
