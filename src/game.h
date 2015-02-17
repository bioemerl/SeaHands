#pragma once
  
#include "types.h"

  
//static int framespassed;
#define GAMELOOP_TIMER_INTERVALL 45 // 40 FPS

void update_game(GameData* gamedata);
void initialize_islands(GameData* gamedata);
void initialize_player(GameData* gamedata);
void initialize_ships(GameData* gamedata);
void update_ships(GameData* gamedata);
void update_islands(GameData* gamedata);
void create_ship(GameData* gamedata, int8_t islandnumber, int8_t destinationnumber);
void destroy_ship(GameData* gamedata, int shipnumber, int reacheddestination);
int random(int max); //thanks to user FlashBIOS on the pebble forums for this
void update_player(GameData* gamedata);
int finddistance(int x1, int y1, int x2, int y2);
void burn_player_cargo(GameData* gamedata);
ResourceValues getmoneyvalue(GameData* gamedata, int currentisland);