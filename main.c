#include <proc/pic18f4520.h>
#include "atraso.h"
#include "lcd.h"
#include "ssd.h"
#include "keypad.h"
#include "config.h"
#include "serial.h"

#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0


void portempolivre(){
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("MODO TEMPO");
}


void porpeso(){
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("MODO PESO");
}

void descongela(){
    unsigned char desc;
    int i;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("MODO DESCONGELAR");
    atraso_ms(150);
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("(1)Por Peso");
    lcdCommand(L_L2);
    lcdString("(2)Por Tempo");
    
    serial_tx_str(" Digite a opcao de descongelamento:\r\n");
    
    for (i = 0; i < 2; i++) {
        
        desc = serial_rx(2000);
        serial_tx(desc);
        
        switch(desc){
            case '1':
                porpeso();
                break;
            case '2':
                portempolivre();
                break;
                        
        }
        
    }
}



void main(void) {
    unsigned char i;
    unsigned char op, data[12];
    
    TRISA = 0xC3;
    TRISB = 0x03;
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    ADCON1 = 0x06;
    
    lcdInit();
    serial_init();
    ssdInit();
    kpInit();
    
    
    TRISCbits.TRISC7 = 1; 
    TRISCbits.TRISC6 = 0; 
    PORTB = 0;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("(1) Pipoca");
    lcdCommand(L_L2);
    lcdString("(2) Descongelar");
    
    serial_tx_str(" Digite a opcao:\r\n");
    
    for (i = 0; i < 2; i++) {
        //if (!(i % 16)) {
           // serial_tx_str("\r\n");
        //}
        
        op = serial_rx(2000);
        data[i] = op;
        serial_tx(op);
        
        switch(op){
            case '1':
                lcdCommand(L_CLR);
                lcdCommand(L_L1);
                lcdString("MODO PIPOCA");
                //crono();
                break;
            case '2':
                descongela();
                break;
                        
        }
        
    }
    
    
    
    atraso_ms(100);
    while (1);
}


