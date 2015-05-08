#pragma once
#include "types.h"

extern const int METALPRODUCTIONVALUE;
extern const int WOODPRODUCTIONVALUE;
extern const int STONEPRODUCTIONVALUE;
extern const int FOODPRODUCTIONVALUE;

void initialize_islands(GameData* gamedata);
void update_islands(GameData* gamedata);
void initialize_island(GameData* gamedata, int8_t islandnumber, int8_t islandtype, int x, int y, int8_t overridemetal, int8_t overridewood,
                       int8_t overridestone, int8_t overridefood);
void updateislandresource(GameData* gamedata, int islandnumber, int res1adjust, int res2adjust, int res3adjust, int res4adjust);
