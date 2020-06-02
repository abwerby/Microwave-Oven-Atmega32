/*
 * MicroWave.c
 *
 * Created: 6/2/2020 12:18:48 AM
 * Author : abdelrhman werby
 */ 

#include <avr/io.h>
#include "avr/interrupt.h"
#include "avr/delay.h"
#include "LCD_interface.h"
#include "DIO_int.h"
#include "Timer.h"
#include "microwave_int.h"

#define INIT_TICK 178

extern uint32_t Tick = 0;

void init_system();


int main(void)
{

	init_system();
	
    while (1) 
    {	
		state();
    }
}


void init_system()
{
	DIO_VidSetPortDirection(PORTD, OUTPUT);
	DIO_VidSetPortDirection(PORTA, OUTPUT);
	DIO_VidSetPortDirection(PORTC, OUTPUT);
	DIO_VidSetPortDirection(PORTB, INPUT);
	DIO_VidSetPortValue(PORTB, HIGH);

	LCD_vidInit();
	
	/* Generate interrupt every 10ms */
	TIM0_OVF_Init(TIMER_PRESCALER_1024, INIT_TICK);
	state = IDLE;
}


/* Generate interrupt every 10ms */
ISR(TIMER0_OVF_vect)
{
	Tick++;
	TCNT0 = INIT_TICK;
}

