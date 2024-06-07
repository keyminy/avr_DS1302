#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "DS1302.h"


int main(void)
{
	DS1302_Init(); //초기화 코드
	DS1302 myTime;
	myTime.year = 20;
	myTime.month = 12;
	myTime.date = 28;
	myTime.dayOfWeek = 1;
	myTime.hour = 12;
	myTime.min = 19;
	myTime.sec = 0;

	DS1302_SetTimeDate(myTime); //DS1302 Chip에 시간정보 저장
	while (1)
	{
		DS1302_GetTime(&myTime); // 읽어오기(시간만)
		DS1302_GetDate(&myTime); // 읽어오기(날짜만)
		//2020.12.28 12:22:00
		print("20%02d.%02d.%02d %02d:%02d:%02d\n"
		,myTime.year
		,myTime.month
		,myTime.date
		,myTime.hour
		,myTime.min
		,myTime.sec
		);
		_delay_ms(1000);
	}
}