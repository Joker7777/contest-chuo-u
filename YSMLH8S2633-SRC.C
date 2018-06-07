/*********************************************************************
*****	YSML													******
*****	H8S/2633F用ライブラリ									******
*****															******
*********************************************************************/
#include "ysml.h"
#include <stdio.h>

#define	DEBUG_LIB

void error(char *s) { puts(s); }

#define	MSTPCRA	(*((volatile unsigned char *)0xFFFDE8))
#define	MSTPCRB	(*((volatile unsigned char *)0xFFFDE9))
#define	MSTPCRC	(*((volatile unsigned char *)0xFFFDEA))

#define	IPRA	(*((volatile unsigned char *)0xFFFEC0))
#define	IPRB	(*((volatile unsigned char *)0xFFFEC1))
#define	IPRC	(*((volatile unsigned char *)0xFFFEC2))
#define	IPRD	(*((volatile unsigned char *)0xFFFEC3))
#define	IPRE	(*((volatile unsigned char *)0xFFFEC4))
#define	IPRF	(*((volatile unsigned char *)0xFFFEC5))
#define	IPRG	(*((volatile unsigned char *)0xFFFEC6))
#define	IPRH	(*((volatile unsigned char *)0xFFFEC7))
#define	IPRI	(*((volatile unsigned char *)0xFFFEC8))
#define	IPRJ	(*((volatile unsigned char *)0xFFFEC9))
#define	IPRK	(*((volatile unsigned char *)0xFFFECA))
#define	IPRL	(*((volatile unsigned char *)0xFFFECB))
#define	IPRO	(*((volatile unsigned char *)0xFFFECE))

/*********************************************************************
*****	AD変換入力												******
*********************************************************************/

#define	ADDRAH	(*((volatile unsigned char *)0xFFFF90))
#define	ADDRAL	(*((volatile unsigned char *)0xFFFF91))
#define	ADDRBH	(*((volatile unsigned char *)0xFFFF92))
#define	ADDRBL	(*((volatile unsigned char *)0xFFFF93))
#define	ADDRCH	(*((volatile unsigned char *)0xFFFF94))
#define	ADDRCL	(*((volatile unsigned char *)0xFFFF95))
#define	ADDRDH	(*((volatile unsigned char *)0xFFFF96))
#define	ADDRDL	(*((volatile unsigned char *)0xFFFF97))
#define	ADCSR	(*((volatile unsigned char *)0xFFFF98))
/*--------------------------------------------------------------------
-----	ポーリング版											------
---------------------------------------------------------------------*/
/*********************************************************************
***** 初期設定													******
*********************************************************************/
void PAdInit(int ch)
{
	MSTPCRA &= ~0x02;		//モジュールストップモード解除
	
	while (ADCSR & 0x20)	//AD変換中なら終了までまつ
		;
	ADCSR &= ~0x80;			//ADFクリア
}

/*********************************************************************
***** ADからの入力
*********************************************************************/
unsigned short PAdIn(int ch)
{
	unsigned char high, low;

#ifdef DEBUG_LIB
	if (ch > 15 || ch < 0) {
		error("関数PAdIn チャンネルの範囲が不正です");
	}
#endif

	ADCSR = ch;				//チャンネル選択
	
	ADCSR |= 0x20;			//変換開始
	while (ADCSR & 0x20)	//変換終了まで待つ
		;
	ADCSR &= ~0x80;			//ADFクリア
	
	switch (ch) {
	case 0:	case 4:	case  8: case 12: high = ADDRAH;	low	 = ADDRAL;	break;
	case 1:	case 5:	case  9: case 13: high = ADDRBH;	low	 = ADDRBL;	break;
	case 2:	case 6:	case 10: case 14: high = ADDRCH;	low	 = ADDRCL;	break;
	case 3:	case 7:	case 11: case 15: high = ADDRDH;	low	 = ADDRDL;	break;
	}
	
	return (high << 2) + (low >> 6);
}

/*--------------------------------------------------------------------
-----	割り込み版												------
---------------------------------------------------------------------*/
/*********************************************************************
***** 割り込み優先レベルを設定する								******
*********************************************************************/
void IAdIntLevel(int ch, int level)
{
#ifdef DEBUG_LIB
	if (level > 7) {
		error("関数 IAdIntLevel 割り込みレベルの値が不正です");
	}
#endif

	IPRE &= ~0x07;
	IPRE |= level;
}

/*********************************************************************
***** 初期設定													******
*********************************************************************/
void IAdInit(int ch)
{
	MSTPCRA &= ~0x02;		//モジュールストップモード解除

	while (ADCSR & 0x20)	//AD変換中なら終了までまつ
		;
	ADCSR &= ~0x80;			//ADFクリア
	
	IPRE &= ~0x07;
	IPRE |=  0x01;			//割り込み優先レベル1
}

/*********************************************************************
***** AD変換スタート											******
*********************************************************************/
void IAdStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 15 || ch < 0) {
		error("関数IAdStart チャンネルの範囲が不正です");
	}
#endif

	ADCSR = (0x60+ch);		//チャンネル選択および変換開始
}

/*********************************************************************
***** ADからの入力 割り込み関数内で使用
*********************************************************************/
unsigned short IAdIn(int ch)
{
	unsigned char high, low;

#ifdef DEBUG_LIB
	if (ch > 15 || ch < 0) {
		error("関数IAdIn チャンネルの範囲が不正です");
	}
#endif

	ADCSR &= ~0x80;			//ADFクリア
	
	switch (ch) {
	case 0:	case 4:	case  8: case 12: high = ADDRAH;	low	 = ADDRAL;	break;
	case 1:	case 5:	case  9: case 13: high = ADDRBH;	low	 = ADDRBL;	break;
	case 2:	case 6:	case 10: case 14: high = ADDRCH;	low	 = ADDRCL;	break;
	case 3:	case 7:	case 11: case 15: high = ADDRDH;	low	 = ADDRDL;	break;
	}
	
	return (high << 2) + (low >> 6);
}

/*********************************************************************
*****　DA変換出力
*********************************************************************/

#define	DADR0	(*((volatile unsigned char *)0xFFFFA4))
#define	DADR1	(*((volatile unsigned char *)0xFFFFA5))
#define	DACR01	(*((volatile unsigned char *)0xFFFFA6))
#define	DADR2	(*((volatile unsigned char *)0xFFFDAC))
#define	DADR3	(*((volatile unsigned char *)0xFFFDAD))
#define	DACR23	(*((volatile unsigned char *)0xFFFDAE))

/*********************************************************************
*****　初期設定
*********************************************************************/
void DaInit(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 3 || ch < 0) {
		error("関数DaInit チャンネルの範囲が不正です");
	}
#endif
	
	if (ch < 2) {
		MSTPCRA &= ~0x04;
	}
	else {
		MSTPCRC &= ~0x20;
	}
}

/*********************************************************************
***** DAへのデータ出力および変換開始
*********************************************************************/
void DaOut(int ch, unsigned char data)
{
#ifdef DEBUG_LIB
	if (ch > 3 || ch < 0) {
		error("関数DaOut チャンネルの範囲が不正です");
	}
#endif
	
	switch (ch) {
	case 0:
		DADR0 = data;		//データ出力
		DACR01 |= 0x40;		//変換開始
		break;
	case 1:
		DADR1 = data;		//データ出力
		DACR01 |= 0x80;		//変換開始
		break;
	case 2:
		DADR2 = data;		//データ出力
		DACR23 |= 0x40;		//変換開始
		break;
	case 3:
		DADR3 = data;		//データ出力
		DACR23 |= 0x80;		//変換開始
		break;
	}
}

/*********************************************************************
***** DA変換ストップ&0出力
*********************************************************************/
void DaStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 3 || ch < 0) {
		error("関数DaStop チャンネルの範囲が不正です");
	}
