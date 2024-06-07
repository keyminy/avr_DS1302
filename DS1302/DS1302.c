#include "DS1302.h"


void DS1302_Init(){
	DS1302_CLK_DDR |= (1<<DS1302_CLK);// 출력모드로
	DS1302_DAT_DDR |= (1<<DS1302_DAT);
	DS1302_RST_DDR |= (1<<DS1302_RST);//RST : CHIP ENABLE이에요
	DS1302_CLK_PORT &= ~(1<<DS1302_CLK);//initial Clock should be LOW;
	DS1302_DAT_PORT |= (1<<DS1302_DAT);//assume initial data is HIGH
	DS1302_RST_PORT &= ~(1<<DS1302_RST_PORT);//initial RST(=CE) should be LOW;
}

void DS1302_Selected(){
	DS1302_RST_PORT |= (1<<DS1302_RST); //HIGH로
}

void DS1302_Deselected(){
	DS1302_RST_PORT &= ~(1<<DS1302_RST); //LOW로
}

void DS1302_Clock(){
	// HIGH -> LOW
	DS1302_CLK_PORT |= (1<<DS1302_CLK);
	DS1302_CLK_PORT &= ~(1<<DS1302_CLK);
}

void DS1302_DataBitSet(){
	DS1302_DAT_PORT |= (1<<DS1302_DAT);
}

void DS1302_DataBitReset(){
	DS1302_DAT_PORT &= ~(1<<DS1302_DAT);
}

void DS1302_Change_ReadMode(){
	DS1302_DAT_DDR &= ~(1<<DS1302_DAT);
}
void DS1302_Change_WriteMode(){
	DS1302_DAT_DDR  |= 1<<DS1302_DAT;
}

// 입력값 -> bcd로 변환
// 그냥 매개변수에 59를 넣어 전송하는 것은 bcd코드가 아닙니다(bcd코드 변경필요)
uint8_t decimal_to_bcd(uint8_t decimal){
	//매개변수 : 59, 5와 9를 16진수의 상위 4빗, 하위 4빗으로 나누어야함
	return ( ((decimal/10)<<4) | (decimal%10) );
}

uint8_t bcd_to_decimal(uint8_t bcd){
	return ( ((bcd>>4)*10) + (bcd&0x0f) );
}

void DS1302_TxData(uint8_t txData){
	DS1302_Change_WriteMode();
	for(int i=0;i<8;i++){
		//note : 하위비트부터 전송해야함
		// 하위비트부터 시작해서 -> 상위비트로
		// 1bit보내고, clk올렸다내리고, 1bit보내고 clk올렸다내리고
		// 8bit(=1byte이렇게 해야함) -> for문 사용
		for(int i=0;i<8;i++){
			if(txData & (1<<i)){
				//해당 if문 txData가 1인지 0인지 판단함
				//i는 0부터시작하니까 0번에대한 bit부터 &연산
				//1이면 실행
				DS1302_DataBitSet();
				}else{
				//0이면 실행
				DS1302_DataBitReset();
			}
			//여기서 clk을 올렸다 내린다
			DS1302_Clock();
		}
	}
}


void DS1302_WriteData(uint8_t address,uint8_t data){
	//1.CE pin을 HIGH로
	DS1302_Selected();
	//2. 주소를 전송한다
	DS1302_TxData(address);
	//3.데이터값 전송한다
	DS1302_TxData(decimal_to_bcd(data));
	//마지막4. CE pin을 LOW로
	DS1302_Deselected();
}

void DS1302_SetTimeDate(DS1302 timeDate){
	DS1302_WriteData(ADDR_SEC,timeDate.sec);
	DS1302_WriteData(ADDR_MIN,timeDate.min);
	DS1302_WriteData(ADDR_HOUR,timeDate.hour);
	DS1302_WriteData(ADDR_DATE,timeDate.date);
	DS1302_WriteData(ADDR_MONTH,timeDate.month);
	DS1302_WriteData(ADDR_DAYOFWEEK,timeDate.dayOfWeek);
	DS1302_WriteData(ADDR_YEAR,timeDate.year);
}

uint8_t DS1302_RxData(){
	//fallingedge에서, 8bit를 읽어 return함
	uint8_t rxData = 0; //해당 변수에 결과값 넣음
	DS1302_Change_ReadMode();
	//for문 8번
	for(int i=0;i<8;i++){
		//해당 bit가 1이면?
		if(DS1302_DAT_PIN &(1<<DS1302_DAT))
		rxData |= 1<<i;
		//해당 bit가 0일때
		else
		//rxData = 1 << i;// 이미 초기값이 0이라 큰 의미는 없습니다.
		//정확한 표기법은 이것임
		rxData &= ~(1 << i);
		//(M2)삼항연산자.
		//rxData |=  (DS1302_DAT_PIN &(1<<DS1302_DAT)) ? (1<<i) : 0;
		
		//데이터 읽고난 담엔, clock을 줍니다.(읽은 후, 7번 clock발생)
		if(i!=7){
			DS1302_Clock(); //7번 clock
		}
	}
	return rxData;
}

uint8_t DS1302_ReadData(uint8_t address){
	//읽는값은 1byte인 8bit이다
	uint8_t rxData = 0;
	DS1302_Selected(); // CE goes to HIGH
	DS1302_TxData(address+1);
	rxData = DS1302_RxData(); // 1byte를 읽어 return
	DS1302_Deselected(); // CE goes to LOW
	return bcd_to_decimal(rxData);
}

void DS1302_GetTime(DS1302* timeDate){
	timeDate->sec = DS1302_ReadData(ADDR_SEC);
	timeDate->min = DS1302_ReadData(ADDR_MIN);
	timeDate->hour = DS1302_ReadData(ADDR_HOUR);
}

void DS1302_GetDate(DS1302* timeDate){
	timeDate->date = DS1302_ReadData(ADDR_DATE);
	timeDate->month = DS1302_ReadData(ADDR_MONTH);
	timeDate->dayOfWeek = DS1302_ReadData(ADDR_DAYOFWEEK);
	timeDate->year = DS1302_ReadData(ADDR_YEAR);
}

