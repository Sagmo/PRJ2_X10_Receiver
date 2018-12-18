/*
 * Interrupt.h
 *
 * Created: 16-12-2018 21:24:33
 *  Author: victo
 */ 


#include <avr/io.h>
#include "Interrupt.h"


void InitTimerInterrupts()
{	
	// TIMER 3 OVERFLOW INTERRUPT ENABLE
	TIMSK3 |= 0b00000001;
}

void InitZerocrossInterrupt()
{
	EIMSK |= 0b00000001; // Enable local interrupt INT0
	EICRA |= 0b00000001; // Enable any edge interrupt
}


void initPorts()
{
	DDRD &= 0b11111100; //PD0: ZC, PD1: X10
}


void Timer1Init()
{	
	// Toggle OC1A on compare match
	// Clock prescaler = 1
	TCCR1A = 0b01000000;
	TCCR1B = 0b00001001;
	OCR1A = 66; // 119,4 kHz square signal 50% duty cycle - f=fcpu/(2*prescaler*(1+OCRn)
}

void Timer3AInit(long Delay_ms, long Prescaler)
{
	TCCR3B = 0; // STOP TIMER
	TCNT3 = (65536 - ((Delay_ms * 16000) / Prescaler)); // Udregner delay på baggrund af TOP og prescaler. Antages 16 MHz cpu.
	TCCR3A = 0; // Normal operation
	
	if (Prescaler == 1)
	{
		TCCR3B = 0b00000001; // 1 Prescaler
	}
	else if (Prescaler == 8)
	{
		TCCR3B = 0b00000010; // 8 Prescaler
	}
	else if (Prescaler == 64)
	{
		TCCR3B = 0b00000011; // 64 Prescaler
	}
	else if (Prescaler == 256)
	{
		TCCR3B = 0b00000100; // 256 Prescaler
	}
	else if (Prescaler == 1024)
	{
		TCCR3B = 0b00000101; // 1024 Prescaler
	}
}
void Timer1Reset()
{
	
	TCCR1B = 0; // STOP TIMER
	TCNT1 = 0; //SET TIMER TO 0
}
void Timer3AReset()
{
	TCCR3B = 0; // STOP TIMER
	TIFR3 = 0b00000001; // RESET OVERFLOW FLAG
}