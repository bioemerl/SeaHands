#pragma once
#include "types.h"

//ships should be packets of information
//a ship should have a type.  Warship, cargoship, buildingship
//all ships have a source, who they get resources from, a destination they give resources to
//all ships should be persistent, an island should build a ship, rather than generating one to destroy it
//ships should have an owner, a command, a cargo-hold, and command info

void initialize_ships(GameData* gamedata);
void update_ships(GameData* gamedata);
//void create_ship(GameData* gamedata, int8_t islandnumber, int8_t destinationnumber);
void create_ship(GameData* gamedata, int8_t ownernumber);
void give_ship_order(GameData* gamedata, int8_t ownernumber, int8_t shipnumber, char ordercharacter, int8_t orderinfo[3]);
void destroy_ship(GameData* gamedata, int shipnumber);

//removes and gives cargo to islands once the ship reaches the island (or player)
int ship_take_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource);
int ship_give_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource);
//buys or sells cargo to islands once the ship reaches the island.
void ship_buy_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource);
void ship_sell_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource);
int8_t move_ship(GameData* gamedata, int8_t destination, int8_t shipnumber);
