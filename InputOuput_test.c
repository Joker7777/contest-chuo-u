/* I/O�|�[�g�̐ݒ�A�g�p��
   P10�`P13����́AP14�`P17���o�͂ɐݒ肵�A
   P10�`P13�ɓ��͂��ꂽ�M����P14�`P17�ɏo�͂���B*/

#include"ioh82633.h"
#include<stdio.h>
#include<sysio.h>

void main() {
	P1DDR.BYTE = 0xF0;		//P10�`P17�̏����ݒ�(11110000: 0����,1�o��) 
	while(1){
		P1DR.BIT.B4 = PORT1.BIT.B0;		//P10(CN2_26)�̓��͂�P14(CN2_22)����o��
		//P1DR.BIT.B5 = PORT1.BIT.B1;		//P11(CN2_25)�̓��͂�P15(CN2_21)����o��
		//P1DR.BIT.B6 = PORT1.BIT.B2;		//P12(CN2_24)�̓��͂�P16(CN2_20)����o��
		//P1DR.BIT.B7 = PORT1.BIT.B3;		//P13(CN2_23)�̓��͂�P17(CN2_19)����o��
		printf("a");
	}
}

/* ���o�͒[�q�̃s���ԍ��͎戵�������Ŋm�F����B
	P10�`P13 �� CN2��26�ԁ`23�ԂɑΉ�
	P14�`P17 �� CN2��22�ԁ`19�ԂɑΉ�
	CN2��27,28,49,50�Ԃ�GND�ɑΉ�
	CN2�� 1, 2�Ԃ�Vcc(�{5.0V)�ɑΉ� */