#endif
	
	switch (ch) {
	case 0:
		DADR0 = 0;
		DACR01 &= ~0x40;		//変換終了
		break;
	case 1:
		DADR1 = 0;
		DACR01 &= ~0x80;		//変換終了
		break;
	case 2:
		DADR2 = 0;
		DACR23 &= ~0x40;		//変換終了
		break;
	case 3:
		DADR3 = 0;
		DACR23 &= ~0x80;		//変換終了
		break;
	}
}

/*********************************************************************
*****　インターバルタイマ
*********************************************************************/
//共通
#define	TSTR	(*((volatile unsigned char *)0xFFFEB0))
#define	TSYR	(*((volatile unsigned char *)0xFFFEB1))

//CH0
#define	TCR0	(*((volatile unsigned char *)0xFFFF10))
#define	TMDR0	(*((volatile unsigned char *)0xFFFF11))
#define	TIOR0H	(*((volatile unsigned char *)0xFFFF12))
#define	TIOR0L	(*((volatile unsigned char *)0xFFFF13)
#define	TIER0	(*((volatile unsigned char *)0xFFFF14))
#define	TSR0	(*((volatile unsigned char *)0xFFFF15))
#define	TCNT0	(*((volatile unsigned short *)0xFFFF16))
#define	TGR0A	(*((volatile unsigned short *)0xFFFF18))
#define	TGR0B	(*((volatile unsigned short *)0xFFFF1A))
#define	TGR0C	(*((volatile unsigned short *)0xFFFF1C))
#define	TGR0D	(*((volatile unsigned short *)0xFFFF1E))
//CH1
#define	TCR1	(*((volatile unsigned char *)0xFFFF20))
#define	TMDR1	(*((volatile unsigned char *)0xFFFF21))
#define	TIOR1	(*((volatile unsigned char *)0xFFFF22))
#define	TIER1	(*((volatile unsigned char *)0xFFFF24))
#define	TSR1	(*((volatile unsigned char *)0xFFFF25))
#define	TCNT1	(*((volatile unsigned short *)0xFFFF26))
#define	TGR1A	(*((volatile unsigned short *)0xFFFF28))
#define	TGR1B	(*((volatile unsigned short *)0xFFFF2A))
//CH2
#define	TCR2	(*((volatile unsigned char *)0xFFFF30))
#define	TMDR2	(*((volatile unsigned char *)0xFFFF31))
#define	TIOR2	(*((volatile unsigned char *)0xFFFF32))
#define	TIER2	(*((volatile unsigned char *)0xFFFF34))
#define	TSR2	(*((volatile unsigned char *)0xFFFF35))
#define	TCNT2	(*((volatile unsigned short *)0xFFFF36))
#define	TGR2A	(*((volatile unsigned short *)0xFFFF38))
#define	TGR2B	(*((volatile unsigned short *)0xFFFF3A))
//CH3
#define	TCR3	(*((volatile unsigned char *)0xFFFE80))
#define	TMDR3	(*((volatile unsigned char *)0xFFFE81))
#define	TIOR3H	(*((volatile unsigned char *)0xFFFE82))
#define	TIOR3L	(*((volatile unsigned char *)0xFFFE83)
#define	TIER3	(*((volatile unsigned char *)0xFFFE84))
#define	TSR3	(*((volatile unsigned char *)0xFFFE85))
#define	TCNT3	(*((volatile unsigned short *)0xFFFE86))
#define	TGR3A	(*((volatile unsigned short *)0xFFFE88))
#define	TGR3B	(*((volatile unsigned short *)0xFFFE8A))
#define	TGR3C	(*((volatile unsigned short *)0xFFFE8C))
#define	TGR3D	(*((volatile unsigned short *)0xFFFE8E))
//CH4
#define	TCR4	(*((volatile unsigned char *)0xFFFE90))
#define	TMDR4	(*((volatile unsigned char *)0xFFFE91))
#define	TIOR4	(*((volatile unsigned char *)0xFFFE92))
#define	TIER4	(*((volatile unsigned char *)0xFFFE94))
#define	TSR4	(*((volatile unsigned char *)0xFFFE95))
#define	TCNT4	(*((volatile unsigned short *)0xFFFE96))
#define	TGR4A	(*((volatile unsigned short *)0xFFFE98))
#define	TGR4B	(*((volatile unsigned short *)0xFFFE9A))
//CH5
#define	TCR5	(*((volatile unsigned char *)0xFFFEA0))
#define	TMDR5	(*((volatile unsigned char *)0xFFFEA1))
#define	TIOR5	(*((volatile unsigned char *)0xFFFEA2))
#define	TIER5	(*((volatile unsigned char *)0xFFFEA4))
#define	TSR5	(*((volatile unsigned char *)0xFFFEA5))
#define	TCNT5	(*((volatile unsigned short *)0xFFFEA6))
#define	TGR5A	(*((volatile unsigned short *)0xFFFEA8))
#define	TGR5B	(*((volatile unsigned short *)0xFFFEAA))

/*********************************************************************
*****　インターバル初期化
*****	クロックモード
*****	クロックモード	値		分周比
*****	TIM_CLOCK1		0		1/1
*****	TIM_CLOCK2		1		1/4
*****	TIM_CLOCK3		2		1/16
*****	TIM_CLOCK4		3		1/64
*********************************************************************/
void IntervalInit(int ch, unsigned short count, int clock)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数IntervalInit チャンネルの範囲が不正です");
	}
#endif

	MSTPCRA &= ~0x20;	//モジュールストップ解除
	
	TSTR &= ~(1<<ch);	//カウンタストップ
	
	switch (ch) {
	case 0:
		TCNT0 = 0;		//カウンタクリア
		TCR0 = clock;	//クロック選択
		TCR0 |= 0x20;	//GRAコンペアマッチ
		TGR0A =  count;	//GRA設定
		TIER0 |= 1;		//GRAコンペアマッチ割り込み許可
		IPRF &= ~0x70;
		IPRF |=  0x10;	//割り込みレベル1
		break;
	case 1:
		TCNT1 = 0;
		TCR1 = clock;	//クロック選択
		TCR1 |= 0x20;	//GRAコンペアマッチ
		TGR1A =  count;	//GRA設定
		TIER1 |= 1;		//GRAコンペアマッチ割り込み許可
		IPRF &= ~0x07;
		IPRF |=  0x01;	//割り込みレベル1
		break;
	case 2:
		TCNT2 = 0;
		TCR2 = clock;	//クロック選択
		TCR2 |= 0x20;	//GRAコンペアマッチ
		TGR2A =  count;	//GRA設定
		TIER2 |= 1;		//GRAコンペアマッチ割り込み許可
		IPRG &= ~0x70;
		IPRG |=  0x10;	//割り込みレベル1
		break;
	case 3:
		TCNT3 = 0;
		TCR3 = clock;	//クロック選択
		TCR3 |= 0x20;	//GRAコンペアマッチ
		TGR3A =  count;	//GRA設定
		TIER3 |= 1;		//GRAコンペアマッチ割り込み許可
		IPRG &= ~0x07;
		IPRG |=  0x01;	//割り込みレベル1
		break;
	case 4:
		TCNT4 = 0;
		TCR4 = clock;	//クロック選択
		TCR4 |= 0x20;	//GRAコンペアマッチ
		TGR4A =  count;	//GRA設定
		TIER4 |= 1;		//GRAコンペアマッチ割り込み許可
		IPRH &= ~0x70;
		IPRH |=  0x10;	//割り込みレベル1
		break;
	case 5:
		TCNT5 = 0;
		TCR5 = clock;	//クロック選択
		TCR5 |= 0x20;	//GRAコンペアマッチ
		TGR5A =  count;	//GRA設定
		TIER5 |= 1;		//GRAコンペアマッチ割り込み許可
		IPRH &= ~0x07;
		IPRH |=  0x01;	//割り込みレベル1
		break;
	}
}

/*********************************************************************
*****　インターバルタイマスタート
*********************************************************************/
void IntervalStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数IntervalStart チャンネルの範囲が不正です");
	}
