/*
 * lcd.c
 *
 *  Created on: Sep 30, 2023
 *      Author: omars
 */
#include "gpio.h"
#include "lcd.h"
#include <util/delay.h>
#include "Common_Macros_OS.h"
#include <stdlib.h>

void LCD_sendCommand (uint8 command){
#if (LCD_DATA_BITS_MODE==8)
	GPIO_writePin(RS_PORT_ID,RS_PIN_ID,LOW);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,HIGH);
	_delay_ms(1);
	GPIO_writePort(DATA_PORT_ID,command);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOW);
	_delay_ms(1);
#elif(LCD_DATA_BITS_MODE==4)
	GPIO_writePin(RS_PORT_ID,RS_PIN_ID,LOW);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,HIGH);
	_delay_ms(1);
	GPIO_writePin(DATA_PORT_ID,FIRST_DATA_PIN,GET_BIT(command,PIN4_ID));
	GPIO_writePin(DATA_PORT_ID,SECONED_DATA_PIN,GET_BIT(command,PIN5_ID));
	GPIO_writePin(DATA_PORT_ID,THIRD_DATA_PIN,GET_BIT(command,PIN6_ID));
	GPIO_writePin(DATA_PORT_ID,FOURTH_DATA_PIN,GET_BIT(command,PIN7_ID));
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOW);
	_delay_ms(1);

	GPIO_writePin(RS_PORT_ID,RS_PIN_ID,LOW);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,HIGH);
	_delay_ms(1);
	GPIO_writePin(DATA_PORT_ID,FIRST_DATA_PIN,GET_BIT(command,PIN0_ID));
	GPIO_writePin(DATA_PORT_ID,SECONED_DATA_PIN,GET_BIT(command,PIN1_ID));
	GPIO_writePin(DATA_PORT_ID,THIRD_DATA_PIN,GET_BIT(command,PIN2_ID));
	GPIO_writePin(DATA_PORT_ID,FOURTH_DATA_PIN,GET_BIT(command,PIN3_ID));
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOW);
	_delay_ms(1);

#endif
}

void LCD_displayCharacter(uint8 data){
#if (LCD_DATA_BITS_MODE==8)
	GPIO_writePin(RS_PORT_ID, RS_PIN_ID, HIGH);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID, E_PIN_ID, HIGH);
	_delay_ms(1);
	GPIO_writePort(DATA_PORT_ID,data);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID, E_PIN_ID, LOW);
	_delay_ms(1);
#elif(LCD_DATA_BITS_MODE==4)
	GPIO_writePin(RS_PORT_ID,RS_PIN_ID,HIGH);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,HIGH);
	_delay_ms(1);
	GPIO_writePin(DATA_PORT_ID,FIRST_DATA_PIN,GET_BIT(data,PIN4_ID));
	GPIO_writePin(DATA_PORT_ID,SECONED_DATA_PIN,GET_BIT(data,PIN5_ID));
	GPIO_writePin(DATA_PORT_ID,THIRD_DATA_PIN,GET_BIT(data,PIN6_ID));
	GPIO_writePin(DATA_PORT_ID,FOURTH_DATA_PIN,GET_BIT(data,PIN7_ID));
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOW);
	_delay_ms(1);
	GPIO_writePin(RS_PORT_ID,RS_PIN_ID,HIGH);
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,HIGH);
	_delay_ms(1);
	GPIO_writePin(DATA_PORT_ID,FIRST_DATA_PIN,GET_BIT(data,PIN0_ID));
	GPIO_writePin(DATA_PORT_ID,SECONED_DATA_PIN,GET_BIT(data,PIN1_ID));
	GPIO_writePin(DATA_PORT_ID,THIRD_DATA_PIN,GET_BIT(data,PIN2_ID));
	GPIO_writePin(DATA_PORT_ID,FOURTH_DATA_PIN,GET_BIT(data,PIN3_ID));
	_delay_ms(1);
	GPIO_writePin(E_PORT_ID,E_PIN_ID,LOW);
	_delay_ms(1);

#endif
}

void LCD_init(void){

	GPIO_setupPinDirection(RS_PORT_ID,RS_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(E_PORT_ID,E_PIN_ID,PIN_OUTPUT);
#if (LCD_DATA_BITS_MODE==8)
	GPIO_setupPortDirection(DATA_PORT_ID,PORT_OUTPUT);
	_delay_ms(20); /*الوقت الي بتاخده ال ال سي دي عشان تبدأتشتغل*/
	LCD_sendCommand(FUNC_8BIT_2LINE);
#elif(LCD_DATA_BITS_MODE==4)
	GPIO_setupPinDirection(DATA_PORT_ID,FIRST_DATA_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(DATA_PORT_ID,SECONED_DATA_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(DATA_PORT_ID,THIRD_DATA_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(DATA_PORT_ID,FOURTH_DATA_PIN,PIN_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1); /*الداتا شيت بتقول كدا*/
	LCD_sendCommand( LCD_TWO_LINES_FOUR_BITS_MODE_INIT2); /*الداتا شيت بتقول كدا*/
	LCD_sendCommand(FUNC_4BIT_2LINE);
#endif

	LCD_sendCommand(DISPLAYON_CURSOROFF);
	LCD_sendCommand(CLEAR_DISP);
}

void LCD_displayString(uint8 * str){
	uint8 counter=0;
	while(str[counter]!='\0'){
		LCD_displayCharacter(str[counter]);
		counter++;
	}

}

void LCD_moveCursor(uint8 row ,uint8 col){
	uint8 address;
	uint8 position;
	switch(row){
	case 0:
		address=col;
		break;
	case 1:
		address=col+0x40;
		break;
	case 2:
		address=col+0x10;
		break;
	case 3:
		address=col+0x50;
		break;
	}
	position= (address | (1<<7));/*عشان اقول للكرسور اتحرك علي المكان ده دايما بخلي البيت التامنة بواحد وكدا كدا الادريس عندي سبعة بيت بس*/
	LCD_sendCommand(position);
}

void LCD_displayStringRowColumn(uint8 row,uint8 col,uint8 *str){
	LCD_moveCursor(row ,col);
	LCD_displayString(str);
}

void LCD_clearScreen(void){
	LCD_sendCommand(CLEAR_DISP);
}

void LCD_intgerToString(int data){
	uint8 buff[16];
	itoa(data,buff,10);
	LCD_displayString(buff);
}
