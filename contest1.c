#include "ysml.h"
#include "ioh82633.h"
#include <stdio.h>
#include <sysio.h>

/*********************************************************************
*****	�|�[�����O�ɂ��AD�ϊ�����
*****	���͒[�q
*****	�`�����l��      �[�q��		�s���ԍ�
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

#define WAIT_SHUNJI	(100000)	/* �u����~�̒�~�J�E���g */
#define WAIT_TYPE_A	(50000)	/* ��~�J�E���g_�^�C�vA */
#define WAIT_TYPE_Back	(450000)	/* ���A�����쎞�� */
/////////////////////////////////////////////

#define	FOTO_TRUE	200 // black
#define	DIST_TRUE	170	// 30 cm
#define DIST_STOP	560 // 8 cm

#define VELOCITY 80 // max velocity

void wait(unsigned long count)
{
	unsigned long  x;
	for (x = 0; x < count; x++);
}

void init()
{
	P1DDR.BYTE = 0xF0;	/* �|�[�g�ԍ�1(CN2)�� 0�`7�ԃs���������ݒ肷�� */
						/* 0�`3�ԃs��(P10�`P13)�̓f�[�^���͗p�[�q */
						/* 4�`7�ԃs��(P14�`P17)�̓f�[�^�o�͗p�[�q */									   
	DaInit(0);   		/* D/A0�|�[�g�̏o�͂�����(�E���[�^) */
	DaInit(1);   		/* D/A1�|�[�g�̏o�͂�����(�����[�^) */
	
	DaInit(2);		//DA2�`�����l���̏o�͋���
	PAdInit(DIST1);	//AD�ϊ��평����
	PAdInit(DIST2);
	PAdInit(DIST3);
	PAdInit(FOTO1);
}

void buzzer()
{
	DaOut(2, 255);		// DA2�`�����l���̏o�͓d����5V
	wait(WAIT_BUZZER);
	DaOut(2, 0);		// DA2�`�����l���̏o�͓d����0V
}

/**
 * int move ������@�̎w��
 *
 * 0:	stop
 * 1:	go front
 * 2:	go back
 * 3:	rotate right
 * 4:	rotate left
 * 5:	
 *
 * unsigned short dlt �����w�� 0 - 100
*/
void motor(int move, char dlt)
{
	printf("move: %d, dlt: %d\n", move, (int)dlt);
	switch (move) {
	case 0:	/* �ꎞ��~*/
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 �E���[�^��~*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21             */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 �����[�^��~*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19             */
		DaOut(0, dlt);	    	/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1, dlt);	    	/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_SHUNJI);
		break;
		
	case 1:	/* ���i */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 ���o�����X����*/
		// wait(WAIT_TYPE_A);
		break;
		
	case 2:	/* ��� */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 �E���[�^�t��]*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 �����[�^�t��]*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1, dlt/3);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_Back);
		break;
		
	case 3:	/* �E��] */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 �E���[�^�t��]*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 ���o�����X����*/
		// wait(WAIT_TYPE_A);
		break;
		
	case 4:	/* ����] */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 �����[�^�t��]*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0, dlt);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1, dlt);		/*DA1 pin CN3_18 ���o�����X����*/
		// wait(WAIT_TYPE_A);
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
	// ���͐M���A5~10�񂭂炢�̕���
	for (i = 0; i < 5; i++) {
		dist1 += (PAdIn(DIST1)/5);	//���͐M����AD�ϊ�����
		dist2 += (PAdIn(DIST2)/5);
		dist3 += (PAdIn(DIST3)/5);
		foto1 += (PAdIn(FOTO1)/5);
		
		wait(WAIT_SENSOR);
	}
	printf("input\ndist1: %d\tdist2: %d\tdist3: %d\tfoto1: %d\n",
			dist1, dist2, dist3, foto1);
}

/*
 * dist�ɂ��āA����������<=>�߂����A80 <=> 30�ɕϊ�
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
		return (char)(VELOCITY-dlt/(DIST_STOP - DIST_TRUE)*50);
}

void search()
{
	/*
	 * int flg : ���m��̂�main�ɋA���ă��Z�b�g
	 * 0: DIST3 is invalid
	 * 1: DIST3 is invalid
	 * 2: DIST3 is valid
	*/
	int flg = 0, max = 0, i = 0, j = 0, t = 0;
	
	printf("search\n");
	while(1) {
		printf("while\n");
		input();
		
		if (foto1 > FOTO_TRUE
			|| dist2 > DIST_STOP
			|| dist1 > DIST_STOP
			|| dist3 > DIST_STOP) { // �����m & �߂�����
			motor(0, 0);
			if (dist2 > DIST_TRUE*2
				|| dist1 > DIST_TRUE*2
				|| dist3 > DIST_TRUE*2) { // �ڂ̑O�Ɋ�
				buzzer();
				motor(2, VELOCITY);
				motor(0, 0);
				return;
			}
			motor(2, VELOCITY);
			motor(0, 0);
			continue;
		}
		if (dist2 > DIST_TRUE) { // ��������
			motor(1, normalize(dist2));
			continue;
		}
		if (dist1 > DIST_TRUE) { // �E����
			motor(0, 0);
			t = 0;
			while(1) {
				motor(3, VELOCITY);
				input(); // ���O������Β��ڎ���Ă���
				if (dist2 > DIST_TRUE) {
					motor(0, 0);
					break;
				}
				if (t++ > 5) {
					break;
				}
			}
			continue;
		}
		if (flg == 2) { // ���L��
			if (dist3 > DIST_TRUE) { // ������
				motor(0, 0);
				t = 0;
				while(1) {
					motor(4, VELOCITY);
					input();
					if (dist2 > DIST_TRUE) {
						motor(0, 0);
						break;
					}
					if (t++ > 5) {
						break;
					}
				}
				continue;
			}				
		}
		// DIST3�������Ȃ�A�����Z���T�̌�����𒲂ׂ�
		if (flg == 0) {
			max = dist3;
			flg++;
		}
		else if (flg == 1) {
			if (max < dist3) {
				max = dist3;
			}
			else if (max - dist3 > 50) { // ���������̂Ƃ��̐U�ꕝ�H�덷�H�I�ȁA��������
				flg++;
			}
		}
		motor(1, VELOCITY);
		printf("\n");
	}
}


void main()
{
	init();
	
	while(1) {
		search();
	}
}

