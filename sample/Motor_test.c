/*モータドライバ回路からDCモータの回転を
制御するためのマイコン制御プログラム例*/

#include "ysml.h"
#include "ioh82633.h"
#include <stdio.h>
#include <sysio.h>

#define WAIT_SHUNJI	(100000)	/* 瞬時停止の停止カウント */
#define WAIT_TYPE_A	(350000)	/* 停止カウント_タイプA */
#define WAIT_TYPE_B	(100000)	/* 停止カウント_タイプB */

void wait(unsigned long count){
	unsigned long x;
	for (x = 0; x < count; x++);}

void main()
{
	unsigned short stop1;
/***** センサ判定レベルの設定 *****/
	stop1=1;
	P1DDR.BYTE = 0xF0;	/* ポート番号1(CN2)の 0～7番ピンを初期設定する */
						/* 0～3番ピン(P10～P13)はデータ入力用端子 */
						/* 4～7番ピン(P14～P17)はデータ出力用端子 */									   
	DaInit(0);   		/* D/A0ポートの出力を許可(右モータ) */
	DaInit(1);   		/* D/A1ポートの出力を許可(左モータ) */
	
/***** ロボット動作制御の実行 *****/
	while (stop1) 
	{	/* 一時停止*/
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ停止*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21             */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ停止*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19             */
		DaOut(0,0);	    	/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,0);	    	/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		
		/* 後退 */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ逆回転*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ逆回転*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0,60);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,60);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		
		/* 瞬時停止 切り替え時の貫通電流を防止 */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* 左回転 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ逆回転*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0,80);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,80);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		
		/* 瞬時停止 切り替え時の貫通電流を防止 */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* 右回転 */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ逆回転*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,80);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,80);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		
		/* 瞬時停止 切り替え時の貫通電流を防止 */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* 徐行直進 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,50);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,50);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_A);
		
		/* 直進 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,80);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,80);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_B);
		
		/* 右方前進 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,50);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,90);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_B);
		
		/* 左方前進 */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,90);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,50);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_B);
		
		/* 瞬時停止 切り替え時の貫通電流を防止 */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* 右回転 */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 右モータ逆回転*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 左モータ順回転*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,60);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,60);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_B);	
		
		/* 瞬時停止 切り替え時の貫通電流を防止 */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* 左回転 */	
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 右モータ順回転*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 左モータ逆回転*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0,60);		/*DA0 pin CN3_17 右バランス調整*/
		DaOut(1,60);		/*DA1 pin CN3_18 左バランス調整*/
		wait(WAIT_TYPE_B);
	}	/* while (stop1) の最終行 */
}
