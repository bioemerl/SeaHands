#include <pebble.h>
#include "player.h"
#include "types.h"
#include "game.h"
  
const int BASE_VALUE_UPGRADESPEED = 100;
const int BASE_VALUE_UPGRADECARGO = 10;
const int BASE_PLAYER_SPEED = 4;
const int BASE_PLAYER_CARGO = 10;
const int WIND_SPEED_SECTOR_SIZE = 100;


void initialize_player(GameData* gamedata){
  //kill all layers
  for(int i = 0; i < 4; i++){
    gamedata->currentmenu[i] = 0;
  }
  gamedata->menulayer = 0;
  gamedata->gamemode = 'p';
  //initiate player values
  gamedata->playerxvelocity = 0;
  gamedata->playeryvelocity = gamedata->currentspeed;
  gamedata->playerx = 50;
  gamedata->playery = 50;
  //initiate player cargo
  gamedata->playercargo[0] = 0;
  gamedata->playercargo[1] = 10;
  gamedata->playercargo[2] = 0;
  gamedata->playercargo[3] = 10;
  gamedata->playercargo[4] = 5;
  gamedata->currentwindspeed = calculatewindspeed(gamedata);
  gamedata->playersailsdeployed = 0;
  //gamedata->maxplayercargo = BASE_PLAYER_CARGO;
}

void burn_player_cargo(GameData* gamedata){
  //APP_LOG(APP_LOG_LEVEL_INFO, "Speed = %i: X: %i Y: %i", calculatewindspeed(gamedata), gamedata->playerx, gamedata->playery);
  if(random(5) == 2){
    gamedata->playercargo[4] += -1; //burn supplies
  }
  if(gamedata->playercargo[4] <= 0){
    initialize_player(gamedata);
    gamedata->playerwallet = (gamedata->playerwallet -(gamedata->playerwallet/5));
  }
}

void update_player(GameData* gamedata){
  if(gamedata->gamemode == 'p')//if gamemode is overworld play
    update_player_movement(gamedata);
  if(gamedata->gamemode == 'm')//if gamemode is menus
    update_player_menu(gamedata);    
}



void update_player_menu(GameData* gamedata){
  if(gamedata->menulayer == 0)
    menuzeroupdate(gamedata);
  if(gamedata->menulayer == 1)
    menuoneupdate(gamedata); 
  if(gamedata->menulayer == 2)
    menutwoupdate(gamedata);
  if(gamedata->menulayer == 3)
    menuthreeupdate(gamedata);
}

void updatemenuselection(GameData* gamedata, int menulayer, int layeritemscount){
  if(gamedata->downhit == 1 && gamedata->uphit == 0 && gamedata->buttonrelease == 1){
      gamedata->currentmenu[menulayer] = ((gamedata->currentmenu[menulayer] + 1) % layeritemscount);
      gamedata->buttonrelease = 0;
  }
  //UP PRESSED
  if(gamedata->uphit == 1 && gamedata->downhit == 0 && gamedata->buttonrelease == 1){
    gamedata->currentmenu[menulayer] = ((gamedata->currentmenu[menulayer] - 1) % layeritemscount);
    gamedata->buttonrelease = 0;
    if(gamedata->currentmenu[menulayer] < 0)
      gamedata->currentmenu[menulayer] = layeritemscount - 1;
  }
}

