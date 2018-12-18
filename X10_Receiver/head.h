#ifndef _HEAD_HEAD_
#define _HEAD_HEAD_

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000
#include <util/delay.h>

#define SIZE 14

#include "Light.h"
#include "interrupt.h"

int comp_X10(int Set_X10[], int X10_IN[], int start_, int end);



#endif // _HEAD_HEAD_
