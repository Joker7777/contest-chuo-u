/*
　サンプルプログラム
　タイマー割込みを利用し、一定間隔でメッセージを表示する

　!!!!必ず割込みベクタの設定を行って下さい。!!!!
　割込みベクタの設定は、プロジェクトの設定の「割込み」で行う。
　設定値は以下のとおり。
　
　ベクタ番号　割込み関数名　言語
　25          timer         C

*/
#include <stdio.h>
#include <sysio.h>

#define	TCSR0		(*((volatile unsigned short *)0xffff74))
#define	TCSR0_R		(*((volatile unsigned char *)0xffff74))

#define	TCNT0_W		(*((volatile unsigned char *)0xffff74))
#define	TCNT0_R		(*((volatile unsigned char *)0xffff75))
#define	RSTCSR0_W	(*((volatile unsigned char *)0xffff76))
#define	RSTCSR0_R	(*((volatile unsigned char *)0xffff77))

#define	IPRD		(*((volatile unsigned char *)0xfffec3))

void interrupt timer(void)
{
	unsigned int dum;
__break__
	dum = TCSR0_R;
	dum = 0xa500+(dum&0x7f);
	TCSR0 = dum;
	puts("Welcome to Contest !!!");
}

void foo2()
{
	int z;
	
	z = 1;
	
}

int c;

void main()
{
	int x, y;
	puts("*** Set 25 Timer and Start ***");
	x = 5;
	x = 6;
	c = 1;
	
	for (x = 1; x < 10; x++) {
		y = 1;
	}
	
	y = 10;
	
	foo2();
	
	IPRD = 0x17;	//割込み優先順位1 上位4ビットが優先順位
	TCSR0 = 0xa51f;
	TCSR0 = 0xa53f;

	_ei()		;	//割込み優先順位1以上の割込みを許可
	while (1)
	{
		x = 1;
		y = 2;
		x = 3;
		y = 4;
	}
}
