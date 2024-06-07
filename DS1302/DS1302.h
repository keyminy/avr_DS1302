/*
 * DS1302.h
 *
 * Created: 2024-06-08 오전 1:55:19
 *  Author: me
 */ 


#ifndef DS1302_H_
#define DS1302_H_

#include <avr/io.h>

#define DS1302_CLK_DDR DDRG
#define DS1302_CLK_PORT PORTG
#define DS1302_DAT_DDR DDRG
#define DS1302_DAT_PORT PORTG
#define DS1302_DAT_PIN PING
#define DS1302_RST_DDR DDRG
#define DS1302_RST_PORT PORTG

#define DS1302_CLK 2
#define DS1302_DAT 3
#define DS1302_RST 4

#define ADDR_SEC 0x80
#define ADDR_MIN 0x82
#define ADDR_HOUR 0x84
#define ADDR_DATE 0x86
#define ADDR_MONTH 0x88
#define ADDR_DAYOFWEEK 0x8a
#define ADDR_YEAR 0x8c

typedef struct _ds1302{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint8_t dayOfWeek;
	uint8_t year;
} DS1302;

void DS1302_Init();
void DS1302_Selected();
void DS1302_Deselected();
void DS1302_Clock();
void DS1302_DataBitSet();
void DS1302_DataBitReset();
void DS1302_Change_ReadMode();
void DS1302_Change_WriteMode();
uint8_t decimal_to_bcd(uint8_t decimal);
uint8_t bcd_to_decimal(uint8_t bcd);
void DS1302_TxData(uint8_t txData);
void DS1302_WriteData(uint8_t address,uint8_t data);
void DS1302_SetTimeDate(DS1302 timeDate);
uint8_t DS1302_RxData();
uint8_t DS1302_ReadData(uint8_t address);
void DS1302_GetTime(DS1302* timeDate);
void DS1302_GetDate(DS1302* timeDate);

#endif /* DS1302_H_ */