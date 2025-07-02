/*
 * 用来处理舵机的底层通信协议
 *--------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2024/08/05
 */
#include <iostream>
#include "FashionStar_UartServoProtocol.h"

using namespace itas109;
using namespace fsuservo;

FSUS_Protocol::FSUS_Protocol(std::string port_name,  itas109::BaudRate baudrate){
    // 记录端口号
    this->port_name = port_name;
    this->baudrate = baudrate;
    // 初始化串口对象
    this->serial.init(port_name, baudrate);
    // 打开端口
	this->serial.open();
	// 判断端口是否有效
    // TODO 处理异常
	if(!this->serial.isOpened()){
		std::cout << "Uart " <<  port_name << " is not valid " << std::endl;
        exit(-1);
	}
}

 // 获取当前的时间戳，单位ms
uint32_t FSUS_Protocol::millis(){
    // 延时函数定义
#ifdef _WIN32
    // cout << "millis = " << (uint32_t)GetTickCount() << endl;
    return (uint32_t)GetTickCount();
#else
    struct timespec _t;
    clock_gettime(CLOCK_REALTIME, &_t);
    return (uint32_t)(_t.tv_sec*1000 + lround(_t.tv_nsec/1.0e6));
#endif


}

void FSUS_Protocol::delay_ms(uint32_t microsecond){
    // 延时函数定义
    #ifdef _WIN32
        Sleep(microsecond); // ms
    #else
        usleep(1000 * microsecond); // ms
    #endif
}

// 通过串口发送一个字节
void FSUS_Protocol::write(uint8_t value){
    this->serial.writeData((char *)&value, 1);
}

// 读取单个字节
bool FSUS_Protocol::read(uint8_t* value){
    if (recv_queue.empty()){
        return false;
    }

    *value = recv_queue.front();
    recv_queue.pop();

    // cout << "read byet" << (*value) << endl;
    return true;
}

// 读入所有
int FSUS_Protocol::readAll(uint8_t* buffer){
    return this->serial.readAllData((char *)buffer);
}

// 串口当前是否有数据读入
int FSUS_Protocol::available(){
    int recv_nbyte; // 接收字节数
    // 串口数据接收更新
    recv_nbyte = readAll(tmp_recv_buffer);

    // if(recv_nbyte <= 0){
    //     cout << "recv " << recv_nbyte << "" << endl;
    // }

    // 将数据压入队列
    for(int ibyte = 0; ibyte < recv_nbyte; ibyte++){
        recv_queue.push(tmp_recv_buffer[ibyte]);
    }

    // 返回接收缓冲区的长度
    return recv_queue.size();
}

//加工并发送请求数据包
void FSUS_Protocol::sendPack(){
    // 数据加工
    requestPack.header = FSUS_PACK_REQUEST_HEADER; // 数据头
    // 计算校验码
    requestPack.checksum = calcPackChecksum(&requestPack);
    // 通过串口发送出去
     // 创建缓冲区并填充数据
    uint8_t buffer[FSUS_PACK_REQUEST_MAX_SIZE]; // 根据实际最大包大小定义
    uint16_t index = 0;

    // 填充包头
    buffer[index++] = requestPack.header & 0xFF;
    buffer[index++] = requestPack.header >> 8;

    // 填充指令ID
    buffer[index++] = requestPack.cmdId;

    // 填充内容长度
    if(requestPack.content_size < 255) {
        buffer[index++] = requestPack.content_size & 0xFF;
    } else {
        buffer[index++] = 0xFF;
        buffer[index++] = requestPack.content_size & 0xFF;
        buffer[index++] = requestPack.content_size >> 8;
    }

    // 填充内容
    for(uint16_t i = 0; i < requestPack.content_size; i++) {
        buffer[index++] = requestPack.content[i];
    }

    // 填充校验和
    buffer[index++] = requestPack.checksum;

    // 一次性发送整个缓冲区
    this->serial.writeData((char*)buffer, index);
}

// 清空缓冲区
void FSUS_Protocol::emptyCache(){
    // 清空UART接收缓冲区
    available(); // 串口接收更新
    // 清空队列(通过置换)
    std::queue<uint8_t> empty;
	swap(empty, recv_queue);
}

// 初始化响应数据包的数据结构
void FSUS_Protocol::initResponsePack(){
    // 初始化响应包的数据
    responsePack.header = 0;
    responsePack.cmdId = 0;
    responsePack.content_size = 0;
    responsePack.checksum = 0;
    responsePack.recv_cnt = 0;
}