#endif
	
	TSTR |= (1<<ch);
}

/*********************************************************************
*****　インターバルタイマストップ
*********************************************************************/
void IntervalStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数IntervalStop チャンネルの範囲が不正です");
	}
#endif
	
	TSTR &= ~(1<<ch);
}

/*********************************************************************
*****　インターバルタイマ　割り込み内処理
*********************************************************************/
void IntervalIntClear(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数IntervalIntClear チャンネルの範囲が不正です");
	}
#endif

	//割り込み要求フラグクリア
	switch (ch) {
	case 0:	TSR0 &= ~1;	break;
	case 1:	TSR1 &= ~1;	break;
	case 2:	TSR2 &= ~1;	break;
	case 3:	TSR3 &= ~1;	break;
	case 4:	TSR4 &= ~1;	break;
	case 5:	TSR5 &= ~1;	break;
	}
}

/*********************************************************************
*****　カウンタ
*********************************************************************/

/*********************************************************************
*****　カウンタ初期化
*****	クロックモード
*****	クロックモード	値		分周比
*****	TIM_CLOCK1		0		1/1
*****	TIM_CLOCK2		1		1/4
*****	TIM_CLOCK3		2		1/16
*****	TIM_CLOCK4		3		1/64
*********************************************************************/
void CounterInit(int ch, int clock)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数CounterInit チャンネルの範囲が不正です");
	}
#endif

	MSTPCRA &= ~0x20;	//モジュールストップ解除

	TSTR &= ~(1<<ch);	//カウンタストップ
	
	//カウンタのクリア、クロックの選択、およびオーバフローフラグのクリア
	switch (ch) {
	case 0:		TCNT0 = 0; TCR0 = clock; TSR0 &= ~0x3f;		break;
	case 1:		TCNT1 = 0; TCR1 = clock; TSR1 &= ~0x3f;		break;
	case 2:		TCNT2 = 0; TCR2 = clock; TSR2 &= ~0x3f;		break;
	case 3:		TCNT3 = 0; TCR3 = clock; TSR3 &= ~0x3f;		break;
	case 4:		TCNT4 = 0; TCR4 = clock; TSR4 &= ~0x3f;		break;
	case 5:		TCNT5 = 0; TCR5 = clock; TSR5 &= ~0x3f;		break;
	}
}

/*********************************************************************
*****　カウンタスタート
*********************************************************************/
void CounterStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数CounterStart チャンネルの範囲が不正です");
	}
#endif
	
	TSTR |= (1<<ch);
}

/*********************************************************************
*****　カウンタストップ
*********************************************************************/
void CounterStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数CounterStop チャンネルの範囲が不正です");
	}
#endif
	
	TSTR &= ~(1<<ch);
}

/*********************************************************************
*****　カウンタ読み込み
*********************************************************************/
unsigned short CounterRead(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数CounterRead チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	return TCNT0;
	case 1:	return TCNT1;
	case 2:	return TCNT2;
	case 3:	return TCNT3;
	case 4:	return TCNT4;
	case 5:	return TCNT5;
	}
}

/*********************************************************************
*****　カウンタオーバフローのチェック
*********************************************************************/
int CounterCheckOver(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数CounterRead チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	return (TSR0 & 0x10)? 1:0;
	case 1:	return (TSR1 & 0x10)? 1:0;
	case 2:	return (TSR2 & 0x10)? 1:0;
	case 3:	return (TSR3 & 0x10)? 1:0;
	case 4:	return (TSR4 & 0x10)? 1:0;
	case 5:	return (TSR5 & 0x10)? 1:0;
	}
}


/*********************************************************************
*****　パルスの出力
*********************************************************************/
/*********************************************************************
*****　初期化
*****	クロックモード
*****	クロックモード	値		分周比
*****	TIM_CLOCK1		0		1/1
*****	TIM_CLOCK2		1		1/4
*****	TIM_CLOCK3		2		1/16
*****	TIM_CLOCK4		3		1/64
*********************************************************************/
void PulseInit(int ch, unsigned short cyc, unsigned short width, int clock)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseInit チャンネルの範囲が不正です");
	}
#endif

	MSTPCRA &= ~0x20;	//モジュールストップ解除

	TSTR &= ~(1<<ch);	//カウンタストップ
	
	switch (ch) {
	case 0:
		TCNT0 = 0;		//カウンタクリア
		TCR0 = clock;	//クロック選択
		TCR0 |= 0x20;	//GRAコンペアマッチクリア
		TGR0B =  width;	//パルス幅
		TGR0A =  cyc;	//パルス周期
		TIOR0H = 0x16;	//GRA初期出力1、コンペアマッチで出力1
						//GRB初期出力0、コンペアマッチで出力0
		TMDR0 = 0x02;	//PWMモード1
		break;
	case 1:
		TCNT1 = 0;
		TCR1 = clock;	//クロック選択
		TCR1 |= 0x20;	//GRAコンペアマッチクリア
		TGR1B =  width;	//パルス幅
		TGR1A =  cyc;	//パルス周期
		TIOR1 = 0x16;	//GRA初期出力1、コンペアマッチで出力1
						//GRB初期出力0、コンペアマッチで出力0
		TMDR1 = 0x02;	//PWMモード1
		break;
	case 2:
		TCNT2 = 0;
		TCR2 = clock;	//クロック選択
		TCR2 |= 0x20;	//GRAコンペアマッチクリア
		TGR2B =  width;	//パルス幅
		TGR2A =  cyc;	//パルス周期
		TIOR2 = 0x16;	//GRA初期出力1、コンペアマッチで出力1
						//GRB初期出力0、コンペアマッチで出力0
		TMDR2 = 0x02;	//PWMモード1
		break;
	case 3:
		TCNT3 = 0;
		TCR3 = clock;	//クロック選択
		TCR3 |= 0x20;	//GRAコンペアマッチクリア
		TGR3B =  width;	//パルス幅
		TGR3A =  cyc;	//パルス周期
		TIOR3H = 0x16;	//GRA初期出力1、コンペアマッチで出力1
						//GRB初期出力0、コンペアマッチで出力0
		TMDR3 = 0x02;	//PWMモード1
		break;
	case 4:
		TCNT4 = 0;
		TCR4 = clock;	//クロック選択
		TCR4 |= 0x20;	//GRAコンペアマッチクリア
		TGR4B =  width;	//パルス幅
		TGR4A =  cyc;	//パルス周期
		TIOR4 = 0x16;	//GRA初期出力1、コンペアマッチで出力1
						//GRB初期出力0、コンペアマッチで出力0
		TMDR4 = 0x02;	//PWMモード1
	case 5:
		TCNT5 = 0;
		TCR5 = clock;	//クロック選択
		TCR5 |= 0x20;	//GRAコンペアマッチクリア
		TGR5B =  width;	//パルス幅
		TGR5A =  cyc;	//パルス周期
		TIOR5 = 0x02;	//初期出力0 コンペアマッチで1
		TIOR5 = 0x16;	//GRA初期出力1、コンペアマッチで出力1
						//GRB初期出力0、コンペアマッチで出力0
		TMDR5 = 0x02;	//PWMモード1
		break;
	}
}

/*********************************************************************
*****　パルス出力スタート
*********************************************************************/
void PulseStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseStart チャンネルの範囲が不正です");
	}
#endif
	
	TSTR |= (1<<ch);
}

/*********************************************************************
*****　パルス出力ストップ
*********************************************************************/
void PulseStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseStop チャンネルの範囲が不正です");
	}
#endif
	
	TSTR &= ~(1<<ch);
}


