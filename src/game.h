#pragma once
#include "types.h"

//static int framespassed;
  
//idea: every so often, open up a news report that tells the player what is going on
//in the game world (METAL IS EXPENSIVE!)


void initialize_game(GameData* gamedata);
void update_game(GameData* gamedata);
int random(int max); //thanks to user FlashBIOS on the pebble forums for this
int finddistance(int x1, int y1, int x2, int y2);
ResourceValues getmoneyvalue(GameData* gamedata, int currentisland);
void load_data(GameData* gamedata);
void save_data(GameData* gamedata);
int check_for_player_upgrade(GameData* gamedata, int upgradetype);
int check_player_upgrade_price(GameData* gamedata, int upgradetype);
int check_current_button(GameData* gamedata);
void attempt_tutorial(GameData* gamedata, int8_t forcetutorial);
void displaynotification(GameData* gamedata, char notificationtext[100]);
void exitmenus(GameData* gamedata);
