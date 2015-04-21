#include <pebble.h>
#include "shipbattle.h"
#include "game.h"
#include "types.h"
#include "player.h"
#include "ship.h"
  
const int PLAYER_Y_LEVEL = 140;
const int ENEMY_Y_LEVEL = 10;
const int ENEMY_FIRE_SPEED = 25;
const int PLAYER_FIRE_SPEED = 42;
const int PLAYER_MOVE_SPEED = 1;
const int ENEMY_MOVE_SPEED = 1;
const int SHOT_MOVE_SPEED = 3;
const int PLAYER_SHIP_SIZE = 24; //must be higher than 8, devisible by 4
const int ENEMY_SHIP_SIZE = 24; //same as below

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
    shipbattledata->playerx -= PLAYER_MOVE_SPEED;
  }
  if(currentbutton == 2){
    shipbattledata->playerx += PLAYER_MOVE_SPEED;
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
  
  if((shipbattledata->enemyx <= shipbattledata->enemytarget + ENEMY_MOVE_SPEED && shipbattledata->enemyx >= shipbattledata->enemytarget - ENEMY_MOVE_SPEED) && shipbattledata->enemytimer == 0){
    createbullet(shipbattledata, 'e', shipbattledata->enemyx, ENEMY_Y_LEVEL);
    shipbattledata->enemytimer = ENEMY_FIRE_SPEED;
    shipbattledata->enemytarget = ((ENEMY_SHIP_SIZE /2 + ENEMY_SHIP_SIZE + 4)/2) + random(144 - (ENEMY_SHIP_SIZE /2 + ENEMY_SHIP_SIZE + 4));
  }
  
  if(shipbattledata->enemyx < shipbattledata->enemytarget){
    shipbattledata->enemyx += ENEMY_MOVE_SPEED;
  }
  if(shipbattledata->enemyx > shipbattledata->enemytarget){
    shipbattledata->enemyx -= ENEMY_MOVE_SPEED;
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
      if((shipbattledata->shotsx[i] < shipbattledata->enemyx + ENEMY_SHIP_SIZE/2 + ENEMY_SHIP_SIZE/4 && shipbattledata->shotsx[i] > shipbattledata->enemyx - ENEMY_SHIP_SIZE/2 - ENEMY_SHIP_SIZE/4) && (abs(shipbattledata->shotsy[i] - ENEMY_Y_LEVEL) <= ENEMY_SHIP_SIZE/4))
        endgame(shipbattledata, gamedata, 'p');
      //update shot movement
      shipbattledata->shotsy[i] -= SHOT_MOVE_SPEED;
      //destroy bullet if necessary
      if(shipbattledata->shotsy[i] < 0 || shipbattledata->shotsy[i] > 200)
        destroybullet(shipbattledata, i);
    }
    if(shipbattledata->shotsside[i] == 'e'){
      //check for death
      if((shipbattledata->shotsx[i] < shipbattledata->playerx + PLAYER_SHIP_SIZE/2 + PLAYER_SHIP_SIZE/4 && shipbattledata->shotsx[i] > shipbattledata->playerx - PLAYER_SHIP_SIZE/2 - PLAYER_SHIP_SIZE/4) && (abs(shipbattledata->shotsy[i] - PLAYER_Y_LEVEL) <= PLAYER_SHIP_SIZE/4))
        endgame(shipbattledata, gamedata, 'e');
      //update shot movement
      shipbattledata->shotsy[i] += SHOT_MOVE_SPEED;
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
  if(winner == 'e'){
    //if player lost, punish the player
    initialize_player(gamedata);
    gamedata->playerwallet = (gamedata->playerwallet -(gamedata->playerwallet/5));
    //adjust the island hatred a bit less
    manageislandallegiance(gamedata, gamedata->shipsowner[gamedata->playership], -3);
    gamedata->shipsorder[gamedata->playership] = 'n';
  }
  if(gamedata->gamemodeswitchflag1 == 'p' && winner == 'p'){ //if p for pillage
    manageislandallegiance(gamedata, gamedata->shipsowner[gamedata->playership], -5);
    if(gamedata->playercargo[gamedata->shipstype[gamedata->playership]] + gamedata->shipscargo[gamedata->playership] <= gamedata->maxplayercargo)
      gamedata->playercargo[gamedata->shipstype[gamedata->playership]] += gamedata->shipscargo[gamedata->playership];
    if(gamedata->playercargo[4] < gamedata->maxplayercargo)
      gamedata->playercargo[4]++;
    destroy_ship(gamedata, gamedata->playership);
  }
  if(gamedata->gamemodeswitchflag1 == 'h' && winner == 'p'){ //if h for hijack
     manageislandallegiance(gamedata, gamedata->shipsowner[gamedata->playership], -5);
  }
  gamedata->gamemode = 'p';
  gamedata->playership = -1;
  initializebattle(shipbattledata);
}