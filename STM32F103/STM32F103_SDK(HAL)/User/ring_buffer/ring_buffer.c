#include "ring_buffer.h"

// Queue Reset
void RingBuffer_Reset(RingBufferTypeDef *ringBuf){
	// Both the head and tail pointers point to the first address of the buffer.
	ringBuf->head = 0;
	ringBuf->tail = 0;
}

// Creating a Queue
void RingBuffer_Init(RingBufferTypeDef* ringBuf, uint16_t capacity, uint8_t *buffer){
	// Buffer memory request successful
	ringBuf->buffer = buffer;// (uint8_t *)malloc(ringBuf->size);
	ringBuf->head = 0;
	ringBuf->tail = 0;
	// The size needs to be larger than the capacity.
	ringBuf->size = capacity + 1;

	for (uint16_t i=0; i<ringBuf->size; i++){
		ringBuf->buffer[i] = 0;
	}
}

// Getting the capacity of the ring queue
uint16_t RingBuffer_GetCapacity(RingBufferTypeDef *ringBuf){
	return ringBuf->size - 1;
}

// Get the number of bytes already stored in the ring queue
uint16_t RingBuffer_GetByteUsed(RingBufferTypeDef *ringBuf) {
    if (ringBuf->head <= ringBuf->tail) {
        return ringBuf->tail - ringBuf->head;
    } else {
        return ringBuf->size - (ringBuf->head - ringBuf->tail);
    }
}

// Get the remaining capacity of the queue
uint16_t RingBuffer_GetByteFree(RingBufferTypeDef *ringBuf){
	return RingBuffer_GetCapacity(ringBuf) - RingBuffer_GetByteUsed(ringBuf);
}

// Determine if the ring queue is empty
uint8_t RingBuffer_IsEmpty(RingBufferTypeDef *ringBuf){
	return RingBuffer_GetByteFree(ringBuf) == RingBuffer_GetCapacity(ringBuf);
}

// Determine if the ring queue is full
uint8_t RingBuffer_IsFull(RingBufferTypeDef *ringBuf){
	return RingBuffer_GetByteFree(ringBuf) == 0;
}

// Read the ith element according to the index number
uint8_t RingBuffer_GetValueByIndex(RingBufferTypeDef *ringBuf, uint16_t index){
	if (index >= RingBuffer_GetByteUsed(ringBuf)){
		// Index number out of range
		return 0;
	}
	uint16_t rbIdx = (ringBuf->head + index + 1) % ringBuf->size;
	return ringBuf->buffer[rbIdx];
}

// Popup team header element
uint8_t RingBuffer_Pop(RingBufferTypeDef *ringBuf){
	uint8_t temp;
	if(RingBuffer_IsEmpty(ringBuf)){
		return 0;
	}
	else{
		ringBuf->head =(ringBuf->head + 1)%ringBuf->size;
		temp = ringBuf->buffer[ringBuf->head];
		ringBuf->buffer[ringBuf->head] = 0;
		return temp;
	}
	
}


// Write to the last element of the queue
void RingBuffer_Push(RingBufferTypeDef *ringBuf, uint8_t value) {
    if (RingBuffer_IsFull(ringBuf)) {
        return;
    }
    ringBuf->tail = (ringBuf->tail + 1) % ringBuf->size;
    ringBuf->buffer[ringBuf->tail] = value;
}

// Read a single byte
uint8_t RingBuffer_ReadByte(RingBufferTypeDef *ringBuf){
	return RingBuffer_Pop(ringBuf);
}

// Read byte array
void RingBuffer_ReadByteArray(RingBufferTypeDef *ringBuf, uint8_t* dest, uint16_t size){
	for(uint16_t idx=0; idx<size; idx++){
		dest[idx] = RingBuffer_Pop(ringBuf);
	}
}

// Read signed Short integer (two bytes)
int16_t RingBuffer_ReadShort(RingBufferTypeDef *ringBuf){
	int16_t value;
	uint8_t *p = (uint8_t *)&value;
	RingBuffer_ReadByteArray(ringBuf, p, 2);
	return value;
}

// Read unsigned Short integer (two bytes)
uint16_t RingBuffer_ReadUShort(RingBufferTypeDef *ringBuf){
	uint16_t value;
	uint8_t *p = (uint8_t *)&value;
	RingBuffer_ReadByteArray(ringBuf, p, 2);
	return value;
}

// Reads an integer of signed Long type (four bytes)
int32_t RingBuffer_ReadLong(RingBufferTypeDef *ringBuf){
	int32_t value;
	uint8_t *p = (uint8_t *)&value;
	RingBuffer_ReadByteArray(ringBuf, p, 4);
	return value;
}

