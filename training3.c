#include "ysml.h"
#include "ioh82633.h"
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

#define	DIST	0 // AN0, CN3_11
#define	FOTO	8 // AN8, CN3_21
////////////////////////////////////////////
#define	WAIT_SENSOR	(100000)
#define	WAIT_BUZZER	(100000)

#define WAIT_SHUNJI	(100000)	/* 瞬時停止の停止カウント */
#define WAIT_TYPE_A	(350000)	/* 停止カウント_タイプA */
#define WAIT_TYPE_B	(100000)	/* 停止カウント_タイプB */

void wait(unsigned long count)
{
	unsigned long  x;
	
	for (x = 0; x < count; x++);	

}

/**
int move 動作方法の指定
	0:	stop
	1:	go front
	2:	go back
	3:	rotate right
	4:	rotate left

unsigned short dlt 速さ指定 0 - 100
*/

void init()
{
	P1DDR.BYTE = 0xF0;	/* ポート番号1(CN2)の 0～7番ピンを初期設定する */
						/* 0～3番ピン(P10～P13)はデータ入力用端子 */
						/* 4～7番ピン(P14～P17)はデータ出力用端子 */									   
	DaInit(0);   		/* D/A0ポートの出力を許可(右モータ) */
	DaInit(1);   		/* D/A1ポートの出力を許可(左モータ) */
	
	DaInit(2);		//DA2チャンネルの出力許可
	PAdInit(DIST);	//AD変換器初期化
	// PAdInit(FOTO);
}

void motor(int move, char dlt)
{
	printf("move: %d, dlt: %d\n", move, (int)dlt);
	switch (move) {
	case 0:	/* 一時停止*/
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ停止*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21             */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ停止*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19             */
		DaOut(0,0);	    	/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,0);	    	/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		break;
		
	case 1:	/* 直進 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_SHUNJI);
		break;
		
	case 2:	/* 後退 */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ逆回転*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ逆回転*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		break;
		
	case 3:	/* 右回転 */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ逆回転*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0, 80);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, 80);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		break;
		
	case 4:	/* 左回転 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ逆回転*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0,80);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,80);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		break;
		
	default:
		printf("error\n");
	}
	/* 瞬時停止 切り替え時の貫通電流を防止 */
	P1DR.BIT.B4 = 0;
	P1DR.BIT.B5 = 0;
	P1DR.BIT.B6 = 0;
	P1DR.BIT.B7 = 0;
	DaOut(0,0);
	DaOut(1,0);
	wait(WAIT_SHUNJI);
	
	return;
}

void main()
{
	unsigned short dist, foto, dlt;

	init();
	wait(WAIT_SHUNJI);
	wait(WAIT_SHUNJI);
	
	while (1) {
		dist = PAdIn(DIST);			//入力信号をAD変換する
		// foto = PAdIn(FOTO);
		
		printf("sensor: %d\n", dist);
		// printf("sensor\tDig: %u\n", foto);
		
		if (dist < 130) {
			dlt = 0;
		}
		else if (dist > 630) {
			dlt = 300;
		}
		else {
			dlt = dist - 130;
		}
		
		// move
		motor(1, (char)(100-dlt/5));
		
		printf("aa: %c\n", 100-dlt/5);
		
		
		// if (dist >= 430) { // d <= 10cm
		/* if (foto > 100) { // black
			DaOut(2, 255);		//DA2チャンネルの出力電圧を5V	
		}
		else { // d > 10cm or white
			DaOut(2, 0);		// DA2チャンネルの出力電圧を0V
		}*/

		wait(WAIT_SENSOR);
	}
	return;
}
