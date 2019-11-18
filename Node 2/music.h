#ifndef MUSIC_H_
#define MUSIC_H_

void music_init(void);
//void PWM_start(int play);
void play_note(unsigned int freq);
void play_bass(unsigned int freq);
void play_music(int songpick);

#endif