//接收响应包
FSUS_STATUS FSUS_Protocol::recvPack(){
    uint32_t start_time = millis();

    // responsePack.recv_status = 0; //重置标志位
    responsePack.recv_cnt = 0; // 数据帧接收标志位

    while(true){
        // 超时判断
        if((millis() - start_time) > FSUS_TIMEOUT_MS){
            return FSUS_STATUS_TIMEOUT;
        }
        // 等待有字节读入
        if (available()==0){
            continue;
        }
        uint8_t curByte;
        read(&curByte);

        uint8_t curIdx = responsePack.recv_cnt;
        responsePack.recv_buffer[curIdx] = curByte; // 接收一个字节
        responsePack.recv_cnt+=1; // 计数自增

        // 数据接收是否完成
        if(curIdx==1){
            // 校验帧头
            responsePack.header = responsePack.recv_buffer[curIdx-1] | curByte << 8;
            if(responsePack.header!=FSUS_PACK_RESPONSE_HEADER){
                return FSUS_STATUS_WRONG_RESPONSE_HEADER;
            }
        }else if (curIdx==2){
            // 载入cmdId
            responsePack.cmdId = curByte;
            // 检查cmdId是否满足指令范围
            if(responsePack.cmdId > FSUS_CMD_NUM){
                return FSUS_STATUS_UNKOWN_CMD_ID;
            }
        }else if (curIdx==3){
            // 载入Size
            responsePack.content_size = curByte;
            // 判断size是否合法
            if(responsePack.content_size > FSUS_PACK_RESPONSE_MAX_SIZE){
                return FSUS_STATUS_SIZE_TOO_BIG;
            }
        }else if (curIdx < 4+responsePack.content_size){
            // 填充内容
            responsePack.content[curIdx-4] = curByte;
        }else{
            // 接收校验合
            responsePack.checksum = curByte;
            // 检查校验和是否匹配
            FSUS_CHECKSUM_T checksum = calcPackChecksum(&responsePack);
            // if (responsePack.cmdId == FSUS_CMD_QUERY_ANGLE){
            //     checksum -= 0x03;// TODO Delete 不知道为什么要这样
            // }

            if (checksum != responsePack.checksum){
                return FSUS_STATUS_CHECKSUM_ERROR;
            }else{
                return FSUS_STATUS_SUCCESS;
            }
        }
    }
}

//计算CRC校验码
FSUS_CHECKSUM_T FSUS_Protocol::calcPackChecksum(const FSUS_PACKAGE_T *package){
    // uint16_t checksum = 0;
    uint16_t checksum = 0;
    checksum += (package->header & 0xFF);
    checksum += (package->header >> 8);
    checksum += package->cmdId;
    if(package->content_size<255)checksum += (package->content_size & 0xFF);
    else{
        checksum += 0xFF;
        checksum += (package->content_size & 0xFF);
        checksum += (package->content_size >> 8);
    }
    for(uint16_t i=0; i<package->content_size; i++){
        checksum += package->content[i];
    }

    return (FSUS_CHECKSUM_T)(checksum%256);
}
// 获取请求内容的尺寸
FSUS_PACKAGE_SIZE_T FSUS_Protocol::getPackSize(const FSUS_PACKAGE_T *package){
    // 包头(2 uint8_t) + 指令ID(1byte) + 长度(1byte) + 内容 + 校验码(1byte)
    return package->content_size + 5;
}

// 发送PING的请求包
void FSUS_Protocol::sendPing(FSUS_SERVO_ID_T servoId){
    requestPack.cmdId = FSUS_CMD_PING;
    requestPack.content_size = 1;
    requestPack.content[0] = servoId;
    sendPack(); // 发送包
}

// 接收PING的响应包
FSUS_STATUS FSUS_Protocol::recvPing(FSUS_SERVO_ID_T* servoId, bool *isOnline){
    // 接收数据帧
    FSUS_STATUS status = recvPack();

    *servoId = responsePack.content[0]; // 提取舵机ID
    *isOnline = (status == FSUS_STATUS_SUCCESS);
    responsePack.recv_status = status;
    return status;
}

// 发送旋转的请求包
void FSUS_Protocol::sendSetAngle(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle,FSUS_INTERVAL_T interval,FSUS_POWER_T power){
    requestPack.cmdId = FSUS_CMD_SET_ANGLE; // 指令ID
    requestPack.content_size = 7; // 内容长度
    requestPack.content[0]=servoId; //舵机ID
    int16_t angle_int = angle * 10; //舵机的角度
    requestPack.content[1] = angle_int & 0xFF;
    requestPack.content[2] = angle_int >> 8;
    requestPack.content[3] = interval & 0xFF; //周期
    requestPack.content[4] = interval >> 8;
    requestPack.content[5] = power & 0xFF; //功率
    requestPack.content[6] = power >> 8;
    sendPack();
}

// 发送旋转的请求包(指定周期)
void FSUS_Protocol::sendSetAngleByInterval(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle,FSUS_INTERVAL_T interval, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power){
    requestPack.cmdId = FSUS_CMD_SET_ANGLE_BY_INTERVAL; // 指令ID
    requestPack.content_size = 11; // 内容长度
    requestPack.content[0]=servoId; //舵机ID
    int16_t angle_int = angle * 10; //舵机的角度
    requestPack.content[1] = angle_int & 0xFF;
    requestPack.content[2] = angle_int >> 8;
    requestPack.content[3] = interval & 0xFF; //周期
    requestPack.content[4] = interval >> 8;
    requestPack.content[5] = t_acc & 0xFF;
    requestPack.content[6] = t_acc >> 8;
    requestPack.content[7] = t_dec & 0xFF;
    requestPack.content[8] = t_dec >> 8;
    requestPack.content[9] = power & 0xFF; //功率
    requestPack.content[10] = power >> 8;
    sendPack();
}

