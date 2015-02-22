#pragma once
#include "types.h"

void initialize_ships(GameData* gamedata);
void update_ships(GameData* gamedata);
void create_ship(GameData* gamedata, int8_t islandnumber, int8_t destinationnumber);
void destroy_ship(GameData* gamedata, int shipnumber, int reacheddestination);