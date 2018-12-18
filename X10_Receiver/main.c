#include "head.h"

// LOCAL FUNCTIONS
void check_input();
void restart_check();
void check_command();


// Check parts of array
char start     = 0; // 1: Start ok, 0: Start notok
char houseCode = 0; // 1: HC ok, 0: HC: not ok
char unit      = 0; // 1: Unit ok, 0: Unit not ok

char lightOn    = 0; // LED on or off
char six_zero   = 0; // 0: not six zeros, 1: six zeros active, begin look for 2. part
char full_start = 0; // needs a 1 to begin

char counting_zeros = 0;

int twice      = 0; // needs to check twice

// Navigation
int arrIndex_ = 0; // Positon in array
int cnt       = 0; // 0 = 1. address, 1 = 2. address, 2 = 1. command, 3 = 2. command
int cmd       = 0; // Which command should be executed

int zero_count = 0;

/* COMMANDS AND ADDRESSES - SET BY USER */
int set_X10_Address[SIZE]   = {1,1,1,0,0,0,0,1,0,0,0,1,0,1};
int set_X10_Cammond_1[SIZE] = {1,1,1,0,0,0,0,1,0,0,0,1,1,0};
int set_X10_Cammond_2[SIZE] = {1,1,1,0,0,0,0,1,0,0,1,0,1,0};

// Input Array
int X10_IN_[SIZE];


int main(void)
{
  // INIT STUFF
  InitZerocrossInterrupt();
  initPorts();
  initLight();
  restart_check();

  DDRB = 0xFF;
  PORTB = 0;
  DDRA = 0;

  sei();
  while(1) {}
}


// INTERRUPT, READ
ISR(INT0_vect)
{
	_delay_us(12); // DELAY MUST BE ATLEAST 12 MICROS SECONDS TO DETECT X10 SIGNAL (WITH PERFECT SQUARE SIGNAL FROM ANALOG DISCOVERY)
  if(((PIND & 0b10000000) == 0b10000000) && (arrIndex_ == 0))
  { full_start = 1; }
  else if (cnt == 2 && six_zero == 0)
  {
	counting_zeros = 1;
    zero_count++;
    if(zero_count == 6)
      {six_zero = 1; counting_zeros = 0;}
  } else { }

  if(full_start == 1)
  {
	  if(counting_zeros == 1)
	  {
		  //if(cnt == 2) {PORTB |= 0b10000000;}
		  restart_check();
	  }
	  
    
    if((PIND & 0b10000000) == 0b10000000)
      { X10_IN_[arrIndex_] = 1; }
    else
      { X10_IN_[arrIndex_] = 0; }

    arrIndex_++;
    
	check_input();
 
    if(six_zero == 1 && cnt >= 2) { check_command(); }
		

    if(arrIndex_ >= SIZE)
    {
      arrIndex_ = 0;
      full_start = 0;
    }
  }
}


void check_input()
{
	
  // START-CODE
  if(arrIndex_ == 4)
  {
    if(comp_X10(set_X10_Address, X10_IN_, 0, arrIndex_) == 1)
      { start = 1; }
    else
      { restart_check(); }
  }

  // HOUSE-CODE
  else if(start == 1 && (arrIndex_ == 8))
  {
    if(comp_X10(set_X10_Address, X10_IN_, 4, arrIndex_) == 1)
      { houseCode = 1;  }
    else
      { restart_check(); }
  }

  // UNIT-CODE (IF ADDRESS)
  else if((houseCode == 1 && (arrIndex_ == 12)) && (cnt == 0 || cnt == 1))
  {
    if(comp_X10(set_X10_Address, X10_IN_, 8, arrIndex_) == 1)
      { unit = 1; }
	else
	{ restart_check(); }
  }

  // SUFFIX-CODE
  else if(unit == 1 && arrIndex_ == 14 && cnt < 2)
  {
    if(comp_X10(set_X10_Address, X10_IN_, 12, arrIndex_) == 1)
      { cnt++; }
	else	
    { restart_check(); }
  }
}



void check_command()
{
	
  // COMMAND-CODE (IF COMMAND)
  if((houseCode == 1 && (arrIndex_ == 12)) && (cnt == 2 || cnt == 3) )
    {
      // Check Command 1
      if(comp_X10(set_X10_Cammond_1, X10_IN_, 8, arrIndex_) == 1)
      {
        if(cmd == 0 || cmd == 1)
        {
          cmd = 1;
        }
      }
      // Check Command 2
      else if(comp_X10(set_X10_Cammond_2, X10_IN_, 8, arrIndex_) == 1)
      {
        if(cmd == 0 || cmd == 2)
        {
          cmd = 2;
        }
      }
      // Restart
      else { restart_check(); }
    }

  // SUFFIX-CODE
  if(arrIndex_ == 14)
    {
      if(comp_X10(set_X10_Cammond_1, X10_IN_, 12, arrIndex_) == 1)
        {cnt++; }
      else
        { restart_check(); }
    }

  if(cnt == 4)
    {
      // CALL COMMAND
      if(cmd == 1)
      {
        if(lightOn == 0)
        {
          onLight();
		  PORTB |= 0b00000001;
          lightOn = 1;
        }
        else
        {
          offLight();
		  PORTB = 0;
          lightOn = 0;
        }
      }
      else if(cmd == 2)
      {
        // OTHER COMMAND
		PORTB |= 0b00000010;
      }
      restart_check();
    }
}

void restart_check()
{
  start      = 0;
  houseCode  = 0;
  unit       = 0;
  cmd        = 0;
  cnt        = 0;
  arrIndex_  = 0;
  full_start = 0;
  zero_count = 0;
  six_zero   = 0;
  twice      = 0;
  counting_zeros = 0;
}
