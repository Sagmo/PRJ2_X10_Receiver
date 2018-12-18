/*
 * Interrupt.h
 *
 * Created: 16-12-2018 21:24:20
 *  Author: victo
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_


void InitTimerInterrupts();
void InitZerocrossInterrupt();
void initPorts();
void Timer1Init();
void Timer3AInit(long Delay_us, long Prescaler);
void Timer1Reset();
void Timer3AReset();


#endif /* INTERRUPT_H_ */