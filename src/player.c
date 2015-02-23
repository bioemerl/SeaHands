#include <pebble.h>
#include "player.h"
#include "types.h"
#include "game.h"
  
const int BASE_VALUE_UPGRADESPEED = 100;
const int BASE_VALUE_UPGRADECARGO = 10;
const int BASE_PLAYER_SPEED = 4;
const int BASE_PLAYER_CARGO = 10;

void initialize_player(GameData* gamedata){
  //initiate player values
  gamedata->playerxvelocity = 0;
  gamedata->playeryvelocity = BASE_PLAYER_SPEED;
  gamedata->playerx = 50;
  gamedata->playery = 50;
  //initiate player cargo
  gamedata->playercargo[0] = 0;
  gamedata->playercargo[1] = 10;
  gamedata->playercargo[2] = 0;
  gamedata->playercargo[3] = 10;
  gamedata->playerwallet = 500;
  gamedata->maxplayercargo = BASE_PLAYER_CARGO;
}

void burn_player_cargo(GameData* gamedata){
  if(random(3) == 2){
    gamedata->playercargo[1] += -1; //burn wood
    if(gamedata->playercargo[1] < 0){
      initialize_player(gamedata);
    }
  }
  if(random(5) == 4){
    gamedata->playercargo[3] += -1; //burn food
  }
}

void update_player(GameData* gamedata){
  if(gamedata->gamemode == 'p')//if gamemode is overworld play
    update_player_movement(gamedata);
  
  if(gamedata->gamemode == 'm')//if gamemode is menus
    update_player_menu(gamedata);    
  
  
}

//returns 1 for up, 2 for down, and 3 for select
int check_current_button(GameData* gamedata){
  if(gamedata->uphit == 1 && gamedata->downhit == 0)
    return 1;
  if(gamedata->uphit == 0 && gamedata->downhit == 1)
    return 2;
  if(gamedata->uphit == 1 && gamedata->downhit == 1)
    return 3;
  return 0;
}

void update_player_menu(GameData* gamedata){
  if(gamedata->menulayer == 0 && gamedata->downhit == 1 && gamedata->uphit == 0 && gamedata->buttonrelease == 1){
      APP_LOG(APP_LOG_LEVEL_INFO, "RANLAYERZERO");
      gamedata->currentmenu[0] = ((gamedata->currentmenu[0] + 1) % MENUITEMSCNT);
      gamedata->buttonrelease = 0;
    }
    //DOWN PRESSED IN MENU LAYER ONE
    if(gamedata->menulayer == 1 && gamedata->downhit == 1 && gamedata->uphit == 0 && gamedata->buttonrelease == 1){
      gamedata->currentmenu[1] = ((gamedata->currentmenu[1] + 1) % MENU2ITEMSCNT);
      gamedata->buttonrelease = 0;
    }
    
    //UP PRESSED MENU LAYER 0
    if(gamedata->menulayer == 0 && gamedata->uphit == 1 && gamedata->downhit == 0 && gamedata->buttonrelease == 1){
      gamedata->currentmenu[0] = ((gamedata->currentmenu[0] - 1) % MENUITEMSCNT);
      gamedata->buttonrelease = 0;
      if(gamedata->currentmenu[0] < 0)
        gamedata->currentmenu[0] = MENUITEMSCNT - 1;
    }
    //UP PRESSED MENU LAYER 1
    if(gamedata->menulayer == 1 && gamedata->uphit == 1 && gamedata->downhit == 0 && gamedata->buttonrelease == 1){
      gamedata->currentmenu[1] = ((gamedata->currentmenu[1] - 1) % MENUITEMSCNT);
      gamedata->buttonrelease = 0;
      if(gamedata->currentmenu[1] < 0)
        gamedata->currentmenu[1] = MENU2ITEMSCNT - 1;
    }
    
    //IF SELECT IS HIT AND MENULAYER IS 0
    if(gamedata->currentmenu[0] >=0 && gamedata->currentmenu[0] <= 3 && gamedata->menulayer == 0
       && gamedata->uphit == 1 && gamedata->downhit == 1 && gamedata->buttonrelease == 1){
      gamedata->menulayer = 1;
      gamedata->buttonrelease = 1;
    }
    
    
    
    //SELETIONS FOR BUY AND SELL IF LAYER IS ONE
    if(gamedata->menulayer == 1){
      //IF THE FIRST IS SELECTED
      if(gamedata->currentmenu[0] == 0 && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[0] < gamedata->maxplayercargo && gamedata->islandscargo[gamedata->playerisland][0] > 0 && gamedata->playerwallet > gamedata->currentcosts){
        gamedata->islandscargo[gamedata->playerisland][0] += -1;
        gamedata->playercargo[0] += 1;
        gamedata->playerwallet = gamedata->playerwallet - gamedata->currentcosts;
      } // sell item back
      if(gamedata->currentmenu[0] == 0  && gamedata->currentmenu[1] == 1 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[0] > 0 && gamedata->islandscargo[gamedata->playerisland][0] < 100){
        gamedata->islandscargo[gamedata->playerisland][0] += 1;
        gamedata->playercargo[0] += -1;
        gamedata->playerwallet = gamedata->playerwallet + gamedata->currentcosts;
      }
      //IF SECOND IS SELECTED
      if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[1] < gamedata->maxplayercargo && gamedata->islandscargo[gamedata->playerisland][1] > 0 && gamedata->playerwallet > gamedata->currentcosts){
        gamedata->islandscargo[gamedata->playerisland][1] += -1;
        gamedata->playercargo[1] += 1;
        gamedata->playerwallet = gamedata->playerwallet - gamedata->currentcosts;
      } // sell item back
      if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 1 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[1] > 0 && gamedata->islandscargo[gamedata->playerisland][1] < 100){
        gamedata->islandscargo[gamedata->playerisland][1] += 1;
        gamedata->playercargo[1] += -1;
        gamedata->playerwallet = gamedata->playerwallet + gamedata->currentcosts;
      }
      //IF THIRD IS SELETED
      if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[2] < gamedata->maxplayercargo && gamedata->islandscargo[gamedata->playerisland][2] > 0 && gamedata->playerwallet > gamedata->currentcosts){
        gamedata->islandscargo[gamedata->playerisland][2] += -1;
        gamedata->playercargo[2] += 1;
        gamedata->playerwallet = gamedata->playerwallet - gamedata->currentcosts;
      } // sell item back
      if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 1 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[2] > 0 && gamedata->islandscargo[gamedata->playerisland][2] < 100){
        gamedata->islandscargo[gamedata->playerisland][2] += 1;
        gamedata->playercargo[2] += -1;
        gamedata->playerwallet = gamedata->playerwallet + gamedata->currentcosts;
      }
      //IF FOURTH IS SELECTED buy item
      if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[3] < gamedata->maxplayercargo && gamedata->islandscargo[gamedata->playerisland][3] > 0 && gamedata->playerwallet > gamedata->currentcosts){
        gamedata->islandscargo[gamedata->playerisland][3] += -1;
        gamedata->playercargo[3] += 1;
        gamedata->playerwallet = gamedata->playerwallet - gamedata->currentcosts;
      } // sell item back
      if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 1 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[3] > 0 && gamedata->islandscargo[gamedata->playerisland][3] < 100){
        gamedata->islandscargo[gamedata->playerisland][3] += 1;
        gamedata->playercargo[3] += -1;
        gamedata->playerwallet = gamedata->playerwallet + gamedata->currentcosts;
      }
      //IF SECOND MENU THIRD ITEM IS SELETED, REGARDLESS OF FIRST LEVEL, GO BACK TO FIRST MENU
      if(gamedata->currentmenu[1] == 2 && gamedata->uphit == 1 && gamedata->downhit == 1){
        gamedata->menulayer = 0;
        gamedata->currentmenu[1] = 0;
        gamedata->buttonrelease = 0;
      }
    }
    //if up is pressed, 
    //if exit is highlighted leave
    if(gamedata->currentmenu[0] == 4 && gamedata->uphit == 1 && gamedata->downhit == 1){
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
      
      gamedata->currentmenu[0] = 0;
      gamedata->gamemode = 'p';
    }
}

