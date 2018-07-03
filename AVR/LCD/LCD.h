/*
 * LCD.h
 *
 * Created: 7/3/2018 4:58:34 PM
 *  Author: hassa
 */ 


#ifndef LCD_H_
#define LCD_H_

#define   RS   PB0
#define   EN   PB1
#define   LCD_port PORTB
void LCD_INIT();
void LCD_Command(char Command);
void LCD_Write_Data(char Data);
void LCD_CLEAR();
void LCD_Set_cursor(char x, char y);
void LCD_Write_String(char *s);
void LCD_Shift_Right();
void LCD_Shift_left();
void LCD_Write_INT_NUMBER(int value);
char *	ftoa(float f, int * status);
void LCD_Write_Float_Number(float valu);



#endif /* LCD_H_ */