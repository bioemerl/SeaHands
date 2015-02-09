#pragma once

typedef struct{
  char gamemode;
  int currentmenu[5];
  int menulayer;
  int downhit;
  int uphit;
  int buttonrelease;
  int shipsx[10], shipsy[10];
  int islandsx[10], islandsy[10];
  int8_t totalships;
  int8_t islandstypes[10];
  int8_t islandscargo[10][4]; //max of 128 of any resource at an island, 10 islands, 4 ints per cargo
  int8_t shipscargo[10]; //[the amount]
  int8_t shipsisland[10];
  int8_t shipstype[10]; //the boat cargo type
  int8_t playercargo[4]; //the player cargo
  int8_t playerisland;
  int16_t playerx, playery, playerxvelocity, playeryvelocity;
} GameData;