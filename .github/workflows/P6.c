#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include "p5.h"

#define MAX_TEMP 150
#define TIME_BOUNCE 1000000

void adcMode();
void codigo();
void secuencia();

int main(void) {

			pinMode('B',8, OUTPUT);				//configurar LED1
			pinMode('B',9, OUTPUT);				//configurar LED2
			pinMode('B',10, OUTPUT);			//configurar LED3
			pinMode('B',11, OUTPUT);			//configurar LED4


			/*
			digitalWrite('B',8,ON);
			digitalWrite('B',9,ON);
			digitalWrite('B',10,ON);
			digitalWrite('B',11,ON);

			digitalWrite('B',8,OFF);
			digitalWrite('B',9,OFF);
			digitalWrite('B',10,OFF);
			digitalWrite('B',11,OFF);
			 */

			codigo();

    return 0 ;
}
void adcMode(){

	    //clock B
		int *p2SCGC5;
		p2SCGC5 = (int *) 0x40048038;
		*p2SCGC5 |= 0x400;

		int *p2SCGC6; 						//clock del ADC
		p2SCGC6 = (int *) 0x4004803C;
		*p2SCGC6 |= 0x8000000;

		//ADC0_SE13
		int *p2Pcrb3;						// PCR B3
		p2Pcrb3 =  (int *) 0x4004A00C;
		*p2Pcrb3=0;

		int *p2confReg1;					//ADC Configuration Register 1 (ADCx_CFG1)
		p2confReg1 = (int *) 0x4003B008;
		*p2confReg1 = 0;					// 8-bit conversion;

		int *p2confReg2;					//ADC Configuration Register 2 (ADCx_CFG2)
		p2confReg2 = (int *) 0x4003B00C;
		*p2confReg2 = 0;

		int *p2SCR1;						// ADC Status and Control Registers 1 (ADC0_SC1A)
		p2SCR1 = (int *) 0x4003B000;
		*p2SCR1 = 0xD;						//01101

		return;
}
void codigo(){
	int *p2ADCresult = (int *) 0x4003B010;
	adcMode();

	while(1)
	{
		if(*p2ADCresult < MAX_TEMP)
		{
			for(;*p2ADCresult < MAX_TEMP;)
			{
				printf("medicion: %d  -menor a 150- \n",*p2ADCresult);
				adcMode();
			}
		}

		if(*p2ADCresult > MAX_TEMP)
		{

			for(int i=0; i<200; ++i)
			{

				printf("medicion: ( %d ) ------> tiempo:  %d   \n",*p2ADCresult, i);
				adcMode();

				if(*p2ADCresult < MAX_TEMP)
					i=0;
			}

			secuencia();
		}
	}
}

void secuencia(){
	int i;
	//B8-1 LED
	//B9-2 LED
	//B10-3 LED
	//B11-4 LED

	digitalWrite('B',8,ON);		//estado 0:
	for(i=0; i<TIME_BOUNCE; ++i);
	digitalWrite('B',8,OFF);


	digitalWrite('B',9,ON);	       //estado 1:
	for(i=0; i<TIME_BOUNCE; ++i);
	digitalWrite('B',9,OFF);

	digitalWrite('B',10,ON);	//estado 2:
	for(i=0; i<TIME_BOUNCE; ++i);
	digitalWrite('B',10,OFF);

	digitalWrite('B',11,ON);	//estado 3:
	for(i=0; i<TIME_BOUNCE; ++i);
	digitalWrite('B',11,OFF);

	digitalWrite('B',10,ON);	//estado 4:
	for(i=0; i<TIME_BOUNCE; ++i);
	digitalWrite('B',10,OFF);

	digitalWrite('B',9,ON);	   //estado 5:
	for(i=0; i<TIME_BOUNCE; ++i);
	digitalWrite('B',9,OFF);

	digitalWrite('B',8,ON);		//estado 6:
	for(i=0; i<TIME_BOUNCE; ++i);
	digitalWrite('B',8,OFF);
}
