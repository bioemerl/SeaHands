#pragma once
#include "types.h"

//ships should be packets of information
//a ship should have a type.  Warship, cargoship, buildingship
//all ships have a source, who they get resources from, a destination they give resources to
//all ships should be persistent, an island should build a ship, rather than generating one to destroy it
//ships should have an owner, a command, a cargo-hold, and command info

void initialize_ships(GameData* gamedata);
void update_ships(GameData* gamedata);
void create_ship(GameData* gamedata, int8_t islandnumber, int8_t destinationnumber);
void destroy_ship(GameData* gamedata, int shipnumber, int reacheddestination);