void menuzeroupdate(GameData* gamedata){
  int buttonpress = check_current_button(gamedata);
  updatemenuselection(gamedata, 0, MENUITEMSCNT);
  //IF SELECT IS HIT AND MENULAYER IS 0
    if(gamedata->currentmenu[0] >=0 && gamedata->currentmenu[0] <= 3 && gamedata->menulayer == 0
       && gamedata->uphit == 1 && gamedata->downhit == 1 && gamedata->buttonrelease == 1){
      gamedata->menulayer = 1;
      gamedata->buttonrelease = 0;
    }
    if(gamedata->currentmenu[0] == 4 && buttonpress == 3 && gamedata->buttonrelease == 1){
      gamedata->menulayer = 2;
      gamedata->buttonrelease = 0;
    }
    //if exit is highlighted leave
    if(gamedata->currentmenu[0] == 5 && buttonpress == 3){
      if(gamedata->playerx > gamedata->islandsx[gamedata->playerisland]){
        gamedata->playerx = gamedata->islandsx[gamedata->playerisland] + 30;
      }
      else{
        gamedata->playerx = gamedata->islandsx[gamedata->playerisland] - 30;
      }
      if(gamedata->playery > gamedata->islandsy[gamedata->playerisland]){
        gamedata->playery = gamedata->islandsy[gamedata->playerisland] + 30;

      }
      else{
        gamedata->playery = gamedata->islandsy[gamedata->playerisland] - 30;
      }
      gamedata->playersailsdeployed = 0;
      gamedata->currentmenu[0] = 0;
      gamedata->gamemode = 'p';
      gamedata->buttonrelease = 0;
    }
}

void menuoneupdate(GameData* gamedata){
  int buttonpress = check_current_button(gamedata);
  updatemenuselection(gamedata, 1, MENU2ITEMSCNT);
  
  //IF SECOND MENU FIRST ITEM IS SELETED, REGARDLESS OF FIRST LEVEL, GO BACK TO FIRST MENU
  if(gamedata->currentmenu[1] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    gamedata->menulayer = 0;
    gamedata->currentmenu[1] = 0;
    gamedata->buttonrelease = 0;
  }
  //SELETIONS FOR BUY AND SELL IF LAYER IS ONE
  //IF THE FIRST IS SELECTED
  if(gamedata->currentmenu[0] == 0 && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 0, 0, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 0  && gamedata->currentmenu[1] == 2 && buttonpress == 3){
    buysellresources(gamedata, 1, 0, gamedata->playerisland, gamedata->currentcosts);
  }
  //IF SECOND IS SELECTED
  if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 0, 1, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 2 && buttonpress == 3){
    buysellresources(gamedata, 1, 1, gamedata->playerisland, gamedata->currentcosts);
  }
  //IF THIRD IS SELETED
  if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 0, 2, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 2 && buttonpress == 3){
    buysellresources(gamedata, 1, 2, gamedata->playerisland, gamedata->currentcosts);
  }
  //IF FOURTH IS SELECTED buy item
  if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 0, 3, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 2 && buttonpress == 3){
    buysellresources(gamedata, 1, 3, gamedata->playerisland, gamedata->currentcosts);
  }
}

//will buy from islands or sell from islands, buys if 0, sells if 1 islandnumber is the island to buy/sell from
//islandnumber should be whatever the players island is.  Otherwise resource costs will be inaccurate
void buysellresources(GameData* gamedata, int8_t buyorsell, int resource, int islandnumber, int resourcecost){
  if(buyorsell == 0){
    if(gamedata->playercargo[resource] < gamedata->maxplayercargo && gamedata->islandscargo[islandnumber][resource] > 0  && gamedata->playerwallet > gamedata->currentcosts){
      gamedata->islandscargo[gamedata->playerisland][resource] += -1;
      gamedata->playercargo[resource] += 1;
      gamedata->playerwallet = gamedata->playerwallet - resourcecost;
    }
  }
  if(buyorsell == 1){
    if(gamedata->playercargo[resource] > 0 && gamedata->islandscargo[islandnumber][resource] < 100){
      gamedata->islandscargo[gamedata->playerisland][resource] += 1;
      gamedata->playercargo[resource] += -1;
      gamedata->playerwallet = gamedata->playerwallet + resourcecost;
    }
  }
}