// 发送旋转的请求包(指定转速)
void FSUS_Protocol::sendSetAngleByVelocity(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle,FSUS_SERVO_SPEED_T velocity, FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power){
    requestPack.cmdId = FSUS_CMD_SET_ANGLE_BY_VELOCITY; // 指令ID
    requestPack.content_size = 11;           // 内容长度
    requestPack.content[0]=servoId;         //舵机ID
    int16_t angle_int = angle * 10;             //舵机的角度
    uint16_t velocity_int = velocity * 10; // 转速
    requestPack.content[1] = angle_int & 0xFF;
    requestPack.content[2] = angle_int >> 8;
    requestPack.content[3] = velocity_int & 0xFF; //周期
    requestPack.content[4] = velocity_int >> 8;
    requestPack.content[5] = t_acc & 0xFF;
    requestPack.content[6] = t_acc >> 8;
    requestPack.content[7] = t_dec & 0xFF;
    requestPack.content[8] = t_dec >> 8;
    requestPack.content[9] = power & 0xFF; //功率
    requestPack.content[10] = power >> 8;
    sendPack();
}

// 发送舵机角度查询指令
void FSUS_Protocol::sendQueryAngle(FSUS_SERVO_ID_T servoId){
    requestPack.cmdId = FSUS_CMD_QUERY_ANGLE;
    requestPack.content_size = 1;
    requestPack.content[0] = servoId;
    sendPack();
}

// 接收角度查询指令
FSUS_STATUS FSUS_Protocol::recvQueryAngle(FSUS_SERVO_ID_T *servoId, FSUS_SERVO_ANGLE_T *angle){
    FSUS_STATUS status = recvPack();
    int16_t angleVal;
    uint8_t* angleValPtr = (uint8_t*)&angleVal;

    // angleVal = responsePack.content[1] + responsePack.content[2] << 8;
    // if(status == FSUS_STATUS_SUCCESS){
    // 偶尔会出现校验和错误的情况, 临时允许
    if(status == FSUS_STATUS_SUCCESS || status==FSUS_STATUS_CHECKSUM_ERROR){
        (*servoId) = responsePack.content[0];
        angleValPtr[0] = responsePack.content[1];
        angleValPtr[1] = responsePack.content[2];
        (*angle) = 0.1*(float)angleVal;
        // (*angle) = 0.1 * (uint16_t)(responsePack.content[1] | responsePack.content[2]<< 8);
    }
    responsePack.recv_status = status;
    return status;
}

// 发送旋转的请求包(多圈)
void FSUS_Protocol::sendSetAngleMTurn(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle,FSUS_INTERVAL_T_MTURN interval,FSUS_POWER_T power){
    requestPack.cmdId = FSUS_CMD_SET_ANGLE_MTURN; // 指令ID
    requestPack.content_size = 11; // 内容长度
    requestPack.content[0]=servoId; //舵机ID
    int32_t angle_long = angle * 10; //舵机的角度
    // 角度
    requestPack.content[1] = angle_long & 0xFF;
    requestPack.content[2] = (angle_long >> 8) & 0xFF;
    requestPack.content[3] = (angle_long >> 16) & 0xFF;
    requestPack.content[4] = (angle_long >> 24) & 0xFF;
    // 周期
    requestPack.content[5] = interval & 0xFF;
    requestPack.content[6] = (interval >> 8) & 0xFF;
    requestPack.content[7] = (interval >> 16) & 0xFF;
    requestPack.content[8] = (interval >> 24) & 0xFF;
    // 功率
    requestPack.content[9] = power & 0xFF;
    requestPack.content[10] = (power >> 8) & 0xFF;
    sendPack();
}

// 发送旋转的请求包(多圈+指定周期)
void FSUS_Protocol::sendSetAngleMTurnByInterval(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_INTERVAL_T_MTURN interval, \
    FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power){

    requestPack.cmdId = FSUS_CMD_SET_ANGLE_MTURN_BY_INTERVAL; // 指令ID
    requestPack.content_size = 15; // 内容长度
    requestPack.content[0]=servoId; //舵机ID
    int32_t angle_long = angle * 10; //舵机的角度
    // 角度
    requestPack.content[1] = angle_long & 0xFF;
    requestPack.content[2] = (angle_long >> 8) & 0xFF;
    requestPack.content[3] = (angle_long >> 16) & 0xFF;
    requestPack.content[4] = (angle_long >> 24) & 0xFF;
    // 周期
    requestPack.content[5] = interval & 0xFF;
    requestPack.content[6] = (interval >> 8) & 0xFF;
    requestPack.content[7] = (interval >> 16) & 0xFF;
    requestPack.content[8] = (interval >> 24) & 0xFF;
    // 加速时间
    requestPack.content[9] = t_acc & 0xFF;
    requestPack.content[10] = (t_acc >> 8) & 0xFF;
    // 减速时间
    requestPack.content[11] = t_dec & 0xFF;
    requestPack.content[12] = (t_dec >> 8) & 0xFF;
    // 功率
    requestPack.content[13] = power & 0xFF;
    requestPack.content[14] = (power >> 8) & 0xFF;
    sendPack();
}

