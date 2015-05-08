#pragma once
#include "types.h"
  
extern const int8_t PLAYER_Y_LEVEL;
extern const int8_t ENEMY_Y_LEVEL;
extern const int8_t ENEMY_FIRE_SPEED;
extern const int8_t PLAYER_FIRE_SPEED;
extern const int8_t PLAYER_MOVE_SPEED;
extern const int8_t ENEMY_MOVE_SPEED;
extern const int8_t SHOT_MOVE_SPEED;
extern const int8_t PLAYER_SHIP_SIZE;
extern const int8_t ENEMY_SHIP_SIZE;

void initializebattle(ShipBattleData* shipbattledata);
void updatebattle(ShipBattleData* shipbattledata, GameData* gamedata);
void updateplayer(ShipBattleData* shipbattledata, GameData* gamedata);
void updateenemy(ShipBattleData* shipbattledata);
void updateshots(ShipBattleData* shipbattledata, GameData* gamedata);
void createbullet(ShipBattleData* shipbattledata, char loyalty, int xpos, int ypos);
void destroybullet(ShipBattleData* shipbattledata, int bulletnumber);
void endgame(ShipBattleData* shipbattledata, GameData* gamedata, char winner);
