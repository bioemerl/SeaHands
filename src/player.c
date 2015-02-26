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
  gamedata->playeryvelocity = gamedata->currentspeed;
  gamedata->playerx = 50;
  gamedata->playery = 50;
  //initiate player cargo
  gamedata->playercargo[0] = 0;
  gamedata->playercargo[1] = 10;
  gamedata->playercargo[2] = 0;
  gamedata->playercargo[3] = 10;
  gamedata->playerwallet = 500;
  //gamedata->maxplayercargo = BASE_PLAYER_CARGO;
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
  if(gamedata->menulayer == 0)
    menuzeroupdate(gamedata);
  if(gamedata->menulayer == 1)
    menuoneupdate(gamedata); 
  if(gamedata->menulayer == 2)
    menutwoupdate(gamedata);
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
   //if exit is highlighted leave
    if(gamedata->currentmenu[0] == 4 && buttonpress == 3 && gamedata->buttonrelease == 1){
      gamedata->menulayer = 2;
      gamedata->buttonrelease = 0;
    }
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
      
      gamedata->currentmenu[0] = 0;
      gamedata->gamemode = 'p';
    }
}

void menuoneupdate(GameData* gamedata){
  int buttonpress = check_current_button(gamedata);
  updatemenuselection(gamedata, 1, MENU2ITEMSCNT);

  //SELETIONS FOR BUY AND SELL IF LAYER IS ONE
  //IF THE FIRST IS SELECTED
  if(gamedata->currentmenu[0] == 0 && gamedata->currentmenu[1] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    buysellresources(gamedata, 0, 0, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 0  && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 1, 0, gamedata->playerisland, gamedata->currentcosts);
  }
  //IF SECOND IS SELECTED
  if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    buysellresources(gamedata, 0, 1, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 1, 1, gamedata->playerisland, gamedata->currentcosts);
  }
  //IF THIRD IS SELETED
  if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    buysellresources(gamedata, 0, 2, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 1, 2, gamedata->playerisland, gamedata->currentcosts);
  }
  //IF FOURTH IS SELECTED buy item
  if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    buysellresources(gamedata, 0, 3, gamedata->playerisland, gamedata->currentcosts);
  } // sell item back
  if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 1 && buttonpress == 3){
    buysellresources(gamedata, 1, 3, gamedata->playerisland, gamedata->currentcosts);
  }
  //IF SECOND MENU THIRD ITEM IS SELETED, REGARDLESS OF FIRST LEVEL, GO BACK TO FIRST MENU
  if(gamedata->currentmenu[1] == 2 && buttonpress == 3){
    gamedata->menulayer = 0;
    gamedata->currentmenu[1] = 0;
    gamedata->buttonrelease = 0;
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
    int upgradeprice = check_player_upgrade_price(gamedata, 0);
    if(gamedata->playerwallet >= upgradeprice){
      gamedata->cargolevel++;
      gamedata->maxplayercargo += 5;
      gamedata->playerwallet += -upgradeprice;
    }
  }
  if(gamedata->currentmenu[2] == 1 && buttonpress == 3 && gamedata->buttonrelease == 1){
    gamedata->buttonrelease = 0;
    int upgradeprice = check_player_upgrade_price(gamedata, 1);
    if(gamedata->playerwallet >= upgradeprice){
      gamedata->currentspeed++;
      gamedata->speedlevel++;
      gamedata->playerwallet += -upgradeprice;
    }
  }
  if(gamedata->currentmenu[2] == 2 && buttonpress == 3){
    gamedata->menulayer = 0;
    gamedata->currentmenu[2] = 0;
    gamedata->buttonrelease = 0;
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
  if(buttonhit == 3){
    gamedata->playerx += gamedata->playerxvelocity;
    gamedata->playery += gamedata->playeryvelocity;
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
}

