/*���[�^�h���C�o��H����DC���[�^�̉�]��
���䂷�邽�߂̃}�C�R������v���O������*/

#include "ysml.h"
#include "ioh82633.h"
#include <stdio.h>
#include <sysio.h>

#define WAIT_SHUNJI	(100000)	/* �u����~�̒�~�J�E���g */
#define WAIT_TYPE_A	(350000)	/* ��~�J�E���g_�^�C�vA */
#define WAIT_TYPE_B	(100000)	/* ��~�J�E���g_�^�C�vB */

void wait(unsigned long count){
	unsigned long x;
	for (x = 0; x < count; x++);}

void main()
{
	unsigned short stop1;
/***** �Z���T���背�x���̐ݒ� *****/
	stop1=1;
	P1DDR.BYTE = 0xF0;	/* �|�[�g�ԍ�1(CN2)�� 0�`7�ԃs���������ݒ肷�� */
						/* 0�`3�ԃs��(P10�`P13)�̓f�[�^���͗p�[�q */
						/* 4�`7�ԃs��(P14�`P17)�̓f�[�^�o�͗p�[�q */									   
	DaInit(0);   		/* D/A0�|�[�g�̏o�͂�����(�E���[�^) */
	DaInit(1);   		/* D/A1�|�[�g�̏o�͂�����(�����[�^) */
	
/***** ���{�b�g���쐧��̎��s *****/
	while (stop1) 
	{	/* �ꎞ��~*/
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 �E���[�^��~*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21             */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 �����[�^��~*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19             */
		DaOut(0,0);	    	/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,0);	    	/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_A);
		
		/* ��� */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 �E���[�^�t��]*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 �����[�^�t��]*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0,60);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,60);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_A);
		
		/* �u����~ �؂�ւ����̊ђʓd����h�~ */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* ����] */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 �����[�^�t��]*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0,80);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,80);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_A);
		
		/* �u����~ �؂�ւ����̊ђʓd����h�~ */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* �E��] */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 �E���[�^�t��]*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,80);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,80);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_A);
		
		/* �u����~ �؂�ւ����̊ђʓd����h�~ */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* ���s���i */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,50);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,50);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_A);
		
		/* ���i */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,80);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,80);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_B);
		
		/* �E���O�i */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,50);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,90);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_B);
		
		/* �����O�i */
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,90);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,50);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_B);
		
		/* �u����~ �؂�ւ����̊ђʓd����h�~ */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* �E��] */
		P1DR.BIT.B4 = 0;	/*P14 pin CN2_22 �E���[�^�t��]*/
		P1DR.BIT.B5 = 1;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 1;	/*P16 pin CN2_20 �����[�^����]*/
		P1DR.BIT.B7 = 0;	/*P17 pin CN2_19               */
		DaOut(0,60);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,60);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_B);	
		
		/* �u����~ �؂�ւ����̊ђʓd����h�~ */
		P1DR.BIT.B4 = 0;
		P1DR.BIT.B5 = 0;
		P1DR.BIT.B6 = 0;
		P1DR.BIT.B7 = 0;
		DaOut(0,0);
		DaOut(1,0);
		wait(WAIT_SHUNJI);
		
		/* ����] */	
		P1DR.BIT.B4 = 1;	/*P14 pin CN2_22 �E���[�^����]*/
		P1DR.BIT.B5 = 0;	/*P15 pin CN2_21               */
		P1DR.BIT.B6 = 0;	/*P16 pin CN2_20 �����[�^�t��]*/
		P1DR.BIT.B7 = 1;	/*P17 pin CN2_19               */
		DaOut(0,60);		/*DA0 pin CN3_17 �E�o�����X����*/
		DaOut(1,60);		/*DA1 pin CN3_18 ���o�����X����*/
		wait(WAIT_TYPE_B);
	}	/* while (stop1) �̍ŏI�s */
}
