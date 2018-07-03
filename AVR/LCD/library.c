/*
 * LCD.c
 *
 * Created: 7/3/2018 4:57:02 PM
 * Author : hassa
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "LCD.h"
void LCD_INIT(){
		DDRB |=(1<<PB0); //RS pin as output
		DDRB |=(1<<PB1); //EN pin as output
		DDRB |=(1<<PB4); //D4 pin as output
		DDRB |=(1<<PB5); //D5  pin as output
		DDRB |=(1<<PB6); //D6 pin as output
		DDRB |=(1<<PB7); //D7 pin as output
	LCD_Command(0x33); // function  set 8bit mode interface width=8bit and 5*8 dot format and 2 line on
	LCD_Command(0x32); // function set 8bit mode again interface width=8bit and 5*8 dot format and 2 line on
	LCD_Command(0x28); // function set 4bit mode (interface width=4bit and 5*8 dot format and 2 line on)
	LCD_Command(0x0C); // Display on and  cursor oFF and blink off
	LCD_Command(0x06); // Address counter (AC) Automatic increment and no shift
}

void LCD_Command(char Command){
LCD_port = (LCD_port & 0x0F) |  (Command &0xF0) ; //send upper nibble
LCD_port &=~(1<<RS); //Rest Rs (Rs=zero) 
LCD_port |=(1<<EN); //set EN= 1 
_delay_ms(5);
LCD_port &=~(1<<EN); //Rest EN=zero
LCD_port= (LCD_port & 0x0F) |(Command <<4); //send Lower nibble 
LCD_port |=(1<<EN); //set EN= 1
_delay_ms(5);
LCD_port &=~(1<<EN); //Rest EN=zero
}
void LCD_Write_Data(char Data){
LCD_port = (LCD_port & 0x0F) |  (Data &0xF0) ; //send upper nibble
LCD_port |=(1<<RS); //Rest Rs (Rs=zero)
LCD_port |=(1<<EN); //set EN= 1
_delay_us(20);
LCD_port &=~(1<<EN); //Rest EN=zero
LCD_port= (LCD_port & 0x0F) |(Data <<4); //send Lower nibble
LCD_port |=(1<<EN); //set EN= 1
LCD_port &=~(1<<EN); //Rest EN=zero

}
void LCD_CLEAR(){
	
	LCD_Command(0x01);  // this command clear display because D0=1 and D2 To D7 equal zero
}
void LCD_Set_cursor(char x, char y){
	  char position; // POsitin char 
    if (x == 1) {
        position = 0x80 + y - 1;
        LCD_Command(position); // write the command into LCD selected address cursor
        /*  x = number of line 
         * at y=0 the the cursor at first column  and start
         *  address DDRAM =00H and End address 27H First line 
         * and B7=1 selected DDRAM address into LCD 0xC0 sum of operation
         * this low 0x80 +y-1 find the position of Cursor in first line 
         */
    } else if (x == 2) {
        position = 0xC0 + y - 1;
        LCD_Command(position); // write the command into LCD selected address cursor
        /* x = number of line 
         *  at y=0 the the cursor at first column  and start
         *  address DDRAM =40H and End address 67H sec line 
         * and B7=1 selected DDRAM address into LCD  0xC0 sum of operation
         * this low 0xC0 +y-1 find the position of Cursor in sec line 
         */


    }
}
void LCD_Write_String(char *s){
	
	    /*identifier pointer working counter address into RAM MUC 
     * each char its address the pointer point the current address
     * such as  the s pointer each address char
     * and method LCD write char take the char into LCD 
     
     */
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        LCD_Write_Data(s[i]);
    }
}
/* shifted display and cursor 
 * B3 B2    sate
 * 0  0    cursor shift left
 * 0  1    cursor shift right
 * 1  0    cursor and display shifted left
 * 1  1    cursor and display shifted right
 */
void LCD_Shift_Right(){
	   LCD_Command(0x1C); //    cursor and display shifted right
}
void LCD_Shift_left(){
	 LCD_Command(0x18); //cursor and display shifted Left
}
void LCD_Write_INT_NUMBER(int value){
	  // Convert integer number to String
	  unsigned char buffer[8]; // buffer size 8 bit
	  LCD_Write_String(itoa (buffer, value, 10));

}
char *	ftoa(float f, int * status){
	static char		buf[17];
	char *			cp = buf;
	unsigned long	l, rem;

	if(f < 0) {
		*cp++ = '-';
		f = -f;
	}
	l = (unsigned long)f;
	f -= (float)l;
	rem = (unsigned long)(f * 1e6);
	sprintf(cp, "%lu.%6.6lu", l, rem);
	return buf;
}
void LCD_Write_Float_Number(float valu){
	   char *buf;
	   int status;
	   buf = ftoa(valu, &status);
	   LCD_Write_String(buf);
}

