#pragma once

extern const int GAMELOOP_TIMER_INTERVALL;
extern const int DATA_KEY;
extern const int VERSION_KEY;
extern const int TOTALISLANDS;
extern const int MENUITEMSCNT;
extern const int MENU2ITEMSCNT;
extern const int MENU3ITEMSCNT;
extern const int MENU4ITEMSCNT;
extern const int MENU6ITEMSCNT;
extern const int BASE_PRICE_SUPPLIES;

typedef struct{
  char gamemode;
  char currentevent;
  int eventhour;
  int eventday;
  int8_t tutorialcounter;
  
  int currentmenu[10];
  int menulayer;
  char notificationtext[150];
  
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
  int8_t islandscargo[10][4]; //320 bits
  int8_t shipsisland[10]; //80 bits
  int8_t shipstype[10]; //80 bits
  int8_t playercargo[5]; //32 bits
  int16_t playerwallet; //16 bits
  int8_t totalships; //8 bits
  
  int8_t maxplayercargo, currentspeed; //16 bits
  int8_t cargolevel, speedlevel, hascompass, hasmap, hascannons, canbuild, canpillage; //56 bits
  
  int8_t metalmultiplier, stonemultiplier, woodmultiplier, foodmultiplier; //32 bits
  int8_t metaldivisor, stonedivisor, wooddivisor, fooddivisor; //32 bits
  char currentevent; //8 bits
  int8_t eventhour; //8 bits
  int16_t eventday;//16 bits
  //1024 bits total
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