/*********************************************************************
*****　インプットキャプチャ
*********************************************************************/
/*********************************************************************
*****　インプットキャプチャ初期設定
*****  mode
*****	INPCAP_MODE0	0	立ち上がりエッジでキャプチャ
*****	INPCAP_MODE1	1	立ち下がりエッジでキャプチャ
*****	INPCAP_MODE2	2	両エッジでキャプチャ
*****	クロックモード
*****	クロックモード	値		分周比
*****	TIM_CLOCK1		0		1/1
*****	TIM_CLOCK2		1		1/4
*****	TIM_CLOCK3		2		1/16
*****	TIM_CLOCK4		3		1/64
*********************************************************************/
void InputCaptureInit(int ch, int mode, int clock)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数InputCaptureInit チャンネルの範囲が不正です");
	}
#endif

	MSTPCRA &= ~0x20;	//モジュールストップ解除

	TSTR &= ~(1<<ch);	//カウンタストップ
	
	switch (ch) {
	case 0:
		TSR0 &= ~0x3f;		//フラグクリア
		TCNT0 = 0;			//カウンタクリア
		TIOR0H |= (8+mode);	//GRAへインプット
		TCR0 = clock;		//クロック選択
		TCR0 |= 0x20;		//GRAインプットキャプチャでクリア
		IPRF &= ~0x70;
		IPRF |=  0x10;	//割り込みレベル1
		break;
	case 1:
		TSR1 &= ~0x3f;		//フラグクリア
		TCNT1 = 0;
		TIOR1 |= (8+mode);	//GRAへインプット
		TCR1 = clock;		//クロック選択
		TCR1 |= 0x20;		//GRAインプットキャプチャでクリア
		IPRF &= ~0x07;
		IPRF |=  0x01;	//割り込みレベル1
		break;
	case 2:
		TSR2 &= ~0x3f;		//フラグクリア
		TCNT2 = 0;
		TIOR2 |= (8+mode);	//GRAへインプット
		TCR2 = clock;		//クロック選択
		TCR2 |= 0x20;		//GRAインプットキャプチャでクリア
		IPRG &= ~0x70;
		IPRG |=  0x10;	//割り込みレベル1
		break;
	case 3:
		TSR3 &= ~0x3f;		//フラグクリア
		TCNT3 = 0;
		TIOR3H |= (8+mode);	//GRAへインプット
		TCR3 = clock;		//クロック選択
		TCR3 |= 0x20;		//GRAインプットキャプチャでクリア
		IPRG &= ~0x07;
		IPRG |=  0x01;	//割り込みレベル1
		break;
	case 4:
		TSR4 &= ~0x3f;		//フラグクリア
		TCNT4 = 0;
		TIOR4 |= (8+mode);	//GRAへインプット
		TCR4 = clock;		//クロック選択
		TCR4 |= 0x20;		//GRAインプットキャプチャでクリア
		IPRH &= ~0x70;
		IPRH |=  0x10;	//割り込みレベル1
		break;
	case 5:
		TSR5 &= ~0x3f;		//フラグクリア
		TCNT5 = 0;
		TIOR5 |= (8+mode);	//GRAへインプット
		TCR5 = clock;		//クロック選択
		TCR5 |= 0x20;		//GRAインプットキャプチャでクリア
		IPRH &= ~0x07;
		IPRH |=  0x01;	//割り込みレベル1
		break;
	}
	
	//カウンタスタート
	TSTR |= (1<<ch);
}

/*********************************************************************
*****　インプトキャプチャ割り込みスタート
*********************************************************************/
void InputCaptureStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数InputCaptureStart チャンネルの範囲が不正です");
	}
#endif

	//インプットキャプチャ割り込み許可
	switch (ch) {
	case 0:	TIER0 |= 0x1;	break;
	case 1:	TIER1 |= 0x1;	break;
	case 2:	TIER2 |= 0x1;	break;
	case 3:	TIER3 |= 0x1;	break;
	case 4:	TIER4 |= 0x1;	break;
	case 5:	TIER5 |= 0x1;	break;
	}

}


/*********************************************************************
*****　インプトキャプチャ割り込み停止
*********************************************************************/
void InputCaptureStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数InputCaptureStop チャンネルの範囲が不正です");
	}
#endif

	//インプットキャプチャ割り込み許可
	switch (ch) {
	case 0:	TIER0 &= ~0x1;	break;
	case 1:	TIER1 &= ~0x1;	break;
	case 2:	TIER2 &= ~0x1;	break;
	case 3:	TIER3 &= ~0x1;	break;
	case 4:	TIER4 &= ~0x1;	break;
	case 5:	TIER5 &= ~0x1;	break;
	}
}

/*********************************************************************
*****　インプットキャプチャ割り込み内処理
*********************************************************************/
unsigned short InputCaptureIntClear(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数InputCaptureIntClear チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:		TSR0 &= ~0x3f;	return TGR0A;
	case 1:		TSR1 &= ~0x3f;	return TGR1A;
	case 2:		TSR2 &= ~0x3f;	return TGR2A;
	case 3:		TSR3 &= ~0x3f;	return TGR3A;
	case 4:		TSR4 &= ~0x3f;	return TGR4A;
	case 5:		TSR5 &= ~0x3f;	return TGR5A;
	}
}

/*********************************************************************
*****	インプットキャプチャオーバーフローチェック
*********************************************************************/
int InputCaptureCheckOver(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数InputCaptureCheckOver チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:		return (TSR0 & 0x10) ? 1:0;
	case 1:		return (TSR1 & 0x10) ? 1:0;
	case 2:		return (TSR2 & 0x10) ? 1:0;
	case 3:		return (TSR3 & 0x10) ? 1:0;
	case 4:		return (TSR4 & 0x10) ? 1:0;
	case 5:		return (TSR5 & 0x10) ? 1:0;
	}
}

/*********************************************************************
*****	パルス数のカウント
*********************************************************************/

/*********************************************************************
*****	パルス数のカウント初期設定
*****  mode
*****	PULSECNT_MODE0	0	TCLKA入力
*****	PULSECNT_MODE1	1	TCLKB
*****	PULSECNT_MODE2	2	TCLKC
*****	PULSECNT_MODE3	3	TCLKD
*****	クロックモード
*****	クロックモード	値		分周比
*****	TIM_CLOCK1		0		1/1
*****	TIM_CLOCK2		1		1/4
*****	TIM_CLOCK3		2		1/16
*****	TIM_CLOCK4		3		1/64
*********************************************************************/
void PulseCountInit(int ch, int mode, int clock)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseCountInit チャンネルの範囲が不正です");
	}
#endif

	MSTPCRA &= ~0x20;	//モジュールストップ解除

	TSTR &= ~(1<<ch);	//カウンタストップ
	
	switch (ch) {
	case 0:
		TSR0 &= ~0x3f;		//フラグクリア
		TCNT0 = 0;			//カウンタクリア
		TCR0 = (4+mode);	//外部クロック選択
		break;
	case 1:
#ifdef DEBUG_LIB
		if (mode == PULSECNT_MODE2 || mode == PULSECNT_MODE3) {
			error("関数PulseCountInit 入力端子が不正です。");
		}
#endif
		TSR1 &= ~0x3f;		//フラグクリア
		TCNT1 = 0;			//カウンタクリア
		TCR1 = (4+mode);	//外部クロック選択
		break;
	case 2:
#ifdef DEBUG_LIB
		if (mode == PULSECNT_MODE3) {
			error("関数PulseCountInit 入力端子が不正です。");
		}
#endif
		TSR2 &= ~0x3f;		//フラグクリア
		TCNT2 = 0;			//カウンタクリア
		TCR2 = (4+mode);	//外部クロック選択
		break;
	case 3:
#ifdef DEBUG_LIB
		if (mode != PULSECNT_MODE0) {
			error("関数PulseCountInit 入力端子が不正です。");
		}
#endif
		TSR3 &= ~0x3f;		//フラグクリア
		TCNT3 = 0;			//カウンタクリア
		TCR3 = (4+mode);	//外部クロック選択
		break;
	case 4:
#ifdef DEBUG_LIB
		if (mode == PULSECNT_MODE1 || mode == PULSECNT_MODE3) {
			error("関数PulseCountInit 入力端子が不正です。");
		}
#endif
		if (mode == PULSECNT_MODE2) mode = PULSECNT_MODE1;
		TSR4 &= ~0x3f;		//フラグクリア
		TCNT4 = 0;			//カウンタクリア
		TCR4 = (4+mode);	//外部クロック選択
		break;
	case 5:
#ifdef DEBUG_LIB
		if (mode == PULSECNT_MODE1) {
			error("関数PulseCountInit 入力端子が不正です。");
		}
#endif
		if (mode == PULSECNT_MODE2) mode = PULSECNT_MODE1;
		TSR5 &= ~0x3f;		//フラグクリア
		TCNT5 = 0;			//カウンタクリア
		TCR5 = (4+mode);	//外部クロック選択
		break;
	}
}

