#ifndef GAME_H_
#define GAME_H_
int game_init(void);
struct screen* start_game(struct screen* menu, char* direction, int* status, int difficulty);
#endif