// Reads an integer of unsigned Long type (four bytes)
uint32_t RingBuffer_ReadULong(RingBufferTypeDef *ringBuf){
	uint32_t value;
	uint8_t *p = (uint8_t *)&value;
	RingBuffer_ReadByteArray(ringBuf, p, 4);
	return value;
}

// Read floating point numbers (four bytes)
float RingBuffer_ReadFloat(RingBufferTypeDef *ringBuf){
	float value;
	uint8_t *p = (uint8_t *)&value;
	RingBuffer_ReadByteArray(ringBuf, p, 4);
	return value;
}

// Write single byte
void RingBuffer_WriteByte(RingBufferTypeDef *ringBuf, uint8_t value){
	RingBuffer_Push(ringBuf, value);
}

// Write to byte array
void RingBuffer_WriteByteArray(RingBufferTypeDef *ringBuf, uint8_t* src, uint16_t size){
	for(uint16_t idx=0; idx<size; idx++){
		RingBuffer_Push(ringBuf, src[idx]);
	}
}
// Write signed Short integer (two bytes)
void RingBuffer_WriteShort(RingBufferTypeDef *ringBuf, int16_t value){
	uint8_t *p= (uint8_t *)&value;
	RingBuffer_WriteByteArray(ringBuf, p, 2);
}

// Write unsigned Short integer (two bytes)
void RingBuffer_WriteUShort(RingBufferTypeDef *ringBuf, uint16_t value){
	uint8_t *p= (uint8_t *)&value;
	RingBuffer_WriteByteArray(ringBuf, p, 2);
}

// Write integer of signed Long type (four bytes)
void RingBuffer_WriteLong(RingBufferTypeDef *ringBuf, int32_t value){
	uint8_t *p= (uint8_t *)&value;
	RingBuffer_WriteByteArray(ringBuf, p, 4);
}

// Write an integer of unsigned Long type (four bytes)
void RingBuffer_WriteULong(RingBufferTypeDef *ringBuf, uint32_t value){
	uint8_t *p= (uint8_t *)&value;
	RingBuffer_WriteByteArray(ringBuf, p, 4);
}

// Write floating point number (four bytes)
void RingBuffer_WriteFloat(RingBufferTypeDef *ringBuf, float value){
	uint8_t *p= (uint8_t *)&value;
	RingBuffer_WriteByteArray(ringBuf, p, 4);
}


// Calculate the checksum
uint8_t RingBuffer_GetChecksum(RingBufferTypeDef *ringBuf) {
    uint16_t nByte = RingBuffer_GetByteUsed(ringBuf);
    uint32_t bSum = 0;
    for (uint16_t i = 0; i < nByte; i++) {
        uint16_t rbIdx = (ringBuf->head + i + 1) % ringBuf->size;
        bSum = (bSum + ringBuf->buffer[rbIdx]) % 256;
    }
    return (uint8_t)bSum;
}

uint8_t RingBuffer_PeekByte(RingBufferTypeDef *ringBuf, uint16_t index) {
    if (index >= RingBuffer_GetByteUsed(ringBuf)) return 0;
    uint16_t rbIdx = (ringBuf->head + index + 1) % ringBuf->size;
    return ringBuf->buffer[rbIdx];
}

uint16_t RingBuffer_PeekUShort(RingBufferTypeDef *ringBuf, uint16_t index) {
    if (index + 1 >= RingBuffer_GetByteUsed(ringBuf)) return 0;
    uint16_t rbIdx = (ringBuf->head + index + 1) % ringBuf->size;
    return (ringBuf->buffer[rbIdx] | (ringBuf->buffer[(rbIdx + 1) % ringBuf->size] << 8));
}

// Synchronized Command Length Write Support
void RingBuffer_WriteUShortWithFlag(RingBufferTypeDef *ringBuf, uint16_t value) {
    RingBuffer_WriteByte(ringBuf, 0xFF);       // Write Sync Flag
    RingBuffer_WriteUShort(ringBuf, value);    // Write 16-bit length
}
// Enhanced read logic to resolve synchronization lengths
uint16_t RingBuffer_ReadUShortWithFlag(RingBufferTypeDef *ringBuf, uint8_t *isSync) {
    uint8_t flag = RingBuffer_ReadByte(ringBuf);
    *isSync = (flag == 0xFF) ? 1 : 0;
    if (*isSync) {
        return RingBuffer_ReadUShort(ringBuf); // Read 16-bit length
    } else {
        return (uint16_t)flag;                // Returns the 8-bit length directly
    }
}