/*********************************************************************
*****	パルスカウントスタート
*********************************************************************/
void PulseCountStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseCountStart チャンネルの範囲が不正です");
	}
#endif

	TSTR |= (1<<ch);
}

/*********************************************************************
*****	パルスカウントストップ
*********************************************************************/
void PulseCountStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseCountStop チャンネルの範囲が不正です");
	}
#endif

	TSTR &= ~(1<<ch);
}

/*********************************************************************
*****	パルスカウントの読み込みおよび0クリア
*********************************************************************/
unsigned short PulseCountRead(int ch)
{
	unsigned short cnt;
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseCountRead チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	cnt = TCNT0;	TCNT0 = 0;	break;
	case 1:	cnt = TCNT1;	TCNT1 = 0;	break;
	case 2:	cnt = TCNT2;	TCNT2 = 0;	break;
	case 3:	cnt = TCNT3;	TCNT3 = 0;	break;
	case 4:	cnt = TCNT4;	TCNT4 = 0;	break;
	case 5:	cnt = TCNT5;	TCNT5 = 0;	break;
	}

	return cnt;
}


/*********************************************************************
*****	パルスカウントオーバーフローチェック
*********************************************************************/
int PulseCountCheckOver(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数PulseCountCheckOver チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:		return (TSR0 & 0x10) ? 1:0;
	case 1:		return (TSR1 & 0x10) ? 1:0;
	case 2:		return (TSR2 & 0x10) ? 1:0;
	case 3:		return (TSR3 & 0x10) ? 1:0;
	case 4:		return (TSR4 & 0x10) ? 1:0;
	case 5:		return (TSR5 & 0x10) ? 1:0;
	}
}


/*********************************************************************
*****	タイマーチャンネルのリセット
*********************************************************************/
void TimerReset(int ch)
{
	int bit;
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数TimerReset チャンネルの範囲が不正です");
	}
#endif

	bit = ~(1<<ch);
	//共通レジスタ
	TSTR &= bit;
	
	switch (ch) {
	case 0:
		TMDR0 = 0;
		TCNT0 = 0;
		TGR0A = 0;
		TGR0B = 0;
		TCR0 = 0;
		TIOR0H = 0;
		TSR0 &= 0;
		TIER0 = 0;
		break;
	case 1:
		TMDR1 = 0;
		TCNT1 = 0;
		TGR1A = 0;
		TGR1B = 0;
		TCR1 = 0;
		TIOR1 = 0;
		TSR1 &= 0;
		TIER1 = 0;
		break;
	case 2:
		TMDR2 = 0;
		TCNT2 = 0;
		TGR2A = 0;
		TGR2B = 0;
		TCR2 = 0;
		TIOR2 = 0;
		TSR2 &= 0;
		TIER2 = 0;
		break;
	case 3:
		TMDR3 = 0;
		TCNT3 = 0;
		TGR3A = 0;
		TGR3B = 0;
		TCR3 = 0;
		TIOR3H = 0;
		TSR2 &= 0;
		TIER3 = 0;
		break;
	case 4:
		TMDR4 = 0;
		TCNT4 = 0;
		TGR4A = 0;
		TGR4B = 0;
		TCR4 = 0;
		TIOR4 = 0;
		TSR4 &= 0;
		TIER4 = 0;
		break;
	case 5:
		TMDR5 = 0;
		TCNT5 = 0;
		TGR5A = 0;
		TGR5B = 0;
		TCR5 = 0;
		TIOR5 = 0;
		TSR5 &= 0;
		TIER5 = 0;
		break;
	}
}

/*********************************************************************
***** 割り込み優先レベルを設定する								******
*********************************************************************/
void TimerIntLevel(int ch, int level)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数TimerIntLevel チャンネルの範囲が不正です");
	}
	if (level > 7) {
		error("関数 TimerIntLevel 割り込みレベルの値が不正です");
	}
#endif
	switch (ch) {
	case 0:	IPRF &= ~0x70;	IPRF |= (level<<4);	break;
	case 1:	IPRF &= ~0x07;	IPRF |= (level);	break;
	case 2:	IPRG &= ~0x70;	IPRG |= (level<<4);	break;
	case 3:	IPRG &= ~0x07;	IPRG |= (level);	break;
	case 4:	IPRH &= ~0x70;	IPRH |= (level<<4);	break;
	case 5:	IPRH &= ~0x07;	IPRH |= (level);	break;
	}
}
/********************************************************************
*****	オーバーフローフラグをクリアする
*********************************************************************/
void TimerOverFlowClear(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数TimerOverFlowClear チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	TSR0 &= ~0x10;	return;
	case 1:	TSR1 &= ~0x10;	return;
	case 2:	TSR2 &= ~0x10;	return;
	case 3:	TSR3 &= ~0x10;	return;
	case 4:	TSR4 &= ~0x10;	return;
	case 5:	TSR5 &= ~0x10;	return;
	}
}

/********************************************************************
*****	オーバーフロー割り込みを発生させる
*********************************************************************/
void TimerOverFlowStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数TimerOverFlowStart チャンネルの範囲が不正です");
	}
#endif

	//割り込みレベルを1にする
	switch (ch) {
	case 0:	IPRF &= ~0x70;	IPRF |= (1<<4);	break;
	case 1:	IPRF &= ~0x07;	IPRF |= (1);	break;
	case 2:	IPRG &= ~0x70;	IPRG |= (1<<4);	break;
	case 3:	IPRG &= ~0x07;	IPRG |= (1);	break;
	case 4:	IPRH &= ~0x70;	IPRH |= (1<<4);	break;
	case 5:	IPRH &= ~0x07;	IPRH |= (1);	break;
	}

	switch (ch) {
	case 0:	TIER0 |= 0x10;	return;
	case 1:	TIER1 |= 0x10;	return;
	case 2:	TIER2 |= 0x10;	return;
	case 3:	TIER3 |= 0x10;	return;
	case 4:	TIER4 |= 0x10;	return;
	case 5:	TIER5 |= 0x10;	return;
	}
}


/********************************************************************
*****	オーバーフロー割り込みを停止する
*********************************************************************/
void TimerOverFlowStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 5 || ch < 0) {
		error("関数TimerOverFlowStop チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	TIER0 &= ~0x10;	return;
	case 1:	TIER1 &= ~0x10;	return;
	case 2:	TIER2 &= ~0x10;	return;
	case 3:	TIER3 &= ~0x10;	return;
	case 4:	TIER4 &= ~0x10;	return;
	case 5:	TIER5 &= ~0x10;	return;
	}
}


/*********************************************************************
*****	シリアルポート入出力
*********************************************************************/

#define	SMR0	(*((volatile unsigned char *)0xFFFF78))
#define	BRR0	(*((volatile unsigned char *)0xFFFF79))
#define	SCR0	(*((volatile unsigned char *)0xFFFF7A))
#define	TDR0	(*((volatile unsigned char *)0xFFFF7B))
#define	SSR0	(*((volatile unsigned char *)0xFFFF7C))
#define	RDR0	(*((volatile unsigned char *)0xFFFF7D))

