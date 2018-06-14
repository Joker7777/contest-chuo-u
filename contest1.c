#include "ysml.h"
#include "ioh82633.h"
#include <stdio.h>
#include <sysio.h>

/*********************************************************************
*****	ポーリングによるAD変換入力
*****	入力端子
*****	チャンネル      端子名		ピン番号
*****	0				AN0			CN3_11	DIST1
*****	1				AN1			CN3_12	DIST2
*****	2				AN2			CN3_13	DIST3
*****	3				AN3			CN3_14
*****	4				AN4			CN3_15
*****	5				AN5			CN3_16
*****	6			   (DA0)
*****	7			   (DA1)
*****	8				AN8			CN3_21	FOTO1
*****	9				AN9			CN3_22	FOTO2
*****	10				AN10		CN3_23	FOTO3
*****	11				AN11		CN3_24
*****	12				AN12		CN3_25
*****	13				AN13		CN3_26
*****	14			   (DA2)
*****	15			   (DA3)
*********************************************************************/

#define	DIST1	0 // AN0, CN3_11
#define	DIST2	1 // AN1, CN3_12
#define	DIST3	2 // AN2, CN3_13
#define	FOTO1	8 // AN8, CN3_21
////////////////////////////////////////////

#define	WAIT_SENSOR	(100000/20) // input() 0.1 seconds
#define	WAIT_BUZZER	(500000) // 2 seconds

#define WAIT_SHUNJI	(100000)	/* 瞬時停止の停止カウント */
#define WAIT_TYPE_A	(1000)	/* 停止カウント_タイプA */
#define WAIT_TYPE_Back	(400000)	/* 復帰時動作時間 */
/////////////////////////////////////////////

#define	FOTO_TRUE	200 // black
#define	DIST_TRUE	160	// 30 cm
#define DIST_STOP	560 // 8 cm

#define VELOCITY 80 // max velocity

void wait(unsigned long count)
{
	unsigned long  x;
	for (x = 0; x < count; x++);
}

void init()
{
	P1DDR.BYTE = 0xF0;	/* ポート番号1(CN2)の 0～7番ピンを初期設定する */
						/* 0～3番ピン(P10～P13)はデータ入力用端子 */
						/* 4～7番ピン(P14～P17)はデータ出力用端子 */									   
	DaInit(0);   		/* D/A0ポートの出力を許可(右モータ) */
	DaInit(1);   		/* D/A1ポートの出力を許可(左モータ) */
	
	DaInit(2);		//DA2チャンネルの出力許可
	PAdInit(DIST1);	//AD変換器初期化
	PAdInit(DIST2);
	PAdInit(DIST3);
	PAdInit(FOTO1);
}

void buzzer()
{
	DaOut(2, 255);		// DA2チャンネルの出力電圧を5V
	wait(WAIT_BUZZER);
	DaOut(2, 0);		// DA2チャンネルの出力電圧を0V
}

/**
 * int move 動作方法の指定
 *
 * 0:	stop
 * 1:	go front
 * 2:	go back
 * 3:	rotate right
 * 4:	rotate left
 * 5:	
 *
 * unsigned short dlt 速さ指定 0 - 100
*/
void motor(int move, char dlt)
{
	printf("move: %d, dlt: %d\n", move, (int)dlt);
	switch (move) {
	case 0:	/* 一時停止*/
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ停止*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21             */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ停止*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19             */
		DaOut(0, dlt);	    	/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, dlt);	    	/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_SHUNJI);
		break;
		
	case 1:	/* 直進 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		break;
		
	case 2:	/* 後退 */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ逆回転*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ逆回転*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, dlt/3);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_Back);
		break;
		
	case 3:	/* 右回転 */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ逆回転*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		break;
		
	case 4:	/* 左回転 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ逆回転*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		break;
		
	default:
		printf("error\n");
	}
}


unsigned short dist1 = 0, dist2 = 0, dist3 = 0, foto1 = 0;

void input()
{
	int i;
	dist1 = 0;
	dist2 = 0;
	dist3 = 0;
	foto1 = 0;
	// 入力信号、5~10回くらいの平均
	for (i = 0; i < 5; i++) {
		dist1 += (PAdIn(DIST1)/5);	//入力信号をAD変換する
		dist2 += (PAdIn(DIST2)/5);
		dist3 += (PAdIn(DIST3)/5);
		foto1 += (PAdIn(FOTO1)/5);
		
		wait(WAIT_SENSOR);
	}
}

/*
 * distについて、距離が遠い<=>近いを、80 <=> 20に変換
*/
char normalize(unsigned short dist) {
	int dlt;
	if (dist < DIST_TRUE) {
			dlt = 0;
		}
		else if (dist > DIST_STOP) {
			dlt = DIST_STOP - DIST_TRUE;
		}
		else {
			dlt = dist - DIST_TRUE;
		}
		
		// move
		return (char)(VELOCITY-dlt/(DIST_STOP - DIST_TRUE)*VELOCITY);
}

/**
 * int move0 前の動作
 * int move1 後の動作
 * char dlt 変化前の速さ
*/
void change(int move0, int move1, char dlt) {
	int i;
	if (move0 == 0) {
		for(i = 0; i < 10; i++) {
			motor(move1, dlt*i/10);
		}
	}
	else {
		for(i = 9; i >= 0; i--) {
			motor(move0, dlt*i/10);
		}
	}
}

void search()
{
	/*
	 * int flg : 検知後のみmainに帰ってリセット
	 * 0: DIST3 is invalid
	 * 1: DIST3 is invalid
	 * 2: DIST3 is valid
	*/
	int flg = 0, max = 0, i = 0, j = 0;
	
	printf("search\n");
	while(1) {
		motor(1, VELOCITY);
		input();
		printf("while %d\ndist1: %d\tdist2: %d\tdist3: %d\tfoto1: %d\n",
				i++, dist1, dist2, dist3, foto1);
		
		if (foto1 > FOTO_TRUE || dist2 > DIST_STOP) { // 黒検知
			motor(0, 0);
			if (dist2 > DIST_TRUE) { // 目の前に缶
				buzzer();
				motor(2, VELOCITY);
				motor(0, 0);
				return;
			}
			motor(2, VELOCITY);
			motor(0, 0);
			continue;
		}
		if (dist2 > DIST_TRUE) { // 中央反応
			motor(1, normalize(dist2));
			continue;
		}
		if (dist1 > DIST_TRUE) { // 右反応
			motor(0, 0);
			while(1) {
				motor(3, VELOCITY);
				input(); // ラグがあれば直接取ってくる
				if (dist2 > DIST_TRUE) {
					motor(0, 0);
					break;
				}
			}
		}
		if (flg == 2) { // 左有効
			if (dist3 > DIST_TRUE) { // 左反応
				motor(0, 0);
				while(1) {
					motor(4, VELOCITY);
					input();
					if (dist2 > DIST_TRUE) {
						motor(0, 0);
						break;
					}
				}
			}				
		}
		// DIST3が無効なら、距離センサの減少具合を調べる
		if (flg == 0) {
			max = dist3;
			flg++;
			continue;
		}
		if (flg == 1) {
			if (max < dist3) {
				max = dist3;
				continue;
			}
			if (max - dist3 > 50) { // 同じ距離のときの振れ幅？誤差？的な、減少判定
				flg++;
			}
		}
		printf("\n");
	}
}


void main()
{
	int i;

	init();
	
	while(1) {
		search();
	}
}
