/*
 * Challenge2.c
 *
 *  Created on: Oct 4, 2023
 *      Author: omars
 */

#include "keypad.h"
#include "lcd.h"
#include <util/delay.h>
#include <stdlib.h>
#include "Standerd_Types_OS.h"

int main(void){
	LCD_init();
	uint8 vote1=0,vote2=0,vote3=0,vote4=0,key;
	LCD_displayString("**VotingSystem**");
	LCD_moveCursor(1,0);
	LCD_displayString("By: Omar Shaaban");
	_delay_ms(5000);
	LCD_sendCommand(CLEAR_DISP);
	while(1){
		LCD_moveCursor(0,0);
		LCD_displayString("Cand1=");
		LCD_intgerToString(vote1);

		LCD_moveCursor(0,8);
		LCD_displayString("Cand2=");
		LCD_intgerToString(vote2);

		LCD_moveCursor(1,0);
		LCD_displayString("Cand3=");
		LCD_intgerToString(vote3);

		LCD_moveCursor(1,8);
		LCD_displayString("Cand4=");
		LCD_intgerToString(vote4);
		key=KEYPAD_getPressedKey();
		switch(key){
		case 1:
			vote1++;
			break;
		case 2:
			vote2++;
			break;
		case 3:
			vote3++;
			break;
		case 4:
			vote4++;
			break;
		case 13:
			LCD_sendCommand(CLEAR_DISP);
			if(vote1>vote2 && vote1>vote3 && vote1>vote4){
				LCD_displayString("Cand1 is");
				LCD_moveCursor(1,0);
				LCD_displayString("the winner!!");
			}
			else if(vote2>vote1 && vote2>vote3 && vote2>vote4){
				LCD_displayString("Cand2 is");
				LCD_moveCursor(1,0);
				LCD_displayString("the winner!!");
			}
			else if(vote3>vote2 && vote3>vote1 && vote3>vote4){
				LCD_displayString("Cand3 is");
				LCD_moveCursor(1,0);
				LCD_displayString("the winner!!");
			}
			else if(vote4>vote2 && vote4>vote3 && vote4>vote1){
				LCD_displayString("Cand4 is");
				LCD_moveCursor(1,0);
				LCD_displayString("the winner!!");
			}
			_delay_ms(5000);
			LCD_sendCommand(CLEAR_DISP);
			vote1=0,vote2=0,vote3=0,vote4=0;
			break;
		}
	}
}
