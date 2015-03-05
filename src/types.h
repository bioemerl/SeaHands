#pragma once

#define GAMELOOP_TIMER_INTERVALL 45
extern const int DATA_KEY;
extern const int VERSION_KEY;
extern const int TOTALISLANDS;
extern const int MENUITEMSCNT;
extern const int MENU2ITEMSCNT;
extern const int MENU3ITEMSCNT;
extern const int BASE_PRICE_SUPPLIES;

typedef struct{
  char gamemode;
  
  int currentmenu[5];
  int menulayer;
  
  int16_t currentcosts;
  
  int downhit;
  int uphit;
  int buttonrelease;
  
  int16_t shipsx[10], shipsy[10];
  int8_t totalships;
  int8_t shipscargo[10]; //[the amount]
  int8_t shipsisland[10];
  int8_t shipstype[10]; //the boat cargo type
  
  int16_t islandsx[10], islandsy[10];
  int8_t islandstypes[10];
  int8_t islandscargo[10][4]; //max of 128 of any resource at an island, 10 islands, 4 ints per cargo

  
  int8_t playercargo[5]; //the player cargo
  int8_t playerisland;
  int16_t playerwallet;
  int16_t playerx, playery;
  int8_t playerxvelocity, playeryvelocity;
  int8_t currentwindspeed;
  
  //player upgrades
  int8_t maxplayercargo, currentspeed;
  int8_t cargolevel, speedlevel, hascompass, hasmap, hascannons, canbuild, canpillage;
} GameData;

typedef struct{
  int16_t metalvalue,
         stonevalue,
         woodvalue,
         foodvalue;
} ResourceValues;

typedef struct{
  int16_t shipsx[10], shipsy[10]; // 320 bits
  //int16_t islandsx[10], islandsy[10]; //320 bits
  //int8_t islandstypes[10]; //80 bits
  int8_t islandscargo[10][4]; //320 bits
  int8_t shipsisland[10]; //80 bits
  int8_t shipstype[10]; //80 bits
  int8_t playercargo[5]; //32 bits
  int16_t playerwallet; //16 bits
  //int16_t playerx, playery; //32 bits
  //int8_t playerxvelocity, playeryvelocity; //16 bits
  //total: 1296 bytes, less now.
  //max 2048 bytes
  int8_t maxplayercargo, currentspeed; 
  int8_t cargolevel, speedlevel, hascompass, hasmap, hascannons, canbuild, canpillage;
} SaveValues;

typedef struct{
  int16_t playerx,
          enemyx,
          enemytimer,
          shiptimer,
          shotsx[10],
          shotsy[10];
  
  int8_t numberofshots;
  char shotsside[10];
          
} ShipBattleData;