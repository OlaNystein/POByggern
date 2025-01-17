#include "note.h"
#ifndef SONGS_H
#define SONGS_H



/*
########################
########################
SUPER MARIO
*/
//Mario main theme melody
int mario_melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0,NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0,NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int mario_tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  16, 16, 16,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  16, 16, 16,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};


int tetris_theme[] = {
    NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,
    NOTE_A4, 0, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
    NOTE_B4, 0, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
    NOTE_C5, NOTE_A4, 0, NOTE_A4, 0,

    0, NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
    NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
    NOTE_B4, 0, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
    NOTE_C5, NOTE_A4, 0, NOTE_A4, 0


};

int tetris_tempo[] ={
    12, 6, 6, 6, 3, 3, 6, 6,
    11, 1, 6, 6, 12, 6, 6,
    11, 1, 6, 6, 12, 12,
    12, 11, 1, 12, 12,

    6, 12, 6, 12, 6, 6,
    18, 6, 12, 6, 6,
    11, 1, 6, 6, 12, 12,
    12, 11, 1, 12, 12
};
#endif