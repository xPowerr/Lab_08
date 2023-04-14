/*
 * File:   newmain.c
 * Author: Gabriel Carrera 21216
 *
 * Created on April 14, 2023, 10:34 AM
 */

#define _XTAL_FREQ 8000000

//variables
const char data = 'A'; // Mostrar la literal A en la terminal

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>


// --------------- Rutina de  interrupciones --------------- 
void __interrupt() isr(void){
    // Recepcion de datos
    if(PIR1bits.RCIF) // Bandera de interrucpcion de eusart
    {
        PORTB = RCREG; //Revisar RCreg y lo guardarlo
    }
}

// --------------- Prototipos --------------- 
void setup(void);
void setupUART(void);

// --------------- main --------------- 
void main(void){
    setup();
    setupUART();
    
    // Envio de datos
    while (1){
        __delay_ms(500);
        
        if (PIR1bits.TXIF){ // Si txif esta disponible 
            TXREG = data; // Enviar el valor de data
        }        
    }       
}


// --------------- Setup General --------------- 
void setup(void){
    // --------------- Definir analogicas --------------- 
    ANSEL = 0; 
    ANSELH = 0;
    
    TRISB = 0; // PORTB como salida
    PORTB = 0; // Iniciar PORTB
    
// --------------- Oscilador --------------- 
    OSCCONbits.IRCF = 0b111; // 8 MHz
    OSCCONbits.SCS = 1; // Seleccionar oscilador interno

    // --------------- Banderas e interrupciones ---------------
    PIR1bits.RCIF = 0; // Bandera de la recepcion
    PIE1bits.RCIE = 1; // Interrupcion de la recepcion 
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
}
    
void setupUART(void){
   // Paso 1: Configurar velocidad baud rate
    
    SPBRG = 12;                 // Valor para 9600 de baud rate
    
    // Paso 2:
    
    TXSTAbits.SYNC = 0;         // Modo Asíncrono
    RCSTAbits.SPEN = 1;         // Habilitar UART
    
    // Paso 3:
    // Usar 8 bits
    
    // Paso 4:
    TXSTAbits.TXEN = 1;         // Habilitamos la transmision
    PIR1bits.TXIF = 0;
    RCSTAbits.CREN = 1;         // Habilitamos la recepcion 
}    