// 发送旋转的请求包(多圈+指定转速)
void FSUS_Protocol::sendSetAngleMTurnByVelocity(FSUS_SERVO_ID_T servoId, FSUS_SERVO_ANGLE_T angle, FSUS_SERVO_SPEED_T velocity, \
    FSUS_INTERVAL_T t_acc, FSUS_INTERVAL_T t_dec, FSUS_POWER_T power){
    // 请求头
    requestPack.cmdId = FSUS_CMD_SET_ANGLE_MTURN_BY_VELOCITY; // 指令ID
    requestPack.content_size = 13;             // 内容长度
    requestPack.content[0]=servoId;            //舵机ID
    int32_t angle_long = angle * 10;              //舵机的角度
    uint16_t velocity_int = velocity * 10; // 舵机转速 单位0.1°/s
    // 角度
    requestPack.content[1] = angle_long & 0xFF;
    requestPack.content[2] = (angle_long >> 8) & 0xFF;
    requestPack.content[3] = (angle_long >> 16) & 0xFF;
    requestPack.content[4] = (angle_long >> 24) & 0xFF;
    // 周期
    requestPack.content[5] = velocity_int & 0xFF;
    requestPack.content[6] = (velocity_int >> 8) & 0xFF;
    // 加速时间
    requestPack.content[7] = t_acc & 0xFF;
    requestPack.content[8] = (t_acc >> 8) & 0xFF;
    // 减速时间
    requestPack.content[9] = t_dec & 0xFF;
    requestPack.content[10] = (t_dec >> 8) & 0xFF;
    // 功率
    requestPack.content[11] = power & 0xFF;
    requestPack.content[12] = (power >> 8) & 0xFF;
    sendPack();
}

// 发送舵机角度查询指令
void FSUS_Protocol::sendQueryAngleMTurn(FSUS_SERVO_ID_T servoId){
    requestPack.cmdId = FSUS_CMD_QUERY_ANGLE_MTURN;
    requestPack.content_size = 1;
    requestPack.content[0] = servoId;
    sendPack();
}

// 接收角度查询指令(多圈模式)
FSUS_STATUS FSUS_Protocol::recvQueryAngleMTurn(FSUS_SERVO_ID_T *servoId, FSUS_SERVO_ANGLE_T *angle){
    FSUS_STATUS status = recvPack();
    int32_t angleVal;
    uint8_t* angleValPtr = (uint8_t*)&angleVal;

    // 偶尔会出现校验和错误的情况, 临时允许
    if(status == FSUS_STATUS_SUCCESS || status==FSUS_STATUS_CHECKSUM_ERROR){
        (*servoId) = responsePack.content[0];

        angleValPtr[0] = responsePack.content[1];
        angleValPtr[1] = responsePack.content[2];
        angleValPtr[2] = responsePack.content[3];
        angleValPtr[3] = responsePack.content[4];

        (*angle) = 0.1*angleVal;
    }
    responsePack.recv_status = status;
    return status;
}

// 发送阻尼模式
void FSUS_Protocol::sendDamping(FSUS_SERVO_ID_T servoId, FSUS_POWER_T power){
    requestPack.cmdId = FSUS_CMD_DAMPING;
    requestPack.content_size = 3;
    requestPack.content[0] = servoId;
    requestPack.content[1] = power & 0xFF;
    requestPack.content[2] = power >> 8;
    sendPack();
}

// 发送重置用户数据
void FSUS_Protocol::sendResetUserData(FSUS_SERVO_ID_T servoId){
    requestPack.cmdId = FSUS_CMD_RESET_USER_DATA;
    requestPack.content_size = 1;
    requestPack.content[0] = servoId;
    sendPack();
}

// 接收重置用户数据
FSUS_STATUS FSUS_Protocol::recvResetUserData(FSUS_SERVO_ID_T *servoId, bool *result){
    FSUS_STATUS status = recvPack();
    if(status == FSUS_STATUS_SUCCESS){
        *servoId = responsePack.content[0];
        *result = responsePack.content[1];
    }

    return status;
}

// 发送数据读取指令
void FSUS_Protocol::sendReadData(FSUS_SERVO_ID_T servoId, uint8_t address){
    requestPack.cmdId = FSUS_CMD_READ_DATA;
    requestPack.content_size = 2;
    requestPack.content[0] = servoId;
    requestPack.content[1] = address;
    sendPack();
}

// 接收数据读取指令
FSUS_STATUS FSUS_Protocol::recvReadData(FSUS_SERVO_ID_T *servoId, uint8_t *address, uint8_t *contentLen, uint8_t *content){
    FSUS_STATUS status = recvPack();
    if(status == FSUS_STATUS_SUCCESS){
        *servoId = responsePack.content[0];
        *address = responsePack.content[1]; // 获取地址位
        *contentLen = responsePack.content_size - 2; // 计算得到数据位的长度
        // 数据拷贝
        for(uint16_t i=0; i<*contentLen; i++){
            content[i] = responsePack.content[i+2];
        }
    }
    return status;
}

// 发送数据写入指令
void FSUS_Protocol::sendWriteData(FSUS_SERVO_ID_T servoId, uint8_t address, uint8_t contentLen, uint8_t *content){
    requestPack.cmdId = FSUS_CMD_WRITE_DATA;
    requestPack.content_size = 2+contentLen;
    requestPack.content[0] = servoId;
    requestPack.content[1] = address;
    for(uint16_t i=0; i<contentLen; i++){
        requestPack.content[i+2] = content[i];
    }
    sendPack();
}

// 接收数据写入指令
FSUS_STATUS FSUS_Protocol::recvWriteData(FSUS_SERVO_ID_T *servoId, uint8_t *address, bool *result){
    FSUS_STATUS status = recvPack();
    if(status == FSUS_STATUS_SUCCESS){
        *servoId = responsePack.content[0];
        *address = responsePack.content[1]; // 获取地址位
        *result = responsePack.content[2];
    }
    return status;
}

