
#ifndef _MAIN_H_
#define _MAIN_H_

#include <xc.h>

#define OSC_FREQ (16000000UL)
#define OSC_PER_INST (4)

typedef unsigned char uchar8; //无符号字符型8位 0 ~ 255
typedef signed char char8;  //有符号字符型8位 127 ~ -128
typedef unsigned short int uint16;  //无符号整型16位 0 ~ 65535
typedef signed short int   int16; //有符号整型16位 32767 ~ -32768
typedef unsigned int    uint16;  //无符号整型16位 0 ~ 65535
typedef signed int    int16; //有符号整型16位 32767 ~ -32768
typedef signed short long   long24; //有符号长整型24位 8388607 ~ -8388608 
typedef unsigned short long   ulong24; //无符号长整型24位 0 ~ 16777215
typedef signed long   long32; //2147483647 ~ -2147483648
typedef unsigned long   ulong32; //0 ~ 4294967295
typedef signed long long    long32; //2147483647 ~ -2147483648
typedef unsigned long long    ulong32; //0 ~ 4294967295

typedef float   fp24; //默认24位
typedef double  fp32; //默认24位

#define FALSE (0)
#define TRUE  (1)

#define RETURN_NORMAL  (0)
#define RETURN_ERROR  (1)


#define ERROR_SCH_TOO_MANY_TASKS (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)

#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK (3)
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER (3)

#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START (4)
#define ERROR_SCH_LOST_SLAVE (5)

#define ERROR_SCH_CAN_BUS_ERROR (6)

#define ERROR_I2C_WRITE_BYTE (10)
#define ERROR_I2C_READ_BYTE (11)
#define ERROR_I2C_WRITE_BYTE_AT24C64 (12)
#define ERROR_I2C_READ_BYTE_AT24C64 (13)
#define ERROR_I2C_DS1621 (14)

#define ERROR_USART_TI (21)
#define ERROR_USART_WRITE_CHAR (22)

#define ERROR_SPI_EXCHANGE_BYTES_TIMEOUT (31)
#define ERROR_SPI_X25_TIMEOUT (32)
#define ERROR_SPI_MAX1110_TIMEOUT (33)

#define ERROR_ADC_MAX150_TIMEOUT (44)

#endif
