/*
�@�T���v���v���O����
�@�^�C�}�[�����݂𗘗p���A���Ԋu�Ń��b�Z�[�W��\������

�@!!!!�K�������݃x�N�^�̐ݒ���s���ĉ������B!!!!
�@�����݃x�N�^�̐ݒ�́A�v���W�F�N�g�̐ݒ�́u�����݁v�ōs���B
�@�ݒ�l�͈ȉ��̂Ƃ���B
�@
�@�x�N�^�ԍ��@�����݊֐����@����
�@25          timer         C

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
	
	IPRD = 0x17;	//�����ݗD�揇��1 ���4�r�b�g���D�揇��
	TCSR0 = 0xa51f;
	TCSR0 = 0xa53f;

	_ei()		;	//�����ݗD�揇��1�ȏ�̊����݂�����
	while (1)
	{
		x = 1;
		y = 2;
		x = 3;
		y = 4;
	}
}
