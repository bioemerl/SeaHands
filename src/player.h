#pragma once
#include "types.h"
  
extern const int BASE_VALUE_UPGRADECARGO;
extern const int BASE_VALUE_UPGRADESPEED;
extern const int BASE_PLAYER_SPEED;
extern const int BASE_PLAYER_CARGO;
  
void initialize_player(GameData* gamedata);
void update_player(GameData* gamedata);
void burn_player_cargo(GameData* gamedata);
void update_player_menu(GameData* gamedata);
void update_player_movement(GameData* gamedata);
int check_current_button(GameData* gamedata);
void menuzeroupdate(GameData* gamedata);
void menuoneupdate(GameData* gamedata);
void menutwoupdate(GameData* gamedata);
void updatemenuselection(GameData* gamedata, int menulayer, int layeritemscount);
void buysellresources(GameData* gamedata, int8_t buyorsell, int resource, int islandnumber, int resourcecost);