void menutwoupdate(GameData* gamedata){
  int buttonpress = check_current_button(gamedata);
  updatemenuselection(gamedata, 2, MENU3ITEMSCNT);
  if(gamedata->currentmenu[2] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    gamedata->buttonrelease = 0;
    if(gamedata->playerwallet >= BASE_PRICE_SUPPLIES && gamedata->playercargo[4] < 10 
       && gamedata->playercargo[3] > 0 && gamedata->playercargo[1] > 0){
      gamedata->playercargo[4]++;
      gamedata->playerwallet += -BASE_PRICE_SUPPLIES;
      gamedata->playercargo[3]--;
      gamedata->playercargo[1]--;
    }
  }
  if(gamedata->currentmenu[2] == 1 && buttonpress == 3 && gamedata->buttonrelease == 1){
   gamedata->buttonrelease = 0;
    int upgradeprice = check_player_upgrade_price(gamedata, 0);
    if(gamedata->playerwallet >= upgradeprice){
      gamedata->cargolevel++;
      gamedata->maxplayercargo += 5;
      gamedata->playerwallet += -upgradeprice;
    } 
  }
  if(gamedata->currentmenu[2] == 2 && buttonpress == 3){
    int upgradeprice = check_player_upgrade_price(gamedata, 1);
    if(gamedata->playerwallet >= upgradeprice){
      gamedata->currentspeed++;
      gamedata->speedlevel++;
      gamedata->playerwallet += -upgradeprice;
    }
  }
  if(gamedata->currentmenu[2] == 3 && buttonpress == 3){
    gamedata->menulayer = 0;
    gamedata->currentmenu[2] = 0;
    gamedata->buttonrelease = 0;
  }
}

void menuthreeupdate(GameData* gamedata){
  int buttonpress = check_current_button(gamedata);
  updatemenuselection(gamedata, 3, MENU4ITEMSCNT);
  if(gamedata->currentmenu[3] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    //pillage
    gamedata->gamemodeswitchflag1 = 'p';
    gamedata->gamemode = 'b';
  }
  if(gamedata->currentmenu[3] == 1 && buttonpress == 3 && gamedata->buttonrelease == 1){
    //exit menu
    gamedata->playership = -1;
    gamedata->currentmenu[3] = 0;
    gamedata->gamemode = 'p';
    gamedata->playerx += 6;
    gamedata->playery += 6;
  }
}

void update_player_movement(GameData* gamedata){
  int buttonhit = check_current_button(gamedata);
  
  //UP TRIGGER
  if(buttonhit == 1){   
    //move the velocity in a square
    if(gamedata->playerxvelocity >= 0 && gamedata->playeryvelocity > 0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity--;
    }
    if(gamedata->playerxvelocity <= 0 && gamedata->playeryvelocity < 0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity++;
    }
    if(gamedata->playerxvelocity > 0 && gamedata->playeryvelocity <= 0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity--;
    }
    if(gamedata->playerxvelocity < 0 && gamedata->playeryvelocity >= 0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity++;
    }
  }
  
  //DOWN TRIGGER
  if(buttonhit == 2){
    //move the velocity in a counterclockwise square
    if(gamedata->playerxvelocity > 0 && gamedata->playeryvelocity >= 0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity++;
    }
    if(gamedata->playerxvelocity >= 0 && gamedata->playeryvelocity < 0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity++;
    }
    if(gamedata->playerxvelocity < 0 && gamedata->playeryvelocity <= 0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity--;
    }
    if(gamedata->playerxvelocity <= 0 && gamedata->playeryvelocity > 0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity--;
    }
  }
  
  //SELECT TRIGGER
  if(buttonhit == 3 && gamedata->buttonrelease == 1){
   if(gamedata->playersailsdeployed == 0){
     gamedata->playersailsdeployed = 1;
    }
    else{
      gamedata->playersailsdeployed = 0;
    }
    gamedata->buttonrelease = 0;
  }
  
  
  if(gamedata->playersailsdeployed == 1){
    //cause base player movement
    gamedata->playerx += gamedata->playerxvelocity;
    gamedata->playery += gamedata->playeryvelocity;
    //adjust for wind
    if(gamedata->currentwindspeed == 1){
      gamedata->playerx += 0;
      gamedata->playery += 2;
    }
    if(gamedata->currentwindspeed == 2){
      gamedata->playerx += 1;
      gamedata->playery += 1;
    }
    if(gamedata->currentwindspeed == 3){
      gamedata->playerx += 2;
      gamedata->playery += 0;
    }
    if(gamedata->currentwindspeed == 4){
      gamedata->playerx += 1;
      gamedata->playery += -1;
    }
    if(gamedata->currentwindspeed == 5){
      gamedata->playerx += 0;
      gamedata->playery += -2;
    }
    if(gamedata->currentwindspeed == 6){
      gamedata->playerx += -1;
      gamedata->playery += -1;
    }
    if(gamedata->currentwindspeed == 7){
      gamedata->playerx += -2;
      gamedata->playery += 0;
    }
    if(gamedata->currentwindspeed == 8){
      gamedata->playerx += -1;
      gamedata->playery += 1;
    }
    if(gamedata->playerx%WIND_SPEED_SECTOR_SIZE == 0 || gamedata->playery%WIND_SPEED_SECTOR_SIZE == 0){
      gamedata->currentwindspeed = calculatewindspeed(gamedata);
    }
  }
  
  //if player colides with an island, set the game-mode to showing the menu
  for(int i = 0; i < TOTALISLANDS; i++){
    if(finddistance(gamedata->playerx, gamedata->playery, gamedata->islandsx[i], gamedata->islandsy[i]) <= 25*25){ //(size of island)
      gamedata->playerisland = i;
      gamedata->buttonrelease = 0;
      if(gamedata->gamemode == 'p')
      gamedata->menulayer = 0;
      gamedata->gamemode = 'm';
      break;
    }
  }
  //APP_LOG(APP_LOG_LEVEL_INFO, "NUMBEROFSHIPS %i", gamedata->totalships);
  for(int i = 0; i <= gamedata->totalships; i++){
    //APP_LOG(APP_LOG_LEVEL_INFO, "TEST: %i", (finddistance(gamedata->playerx, gamedata->playery, gamedata->shipsx[i], gamedata->shipsy[i]) <= 8*8));
    if(finddistance(gamedata->playerx, gamedata->playery, gamedata->shipsx[i], gamedata->shipsy[i]) <= 8*8){
      //open up a menu
      gamedata->playership = i;
      APP_LOG(APP_LOG_LEVEL_INFO, "HIT SHIP");
      gamedata->gamemode = 'm';
      gamedata->menulayer = 3;
    }
  }
}

