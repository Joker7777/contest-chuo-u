#include "ysml.h"
#include <stdio.h>
#include <sysio.h>

/*********************************************************************
*****	ポーリングによるAD変換入力
*****	入力端子
*****	チャンネル      端子名		ピン番号
*****	0				AN0			CN3_11
*****	1				AN1			CN3_12
*****	2				AN2			CN3_13
*****	3				AN3			CN3_14
*****	4				AN4			CN3_15
*****	5				AN5			CN3_16
*****	6			   (DA0)
*****	7			   (DA1)
*****	8				AN8			CN3_21
*****	9				AN9			CN3_22
*****	10				AN10		CN3_23
*****	11				AN11		CN3_24
*****	12				AN12		CN3_25
*****	13				AN13		CN3_26
*****	14			   (DA2)
*****	15			   (DA3)
*********************************************************************/

#define	AN0	0
// #define	AN8	8
// #define	AN9	9

#define	WAIT_SENSOR	(100000)
#define	WAIT_BUZZER	(100000)

void wait(unsigned long count)
{
	unsigned long x;
	for (x = 0; x < count; x++);
}


void main()
{
	unsigned short data0, data1, data2;
	int bzz = 0;

	DaInit(2);		//DA2チャンネルの出力許可
	PAdInit(AN0);	//AD変換器初期化
	// PAdInit(AN8);
	// PAdInit(AN9);
	
	while(1) {
		data0 = PAdIn(AN0);			//入力信号をAD変換する
		// data1 = PAdIn(AN8);
		// data2 = PAdIn(AN9);
		printf("sensor: %d\n", data0);
		
		if (data0 >= 430) {
			DaOut(2, 255);		//DA2チャンネルの出力電圧を0Vから5V まで増加	
		}
		else {
			DaOut(2, 0);		// DA2チャンネルの出力電圧を5Vから0V まで減少
		}
		
		wait(WAIT_BUZZER);
		wait(WAIT_SENSOR);
	}
}
