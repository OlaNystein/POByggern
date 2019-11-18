#include "setup.h"
#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"
#include "note.h"
#include "song.h"
#include "solenoid.h"
#define FCPU 16000000

int linesize = 48;
int lines, size;
static int mariomspb = (250);
static int counter = 0;

void music_init(void){
    //init treble pwm
    DDRE |= (1 << PE3);
    TCCR3A |= (1 << COM3A0);
    TCCR3A &= ~(1 << COM3A1) & ~(1 << WGM31) & ~(1 << WGM30) ;
    TCCR3B |= (1 << WGM32);
    TCCR3B &= ~(1 << WGM33); //set mode CTC, toggle OCR1A when compare match
    TCCR3B |= (1 << CS30);// | (1 << CS30);
    TCCR3B &= ~(1 << CS32) & ~(1 << CS31);// & ~(1 << CS10); //prescaler/1

    //init bass pwm
    //init treble pwm
    /* DDRB |= (1 << PB4);
    TCCR2A |= (1 << COM2A0);
    TCCR2A &= ~(1 << COM2A1) & ~(1 << WGM21) & ~(1 << WGM20) ;
    TCCR2B |= (1 << WGM22);
    TCCR2B &= ~(1 << WGM23); //set mode CTC, toggle OCR1A when compare match
    TCCR2B |= (1 << CS21);// | (1 << CS41);
    TCCR2B &= ~(1 << CS22) & ~(1 << CS20);// & ~(1 << CS10); //prescaler/64*/
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

void play_bass(unsigned int freq){
    if(freq == 0){
        TCCR2B &= ~(1 << CS21);// & ~(1 << CS41);
    }
    else {
        OCR2A = (FCPU/(2*8*freq)) -1;
        TCCR2B |= (1 << CS21);// | (1 << CS41);
    }
}

void play_music(int songpick){
    switch(songpick){
    
        case 1:
            size = 78;
            
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
                /*if(currNote == size-1){
                    currNote = 0;
                }*/
            }
            break;
        case 2:
            lines = 48;
            int basscounter = 0;
            int treblecounter = 0;
            int bassnote = -1;
            int treblenote = -1;
            for (int i = 48*24; i > 0; i--){
                /*if (basscounter == 0){
                    bassnote++;
                    play_bass(tetris_theme_bass[bassnote]*4);
                    basscounter = tetris_tempo_bass[bassnote];
                }*/
                if(treblecounter == 0){
                    treblenote++;
                    play_note(tetris_theme[treblenote]);
                    treblecounter = tetris_tempo[treblenote];
                }
                if ((i % 12) == 0){
                    solenoid_pulse();
                }
                //basscounter--;
                treblecounter--;
                _delay_ms(500);
                //play_bass(0);
                //play_note(0);
                //printf("note: %d\tcounter: %d\r\n", tetris_theme_bass[bassnote], basscounter);
            }
            /*for(int currNote = 0; currNote < size; currNote++){
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
                printf("%d\r\n", currNote);
            }*/
            break;
    }
    play_note(0);
    play_bass(0);
}
