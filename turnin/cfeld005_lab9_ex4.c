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
enum SM4_States {wait, inc, holdInc, dec, holdDec} SM4_State;

unsigned char tmpB;
unsigned char frequency;

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

void TickFCT_Sound(){
  switch (SM3_State) {
    case on:
      SM3_State = off;
      break;
    case off:
      SM3_State = on;
      break;
  }
}

void TickFCT_FrequencyAdjust(unsigned char tmpA){
  switch (SM4_State) {
    case wait:
      if (tmpA == 0x01){
        SM4_State = inc;
      } else if (tmpA == 0x02){
        SM4_State = dec;
      } else {
        SM4_State = wait;
      }
      break;
    case inc:
      SM4_State = holdInc;
      break;
    case holdInc:
      if (tmpA == 0x01){
        SM4_State = holdInc;
      } else if (tmpA == 0x02){
        SM4_State = dec;
      } else {
        SM4_State = wait;
      }
      break;
    case dec:
      SM4_State = holdDec;
      break;
    case holdDec:
      if (tmpA == 0x01){
        SM4_State = inc;
      } else if (tmpA == 0x02){
        SM4_State = holdDec;
      } else {
        SM4_State = wait;
      }
      break;
  }

  switch (SM4_State) {
    case inc:
      if (frequency < 255){
        frequency++;
      }
      break;
    case dec:
      if (frequency > 1) {
        frequency--;
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
    frequency = 0x02;

    SM1_State = l1;

    unsigned short c1 = 300;
    unsigned short c2 = 1000;
    unsigned short c3 = 2;

    unsigned char tmpA = 0x00;

    while (1) {
      tmpA = ~PINA;

      TickFCT_FrequencyAdjust(tmpA);

      if (c1 == 300){
        TickFCT_ThreeLEDsSM();
        c1 = 0;
      }

      if (c2 == 1000){
        TickFCT_BlinkingLEDSM();
        c2 = 0;
      }

      if (tmpA & 0x04){
        if (c3 >= frequency){
          TickFCT_Sound();
          c3 = 0;
        }
        c3++;
      }

      TickFCT_CombineLEDsSM();

      c1++;
      c2++;

      PORTB = tmpB;

      while(!TimerFlag);
      TimerFlag=0;

    }
    return 1;
}