//returns a value 1 to 8 depending on the players position and so on
//1 to 8 will refer to directions of wind, with 1 being flat down
//wind will always be speed of "(0,3), (3,0), (2,2)"
// a value of zero means no wind, and no change to the player.
int calculatewindspeed(GameData* gamedata){
  //get the current hour here:
  //going to base this on a 12 hour time?
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  int hour = 0;
  int currentspeed;
  hour = tick_time->tm_hour;
  //hour is a number between 1 and 24
  //12 is noon 24 is midnight
  //and so on
  if(hour <= 12 && hour >= 6){
    if((gamedata->playery >=0 && gamedata->playery <=1250) || (gamedata->playery <= -1250 && gamedata->playery >= -2500)){
      //head right
      currentspeed = 1 + abs((((hour+gamedata->playerx)/WIND_SPEED_SECTOR_SIZE) + ((hour+gamedata->playery)/WIND_SPEED_SECTOR_SIZE) * hour)%4);
    }
    else{
      //head left
      currentspeed = 9 - (1 + abs((((hour+gamedata->playerx)/WIND_SPEED_SECTOR_SIZE) + ((hour+gamedata->playery)/WIND_SPEED_SECTOR_SIZE) * hour)%3));
    }
  }
  else if(hour > 12){
    if((gamedata->playery >=0 && gamedata->playery <=1250) || (gamedata->playery <= -1250 && gamedata->playery >= -2500)){
      //head left
      currentspeed = 9 - (1 + abs((((hour+gamedata->playerx)/WIND_SPEED_SECTOR_SIZE) + ((hour+gamedata->playery)/WIND_SPEED_SECTOR_SIZE) * hour)%3));
    }
    else{
      //head right
      currentspeed = 1 + abs((((hour+gamedata->playerx)/WIND_SPEED_SECTOR_SIZE) + ((hour+gamedata->playery)/WIND_SPEED_SECTOR_SIZE) * hour)%4);
    }
  }
  else{
    //no wind
    currentspeed = 0;
  }
  currentspeed = abs(currentspeed);
  
  return currentspeed;
}
