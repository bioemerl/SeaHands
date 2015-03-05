#include <pebble.h>
#include "shipbattle.h"
#include "game.h"
#include "types.h"
  
const int PLAYER_Y_LEVEL = 140;
const int ENEMY_Y_LEVEL = 10;
const int ENEMY_FIRE_SPEED = 100;
const int PLAYER_FIRE_SPEED = 100;
  
void initializebattle(ShipBattleData* shipbattledata){
  shipbattledata->playerx = 0;
  shipbattledata->enemyx = 0;
  shipbattledata->numberofshots = 0;
  shipbattledata->shiptimer = 0;
  shipbattledata->enemytimer = 0;
  for(int i = 0; i < 10; i++){
    shipbattledata->shotsx[i] = 0;
    shipbattledata->shotsy[i] = 0;
    shipbattledata->shotsside[i] = 'x';
  }
}

void updatebattle(ShipBattleData* shipbattledata, GameData* gamedata){
  updateplayer(shipbattledata, gamedata);
  updateenemy(shipbattledata);
  updateshots(shipbattledata, gamedata);
}

void updateplayer(ShipBattleData* shipbattledata, GameData* gamedata){
  if(shipbattledata->shiptimer > 0)
    shipbattledata->shiptimer--;
  int currentbutton = check_current_button(gamedata);
  if(currentbutton == 1){
    shipbattledata->playerx--;
  }
  if(currentbutton == 2){
    shipbattledata->playerx++;
  }
  if(currentbutton == 3 && gamedata->buttonrelease == 1 && shipbattledata->shiptimer == 0){
    shipbattledata->shiptimer = PLAYER_FIRE_SPEED;
    gamedata->buttonrelease = 0;
    createbullet(shipbattledata, 'p', shipbattledata->playerx, PLAYER_Y_LEVEL);
  }
}

void updateenemy(ShipBattleData* shipbattledata){
  if(shipbattledata->enemytimer > 0)
    shipbattledata->enemytimer--;
  if(shipbattledata->enemyx < shipbattledata->playerx){
    shipbattledata->enemyx++;
  }
  if(shipbattledata->enemyx > shipbattledata->playerx){
    shipbattledata->enemyx--;
  }
  if(shipbattledata->enemyx == shipbattledata->playerx && shipbattledata->enemytimer == 0){
    shipbattledata->enemytimer = ENEMY_FIRE_SPEED;
    createbullet(shipbattledata, 'e', shipbattledata->enemyx, ENEMY_Y_LEVEL);
  }
}

void updateshots(ShipBattleData* shipbattledata, GameData* gamedata){
  for(int i = 0; i <= shipbattledata->numberofshots; i++){
    
    if(shipbattledata->shotsside[i] == 'p'){
      //check for death
      if(shipbattledata->shotsx[i] == shipbattledata->enemyx && shipbattledata->shotsy[i] == ENEMY_Y_LEVEL)
        endgame(shipbattledata, gamedata, 'p');
      //update shot movement
      shipbattledata->shotsy[i]--;
      //destroy bullet if necessary
      if(shipbattledata->shotsy[i] < 0 || shipbattledata->shotsy[i] > 200)
        destroybullet(shipbattledata, i);
    }
    if(shipbattledata->shotsside[i] == 'e'){
      //check for death
      if(shipbattledata->shotsx[i] == shipbattledata->playerx && shipbattledata->shotsy[i] == PLAYER_Y_LEVEL)
        endgame(shipbattledata, gamedata, 'e');
      //update shot movement
      shipbattledata->shotsy[i]++;
      //destroy bullet if necessary
      if(shipbattledata->shotsy[i] < 0 || shipbattledata->shotsy[i] > 200)
        destroybullet(shipbattledata, i);
    }
    
  }
}

void createbullet(ShipBattleData* shipbattledata, char loyalty, int xpos, int ypos){
  if(shipbattledata->numberofshots < 10){
    shipbattledata->numberofshots++;
    shipbattledata->shotsx[shipbattledata->numberofshots] = xpos;
    shipbattledata->shotsy[shipbattledata->numberofshots] = ypos;
    shipbattledata->shotsside[shipbattledata->numberofshots] = loyalty;
  }
  else{
    APP_LOG(APP_LOG_LEVEL_INFO, "TRIED TO CREATE MORE THAN 10 SHOTS");
  }
}

void destroybullet(ShipBattleData* shipbattledata, int bulletnumber){
  if(shipbattledata->numberofshots > 0){
    for(int i = bulletnumber; i <= shipbattledata->numberofshots; i++){
      shipbattledata->shotsx[i] = shipbattledata->shotsx[i + 1];
      shipbattledata->shotsy[i] = shipbattledata->shotsy[i + 1];
      shipbattledata->shotsside[i] = shipbattledata->shotsside[i + 1];
    }
    shipbattledata->numberofshots--;
  }
  else{
    APP_LOG(APP_LOG_LEVEL_INFO, "TRIED TO REMOVE SHOTS THAT DO NOT EXIST");
  }
}

void endgame(ShipBattleData* shipbattledata, GameData* gamedata, char winner){
  gamedata->gamemode = 'p';
  initializebattle(shipbattledata);
}