// 发送设置原点的请求包
void FSUS_Protocol::sendSetOriginPoint(FSUS_SERVO_ID_T servoId){
    requestPack.cmdId = FSUS_CMD_SET_ORIGIN_POINT;
    requestPack.content_size = 2;
    requestPack.content[0] = servoId;
    requestPack.content[1] = 0;
    sendPack(); // 发送包
}

// 发送清除多圈圈数请求包
void FSUS_Protocol::sendResetMultiTurnAngle(FSUS_SERVO_ID_T servoId){
    requestPack.cmdId = FSUS_CMD_RESET_MTURN_ANGLE;
    requestPack.content_size = 1;
    requestPack.content[0] = servoId;
    sendPack(); // 发送包
}

// 发送舵机开启异步指令（可以存储一次命令）
void FSUS_Protocol::sendServoBeginAsync(){
    requestPack.cmdId = FSUS_CMD_BEGIN_ASYNC;
    requestPack.content_size = 0;
    requestPack.content[0] = 0;
    sendPack();
}
// 发送舵机停止异步指令
void FSUS_Protocol::sendServoEndAsync(FSUS_MODE_T mode){
/*mode
 0：执行存储的命令
 1：取消存储的命令
*/
    requestPack.cmdId = FSUS_CMD_END_ASYNC;
    requestPack.content_size = 1;
    requestPack.content[0] = mode;
    sendPack();
}

// 发送舵机数据监控查询指令
void FSUS_Protocol::sendServoMonitor(FSUS_SERVO_ID_T servoId){
    requestPack.cmdId = FSUS_CMD_SET_SERVO_ReadData;
    requestPack.content_size = 1;
    requestPack.content[0] = servoId;
    sendPack();
}

// 接收舵机数据监控查询指令
FSUS_STATUS FSUS_Protocol::recvQueryServoMonitor(FSUS_SERVO_ID_T *servoId, FSUS_ServoMonitor *servodata){
    FSUS_STATUS status = recvPack();
    double temp;//温度数据转换

    if(status != FSUS_STATUS_SUCCESS)servodata->status = 256;

    if(status == FSUS_STATUS_SUCCESS){
        servodata->id = responsePack.content[0];
        servodata->voltage = (int16_t)((responsePack.content[2] << 8) | responsePack.content[1]);
        servodata->current = (int16_t)((responsePack.content[4] << 8) | responsePack.content[3]);
        servodata->power = (int16_t)((responsePack.content[6] << 8) | responsePack.content[5]);
        servodata->temperature = (int16_t)((responsePack.content[8] << 8) | responsePack.content[7]);
		temp = (double)servodata->temperature;
		servodata->temperature = 1 / (log(temp / (4096.0f - temp)) / 3435.0f + 1 / (273.15 + 25)) - 273.15;
        servodata->status = responsePack.content[9];
        servodata->angle = (int32_t)((responsePack.content[13] << 24) | (responsePack.content[12] << 16) | (responsePack.content[11] << 8) | responsePack.content[10]);
        servodata->angle = (float)(servodata->angle/10.0);
		servodata->circle_count = (int16_t)((responsePack.content[15] << 8) | responsePack.content[14]);
    }
    return status;
}

// 发送舵机控制模式停止指令
void FSUS_Protocol::sendStopOnControlMode(FSUS_SERVO_ID_T servoId, FSUS_MODE_T mode, FSUS_POWER_T power){
//mode 指令停止形式
//0-停止后卸力(失锁)
//1-停止后保持锁力
//2-停止后进入阻尼状态
    requestPack.cmdId = FSUS_CMD_CONTROL_MODE_STOP;
    requestPack.content_size = 4;
    requestPack.content[0] = servoId;
    mode = mode | 0x10;
    requestPack.content[1] = mode;
    requestPack.content[2] = power & 0xFF;
    requestPack.content[3] = power >> 8;
    sendPack();
}

