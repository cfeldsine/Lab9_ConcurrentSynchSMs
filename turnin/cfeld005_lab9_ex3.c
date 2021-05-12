/*	Author: lab
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif


enum SM1_States {l1, l2, l3} SM1_State;
enum SM2_States {on, off} SM2_State, SM3_State;

unsigned char tmpB;

void TickFCT_ThreeLEDsSM(){
  switch(SM1_State){
    case l1:
      SM1_State = l2;
      break;
    case l2:
      SM1_State = l3;
      break;
    case l3:
      SM1_State = l1;
      break;
  }
}

void TickFCT_BlinkingLEDSM(){
  switch(SM2_State) {
    case on:
      SM2_State = off;
      break;
    case off:
      SM2_State = on;
      break;
  }
}

void TickFCT_Sound(unsigned char tmpA){
  switch (SM3_State) {
    case on:
      if (tmpA & 0x04){
        SM3_State = off;
      }
      break;
    case off:
      if (tmpA & 0x04){
        SM3_State = on;
      }
      break;
  }
}

void TickFCT_CombineLEDsSM(){
  tmpB = 0x00;
  switch (SM1_State) {
    case l1:
      tmpB = tmpB | 0x01;
      break;
    case l2:
      tmpB = tmpB | 0x02;
      break;
    case l3:
      tmpB = tmpB | 0x04;
      break;
  }

  switch(SM2_State) {
    case on:
      tmpB = tmpB | 0x08;
      break;
    case off:
      break;
  }

  switch(SM3_State) {
    case on:
      tmpB = tmpB | 0x10;
      break;
    case off:
      break;
  }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(1);
    TimerOn();

    tmpB = 0x00;

    SM1_State = l1;

    unsigned short c1 = 300;
    unsigned short c2 = 1000;
    unsigned short c3 = 2;

    unsigned char tmpA = 0x00;

    while (1) {
      tmpA = ~PINA;

      if (c1 == 300){
        TickFCT_ThreeLEDsSM();
        c1 = 0;
      }
      if (c2 == 1000){
        TickFCT_BlinkingLEDSM();
        c2 = 0;
      }


      if (c3 == 2){
        TickFCT_Sound(tmpA);
        c3 = 0;
      }
      c3++;


      TickFCT_CombineLEDsSM();

      c1++;
      c2++;

      PORTB = tmpB;

      while(!TimerFlag);
      TimerFlag=0;

    }
    return 1;
}
