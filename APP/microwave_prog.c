/*
 * microwave_prog.c
 *
 * Created: 6/2/2020 8:14:56 AM
 *  Author: abdelrhman
 */ 

#include "stdint.h"
#include "LCD_interface.h"
#include "DIO_int.h"
#include "microwave_int.h"

uint32_t coock_time = 0;
uint32_t start_heat_time = 0;
uint32_t Rtime = 0;
uint32_t pause_time = 0;
uint32_t off_time = 0;
extern uint32_t Tick;

void IDLE()
{
	if ( last_state != IDLE)
	{
		DIO_VidSetPinValue(PORTC, MOTOR_PIN, LOW);
		DIO_VidSetPinValue(PORTC, HEATER_PIN, LOW);
		DIO_VidSetPinValue(PORTC, RED_PIN, LOW);
		DIO_VidSetPinValue(PORTC, YELLOW_PIN, LOW);
		DIO_VidSetPinValue(PORTC, GREEN_PIN, HIGH);
		
		LCD_vidSendCommand(lcd_Clear);
		LCD_vidWriteString("TIME MM:SS", 10);
		Gotoxy(1,5);
		LCD_vidWriteString("00:00", 5);
		last_state = IDLE;

	}

	
	if (!DIO_u8GetPinValue(PORTB, START_PIN) && coock_time > 0)
	{
		state = HEAT;
		last_state = IDLE;
	}
	else if (!DIO_u8GetPinValue(PORTB, SS_PIN))
	{
		while(!DIO_u8GetPinValue(PORTB, SS_PIN));
		coock_time += 1000;
		
	}
	else if (!DIO_u8GetPinValue(PORTB, MM_PIN))
	{
		while(!DIO_u8GetPinValue(PORTB, MM_PIN));
		coock_time += 6000;
	}
	else if (!DIO_u8GetPinValue(PORTB, MM10_PIN))
	{
		while(!DIO_u8GetPinValue(PORTB, MM10_PIN));
		coock_time += 60000;
	}
	else if (!DIO_u8GetPinValue(PORTB, STOP_PIN))
	{
		while(!DIO_u8GetPinValue(PORTB, STOP_PIN));
		coock_time = 0;
		Gotoxy(1,5);
		LCD_vidWriteString("00:00", 5);
	}
	
	if (coock_time/6000 >= 10)
	{
		Gotoxy(1,5);
		LCD_vidWriteNumber(coock_time/6000);
	}
	else
	{
		Gotoxy(1,6);
		LCD_vidWriteNumber(coock_time/6000);
	}
	
	if (coock_time >= 6000)
	{
		Gotoxy(1,8);
		LCD_vidWriteNumber((coock_time%6000)/100);
	}
	else
	{
		Gotoxy(1,8);
		LCD_vidWriteNumber(coock_time/100);
	}



}



void HEAT()
{
	if (last_state == IDLE)
	{
		start_heat_time = Tick;
	}
	else if (last_state == PAUSE)
	{
		off_time = Tick - pause_time;
	}
	
	if ( last_state != HEAT)
	{
		DIO_VidSetPinValue(PORTC, MOTOR_PIN, HIGH);
		DIO_VidSetPinValue(PORTC, HEATER_PIN, HIGH);
		DIO_VidSetPinValue(PORTC, RED_PIN, HIGH);
		DIO_VidSetPinValue(PORTC, YELLOW_PIN, LOW);
		DIO_VidSetPinValue(PORTC, GREEN_PIN, LOW);
		
		LCD_vidSendCommand(lcd_Clear);
		Gotoxy(0, 0);
		LCD_vidWriteString("REMAINING TIME", 14);
		Gotoxy(1,5);
		LCD_vidWriteString("00:00", 5);
		last_state = HEAT;
	}
	
	Rtime = off_time + (coock_time + start_heat_time) - Tick;
	
	if (Rtime == 0)
	{
		coock_time = 0;
		Rtime = 0;
		state = IDLE;
	}
	else if (!DIO_u8GetPinValue(PORTB, STOP_PIN))
	{
		while(!DIO_u8GetPinValue(PORTB, STOP_PIN));
		state = PAUSE;
	}
	
	
	print_time(Rtime);

	
}


void PAUSE()
{
	if ( last_state != PAUSE)
	{
		pause_time = Tick;
		DIO_VidSetPinValue(PORTC, MOTOR_PIN, LOW);
		DIO_VidSetPinValue(PORTC, HEATER_PIN, LOW);
		DIO_VidSetPinValue(PORTC, RED_PIN, LOW);
		DIO_VidSetPinValue(PORTC, YELLOW_PIN, HIGH);
		DIO_VidSetPinValue(PORTC, GREEN_PIN, LOW);
		
		LCD_vidSendCommand(lcd_Clear);
		LCD_vidWriteString("PAUSE", 5);
		Gotoxy(1,5);
		LCD_vidWriteString("00:00", 5);
		last_state = PAUSE;
	}
	
	if (!DIO_u8GetPinValue(PORTB, START_PIN))
	{
		while(!DIO_u8GetPinValue(PORTB, START_PIN));
		state = HEAT;
	}
	else if (!DIO_u8GetPinValue(PORTB, STOP_PIN))
	{
		while(!DIO_u8GetPinValue(PORTB, STOP_PIN));
		coock_time = 0;
		state = IDLE;
	}
	
	print_time(Rtime);

}

void print_time(uint32_t time)
{
	if (time/6000 >= 10)
	{
		Gotoxy(1,5);
		LCD_vidWriteNumber(time/6000);
	}
	else
	{
		Gotoxy(1,6);
		LCD_vidWriteNumber(time/6000);
	}
	
	if (time >= 6000)
	{
		if ((time%6000)/100 >= 10)
		{
			Gotoxy(1,8);
			LCD_vidWriteNumber((time%6000)/100);
		}
		else
		{
			Gotoxy(1,8);
			LCD_vidWriteNumber(0);
			Gotoxy(1,9);
			LCD_vidWriteNumber((time%6000)/100);
		}
	}
	else if (time/100 >= 10)
	{
		Gotoxy(1,8);
		LCD_vidWriteNumber(time/100);
	}
	else
	{
		Gotoxy(1,8);
		LCD_vidWriteNumber(0);
		Gotoxy(1,9);
		LCD_vidWriteNumber(time/100);
	}

}
