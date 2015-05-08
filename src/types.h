#pragma once

extern const int8_t GAMELOOP_TIMER_INTERVALL;
extern const int8_t DATA_KEY;
extern const int8_t DATA_VERSION_KEY;
extern const int8_t TOTALISLANDS;
extern const int8_t MENUITEMSCNT;
extern const int8_t MENU2ITEMSCNT;
extern const int8_t MENU3ITEMSCNT;
extern const int8_t MENU4ITEMSCNT;
extern const int8_t MENU6ITEMSCNT;
extern const int8_t MENU7ITEMSCNT;
extern const int8_t MENU8ITEMSCNT;
extern const int8_t MENU9ITEMSCNT;
extern const int8_t MENU10ITEMSCNT;
extern const int8_t MENU11ITEMSCNT;
extern const int8_t MENU12ITEMSCNT;
extern const int8_t MENU13ITEMSCNT;
extern const int8_t MENU14ITEMSCNT;
extern const int8_t MENU15ITEMSCNT;
extern const int8_t BASE_PRICE_SUPPLIES;
extern const int8_t CURRENT_SAVE_VERSION;
extern const int8_t MAX_SHIP_CARGO;
extern const int8_t MAX_ISLAND_CARGO;
extern const int MAX16BITVALUE;

typedef struct{
  char gamemode;
  char currentevent;
  int16_t eventhour;
  int16_t eventday;
  int8_t tutorialcounter;
  
  int8_t currentmenu[20];
  int8_t menulayer;
  char notificationtext[130];
  
  int16_t currentcosts;
  
  int8_t downhit;
  int8_t uphit;
  int8_t buttonrelease;
  
  int16_t shipsx[15], shipsy[15];
  int8_t totalships;
  int8_t shipscargo[15], shipstype[15]; //[the amount]  //the boat cargo type 
  int8_t shipsowner[15];
  char shipsorder[15];
  int8_t shipsorderinfo[15][3];
  int8_t numberofplayerships;
  int8_t currentplayership;
  
  
  int16_t islandsx[11], islandsy[11];
  int8_t islandstypes[11];
  int8_t islandscargo[11][4]; //max of 128 of any resource at an island, 10 islands, 4 ints per cargo
  int8_t islandsallegiance[11];
  int8_t storageexists;
  int16_t storagewallet;
  
  //these adjust how much stuff an island produces, names should be self-descriptive.
  int8_t metalmultiplier;
  int8_t stonemultiplier;
  int8_t woodmultiplier;
  int8_t foodmultiplier;
  int8_t metaldivisor;
  int8_t stonedivisor;
  int8_t wooddivisor;
  int8_t fooddivisor;
  
  
  int8_t playercargo[5]; //the player cargo
  int8_t playerisland;
  int8_t playership;
  int16_t playerwallet;
  int16_t playerx, playery;
  int8_t playerxvelocity, playeryvelocity;
  int8_t playersailsdeployed;
  int8_t currentwindspeed;
   
  //only to be looked at or changed when the gamemode changes between states
  //use different flag for each nested gamelayer change, with the main layer being the base layer of any others
  //say I enter a menu, then enter a game from in the menu, that will change the flag 2, so that the menu layer can
  //also use a flag
  char gamemodeswitchflag1;
  
  //player upgrades
  int8_t maxplayercargo, currentspeed;
  int8_t cargolevel, speedlevel;
} GameData;

typedef struct{
  int16_t metalvalue,
         stonevalue,
         woodvalue,
         foodvalue;
} ResourceValues;

typedef struct{
  int16_t shipsx[10], shipsy[10]; // 320 bits
  int8_t totalships; //8 bits
  int8_t shipscargo[10], shipstype[10]; //160 bits
  int8_t shipsowner[10]; //80 bits
  char shipsorder[10]; //80 bits
  int8_t shipsorderinfo[10][3]; //240 bits
  
  
  int8_t playercargo[5]; //32 bits
  int16_t playerwallet; //16 bits
  
  int8_t islandscargo[10][4]; //320 bits
  int8_t islandsallegiance[10]; //80 bits
  
  int8_t maxplayercargo, currentspeed; //16 bits
  int8_t cargolevel, speedlevel, hascompass, hasmap, hascannons, canbuild, canpillage; //56 bits
  
  int8_t metalmultiplier, stonemultiplier, woodmultiplier, foodmultiplier; //32 bits
  int8_t metaldivisor, stonedivisor, wooddivisor, fooddivisor; //32 bits
  char currentevent; //8 bits
  int8_t eventhour; //8 bits
  int16_t eventday;//16 bits
  //1496 bits total
  //max 2048 bytes
  
} SaveValuesOne;

typedef struct{
  int16_t shipsx[15], shipsy[15]; // 320 bits
  int8_t totalships; //8 bits
  int8_t shipscargo[15], shipstype[15]; //160 bits
  int8_t shipsowner[15]; //80 bits
  char shipsorder[15]; //80 bits
  int8_t shipsorderinfo[15][3]; //240 bits
  
  
  int8_t playercargo[5]; //32 bits
  int16_t playerwallet; //16 bits
  
  int8_t islandscargo[11][4]; //320 bits
  int8_t islandsallegiance[10]; //80 bits
  int8_t storagex;
  int8_t storagey;
  int8_t storageexists;
  int16_t storagewallet;
  int8_t numberofplayerships;
  
  int8_t maxplayercargo, currentspeed; //16 bits
  int8_t cargolevel, speedlevel, hascompass, hasmap, hascannons, canbuild, canpillage; //56 bits
  
  int8_t metalmultiplier, stonemultiplier, woodmultiplier, foodmultiplier; //32 bits
  int8_t metaldivisor, stonedivisor, wooddivisor, fooddivisor; //32 bits
  char currentevent; //8 bits
  int8_t eventhour; //8 bits
  int16_t eventday;//16 bits
  //x bits total
  //max 2048 bytes
  
} SaveValues;

typedef struct{
  int16_t playerx,
          enemyx,
          enemytimer,
          enemytarget,
          shiptimer,
          shotsx[10],
          shotsy[10];
  
  int8_t numberofshots;
  char shotsside[10];
          
} ShipBattleData;