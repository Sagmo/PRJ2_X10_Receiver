/*
 * Light.c
 *
 * Created: 15-12-2018 11:58:10
 *  Author: Laursen
 */ 

#include "Light.h"


void initLight()
{
	DDRC = 0xFF;
}

void onLight()
{
	PORTC |= 0b00000010;
}

void offLight()
{
	PORTC |= ~(0b00000010);
}

void toggleLight()
{
	unsigned char mask;
	mask = 0b00000010;
	PORTC = PINC ^ mask;
}