// 发送舵机同步控制命令
void FSUS_Protocol::sendSyncCommand(FSUS_SERVO_COUNT_T servo_count,FSUS_MODE_T sync_mode, FSUS_Sync_T Syncsend[]){
    requestPack.cmdId = FSUS_CMD_SET_SERVO_SyncCommand;
    uint8_t c_size=0;

    switch (sync_mode){
        case MODE_SET_SERVO_ANGLE:
        /* 同步命令设置舵机的角度(单圈模式）*/
            requestPack.content_size = 3+servo_count*7;
            requestPack.content[0] = 8;
            requestPack.content[1] = 7;
            requestPack.content[2] = servo_count;
            for(int i = 0; i < servo_count; i++)
            {
                requestPack.content[3+c_size*7] = Syncsend[i].id;
                if (Syncsend[i].angle > 180.0f) {
                    requestPack.content[4+c_size*7] = (int16_t)(10 * 180.0f) & 0xFF;
                    requestPack.content[5+c_size*7] = (int16_t)(10 * 180.0f) >> 8;
                } else if (Syncsend[i].angle < -180.0f) {
                    requestPack.content[4+c_size*7] = (int16_t)(10 * -180.0f) & 0xFF;
                    requestPack.content[5+c_size*7] = (int16_t)(10 * -180.0f) >> 8;
                } else {
                    requestPack.content[4+c_size*7] = (int16_t)(10 * Syncsend[i].angle) & 0xFF;
                    requestPack.content[5+c_size*7] = (int16_t)(10 * Syncsend[i].angle) >> 8;
                }
                requestPack.content[6+c_size*7] = Syncsend[i].interval_single & 0xFF; //周期
                requestPack.content[7+c_size*7] = Syncsend[i].interval_single >> 8;
                requestPack.content[8+c_size*7] = Syncsend[i].power & 0xFF; //功率
                requestPack.content[9+c_size*7] = Syncsend[i].power >> 8;
                c_size++;
            }
        break;
        case MODE_SET_SERVO_ANGLE_BY_INTERVAL:
        /* 同步命令设置舵机的角度(单圈模式，指定周期) */
            requestPack.content_size = 3+servo_count*11;
            requestPack.content[0] = 11;
            requestPack.content[1] = 11;
            requestPack.content[2] = servo_count;
            for(int i = 0; i < servo_count; i++)
            {
                requestPack.content[3+c_size*11] = Syncsend[i].id;
                if (Syncsend[i].angle > 180.0f) {
                    requestPack.content[4+c_size*11] = (int16_t)(10 * 180.0f) & 0xFF;
                    requestPack.content[5+c_size*11] = (int16_t)(10 * 180.0f) >> 8;
                } else if (Syncsend[i].angle < -180.0f) {
                    requestPack.content[4+c_size*11] = (int16_t)(10 * -180.0f) & 0xFF;
                    requestPack.content[5+c_size*11] = (int16_t)(10 * -180.0f) >> 8;
                } else {
                    requestPack.content[4+c_size*11] = (int16_t)(10 * Syncsend[i].angle) & 0xFF;
                    requestPack.content[5+c_size*11] = (int16_t)(10 * Syncsend[i].angle) >> 8;
                }
                requestPack.content[6+c_size*11] = Syncsend[i].interval_single & 0xFF; //周期
                requestPack.content[7+c_size*11] = Syncsend[i].interval_single >> 8;
                requestPack.content[8+c_size*11] = Syncsend[i].t_acc & 0xFF;
                requestPack.content[9+c_size*11] = Syncsend[i].t_acc >> 8;
                requestPack.content[10+c_size*11] = Syncsend[i].t_dec & 0xFF;
                requestPack.content[11+c_size*11] = Syncsend[i].t_dec >> 8;
                requestPack.content[12+c_size*11] = Syncsend[i].power & 0xFF; //功率
                requestPack.content[13+c_size*11] = Syncsend[i].power >> 8;
                c_size++;
            }
        break;
        case MODE_SET_SERVO_ANGLE_BY_VELOCITY:
        /* 同步命令设置舵机的角度(单圈模式，指定转速) */
            requestPack.content_size = 3+servo_count*11;
            requestPack.content[0] = 12;
            requestPack.content[1] = 11;
            requestPack.content[2] = servo_count;
            for(int i = 0; i < servo_count; i++)
            {
                requestPack.content[3+c_size*11] = Syncsend[i].id;
                if (Syncsend[i].angle > 180.0f) {
                    requestPack.content[4+c_size*11] = (int16_t)(10 * 180.0f) & 0xFF;
                    requestPack.content[5+c_size*11] = (int16_t)(10 * 180.0f) >> 8;
                } else if (Syncsend[i].angle < -180.0f) {
                    requestPack.content[4+c_size*11] = (int16_t)(10 * -180.0f) & 0xFF;
                    requestPack.content[5+c_size*11] = (int16_t)(10 * -180.0f) >> 8;
                } else {
                    requestPack.content[4+c_size*11] = (int16_t)(10 * Syncsend[i].angle) & 0xFF;
                    requestPack.content[5+c_size*11] = (int16_t)(10 * Syncsend[i].angle) >> 8;
                }
                requestPack.content[6+c_size*11] = (uint16_t)Syncsend[i].velocity & 0xFF; //周期
                requestPack.content[7+c_size*11] = (uint16_t)Syncsend[i].velocity >> 8;
                requestPack.content[8+c_size*11] = Syncsend[i].t_acc & 0xFF;
                requestPack.content[9+c_size*11] = Syncsend[i].t_acc >> 8;
                requestPack.content[10+c_size*11] = Syncsend[i].t_dec & 0xFF;
                requestPack.content[11+c_size*11] = Syncsend[i].t_dec >> 8;
                requestPack.content[12+c_size*11] = Syncsend[i].power & 0xFF; //功率
                requestPack.content[13+c_size*11] = Syncsend[i].power >> 8;
                c_size++;
            }
        break;
        case MODE_SET_SERVO_ANGLE_MTURN:
        /* 同步命令设置舵机的角度(多圈模式) */
            requestPack.content_size = 3+servo_count*11;
            requestPack.content[0] = 13;
            requestPack.content[1] = 11;
            requestPack.content[2] = servo_count;
            for(int i = 0; i < servo_count; i++)
            {
                requestPack.content[3+c_size*11] = Syncsend[i].id;
                if (Syncsend[i].angle > 368640.0f) {
                    requestPack.content[4+c_size*11] = (int32_t)(10 * 368640.0f) & 0xFF;         // 角度
                    requestPack.content[5+c_size*11] = ((int32_t)(10 * 368640.0f) >> 8) & 0xFF;
                    requestPack.content[6+c_size*11] = ((int32_t)(10 * 368640.0f) >> 16) & 0xFF;
                    requestPack.content[7+c_size*11] = ((int32_t)(10 * 368640.0f) >> 24) & 0xFF;
                } else if (Syncsend[i].angle < -368640.0f) {
                    requestPack.content[4+c_size*11] = (int32_t)(10 * -368640.0f) & 0xFF;         // 角度
                    requestPack.content[5+c_size*11] = ((int32_t)(10 * -368640.0f) >> 8) & 0xFF;
                    requestPack.content[6+c_size*11] = ((int32_t)(10 * -368640.0f) >> 16) & 0xFF;
                    requestPack.content[7+c_size*11] = ((int32_t)(10 * -368640.0f) >> 24) & 0xFF;
                } else {
                    requestPack.content[4+c_size*11] = (int32_t)(10 * Syncsend[i].angle) & 0xFF;         // 角度
                    requestPack.content[5+c_size*11] = ((int32_t)(10 * Syncsend[i].angle) >> 8) & 0xFF;
                    requestPack.content[6+c_size*11] = ((int32_t)(10 * Syncsend[i].angle) >> 16) & 0xFF;
                    requestPack.content[7+c_size*11] = ((int32_t)(10 * Syncsend[i].angle) >> 24) & 0xFF;
                }
                requestPack.content[8+c_size*11] = Syncsend[i].interval_multi & 0xFF; //周期
                requestPack.content[9+c_size*11] = (Syncsend[i].interval_multi >> 8) & 0xFF;
                requestPack.content[10+c_size*11] = (Syncsend[i].interval_multi >> 16) & 0xFF;
                requestPack.content[11+c_size*11] = (Syncsend[i].interval_multi >> 24) & 0xFF;
                requestPack.content[12+c_size*11] = Syncsend[i].power & 0xFF; //功率
                requestPack.content[13+c_size*11] = Syncsend[i].power >> 8;
                c_size++;
            }
        break;
        case MODE_SET_SERVO_ANGLE_MTURN_BY_INTERVAL:
        /* 同步命令设置舵机的角度(多圈模式, 指定周期) */
            requestPack.content_size = 3+servo_count*15;
            requestPack.content[0] = 14;
            requestPack.content[1] = 15;
            requestPack.content[2] = servo_count;
            for(int i = 0; i < servo_count; i++)
            {
                requestPack.content[3+c_size*15] = Syncsend[i].id;
                if (Syncsend[i].angle > 368640.0f) {
                    requestPack.content[4+c_size*15] = (int32_t)(10 * 368640.0f) & 0xFF;         // 角度
                    requestPack.content[5+c_size*15] = ((int32_t)(10 * 368640.0f) >> 8) & 0xFF;
                    requestPack.content[6+c_size*15] = ((int32_t)(10 * 368640.0f) >> 16) & 0xFF;
                    requestPack.content[7+c_size*15] = ((int32_t)(10 * 368640.0f) >> 24) & 0xFF;
                } else if (Syncsend[i].angle < -368640.0f) {
                    requestPack.content[4+c_size*15] = (int32_t)(10 * -368640.0f) & 0xFF;         // 角度
                    requestPack.content[5+c_size*15] = ((int32_t)(10 * -368640.0f) >> 8) & 0xFF;
                    requestPack.content[6+c_size*15] = ((int32_t)(10 * -368640.0f) >> 16) & 0xFF;
                    requestPack.content[7+c_size*15] = ((int32_t)(10 * -368640.0f) >> 24) & 0xFF;
                } else {
                    requestPack.content[4+c_size*15] = (int32_t)(10 * Syncsend[i].angle) & 0xFF;         // 角度
                    requestPack.content[5+c_size*15] = ((int32_t)(10 * Syncsend[i].angle) >> 8) & 0xFF;
                    requestPack.content[6+c_size*15] = ((int32_t)(10 * Syncsend[i].angle) >> 16) & 0xFF;
                    requestPack.content[7+c_size*15] = ((int32_t)(10 * Syncsend[i].angle) >> 24) & 0xFF;
                }
                requestPack.content[8+c_size*15] = Syncsend[i].interval_multi & 0xFF; //周期
                requestPack.content[9+c_size*15] = (Syncsend[i].interval_multi >> 8) & 0xFF;
                requestPack.content[10+c_size*15] = (Syncsend[i].interval_multi >> 16) & 0xFF;
                requestPack.content[11+c_size*15] = (Syncsend[i].interval_multi >> 24) & 0xFF;
                requestPack.content[12+c_size*15] = Syncsend[i].t_acc & 0xFF;
                requestPack.content[13+c_size*15] = Syncsend[i].t_acc >> 8;
                requestPack.content[14+c_size*15] = Syncsend[i].t_dec & 0xFF;
                requestPack.content[15+c_size*15] = Syncsend[i].t_dec >> 8;
                requestPack.content[16+c_size*15] = Syncsend[i].power & 0xFF; //功率
                requestPack.content[17+c_size*15] = Syncsend[i].power >> 8;
                c_size++;
            }
        break;
        case MODE_SET_SERVO_ANGLE_MTURN_BY_VELOCITY:
        /* 同步命令设置舵机的角度(多圈模式, 指定转速) */
            requestPack.content_size = 3+servo_count*13;
            requestPack.content[0] = 15;
            requestPack.content[1] = 13;
            requestPack.content[2] = servo_count;
            for(int i = 0; i < servo_count; i++)
            {
                requestPack.content[3+c_size*13] = Syncsend[i].id;
                if (Syncsend[i].angle > 368640.0f) {
                    requestPack.content[4+c_size*13] = (int32_t)(10 * 368640.0f) & 0xFF;         // 角度
                    requestPack.content[5+c_size*13] = ((int32_t)(10 * 368640.0f) >> 8) & 0xFF;
                    requestPack.content[6+c_size*13] = ((int32_t)(10 * 368640.0f) >> 16) & 0xFF;
                    requestPack.content[7+c_size*13] = ((int32_t)(10 * 368640.0f) >> 24) & 0xFF;
                } else if (Syncsend[i].angle < -368640.0f) {
                    requestPack.content[4+c_size*13] = (int32_t)(10 * -368640.0f) & 0xFF;         // 角度
                    requestPack.content[5+c_size*13] = ((int32_t)(10 * -368640.0f) >> 8) & 0xFF;
                    requestPack.content[6+c_size*13] = ((int32_t)(10 * -368640.0f) >> 16) & 0xFF;
                    requestPack.content[7+c_size*13] = ((int32_t)(10 * -368640.0f) >> 24) & 0xFF;
                } else {
                    requestPack.content[4+c_size*13] = (int32_t)(10 * Syncsend[i].angle) & 0xFF;         // 角度
                    requestPack.content[5+c_size*13] = ((int32_t)(10 * Syncsend[i].angle) >> 8) & 0xFF;
                    requestPack.content[6+c_size*13] = ((int32_t)(10 * Syncsend[i].angle) >> 16) & 0xFF;
                    requestPack.content[7+c_size*13] = ((int32_t)(10 * Syncsend[i].angle) >> 24) & 0xFF;
                }
                requestPack.content[8+c_size*13] = (uint16_t)Syncsend[i].velocity & 0xFF; //周期
                requestPack.content[9+c_size*13] = (uint16_t)Syncsend[i].velocity >> 8;
                requestPack.content[10+c_size*13] = Syncsend[i].t_acc & 0xFF;
                requestPack.content[11+c_size*13] = Syncsend[i].t_acc >> 8;
                requestPack.content[12+c_size*13] = Syncsend[i].t_dec & 0xFF;
                requestPack.content[13+c_size*13] = Syncsend[i].t_dec >> 8;
                requestPack.content[14+c_size*13] = Syncsend[i].power & 0xFF; //功率
                requestPack.content[15+c_size*13] = Syncsend[i].power >> 8;
                c_size++;
            }
        break;
        case MODE_Query_SERVO_Monitor:
        /* 同步命令读取舵机的数据*/
            requestPack.content_size = 3+servo_count;
            requestPack.content[0] = 22;
            requestPack.content[1] = 1;
            requestPack.content[2] = servo_count;
            for(int i = 0; i < servo_count; i++)
            {
                requestPack.content[3+i] = Syncsend[i].id;
            }
        break;
    default:
        break;
    }

    sendPack();

}

