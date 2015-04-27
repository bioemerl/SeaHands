#pragma once
#include "types.h"
  
extern const int BASE_VALUE_UPGRADECARGO;
extern const int BASE_VALUE_UPGRADESPEED;
extern const int BASE_PLAYER_SPEED;
extern const int BASE_PLAYER_CARGO;
extern const int WIND_SPEED_SECTOR_SIZE;

  
void initialize_player(GameData* gamedata);
void update_player(GameData* gamedata);
void burn_player_cargo(GameData* gamedata);
void update_player_menu(GameData* gamedata);
void update_player_movement(GameData* gamedata);

void menuzeroupdate(GameData* gamedata);
void menuoneupdate(GameData* gamedata);
void menutwoupdate(GameData* gamedata);
void menuthreeupdate(GameData* gamedata);
void menufourupdate(GameData* gamedata);
void menufiveupdate(GameData* gamedata);
void menusixupdate(GameData* gamedata);
void menusevenupdate(GameData* gamedata);
void menueightupdate(GameData* gamedata);

void updatemenuselection(GameData* gamedata, int menulayer, int layeritemscount);
void buysellresources(GameData* gamedata, int8_t buyorsell, int resource, int islandnumber, int resourcecost);
int calculatewindspeed(GameData* gamedata);
void exitisland(GameData* gamedata);

void update_worldmap(GameData* gamedata);
