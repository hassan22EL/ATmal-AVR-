/*
* EUSART.h
*
* Created: 7/7/2018 7:27:00 PM
*  Author: hassa
*/


#ifndef EUSART_H_
#define EUSART_H_
#define  EVEN   0
#define  ODD    1
#define  NONE   2
#define  False  0
#define  True   1
typedef union {
    int baud ;
	char AsyncDoubleSpeed;
	char DataSizeInBits;
	char ParityEVENorODD;
	char StopBits;
	char USARTInterruptEnable ;
}EUSART_t;
volatile EUSART_t EUSART ;
void EUSART_INIT();
void EUSART_Write(unsigned char data);
char EUSART_TX_Empty();
void EUSART_Write_Text(char *text);
char EUSART_Data_Ready();
char EUSART_Read();
void EUSART_Read_Text(char *Output, unsigned int length);
void EUSART_Transmit( unsigned int data );
unsigned int EUSART_Receive( void );
#endif /* EUSART_H_ */