// 接收舵机同步命令下每个舵机的数据
FSUS_STATUS FSUS_Protocol::recvQuerySyncCommand(FSUS_SERVO_COUNT_T servo_count, FSUS_ServoMonitor Syncmonitor[]) {
    FSUS_STATUS status;
    double temp; // 温度数据转换
    int packet_count = 0;

    while (packet_count < servo_count) {
        status = recvPack();

        if (status != FSUS_STATUS_SUCCESS) {
            // 记录错误，但继续处理其他数据包
            Syncmonitor[packet_count].status = 256;
            packet_count++;
            continue;
        }

        // 检查数据包长度
        if (sizeof(responsePack.content) < 16) {
            return FSUS_STATUS_ERRO;  // 返回错误
        }

        // 解析当前数据包内容
        Syncmonitor[packet_count].id = responsePack.content[0];
        Syncmonitor[packet_count].voltage = (int16_t)((responsePack.content[2] << 8) | responsePack.content[1]);
        Syncmonitor[packet_count].current = (int16_t)((responsePack.content[4] << 8) | responsePack.content[3]);
        Syncmonitor[packet_count].power = (int16_t)((responsePack.content[6] << 8) | responsePack.content[5]);
        Syncmonitor[packet_count].temperature = (int16_t)((responsePack.content[8] << 8) | responsePack.content[7]);

        // 温度计算
        temp = (double)Syncmonitor[packet_count].temperature;
        if (temp > 0 && temp < 4096) {
            Syncmonitor[packet_count].temperature =
                1 / (log(temp / (4096.0 - temp)) / 3435.0 + 1 / (273.15 + 25)) - 273.15;
        } else {
            Syncmonitor[packet_count].temperature = -999.0;  // 默认错误值
        }

        Syncmonitor[packet_count].status = (uint16_t)responsePack.content[9];
        Syncmonitor[packet_count].angle = (int32_t)((responsePack.content[13] << 24) |
                                                    (responsePack.content[12] << 16) |
                                                    (responsePack.content[11] << 8) |
                                                    responsePack.content[10]);
        Syncmonitor[packet_count].angle = (float)Syncmonitor[packet_count].angle / 10.0f;
        Syncmonitor[packet_count].circle_count = (int16_t)((responsePack.content[15] << 8) | responsePack.content[14]);
        // 完成一组数据包解析，计数加一
        packet_count++;

        // 重置 pkg 状态以接收下一组数据
        responsePack.recv_status = 0;
    }
    return FSUS_STATUS_SUCCESS;
}
