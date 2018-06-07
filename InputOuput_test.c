/* I/Oポートの設定、使用例
   P10～P13を入力、P14～P17を出力に設定し、
   P10～P13に入力された信号をP14～P17に出力する。*/

#include"ioh82633.h"
#include<stdio.h>
#include<sysio.h>

void main() {
	P1DDR.BYTE = 0xF0;		//P10～P17の初期設定(11110000: 0入力,1出力) 
	while(1){
		P1DR.BIT.B4 = PORT1.BIT.B0;		//P10(CN2_26)の入力をP14(CN2_22)から出力
		//P1DR.BIT.B5 = PORT1.BIT.B1;		//P11(CN2_25)の入力をP15(CN2_21)から出力
		//P1DR.BIT.B6 = PORT1.BIT.B2;		//P12(CN2_24)の入力をP16(CN2_20)から出力
		//P1DR.BIT.B7 = PORT1.BIT.B3;		//P13(CN2_23)の入力をP17(CN2_19)から出力
		printf("a");
	}
}

/* 入出力端子のピン番号は取扱説明書で確認する。
	P10～P13 は CN2の26番～23番に対応
	P14～P17 は CN2の22番～19番に対応
	CN2の27,28,49,50番はGNDに対応
	CN2の 1, 2番はVcc(＋5.0V)に対応 */