#define	SMR1	(*((volatile unsigned char *)0xFFFF80))
#define	BRR1	(*((volatile unsigned char *)0xFFFF81))
#define	SCR1	(*((volatile unsigned char *)0xFFFF82))
#define	TDR1	(*((volatile unsigned char *)0xFFFF83))
#define	SSR1	(*((volatile unsigned char *)0xFFFF84))
#define	RDR1	(*((volatile unsigned char *)0xFFFF85))

#define	SMR2	(*((volatile unsigned char *)0xFFFF88))
#define	BRR2	(*((volatile unsigned char *)0xFFFF89))
#define	SCR2	(*((volatile unsigned char *)0xFFFF8A))
#define	TDR2	(*((volatile unsigned char *)0xFFFF8B))
#define	SSR2	(*((volatile unsigned char *)0xFFFF8C))
#define	RDR2	(*((volatile unsigned char *)0xFFFF8D))

#define	SMR3	(*((volatile unsigned char *)0xFFFDD0))
#define	BRR3	(*((volatile unsigned char *)0xFFFDD1))
#define	SCR3	(*((volatile unsigned char *)0xFFFDD2))
#define	TDR3	(*((volatile unsigned char *)0xFFFDD3))
#define	SSR3	(*((volatile unsigned char *)0xFFFDD4))
#define	RDR3	(*((volatile unsigned char *)0xFFFDD5))

#define	SMR4	(*((volatile unsigned char *)0xFFFDD8))
#define	BRR4	(*((volatile unsigned char *)0xFFFDD9))
#define	SCR4	(*((volatile unsigned char *)0xFFFDDA))
#define	TDR4	(*((volatile unsigned char *)0xFFFDDB))
#define	SSR4	(*((volatile unsigned char *)0xFFFDDC))
#define	RDR4	(*((volatile unsigned char *)0xFFFDDD))

/*********************************************************************
*****	通信設定
*****	ch		チャンネル
*****	brr		BRR値
****	datlen	COMM_DATA_8			0		//データ長 8ビット
****			COMM_DATA_7			1		//データ長 7ビット
****	pe		COMM_PARITY_OFF		0		//パリティ禁止
****			COMM_PARITY_ON		1		//パリティ許可
*****	par		COMM_PARITY_EVEN	0		//偶数パリティ
****			COMM_PARITY_ODD		1		//奇数パリティ
*****	stop	COMM_STOPBIT_1		0		//ストップビット1
*****			COMM_STOPBIT_2		1		//ストップビット2
*********************************************************************/
void SioCommSet(int ch, unsigned char brr, char datlen, char pe, char par, char stop)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数SioCommSet チャンネルの範囲が不正です");
	}
#endif
	switch (ch) {
	case 0:
		MSTPCRB &= ~0x80;	//モジュールストップ解除
		SMR0 = 0;
		if (stop) SMR0 |= 0x08;
		if (par)  SMR0 |= 0x10;
		if (pe)   SMR0 |= 0x20;
		if (datlen) SMR0 |= 0x40;
		BRR0 = brr;
		break;
	case 1:
		MSTPCRB &= ~0x40;	//モジュールストップ解除
		SMR1 = 0;
		if (stop) SMR1 |= 0x08;
		if (par)  SMR1 |= 0x10;
		if (pe)   SMR1 |= 0x20;
		if (datlen) SMR1 |= 0x40;
		BRR1 = brr;
		break;
	case 2:
		MSTPCRB &= ~0x20;	//モジュールストップ解除
		SMR2 = 0;
		if (stop) SMR2 |= 0x08;
		if (par)  SMR2 |= 0x10;
		if (pe)   SMR2 |= 0x20;
		if (datlen) SMR2 |= 0x40;
		BRR2 = brr;
		break;
	case 3:
		MSTPCRC &= ~0x80;	//モジュールストップ解除
		SMR3 = 0;
		if (stop) SMR3 |= 0x08;
		if (par)  SMR3 |= 0x10;
		if (pe)   SMR3 |= 0x20;
		if (datlen) SMR3 |= 0x40;
		BRR3 = brr;
		break;
	case 4:
		MSTPCRC &= ~0x40;	//モジュールストップ解除
		SMR4 = 0;
		if (stop) SMR4 |= 0x08;
		if (par)  SMR4 |= 0x10;
		if (pe)   SMR4 |= 0x20;
		if (datlen) SMR4 |= 0x40;
		BRR4 = brr;
		break;
	}
}

/*--------------------------------------------------------------------
-----	ポーリング版
---------------------------------------------------------------------*/
/*********************************************************************
*****	受信初期設定
*********************************************************************/
void PSioInInit(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数PSioInInit チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		SCR0 |= 0x10;		//受信許可
		SCR0 &= ~0x40;		//受信割り込み禁止
		break;
	case 1:
		SCR1 |= 0x10;		//受信許可
		SCR1 &= ~0x40;		//受信割り込み禁止
		break;
	case 2:
		SCR2 |= 0x10;		//受信許可
		SCR2 &= ~0x40;		//受信割り込み禁止
		break;
	case 3:
		SCR3 |= 0x10;		//受信許可
		SCR3 &= ~0x40;		//受信割り込み禁止
		break;
	case 4:
		SCR4 |= 0x10;		//受信許可
		SCR4 &= ~0x40;		//受信割り込み禁止
		break;
	}
}

/*********************************************************************
*****	受信
*********************************************************************/
unsigned char PSioIn(int ch)
{
	unsigned char data;
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数PSioIn チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		while ((SSR0 & 0x78) == 0)	//受信完了待ち
			;
		data = RDR0;
		SSR0 &= ~0x78;				//フラグ類クリア
		break;
	case 1:
		while ((SSR1 & 0x78) == 0)	//受信完了待ち
			;
		data = RDR1;
		SSR1 &= ~0x78;				//フラグ類クリア
		break;
	case 2:
		while ((SSR2 & 0x78) == 0)	//受信完了待ち
			;
		data = RDR2;
		SSR2 &= ~0x78;				//フラグ類クリア
		break;
	case 3:
		while ((SSR3 & 0x78) == 0)	//受信完了待ち
			;
		data = RDR3;
		SSR3 &= ~0x78;				//フラグ類クリア
		break;
	case 4:
		while ((SSR4 & 0x78) == 0)	//受信完了待ち
			;
		data = RDR4;
		SSR4 &= ~0x78;				//フラグ類クリア
		break;
	}
	
	return data;
}

/*********************************************************************
*****	送信初期設定
*********************************************************************/
void PSioOutInit(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数PSioOutInit チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		SCR0 |= 0x20;		//送信許可
		SCR0 &= ~0x80;		//送信割り込み禁止
		break;
	case 1:
		SCR1 |= 0x20;		//送信許可
		SCR1 &= ~0x80;		//送信割り込み禁止
		break;
	case 2:
		SCR2 |= 0x20;		//送信許可
		SCR2 &= ~0x80;		//送信割り込み禁止
		break;
	case 3:
		SCR3 |= 0x20;		//送信許可
		SCR3 &= ~0x80;		//送信割り込み禁止
		break;
	case 4:
		SCR4 |= 0x20;		//送信許可
		SCR4 &= ~0x80;		//送信割り込み禁止
		break;
	}
}

/*********************************************************************
*****	送信
*********************************************************************/
void PSioOut(int ch, unsigned char data)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数PSioOut チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		while ((SSR0 & 0x80) == 0)	//送信完了待ち
			;
		TDR0 = data;
		SSR0 &= ~0x80;
		break;
	case 1:
		while ((SSR1 & 0x80) == 0)	//送信完了待ち
			;
		TDR1 = data;
		SSR1 &= ~0x80;
		break;
	case 2:
		while ((SSR2 & 0x80) == 0)	//送信完了待ち
			;
		TDR2 = data;
		SSR2 &= ~0x80;
		break;
	case 3:
		while ((SSR3 & 0x80) == 0)	//送信完了待ち
			;
		TDR3 = data;
		SSR3 &= ~0x80;
		break;
	case 4:
		while ((SSR4 & 0x80) == 0)	//送信完了待ち
			;
		TDR4 = data;
		SSR4 &= ~0x80;
		break;
	}
}

