/*!
\file   seguridad.c
\date   2022-06-28
\brief  Security House.

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
#include <xc.h>
#include <pic16f887.h>  // Header file PIC16f887 definitions
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "lcd.h"
#include "keypad.h"

// Definición de los pines de los LEDs
#define LED_VERDE PORTAbits.RA0
#define LED_AZUL PORTAbits.RA1
#define LED_ROJO PORTAbits.RA2

const char password[8] = {'1','1','1','1','6','0', 0, 0}; // Contraseña predefinida
char pass_user[8];
unsigned char idx = 0;
int intentos = 0;  // Contador de intentos fallidos

void delay_ms_variable(unsigned int ms) {
    while (ms--) {
        __delay_ms(1);  // Llamada a __delay_ms con un valor constante
    }
}

// Prototipos de funciones
void blink_led(unsigned char led, unsigned int on_time, unsigned int off_time, unsigned int duration) {
    unsigned int elapsed_time = 0;

    while (elapsed_time < duration) {
        if (led == 'V') {
            LED_VERDE = 1;  // Enciende LED verde
        } else if (led == 'A') {
            LED_AZUL = 1;   // Enciende LED azul
        } else if (led == 'R') {
            LED_ROJO = 1;   // Enciende LED rojo
        }
        delay_ms_variable(on_time);

        // Apagar LED
        if (led == 'V') {
            LED_VERDE = 0;
        } else if (led == 'A') {
            LED_AZUL = 0;
        } else if (led == 'R') {
            LED_ROJO = 0;
        }
        delay_ms_variable(off_time);

        elapsed_time += on_time + off_time;
    }
}

// Definición de los pines de los LEDs
#define LED_VERDE PORTAbits.RA0  // LED verde conectado a RA0
#define LED_AZUL PORTAbits.RA1   // LED azul conectado a RA1
#define LED_ROJO PORTAbits.RA2   // LED rojo conectado a RA2

void main() {
    char key = '0';
    // Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71;  // Configura oscilador interno (FOSC = 8Mhz)
    
    TRISA = 0b11111000;   // Configura RA0, RA1, RA2 como salidas (bits 0, 1 y 2 en 0)
    ANSELH = 0x00;        // Configura ANSELH (puedes ajustar según tu aplicación)
    ANSEL = 0x00;    
    
    PORTA=0x00;// Configura ANSEL (puedes ajustar según tu aplicación)
    // Agrega un retardo antes de mostrar el mensaje de bienvenida
    __delay_ms(10000);  // Espera 1 segundo
    
    LCD_Init();     // Inicializa el LCD
    keypad_init();  // Inicializa el keypad
    
    // Mostrar mensaje de bienvenida
    LCD_Clear();
    LCD_String_xy(0, 0, "Bienvenido");  // Muestra "Bienvenido" en la primera fila
    __delay_ms(2000);  // Espera 2 segundos para que el usuario vea el mensaje

    while (1) {
        LCD_Clear();
        LCD_String_xy(0, 0, "Press a Key");  // Muestra "Press a Key" en la primera fila
        LCD_Command(0xC0);
        // Captura la clave del usuario
        while (idx <7) {  // Continúa hasta que se presione '#'
            
            key = keypad_getkey();
            if (key != 0) {
                if (key == '#') {
                    // Usuario presionó '#' para terminar la entrada
                    pass_user[idx] = '\0';  // Finaliza la cadena
                    break;  // Sale del bucle de ingreso
                } else {
                    // Crear una cadena temporal para almacenar el carácter numérico
                    char temp_str[2];  
                    temp_str[0] = key;  // Asigna el número a la cadena como carácter
                    temp_str[1] = '\0'; // Termina la cadena con un carácter nulo

                    //LCD_String_xy(1, idx, temp_str);  // Muestra '*' en la segunda fila
                    LCD_Char('*');
                    pass_user[idx++] = key;  // Guarda la clave ingresada en el array
                    //if (idx >= 6) {  // Limitar a 6 caracteres
                    //    break;  // Si la clave es demasiado larga, salir también
                    //}
                }
              }
            __delay_ms(100);
        }

        // Comparar la clave ingresada con la clave predefinida
        if (strncmp(pass_user, password, 6) == 0) {
            LCD_Clear();
            LCD_String_xy(0, 0, "Clave Correcta");
            blink_led('V', 500, 500, 3000);  // Parpadear LED verde por 3 segundos
            intentos = 0;  // Reiniciar intentos
        } else {
            // Contraseña incorrecta
            intentos++;
            LCD_Clear();
            LCD_String_xy(0, 0, "Clave Incorrecta");
            blink_led('A', 300, 700, 2000);  // Parpadear LED azul por 2 segundos

            // Si el usuario falla 3 veces, bloquear el sistema
            if (intentos >= 3) {
                LCD_Clear();
                LCD_String_xy(0, 0, "Sistema Bloqueado");
                blink_led('R', 300, 500, 8000);  // Parpadear LED rojo por 8 segundos
                intentos = 0;  // Reiniciar intentos después del bloqueo
            }
        }

        // Reinicia las variables para el siguiente intento
        __delay_ms(2000);
        LCD_Clear();
        idx = 0;  // Reinicia el índice de la clave ingresada
    }
}
