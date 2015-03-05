#pragma once
#include "types.h"
  
extern const int PLAYER_Y_LEVEL;
extern const int ENEMY_Y_LEVEL;
extern const int ENEMY_FIRE_SPEED;
extern const int PLAYER_FIRE_SPEED;

void initializebattle(ShipBattleData* shipbattledata);
void updatebattle(ShipBattleData* shipbattledata, GameData* gamedata);
void updateplayer(ShipBattleData* shipbattledata, GameData* gamedata);
void updateenemy(ShipBattleData* shipbattledata);
void updateshots(ShipBattleData* shipbattledata, GameData* gamedata);
void createbullet(ShipBattleData* shipbattledata, char loyalty, int xpos, int ypos);
void destroybullet(ShipBattleData* shipbattledata, int bulletnumber);
void endgame(ShipBattleData* shipbattledata, GameData* gamedata, char winner);