/*-------------------------------------------------------------------
-----	割り込み版
--------------------------------------------------------------------*/
/*********************************************************************
*****	割り込み優先レベルを設定する
*********************************************************************/
void ISioIntLevel(int ch, int level)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数ISioIntLevel チャンネルの範囲が不正です");
	}
	if (level > 7) {
		error("関数ISioIntLevel 割り込み優先レベルの範囲が不正です");
	}
#endif
	switch (ch) {
	case 0:	IPRJ &= ~0x07;	IPRJ |= (level);	break;
	case 1:	IPRK &= ~0x70;	IPRK |= (level<<4);	break;
	case 2:	IPRK &= ~0x07;	IPRK |= (level);	break;
	case 3:	IPRO &= ~0x70;	IPRO |= (level<<4);	break;
	case 4:	IPRO &= ~0x07;	IPRO |= (level);	break;
	}
}

/*********************************************************************
*****	受信割り込み開始
*********************************************************************/
void ISioInStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数ISioInInit チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		IPRJ &= ~0x07; IPRJ |= 0x01;	//割り込み優先レベル1
		SCR0 |= 0x50; return;		//受信許可および受信割り込み許可
	case 1:
		IPRK &= ~0x70; IPRK |= 0x10;	//割り込み優先レベル1
		SCR1 |= 0x50; return;		//受信許可および受信割り込み許可
	case 2:
		IPRK &= ~0x07; IPRK |= 0x01;	//割り込み優先レベル1
		SCR2 |= 0x50; return;		//受信許可および受信割り込み許可
	case 3:
		IPRO &= ~0x70; IPRO |= 0x10;	//割り込み優先レベル1
		SCR3 |= 0x50; return;		//受信許可および受信割り込み許可
	case 4:
		IPRO &= ~0x07; IPRO |= 0x01;	//割り込み優先レベル1
		SCR4 |= 0x50; return;		//受信許可および受信割り込み許可
	}
}

/*********************************************************************
*****	受信割り込み終了
*********************************************************************/
void ISioInStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数ISioInInit チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	SCR0 &= ~0x40; return;		//受信割り込み禁止
	case 1:	SCR1 &= ~0x40; return;		//受信割り込み禁止
	case 2:	SCR2 &= ~0x40; return;		//受信割り込み禁止
	case 3:	SCR3 &= ~0x40; return;		//受信割り込み禁止
	case 4:	SCR4 &= ~0x40; return;		//受信割り込み禁止
	}
}

/*********************************************************************
*****	受信	割り込み関数内で使用
*********************************************************************/
unsigned char ISioIn(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数ISioIn チャンネルの範囲が不正です");
	}
#endif

	
	switch (ch) {
	case 0:		SSR0 &= ~0x78; return RDR0;
	case 1:		SSR1 &= ~0x78; return RDR1;
	case 2:		SSR2 &= ~0x78; return RDR2;
	case 3:		SSR3 &= ~0x78; return RDR3;
	case 4:		SSR4 &= ~0x78; return RDR4;
	}
}

/*********************************************************************
*****	送信割り込みスタート
*********************************************************************/
void ISioOutStart(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数ISioOutInit チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		IPRJ &= ~0x07; IPRJ |= 0x01;	//割り込み優先レベル1
		SCR0 |= 0xA0; return;		//送信許可
	case 1:
		IPRK &= ~0x70; IPRK |= 0x10;	//割り込み優先レベル1
		SCR1 |= 0xA0; return;		//送信許可
	case 2:
		IPRK &= ~0x07; IPRK |= 0x01;	//割り込み優先レベル1
		SCR2 |= 0xA0; return;		//送信許可
	case 3:
		IPRO &= ~0x70; IPRO |= 0x10;	//割り込み優先レベル1
		SCR3 |= 0xA0; return;		//送信許可
	case 4:
		IPRO &= ~0x07; IPRO |= 0x01;	//割り込み優先レベル1
		SCR4 |= 0xA0; return;		//送信許可
	}
}

/********************************************************************
*****	送信割り込みを止める									*****
********************************************************************/
void ISioOutStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数ISioOutStop チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	SCR0 &= ~0x80; return;		//送信割り込み禁止
	case 1:	SCR1 &= ~0x80; return;		//送信割り込み禁止
	case 2:	SCR2 &= ~0x80; return;		//送信割り込み禁止
	case 3:	SCR3 &= ~0x80; return;		//送信割り込み禁止
	case 4:	SCR4 &= ~0x80; return;		//送信割り込み禁止
	}
}

