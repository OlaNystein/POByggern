#include "setup.h"
#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"
#include "note.h"
#include "song.h"
#include "solenoid.h"
#define FCPU 16000000

int size = 0;
static int mariomspb = (250);
static int counter = 0;

void music_init(void){
    DDRE |= (1 << PE3);
    TCCR3A |= (1 << COM3A0);
    TCCR3A &= ~(1 << COM3A1) & ~(1 << WGM31) & ~(1 << WGM30) ;
    TCCR3B |= (1 << WGM32);
    TCCR3B &= ~(1 << WGM33); //set mode CTC, toggle OCR1A when compare match
    TCCR3B |= (1 << CS30);// | (1 << CS30);
    TCCR3B &= ~(1 << CS32) & ~(1 << CS31);// & ~(1 << CS10); //prescaler/64
}

/*void PWM_start(int play){
    if(play == 1){
        //prescale /8
        TCCR1B |= (1 << CS11);// | (1 << CS10);
        TCCR1B &= ~(1 << CS12) & ~(1 << CS12);
    }
    else{
        //stop counter
        TCCR1B &= ~(1 << CS10) & ~(1 << CS11) & ~(1 << CS12);
    }
}*/

void play_note(unsigned int freq){
    if(freq == 0){
        TCCR3B &= ~(1 << CS30);
    }
    else {
        OCR3A = (FCPU/(2*freq))-1;
        TCCR3B |= (1 << CS30);
    }
}

void play_music(int songpick){
    switch(songpick){
    
        case 1:
            size = sizeof(mario_melody);
            
            for(int currNote = 0; currNote < size; currNote++){
                int length = mariomspb*(mario_tempo[currNote]);
                play_note(mario_melody[currNote]);
                for (int i = 0; i < length; i++){
                    _delay_ms(1);
                    counter++;
                    if(counter == 48*mariomspb){
                        counter = 0;
                        //solenoid_pulse();
                    }
                }
                if(currNote == size-1){
                    currNote = 0;
                }
            }
            break;
        case 2:
            size = sizeof(tetris_theme);
            for(int currNote = 0; currNote < size; currNote++){
                int length = 500*(tetris_tempo[currNote]);
                play_note(tetris_theme[currNote]);
                for (int i = 0; i < length; i++){
                    _delay_ms(1);
                    counter++;
                    if(counter == 24*mariomspb){
                        counter = 0;
                        //solenoid_pulse();
                    }
                }
                //if(currNote == size-1){
                    //currNote = 0;
                //}
                play_note(0);
            }
            break;
    }
    play_note(0);
}