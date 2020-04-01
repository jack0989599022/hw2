#include "mbed.h"

Serial pc( USBTX, USBRX );
AnalogIn Ain(A0);
AnalogOut Aout(DAC0_OUT);

DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[11] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x0};

int sample = 512, state = 0, frequency = 0, c1 = 0, c2 = 0, c3 = 0;
float ADCdata[512];

int main(){
    for(int i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        
        if((ADCdata[i] > 0.3) && (state == 0)){
            frequency++;
            state = 1;
        }
        
        if(ADCdata[i] < 0.2)
            state = 0;
        
        wait(1./sample);
    }  

    for(int j = 0; j < sample; j++)
        pc.printf("%1.3f\r\n", ADCdata[j]);

    c1 = frequency / 100;
    c2 = frequency / 10 % 10;
    c3 = frequency % 10;    

    while(1){
        redLED = 1;
        greenLED = 0;

        while(Switch == 0){
            redLED = 0;
            greenLED = 1;

            display = table[c1];
            wait(1);
            display = table[c2];
            wait(1);
            display = table[c3] + 128;
            wait(1);
            display = table[10];
        }

        for(float k = 0; k < 2; k += 0.05){
          Aout = 0.5 + 0.5 * sin(k * 2 * 3.14159 * frequency);
          wait(0.001);
        }
    }
}