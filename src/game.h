#pragma once
#include "types.h"

//static int framespassed;


void initialize_game(GameData* gamedata);
void update_game(GameData* gamedata);
int random(int max); //thanks to user FlashBIOS on the pebble forums for this
int finddistance(int x1, int y1, int x2, int y2);
ResourceValues getmoneyvalue(GameData* gamedata, int currentisland);
void load_data(GameData* gamedata);
void save_data(GameData* gamedata);