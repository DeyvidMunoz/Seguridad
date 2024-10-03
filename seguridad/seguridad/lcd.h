/*!
\file   CONFIG.h
\date   2022-06-08
\author Fulvio Vivas <fyvivas@unicauca.edu.co>
\brief  Configuration bits.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2022. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

   
#define RS PORTCbits.RC0  /*PIN 0 of PORTC is assigned for register select Pin of LCD*/
#define EN PORTCbits.RC1  /*PIN 1 of PORTC is assigned for enable Pin of LCD */
#define ldata PORTC  /*PORTD(PD4-PD7) is assigned for LCD Data Output*/ 
#define LCD_Port TRISC  /*define macros for PORTD Direction Register*/
    
void LCD_Init(void);                   /*Initialize LCD*/
void LCD_Command(unsigned char );  /*Send command to LCD*/
void LCD_Char(unsigned char x);    /*Send data to LCD*/
void LCD_String(const char *);     /*Display data string on LCD*/
void LCD_String_xy(char, char , const char *);
void LCD_Clear(void);                  /*Clear LCD Screen*/


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

