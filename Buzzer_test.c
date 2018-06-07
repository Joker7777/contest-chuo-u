/*DA2, DA3 からアナログ電圧を出力し、その出力信号を使って/*
/*警報表示テスト回路のブザーを鳴らす制御プログラム例 */

#include "ysml.h"
#include <stdio.h>
#include <sysio.h>

#define	WAIT_BUZZER	(100000)

void wait(unsigned long count)
{
	unsigned long x;
	for (x = 0; x < count; x++);
}

void main()
{
	int data,data1;
	
	DaInit(2);	//DA2チャンネルの出力許可
	DaInit(3);	//DA3チャンネルの出力許可

	//無限ループ
	while (1)	{
		DaOut(2,255);		//DA2チャンネルの出力電圧を5V に設定
		wait(WAIT_BUZZER);
		
		DaOut(2,0);			//DA2チャンネルの出力電圧を0V に設定
		wait(WAIT_BUZZER);
		
		DaOut(3,255);		//DA3チャンネルの出力電圧を5V に設定
		wait(WAIT_BUZZER);
		
		DaOut(3,0);			//DA3チャンネルの出力電圧を0V に設定
		wait(WAIT_BUZZER);
		
		for (data = 0; data < 255; data++) {
			DaOut(2, data);		//DA2チャンネルの出力電圧を0Vから5V まで増加
		}
		for (data = 255; data > 0; data--) {
			DaOut(2, data);		// DA2チャンネルの出力電圧を5Vから0V まで減少
		}
		wait(WAIT_BUZZER);
		
		for (data1 = 0; data1 < 255; data1++) {
			DaOut(3, data1); 	//DA3チャンネルの出力電圧を0Vから5V まで増加
		}
		for (data1 = 255; data1 > 0; data1--) {
			DaOut(3, data1);	// DA3チャンネルの出力電圧を5Vから0V まで減少
		}
		wait(WAIT_BUZZER);
	}
}
