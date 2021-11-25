#include <proc/pic18f4520.h>
#include "atraso.h"
#include "lcd.h"
#include "ssd.h"
#include "keypad.h"
#include "config.h"
#include "serial.h"
#include "bits.h"

#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0



void crono(int t){
    int min;
    
    min = t/60;
    
    t = t*100;
    
    ssdInit();
    
    TRISD = 0x00;

    
    while(t != 0) {
        
        ssdDigit(((min / 1) % 10), 1);
        ssdDigit(((min / 10) % 10), 0);
        
        ssdDigit(((t / 100) % 10), 3); 
        ssdDigit(((t / 1000) % 6), 2); 
        
        if(((t/1000)%6 == 0)&&((t/100)%10 == 0)&&((t/10)%10 == 0)&&((t/1)%10 == 0)){
            min--;
        }
          
        
        t--;
        ssdUpdate();
        atraso_ms(1);
    }
    
    PORTD = 0x0A;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("PRONTO!");
}

void portempolivre(){
    unsigned int tecla = 16;
    char tec1 = 0, tec2 = 0;
    int min = 0, seg = 0;
    int tempo, m, s;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("MODO TEMPO");
    atraso_ms(200);
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Altere o Tempo");
    
    serial_tx_str(" Altere o tempo usando as teclas.\r\n");
    serial_tx_str(" Use d3 e d5 para 1 min \r\n");
    serial_tx_str(" Use d7 para finalizar:\r\n");
    
    while(tec1 == 0){
        kpDebounce();
        
        if(kpRead() != tecla){
            tecla = kpRead();
            if(bitTst(tecla,0)){
                min++;
                serial_tx(min);
               
                
            }
            if(bitTst(tecla,2)){
                min--;
                serial_tx(min);
                
            }
            if(bitTst(tecla,4)){
                tec1 = 1;
                serial_tx_str(" Minutos alterados\r\n");
            }  
        }
    }
    
    
    serial_tx_str(" Use d3 e d5 para 10 segundos \r\n");
    serial_tx_str(" Use d7 para finalizar:\r\n");
    
    while(tec2 == 0){
        kpDebounce();
        
        if(kpRead() != tecla){
            tecla = kpRead();
            if(bitTst(tecla,0)){
                seg++;
                serial_tx(seg);
               
                
            }
            if(bitTst(tecla,2)){
                seg--;
                serial_tx(seg);
                
            }
            if(bitTst(tecla,4)){
                tec2 = 1;
                serial_tx_str(" Segundos alterados\r\n");
            }  
        }
    }
    
    //conv = min - '0';
    m = min * 60;
    s = seg * 10;
    tempo = m + s;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Descongelando...");
    
    crono(tempo);
}

void porpeso(){
    unsigned int tecla = 16;
    char tec = 0;
    int peso = 0;
    int tempo, p;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("MODO PESO");
    atraso_ms(200);
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Altere o Peso");
    
    serial_tx_str(" Altere o peso usando as teclas.\r\n");
    serial_tx_str(" Use d3 e d5 para 100 gramas \r\n");
    serial_tx_str(" Use d7 para finalizar:\r\n");
    
    
    while(tec == 0){
        kpDebounce();
        
        if(kpRead() != tecla){
            tecla = kpRead();
            if(bitTst(tecla,0)){
                peso++;
                serial_tx(peso);
               
                
            }
            if(bitTst(tecla,2)){
                peso--;
                serial_tx(peso);
                
            }
            if(bitTst(tecla,4)){
                tec = 1;
                serial_tx_str(" Peso alterado\r\n");
            }  
        }
    }
    
    
    p = peso*100;
    
    tempo = (p*165)/100;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("Descongelando...");
    crono(tempo);
}

void descongela(){
    unsigned char desc;
    int i;
    
    lcdCommand(L_CLR);
    lcdCommand(L_L1);
    lcdString("MODO DESCONGELAR");
    atraso_ms(200);
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
    unsigned char op;
    
    TRISA = 0xC3;
    TRISB = 0x03;
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    ADCON1 = 0x06;
    
    lcdInit();
    serial_init();
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
        
        op = serial_rx(2000);
        serial_tx(op);
        
        switch(op){
            case '1':
                lcdCommand(L_CLR);
                lcdCommand(L_L1);
                lcdString("MODO PIPOCA");
                atraso_ms(200);
                lcdCommand(L_CLR);
                lcdCommand(L_L1);
                lcdString("Preparando...");
                crono(140);
                break;
            case '2':
                descongela();
                break;
                        
        }
        
    }
    
    
    
    atraso_ms(100);
    while (1);
}


