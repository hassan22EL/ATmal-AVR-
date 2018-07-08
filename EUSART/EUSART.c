/*
 * EUSART.c
 *
 * Created: 7/7/2018 7:25:31 PM
 * Author : hassa
 */ 

#include <avr/io.h>
#define  F_CPU 16000000UL
#include "EUSART.h"
#include <math.h>
/* Replace with your library code */
void EUSART_INIT()
{
	uint16_t UBBRValue = lrint(( F_CPU / (16UL * EUSART.baud) ) - 1);  
	if (EUSART.AsyncDoubleSpeed == 1) UCSRA = (1 << U2X); //setting the U2X bit to 1 for double speed asynchronous
	//Put the upper part of the baud number here (bits 8 to 11)
	UBRRH = (unsigned char) (UBBRValue >> 8);
	//Put the remaining part of the baud number here
	UBRRL = (unsigned char) UBBRValue;
	//Enable the receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
	if (EUSART.USARTInterruptEnable) UCSRB |= (1 << RXCIE); //Enable interrupt 
	//Set 2 stop bits
	if (EUSART.StopBits == 2) UCSRC = (1 << USBS);
	if (EUSART.ParityEVENorODD == EVEN) UCSRC |= (1 << UPM1); //Sets parity to EVEN
	if (EUSART.ParityEVENorODD == ODD) UCSRC |= (3 << UPM0); //Alternative way to set parity to ODD
	if (EUSART.DataSizeInBits == 6) UCSRC |= (1 << UCSZ0); //6-bit data length
	if (EUSART.DataSizeInBits == 7) UCSRC |= (2 << UCSZ0); //7-bit data length
	if (EUSART.DataSizeInBits == 8) UCSRC |= (3 << UCSZ0); //8-bit data length
	if (EUSART.DataSizeInBits == 9) UCSRC |= (7 << UCSZ0); //9-bit data length
}

void EUSART_Write(unsigned char data){
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}
char EUSART_TX_Empty(){
	return UDRE ;
}
void EUSART_Write_Text(char *text){
	  int i;
	  for(i=0;text[i]!='\0';i++)
	  EUSART_Write(text[i]);
}
char EUSART_Data_Ready(){
return RXC;	
} 
char EUSART_Read(){
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) ) ;
	/* Get and return received data from buffer */
	return UDR;
}
void EUSART_Read_Text(char *Output, unsigned int length){
	  for(int i=0;i<length;i++)
	  Output[i] = EUSART_Read();
}
void EUSART_Transmit( unsigned int data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)));
	/* Copy 9th bit to TXB8 */
	UCSRB &=~(1<<TXB8);
	if ( data & 0x0100 )
	UCSRB |= (1<<TXB8);
	/* Put data into buffer, sends the data */
	UDR = data;
}
unsigned int EUSART_Receive( void )
{
	unsigned char status, resh, resl;
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get status and 9th bit, then data */
	/* from buffer */
	status = UCSRA;
	resh = UCSRB;
	resl = UDR;
	/* If error, return -1 */
	if ( status & (1<<FE)|(1<<DOR)|(1<<PE) )
	return -1;
	/* Filter the 9th bit, then return */
	resh = (resh >> 1) & 0x01; //store nine bit in First bit in Register UCSRB (TXB8)
	return ((resh << 8) | resl); //shift Left 8 bit in UCSRB and OR with UDR (8 bit Data)
}