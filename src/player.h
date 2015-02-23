#pragma once
#include "types.h"
  
extern const int BASE_VALUE_UPGRADECARGO;
extern const int VASE_VALUE_UPGRADESPEED;
extern const int BASE_PLAYER_SPEED;
extern const int BASE_PLAYER_CARGO;
  
void initialize_player(GameData* gamedata);
void update_player(GameData* gamedata);
void burn_player_cargo(GameData* gamedata);
int8_t check_for_player_upgrade(GameData* gamedata);
int8_t check_player_upgrade_price(GameData* gamedata);
void update_player_menu(GameData* gamedata);
void update_player_movement(GameData* gamedata);
int check_current_button(GameData* gamedata);