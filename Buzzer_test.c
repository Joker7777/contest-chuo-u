/*DA2, DA3 ����A�i���O�d�����o�͂��A���̏o�͐M�����g����/*
/*�x��\���e�X�g��H�̃u�U�[��炷����v���O������ */

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
	
	DaInit(2);	//DA2�`�����l���̏o�͋���
	DaInit(3);	//DA3�`�����l���̏o�͋���

	//�������[�v
	while (1)	{
		DaOut(2,255);		//DA2�`�����l���̏o�͓d����5V �ɐݒ�
		wait(WAIT_BUZZER);
		
		DaOut(2,0);			//DA2�`�����l���̏o�͓d����0V �ɐݒ�
		wait(WAIT_BUZZER);
		
		DaOut(3,255);		//DA3�`�����l���̏o�͓d����5V �ɐݒ�
		wait(WAIT_BUZZER);
		
		DaOut(3,0);			//DA3�`�����l���̏o�͓d����0V �ɐݒ�
		wait(WAIT_BUZZER);
		
		for (data = 0; data < 255; data++) {
			DaOut(2, data);		//DA2�`�����l���̏o�͓d����0V����5V �܂ő���
		}
		for (data = 255; data > 0; data--) {
			DaOut(2, data);		// DA2�`�����l���̏o�͓d����5V����0V �܂Ō���
		}
		wait(WAIT_BUZZER);
		
		for (data1 = 0; data1 < 255; data1++) {
			DaOut(3, data1); 	//DA3�`�����l���̏o�͓d����0V����5V �܂ő���
		}
		for (data1 = 255; data1 > 0; data1--) {
			DaOut(3, data1);	// DA3�`�����l���̏o�͓d����5V����0V �܂Ō���
		}
		wait(WAIT_BUZZER);
	}
}
