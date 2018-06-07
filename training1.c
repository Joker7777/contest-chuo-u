#include "ysml.h"
#include <stdio.h>
#include <sysio.h>
#include <stdlib.h>

/*********************************************************************
*****	�|�[�����O�ɂ��AD�ϊ�����
*****	���͒[�q
*****	�`�����l��      �[�q��		�s���ԍ�
*****	0				AN0			CN3_11
*****	1				AN1			CN3_12
*****	2				AN2			CN3_13
*****	3				AN3			CN3_14
*****	4				AN4			CN3_15
*****	5				AN5			CN3_16
*****	6			   (DA0)
*****	7			   (DA1)
*****	8				AN8			CN3_21
*****	9				AN9			CN3_22
*****	10				AN10		CN3_23
*****	11				AN11		CN3_24
*****	12				AN12		CN3_25
*****	13				AN13		CN3_26
*****	14			   (DA2)
*****	15			   (DA3)
*********************************************************************/

#define	DIST	0 // AN0, CN3_11
#define	FOTO	8 // AN8, CN3_21
////////////////////////////////////////////
#define	WAIT_SENSOR	(100000)
#define	WAIT_BUZZER	(100000)

void wait(unsigned long count)
{
	unsigned long x;
	for (x = 0; x < count; x++);
}


void main()
{
	unsigned short dist, foto;

	DaInit(2);		//DA2�`�����l���̏o�͋���
	// PAdInit(DIST);	//AD�ϊ��평����
	PAdInit(FOTO);
	
	while(1) {
		// dist = PAdIn(DIST);			//���͐M����AD�ϊ�����
		foto = PAdIn(FOTO);
		
		// printf("sensor: %d\n", dist);
		printf("sensor\tDig: %u\n", foto);
		
		
		// if (dist >= 430) { // d <= 10cm
		if (foto > 100) { // black
			DaOut(2, 255);		//DA2�`�����l���̏o�͓d����5V	
		}
		else { // d > 10cm or white
			DaOut(2, 0);		// DA2�`�����l���̏o�͓d����0V
		}

		wait(WAIT_SENSOR);
	}
}