/*********************************************************************
*****	送信 割り込み関数内で使用
*********************************************************************/
void ISioOut(int ch, unsigned char data)
{
#ifdef DEBUG_LIB
	if (ch > 4 || ch < 0) {
		error("関数ISioOut チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	TDR0 = data;	SSR0 &= ~0x80; break;
	case 1:	TDR1 = data;	SSR1 &= ~0x80; break;
	case 2:	TDR2 = data;	SSR2 &= ~0x80; break;
	case 3:	TDR3 = data;	SSR3 &= ~0x80; break;
	case 4:	TDR4 = data;	SSR4 &= ~0x80; break;
	}
}

/*********************************************************************
*****	ウオッチドックタイマ
*********************************************************************/

//CH0
#define	TCSR0_R	(*((volatile unsigned char *)0xFFFF74))
#define	TCSR0_W	(*((volatile unsigned short *)0xFFFF74))
#define	TCNT0_R	(*((volatile unsigned char *)0xFFFF75))
#define	TCNT0_W	(*((volatile unsigned short *)0xFFFF74))
#define	RSTCSR_R	(*((volatile unsigned char *)0xFFFF77))
#define	RSTCSR_W	(*((volatile unsigned short *)0xFFFF76))
//CH1
#define	TCSR1_R	(*((volatile unsigned char *)0xFFFFA2))
#define	TCSR1_W	(*((volatile unsigned short *)0xFFFFA2))
#define	TCNT1_R	(*((volatile unsigned char *)0xFFFFA3))
#define	TCNT1_W	(*((volatile unsigned short *)0xFFFFA2))

/*********************************************************************
*****	ウオッチドックタイマの初期設定
*****	WDT_MODE_INT	0	インターバルタイマ
*****	WDT_MODE_WDT	1	ウオッチドッグタイマ
*****	分周			値		分周比
*****	WDT_CLOCK1		0		1/2
*****	WDT_CLOCK2		1		1/64
*****	WDT_CLOCK3		2		1/128
*****	WDT_CLOCK4		3		1/512
*****	WDT_CLOCK5		4		1/2048
*****	WDT_CLOCK6		5		1/8192
*****	WDT_CLOCK7		6		1/32768
*****	WDT_CLOCK8		7		1/131072
*********************************************************************/
void WatchDogInit(int ch, int mode, int fai)
{
#ifdef DEBUG_LIB
	if (ch > 1 || ch < 0) {
		error("関数WatchDogInit チャンネルの範囲が不正です");
	}
#endif

	if (mode) {
		switch (ch) {
		case 0:
			TCSR0_W = 0xa500+(0x40+fai);		//ウオッチドッグタイマ
			RSTCSR_W = 0x5a40;					//内部リセットを発生
			break;
		case 1:
			TCSR1_W = 0xa500+(0x48+fai);		//内部リセットを発生
			break;
		}
	}
	else {
		switch (ch) {
		case 0:
			TCSR0_W = 0xa500+fai;			//インターバルタイマ
			IPRD &= ~0x70;
			IPRD |=  0x10;					//割り込みレベル1
			break;
		case 1:
			TCSR1_W = 0xa500+fai;
			IPRE &= ~0x07;
			IPRE |=  0x01;					//割り込みレベル1
			break;
		}
	}
}

/*********************************************************************
*****	ウオッチドッグタイマ開始
*********************************************************************/
void WatchDogStart(int ch)
{
	unsigned char c;
#ifdef DEBUG_LIB
	if (ch > 1 || ch < 0) {
		error("関数WatchDogStart チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		c = TCSR0_R;
		c |= 0x20;
		TCSR0_W = 0xa500+(0x00ff&c);
		break;
	case 1:
		c = TCSR1_R;
		c |= 0x28;
		TCSR1_W = 0xa500+(0x00ff&c);
		break;
	}
}


/*********************************************************************
*****	ウオッチドッグタイマ停止
*********************************************************************/
void WatchDogStop(int ch)
{
	unsigned char c;
#ifdef DEBUG_LIB
	if (ch > 1 || ch < 0) {
		error("関数WatchDogStop チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		c = TCSR0_R;
		c &= ~0x20;
		TCSR0_W = 0xa500+c;
		break;
	case 1:
		c = TCSR0_R;
		c &= ~0x20;
		TCSR0_W = 0xa500+c;
		break;
	}
}


/*********************************************************************
*****	ウオッチドッグタイマの再設定
*********************************************************************/
void WatchDogWrite(int ch, unsigned char cnt)
{
#ifdef DEBUG_LIB
	if (ch > 1 || ch < 0) {
		error("関数WatchDogWrite チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	TCNT0_W = 0x5a00+(0x00ff&cnt);	break;
	case 1:	TCNT1_W = 0x5a00+(0x00ff&cnt);	break;
	}
}

/*********************************************************************
*****	ウオッチドッグタイマ　割り込み内処理
*********************************************************************/
void WatchDogIntClear(int ch)
{
	unsigned char c;
#ifdef DEBUG_LIB
	if (ch > 1 || ch < 0) {
		error("関数WatchDogIntClear チャンネルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:
		c = TCSR0_R;
		c &= ~0x80;
		TCSR0_W = 0xa500 + c;	//割り込み要求フラグクリア
		break;
	case 1:
		c = TCSR1_R;
		c &= ~0x80;
		TCSR1_W = 0xa500 + c;	//割り込み要求フラグクリア
		break;
	}
}

/*********************************************************************
*****	ウオッチドッグタイマ　オーバーフローの判定
*********************************************************************/
int WatchDogCheckOver(int ch)
{
	unsigned char c;
#ifdef DEBUG_LIB
	if (ch > 1 || ch < 0) {
		error("関数WatchDogCheckOver チャンネルの範囲が不正です");
	}
	if (ch == 1) {
		error("関数WatchDogCheckOver チャンネル1ではこの関数は使えません");
	}
#endif


	c = RSTCSR_R;
	if (c & 0x80) {
		RSTCSR_W = 0xa500;
		return 1;
	}
	else {
		return 0;
	}
}

/*********************************************************************
*****	割り込み優先レベルを設定する
*********************************************************************/
void WatchDogIntLevel(int ch, int level)
{
#ifdef DEBUG_LIB
	if (ch > 1 || ch < 0) {
		error("関数WatchDogIntLevel チャンネルの範囲が不正です");
	}
	if (level > 7) {
		error("関数WatchDogIntLevel 割り込み優先レベルの範囲が不正です");
	}
#endif

	switch (ch) {
	case 0:	IPRD &= ~0x70; IPRD |= (level<<4);	break;
	case 1:	IPRE &= ~0x07; IPRE |= (level);	break;
	}
}

/*********************************************************************
*****	外部割り込み
*********************************************************************/

#define	ISCRH	(*((volatile unsigned char *)0xFFFE12))
#define	ISCRL	(*((volatile unsigned char *)0xFFFE13))
#define	IER		(*((volatile unsigned char *)0xFFFE14))
#define	ISR		(*((volatile unsigned char *)0xFFFE15))

/*********************************************************************
*****	割り込み優先レベルを設定する
*********************************************************************/
void IrqIntLevel(int ch, int level)
{
#ifdef DEBUG_LIB
	if (ch > 7 || ch < 0) {
		error("関数IrqIntLevel チャンネルの範囲が不正です");
	}
	if (level > 7) {
		error("関数IrqIntLevel 割り込み優先レベルの範囲が不正です");
	}
#endif
	// チャンネル2と3、チャンネル4と5、チャンネル6と7は共通なので注意
	switch (ch) {
	case 0:	IPRA &= ~0x70; IPRA |= (level<<4);	break;
	case 1:	IPRA &= ~0x07; IPRA |= (level);		break;
	case 2:
	case 3: IPRB &= ~0x70; IPRB |= (level<<4);	break;
	case 4:
	case 5: IPRB &= ~0x07; IPRB |= (level);		break;
	case 6:
	case 7: IPRC &= ~0x70; IPRC |= (level<<4);	break;
	}
}

/*********************************************************************
*****	外部割り込みの初期設定
*****	IRQ_MODE_LOWLEVEL	0		//LOWレベルで割り込み
*****	IRQ_MODE_DOWN		1		//たち下がりで割り込み
*****	IRQ_MODE_UP			2		//立ち上がりで割り込み
*****	IRQ_MODE_UPDOWN		3		//両エッジで割り込み
*********************************************************************/
void IrqInit(int ch, int mode)
{
#ifdef DEBUG_LIB
	if (ch > 7 || ch < 0) {
		error("関数IrqInit チャンネルの範囲が不正です");
	}
#endif

	//割り込み優先レベルを１にする
	switch (ch) {
	case 0:	IPRA &= ~0x70; IPRA |= 0x10;	break;
	case 1:	IPRA &= ~0x07; IPRA |= 0x01;	break;
	case 2:
	case 3: IPRB &= ~0x70; IPRB |= 0x10;	break;
	case 4:
	case 5: IPRB &= ~0x07; IPRB |= 0x01;	break;
	case 6:
	case 7: IPRC &= ~0x70; IPRC |= 0x10;	break;
	}
	
	switch (ch) {
	case 0:	ISCRL &= ~0x03;	ISCRL |= mode;		break;
	case 1:	ISCRL &= ~0x0c;	ISCRL |= (mode<<2);	break;
	case 2:	ISCRL &= ~0x30;	ISCRL |= (mode<<4);	break;
	case 3:	ISCRL &= ~0xc0;	ISCRL |= (mode<<6);	break;
	case 4:	ISCRH &= ~0x03;	ISCRH |= mode;		break;
	case 5:	ISCRH &= ~0x0c;	ISCRH |= (mode<<2);	break;
	case 6:	ISCRH &= ~0x30;	ISCRH |= (mode<<4);	break;
	case 7:	ISCRH &= ~0xc0;	ISCRH |= (mode<<6);	break;
	}

	IER |= (1<<ch);
}

/*********************************************************************
*****	外部割り込み処理 割り込み関数内で使用
*********************************************************************/
void IrqIntClear(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 7 || ch < 0) {
		error("関数IrqIntClear チャンネルの範囲が不正です");
	}
#endif

	ISR &= ~(1<<ch);
}

/*********************************************************************
*****	外部割り込み終了
*********************************************************************/
void IrqStop(int ch)
{
#ifdef DEBUG_LIB
	if (ch > 7 || ch < 0) {
		error("関数IrqStop チャンネルの範囲が不正です");
	}
#endif
	
	IER &= ~(1<<ch);
}

/*********************************************************************
*****	スリープ
*********************************************************************/

#define	SBYCR	(*((volatile unsigned char *)0xFFFDE4))
#define	LPWRCR	(*((volatile unsigned char *)0xFFFDEC))
/*********************************************************************
*****	スリープへの移行
*****	SLP_MODE_SLP	0	スリープモード
*****	SLP_MODE_SFT	1	ソフトウェアスタンバイモード
*********************************************************************/
void SleepStart(int mode)
{
	LPWRCR &= ~0x40;
	if (mode) {
		SBYCR |= 0x80;
		_asm	SLEEP
		SBYCR &= ~0x80;
	}
	else {
		SBYCR &= ~0x80;
		_asm	SLEEP
	}
}