void update_player_movement(GameData* gamedata){
  int buttonhit = check_current_button(gamedata);
  //UP TRIGGER
  if(buttonhit == 1){    
    //move the velocity in a square
    if(gamedata->playerxvelocity >=0 && gamedata->playeryvelocity >=0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity--;
    }
    if(gamedata->playerxvelocity >=0 && gamedata->playeryvelocity <=0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity--;
    }
    if(gamedata->playerxvelocity <=0 && gamedata->playeryvelocity <=0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity++;
    }
    if(gamedata->playerxvelocity <=0 && gamedata->playeryvelocity >=0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity++;
    }
      
    //APP_LOG(APP_LOG_LEVEL_INFO, "up");
    //gamedata->playerxvelocity++;
  }
  
  //DOWN TRIGGER
  if(buttonhit == 2){
    //move the velocity in a counterclockwise square
    if(gamedata->playerxvelocity >=0 && gamedata->playeryvelocity >=0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity++;
    }
    if(gamedata->playerxvelocity >=0 && gamedata->playeryvelocity <=0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity++;
    }
    if(gamedata->playerxvelocity <=0 && gamedata->playeryvelocity <=0){
      gamedata->playerxvelocity++;
      gamedata->playeryvelocity--;
    }
    if(gamedata->playerxvelocity <=0 && gamedata->playeryvelocity >=0){
      gamedata->playerxvelocity--;
      gamedata->playeryvelocity--;
    }
  }
  
  //SELECT TRIGGER
  if(buttonhit == 3){
    gamedata->playerx += gamedata->playerxvelocity;
    gamedata->playery += gamedata->playeryvelocity;
  }
  
  //if player colides with an island, set the game-mode to showing the menu
  for(int i = 0; i < TOTALISLANDS; i++){
    if(finddistance(gamedata->playerx, gamedata->playery, gamedata->islandsx[i], gamedata->islandsy[i]) <= 25*25){ //(size of island)
      gamedata->playerisland = i;
      if(gamedata->gamemode == 'p')
      gamedata->menulayer = 0;
      gamedata->gamemode = 'm';
      break;
    }
  }
}

int8_t check_for_player_upgrade(GameData* gamedata){
  return 0;
}

int8_t check_player_upgrade_price(GameData* gamedata){
  return 0;
}
