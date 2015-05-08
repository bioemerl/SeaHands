#include <pebble.h>
#include "player.h"
#include "types.h"
#include "game.h"
#include "ship.h"
  
const int BASE_VALUE_UPGRADESPEED = 100;
const int BASE_VALUE_UPGRADECARGO = 10;
const int BASE_PLAYER_SPEED = 4;
const int BASE_PLAYER_CARGO = 10;
const int WIND_SPEED_SECTOR_SIZE = 100;


void initialize_player(GameData* gamedata){
  //kill all layers
  exitmenus(gamedata);
  
  gamedata->menulayer = 0;
  gamedata->gamemode = 'p';
  //initiate player values
  gamedata->playerxvelocity = 0;
  gamedata->playeryvelocity = gamedata->currentspeed;
  gamedata->playerx = 50;
  gamedata->playery = 50;
  //initiate player cargo
  gamedata->playercargo[0] = 40;
  gamedata->playercargo[1] = 40;
  gamedata->playercargo[2] = 40;
  gamedata->playercargo[3] = 40;
  gamedata->playercargo[4] = 5;
  gamedata->currentwindspeed = calculatewindspeed(gamedata);
  gamedata->playersailsdeployed = 0;
  //gamedata->maxplayercargo = BASE_PLAYER_CARGO;
}

void burn_player_cargo(GameData* gamedata){
  //APP_LOG(APP_LOG_LEVEL_INFO, "Speed = %i: X: %i Y: %i", calculatewindspeed(gamedata), gamedata->playerx, gamedata->playery);
  if(random(5) == 2){
    gamedata->playercargo[4] += -1;//burn supplies
    if(gamedata->playercargo[4] == 1)
      displaynotification(gamedata, "\n\nSupplies are low!");
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
  if(gamedata->gamemode == 'w'){//if gamemode is world map
    update_worldmap(gamedata);
    update_player_movement(gamedata);
  }
}

void update_worldmap(GameData* gamedata){
  if(gamedata->downhit == 1 && gamedata->uphit == 1 && gamedata->buttonrelease == 1)
    gamedata->gamemode = 'p';
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
  if(gamedata->menulayer == 4)
    menufourupdate(gamedata);
  if(gamedata->menulayer == 5)
    menufiveupdate(gamedata);
  if(gamedata->menulayer == 6)
    menusixupdate(gamedata);
  if(gamedata->menulayer == 7)
    menusevenupdate(gamedata);
  if(gamedata->menulayer == 8)
    menueightupdate(gamedata);
  if(gamedata->menulayer == 9)
    menunineupdate(gamedata);
  if(gamedata->menulayer == 10)
    menutenupdate(gamedata);
  if(gamedata->menulayer == 11)
    menuelevenupdate(gamedata);
  if(gamedata->menulayer == 12)
    menutwelveupdate(gamedata);
  if(gamedata->menulayer == 13)
    menuthirteenupdate(gamedata);
  if(gamedata->menulayer == 14)
    menufourteenupdate(gamedata);
     
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
    
    //if the island does not like the player, do not allow trade to happen.
    
    if(manageislandallegiance(gamedata, gamedata->playerisland, 0) > -10){
      //IF SELECT IS HIT AND MENULAYER IS 0 go into the second menu layer
      //the second menu layer checks which layer this menu is currently on.  Menu 0 and 1 are tied
      //together
      if(gamedata->currentmenu[0] >=0 && gamedata->currentmenu[0] <= 3 && gamedata->menulayer == 0
         && gamedata->uphit == 1 && gamedata->downhit == 1 && gamedata->buttonrelease == 1){
        gamedata->menulayer = 1;
        gamedata->buttonrelease = 0;
       }
    }
    if(gamedata->currentmenu[0] == 4 && buttonpress == 3 && gamedata->buttonrelease == 1){
      gamedata->menulayer = 2;
      gamedata->buttonrelease = 0;
    }
    
    //if exit is highlighted leave
    if(gamedata->currentmenu[0] == 5 && buttonpress == 3){
      exitisland(gamedata);
      gamedata->playersailsdeployed = 0;
      gamedata->currentmenu[0] = 0;
      gamedata->gamemode = 'p';
      gamedata->buttonrelease = 0;
    }
}

void menuoneupdate(GameData* gamedata){ //buy sell metal wood stone food from islands
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
      if(random(6) == 3)
        manageislandallegiance(gamedata, gamedata->playerisland, 1);
    }
  }
  if(buyorsell == 1){
    if(gamedata->playercargo[resource] > 0 && gamedata->islandscargo[islandnumber][resource] < 100){
      gamedata->islandscargo[gamedata->playerisland][resource] += 1;
      gamedata->playercargo[resource] += -1;
      gamedata->playerwallet = gamedata->playerwallet + resourcecost;
      if(random(6) == 3)
        manageislandallegiance(gamedata, gamedata->playerisland, 1);
    }
  }
}

void menutwoupdate(GameData* gamedata){ //the update menu layer
  int buttonpress = check_current_button(gamedata);
  updatemenuselection(gamedata, 2, MENU3ITEMSCNT);
  //if the island does not like the player, do not allow trade to happen.
    
  if(manageislandallegiance(gamedata, gamedata->playerisland, 0) > -10){
    if(gamedata->currentmenu[2] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
      gamedata->buttonrelease = 0;
      if(gamedata->playerwallet >= BASE_PRICE_SUPPLIES && gamedata->playercargo[4] < 10 
         && gamedata->playercargo[3] > 0 && gamedata->playercargo[1] > 0){
        gamedata->playercargo[4]++;
        gamedata->playerwallet += -BASE_PRICE_SUPPLIES;
        gamedata->playercargo[3]--;
        gamedata->playercargo[1]--;
        if(random(6) == 3)
          manageislandallegiance(gamedata, gamedata->playerisland, 1);
      }
    }
    if(gamedata->currentmenu[2] == 1 && buttonpress == 3 && gamedata->buttonrelease == 1){
     gamedata->buttonrelease = 0;
      int upgradeprice = check_player_upgrade_price(gamedata, 0);
      if(gamedata->playerwallet >= upgradeprice && gamedata->cargolevel < 98){
        gamedata->cargolevel++;
        gamedata->maxplayercargo += 2;
        gamedata->playerwallet += -upgradeprice;
        manageislandallegiance(gamedata, gamedata->playerisland, 1);
      } 
    }
    if(gamedata->currentmenu[2] == 2 && buttonpress == 3){
      int upgradeprice = check_player_upgrade_price(gamedata, 1);
      if(gamedata->playerwallet >= upgradeprice){
        gamedata->currentspeed++;
        gamedata->speedlevel++;
        gamedata->playerwallet += -upgradeprice;
        manageislandallegiance(gamedata, gamedata->playerisland, 1);
      }
    }
    if(gamedata->currentmenu[2] == 3 && buttonpress == 3){
      int upgradeprice = check_player_upgrade_price(gamedata, 1);
      if(gamedata->playerwallet >= upgradeprice && gamedata->currentspeed > 4){
        gamedata->currentspeed--;
        gamedata->speedlevel--;
        gamedata->playerwallet += -upgradeprice;
        manageislandallegiance(gamedata, gamedata->playerisland, 1);
      }
    }
  }
  if(gamedata->currentmenu[2] == 4 && buttonpress == 3){
    gamedata->menulayer = 0;
    gamedata->currentmenu[2] = 0;
    gamedata->buttonrelease = 0;
  }
}

void menuthreeupdate(GameData* gamedata){ //the pillage menu
  int buttonpress = check_current_button(gamedata);
  updatemenuselection(gamedata, 3, MENU4ITEMSCNT);
  if(gamedata->currentmenu[3] == 0 && buttonpress == 3 && gamedata->buttonrelease == 1){
    //exit menu
    exitmenus(gamedata);
  }
  if(gamedata->currentmenu[3] == 1 && buttonpress == 3 && gamedata->buttonrelease == 1){
    //pillage
    gamedata->gamemodeswitchflag1 = 'p';
    gamedata->gamemode = 'b';
  }
}

void menufourupdate(GameData* gamedata){ // the notifications menu
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->buttonrelease == 1){
    gamedata->gamemode = 'p';
    gamedata->menulayer = 0;
    gamedata->currentmenu[4] = 0;
  }
}

void menufiveupdate(GameData* gamedata){ //player menu that can be accessed anywhere
  updatemenuselection(gamedata, 5, MENU6ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[5] == 0 && gamedata->buttonrelease == 1){
    //open the worldmap
    gamedata->gamemode = 'w';
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[5] == 1){
    //show what the current event is
    gamedata->currentmenu[5] = 0;
    gamedata->buttonrelease = 0;
    char eventname[100];
    char eventtitle[20];
    //set eventtitle to whatever the current event currently is.
    if(gamedata->currentevent == 'm')
      snprintf(eventtitle, sizeof(eventtitle), "metal shortage");
    if(gamedata->currentevent == 'w')
      snprintf(eventtitle, sizeof(eventtitle), "wood shortage");
    if(gamedata->currentevent == 's')
      snprintf(eventtitle, sizeof(eventtitle), "stone shortage");
    if(gamedata->currentevent == 'f')
      snprintf(eventtitle, sizeof(eventtitle), "food shortage");
    if(gamedata->currentevent == 'n')
      snprintf(eventtitle, sizeof(eventtitle), "none");
    //put the event title into the rest of the string that displays to the player
    snprintf(eventname, sizeof(eventname), "The current event is %s", eventtitle);
    //trigger a notification
    displaynotification(gamedata, eventname);
  }
  if(buttonpress == 3 && gamedata->currentmenu[5] == 2){
    gamedata->tutorialcounter = -1;
    exitmenus(gamedata);
    
  }
  if(buttonpress == 3 && gamedata->currentmenu[5] == 3){ //the build menu, use to build storage island
    //want to:
    //place 11'th island
    //if the island created flag exists, then do nothing
    //if the island created flag does not exist, than set the island x and y to something other than MAX16BITNUMBER 
    //when updating islands at first, check for collisions with the 11'th island if the x and y are not
    //the max values. 
    //if the player collides with the 11'th island, go into a different menu than with normal islands
    //otherwise the island shouldn't be counted as a regular one at all
    //when the island flag isn't set, the island will only allow you to put resources into it
    //once the island reaches X amount of wood/stone/metal/food it will set the flag to that the isalnd exists
    //and all menu options will be avalible.
    
    //this function should:
    //set the X and Y to non-max values
    //move the player outside of island collision zones.
    gamedata->islandsx[10] = gamedata->playerx;
    gamedata->islandsy[10] = gamedata->playery;
    gamedata->playerx = gamedata->playerx + 25;
    exitmenus(gamedata);
    
    
  }
  if(buttonpress == 3 && gamedata->currentmenu[5] == 4){
    gamedata->currentmenu[5] = 0;
    gamedata->menulayer = 0;
    gamedata->gamemode = 'p';
  }
  
  
}

void menusixupdate(GameData* gamedata){ //the storage core layer
  updatemenuselection(gamedata, 6, MENU7ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[6] == 0 && gamedata->buttonrelease == 1){ //resources
    gamedata->menulayer = 7; //go to resources menu
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[6] == 1  && gamedata->buttonrelease == 1 && gamedata->storageexists == 1){ //ships
    //go to ships menu
    gamedata->menulayer = 9; 
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[6] == 2  && gamedata->buttonrelease == 1){ //money
    gamedata->menulayer = 14;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[6] == 3){ //exit
    gamedata->currentmenu[6] = 0;
    gamedata->menulayer = 0;
    gamedata->gamemode = 'p';
    exitisland(gamedata);
  }
}
  
void menusevenupdate(GameData* gamedata){ //the resource selection for storage and removal
  updatemenuselection(gamedata, 7, MENU8ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[7] == 0 && gamedata->buttonrelease == 1){
    gamedata->menulayer = 8; //go to store/drop menu
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[7] >= 1 && gamedata->currentmenu[7] <= 3  && gamedata->buttonrelease == 1){
    gamedata->menulayer = 8; //go to store/drop menu
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[7] == 4){
    gamedata->currentmenu[7] = 0;
    gamedata->buttonrelease = 0;
    gamedata->menulayer = 6; //go to previous menu
  }
}

  
void menueightupdate(GameData* gamedata){ //the store/drop menu layer
  updatemenuselection(gamedata, 8, MENU9ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[8] == 0 && gamedata->buttonrelease == 1){
    gamedata->currentmenu[8] = 0; //exit
    gamedata->menulayer = 7;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[8] == 1){ //store
    if(gamedata->currentmenu[7] == 0){ //store resources based on previous menu
      //store 1 metal
      if(gamedata->playercargo[0] > 0 && gamedata->islandscargo[10][0] < 100){
        gamedata->islandscargo[10][0] += 1;
        gamedata->playercargo[0] -= 1;
      }

    }
    if(gamedata->currentmenu[7] == 1){
      //store 1 wood
      if(gamedata->playercargo[1] > 0 && gamedata->islandscargo[10][1] < 100){
        gamedata->islandscargo[10][1] += 1;
        gamedata->playercargo[1] -= 1;
      }
    }
    if(gamedata->currentmenu[7] == 2){
      //store 1 stone
      if(gamedata->playercargo[2] > 0 && gamedata->islandscargo[10][2] < 100){
        gamedata->islandscargo[10][2] += 1;
        gamedata->playercargo[2] -= 1;
      }
    }
    if(gamedata->currentmenu[7] == 3){
      //store 1 food
      if(gamedata->playercargo[3] > 0 && gamedata->islandscargo[10][3] < 100){
        gamedata->islandscargo[10][3] += 1;
        gamedata->playercargo[3] -= 1;
      }
    }
    //check to see if the resources total to an amount that allows the island to unlock
    if(gamedata->storageexists == 0){ //if island isn't yet built
      if(gamedata->islandscargo[10][0] >= 40 && gamedata->islandscargo[10][1] >= 40 &&
        gamedata->islandscargo[10][2] >= 40 && gamedata->islandscargo[10][3] >= 40){
        gamedata->storageexists = 1;
        for(int i = 0; i < 4; i++) //remove appropriate amount of resources
          gamedata->islandscargo[10][i] -= 40;
      }
              
    }
  }
  if(buttonpress == 3 && gamedata->currentmenu[8] == 2){ //take
    if(gamedata->currentmenu[7] == 0){ //take resource based on state of previous menu
      //take 1 metal
      if(gamedata->playercargo[0] < gamedata->maxplayercargo && gamedata->islandscargo[10][0] > 0){
        gamedata->islandscargo[10][0] -= 1;
        gamedata->playercargo[0] += 1;
      }
    }
    if(gamedata->currentmenu[7] == 1){
      //take 1 wood
      if(gamedata->playercargo[1] < gamedata->maxplayercargo && gamedata->islandscargo[10][1] > 0){
        gamedata->islandscargo[10][1] -= 1;
        gamedata->playercargo[1] += 1;
      }
    }
    if(gamedata->currentmenu[7] == 2){
      //take 1 stone
      if(gamedata->playercargo[2] < gamedata->maxplayercargo && gamedata->islandscargo[10][2] > 0){
        gamedata->islandscargo[10][2] -= 1;
        gamedata->playercargo[2] += 1;
      }
    }
    if(gamedata->currentmenu[7] == 3){
      //take 1 food
      if(gamedata->playercargo[3] < gamedata->maxplayercargo && gamedata->islandscargo[10][3] > 0){
        gamedata->islandscargo[10][3] -= 1;
        gamedata->playercargo[3] += 1;
      }
    }
  }
  
}
  
void menunineupdate(GameData* gamedata){ //order buy sell menu for ships
  updatemenuselection(gamedata, 9, MENU10ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[9] == 0 && gamedata->buttonrelease == 1){ //order a ship to buy resources
    gamedata->menulayer = 13;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[9] == 1 && gamedata->buttonrelease == 1){ //order a ship to sell resources
    gamedata->menulayer = 10;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[9] == 2 && gamedata->buttonrelease == 1){ //go to order new ship menu
    gamedata->menulayer = 10;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[9] == 3){ //back
    gamedata->menulayer = 6; //go back a menu
    gamedata->currentmenu[9] = 0; //reset this menu
    gamedata->buttonrelease = 0; //ensure no accidental presses occur
  }
  
}
  
void menutenupdate(GameData* gamedata){ //ship selection
  updatemenuselection(gamedata, 10, gamedata->numberofplayerships + 1); //only as many items as there are ships
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[10] == 0  && gamedata->buttonrelease == 1){
    gamedata->menulayer = 9; //go back a layer
    gamedata->currentmenu[10] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[10] == 1){ //select ship 1
    gamedata->currentplayership = find_owned_ship(gamedata, 11);
    gamedata->menulayer = 11; //go to island selection menu
    gamedata->currentmenu[10] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[10] == 2){ //select ship 2
    gamedata->currentplayership = find_owned_ship(gamedata, 12);
    gamedata->menulayer = 11;
    gamedata->currentmenu[10] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[10] == 3){ //select ship 3
    gamedata->currentplayership = find_owned_ship(gamedata, 13);
    gamedata->menulayer = 11;
    gamedata->currentmenu[10] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[10] == 4){ //select ship 4
    gamedata->currentplayership = find_owned_ship(gamedata, 14);
    gamedata->menulayer = 11;
    gamedata->currentmenu[10] = 0;
    gamedata->buttonrelease = 0;
  }
  
}
  
void menuelevenupdate(GameData* gamedata){ //select island
  updatemenuselection(gamedata, 11, MENU12ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[11] < 10 && gamedata->buttonrelease == 1){
    gamedata->menulayer = 12; //progress to resource selection menu
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[11] == 10){
    gamedata->menulayer = 10; //go back a layer
    gamedata->currentmenu[11] = 0;
    gamedata->buttonrelease = 0;
  }
}
  
void menutwelveupdate(GameData* gamedata){ //select the resource
  updatemenuselection(gamedata, 12, MENU13ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  //order buy sell
  //ship
  //island
  //the three bits of info that have to be considered
  //buy and sell are number 1 and 2
  //ships are 1 2 3 4
  //island are 0 1 2 3 4 5 6 7 8 9
  //metal stone food wood is 0 1 2 3 
  //void give_ship_order(GameData* gamedata, int8_t ownernumber, int8_t shipnumber, char ordercharacter, int8_t orderinfo[3]);
  char shiporder = 's';
  int8_t orderinfo[3];
  if(gamedata->currentmenu[9] == 1){
    orderinfo[0] = gamedata->currentmenu[11]; //buy from where
    orderinfo[1] = 10; //delivery to where
  }
  if(gamedata->currentmenu[9] == 2){
    orderinfo[1] = gamedata->currentmenu[11];
    orderinfo[0] = 10;
  }
  
  if(buttonpress == 3 && gamedata->currentmenu[12] == 0 && gamedata->buttonrelease == 1){ //metal
    orderinfo[2] = gamedata->currentmenu[12]; //which resources
    give_ship_order(gamedata, gamedata->shipsowner[gamedata->currentplayership], gamedata->currentplayership, shiporder, orderinfo);
    gamedata->menulayer = 6;
    gamedata->currentmenu[12] = 0;
    gamedata->currentmenu[11] = 0;
    gamedata->currentmenu[10] = 0;
    gamedata->currentmenu[9] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[12] == 1){ //stone
    orderinfo[2] = gamedata->currentmenu[12]; //which resources
    give_ship_order(gamedata, gamedata->shipsowner[gamedata->currentplayership], gamedata->currentplayership, shiporder, orderinfo);
    gamedata->menulayer = 6;
    gamedata->currentmenu[12] = 0;
    gamedata->currentmenu[11] = 0;
    gamedata->currentmenu[10] = 0;
    gamedata->currentmenu[9] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[12] == 2){ //wood
    orderinfo[2] = gamedata->currentmenu[12]; //which resources
    give_ship_order(gamedata, gamedata->shipsowner[gamedata->currentplayership], gamedata->currentplayership, shiporder, orderinfo);
    gamedata->menulayer = 6;
    gamedata->currentmenu[12] = 0;
    gamedata->currentmenu[11] = 0;
    gamedata->currentmenu[10] = 0;
    gamedata->currentmenu[9] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[12] == 3){ //food
    orderinfo[2] = gamedata->currentmenu[12]; //which resources
    give_ship_order(gamedata, gamedata->shipsowner[gamedata->currentplayership], gamedata->currentplayership, shiporder, orderinfo);
    gamedata->menulayer = 6;
    gamedata->currentmenu[12] = 0;
    gamedata->currentmenu[11] = 0;
    gamedata->currentmenu[10] = 0;
    gamedata->currentmenu[9] = 0;
    gamedata->buttonrelease = 0;
  }
  if(buttonpress == 3 && gamedata->currentmenu[12] == 4){ //back
    gamedata->menulayer = 11;
    gamedata->currentmenu[12] = 0;
    gamedata->buttonrelease = 0;
  }
}
  
void menuthirteenupdate(GameData* gamedata){ //select to buy or sell ships 
  updatemenuselection(gamedata, 13, MENU14ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[13] == 0 && gamedata->buttonrelease == 1){
    gamedata->buttonrelease = 0;
    if(gamedata->numberofplayerships < 4 && gamedata->islandscargo[10][1] >= 10 && gamedata->islandscargo[10][2] >= 10){
      //create a ship
      gamedata->numberofplayerships += 1;
      create_ship(gamedata, 10 + gamedata->numberofplayerships);
      gamedata->islandscargo[10][1] -= 10;
      gamedata->islandscargo[10][2] -= 10;
    }
  }
  if(buttonpress == 3 && gamedata->currentmenu[13] == 1 && gamedata->buttonrelease == 1){
    gamedata->buttonrelease = 0;
    if(gamedata->numberofplayerships > 0){
      int shipnumber = find_owned_ship(gamedata, 10 + gamedata->numberofplayerships);
      destroy_ship(gamedata, shipnumber);
      gamedata->numberofplayerships -= 1;
      gamedata->islandscargo[10][1] += 7;
      gamedata->islandscargo[10][2] += 7;
    }
  }
  if(buttonpress == 3 && gamedata->currentmenu[13] == 2){
    gamedata->menulayer = 9;
    gamedata->currentmenu[13] = 0;
    gamedata->buttonrelease = 0;
  }
}

void menufourteenupdate(GameData* gamedata){ //store or take money from core island
  updatemenuselection(gamedata, 14, MENU15ITEMSCNT);
  int buttonpress = check_current_button(gamedata);
  if(buttonpress == 3 && gamedata->currentmenu[14] == 0 && gamedata->buttonrelease == 1){ //withdraw
    if(gamedata->playerwallet <= 10000 && gamedata->storagewallet >= 10){
      gamedata->storagewallet -= 10;
      gamedata->playerwallet += 10;
    }
  }
  if(buttonpress == 3 && gamedata->currentmenu[14] == 1){ //deposit
    if(gamedata->playerwallet >= 10 && gamedata->storagewallet <= 10000){
      gamedata->playerwallet -= 10;
      gamedata->storagewallet += 10;
    }
  }
  if(buttonpress == 3 && gamedata->currentmenu[14] == 2){ //back
    gamedata->menulayer = 6;
    gamedata->currentmenu[14] = 0;
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
    if(finddistance(gamedata->playerx, gamedata->playery, gamedata->islandsx[i], gamedata->islandsy[i]) <= 27*27){ //(size of island)
      gamedata->playerisland = i;
      gamedata->buttonrelease = 0;
      if(gamedata->gamemode == 'p')
      gamedata->menulayer = 0;
      gamedata->gamemode = 'm';
      break;
    }
  }
  if(gamedata->islandsx[10] != MAX16BITVALUE && gamedata->islandsy[10] != MAX16BITVALUE){ //update the storage island
    if(finddistance(gamedata->playerx, gamedata->playery, gamedata->islandsx[10], gamedata->islandsy[10]) <= 12*12){ //(size of island)
      gamedata->playerisland = 10;
      gamedata->buttonrelease = 0;
      if(gamedata->gamemode == 'p')
      gamedata->menulayer = 6; //set the menu layer to the base menu for the storage island
      gamedata->gamemode = 'm';
    }
  }
  //if player collides with a ship, initialize ship menu menu, unless it is an attack ship
  for(int i = 0; i <= gamedata->totalships; i++){
    //APP_LOG(APP_LOG_LEVEL_INFO, "TEST: %i", (finddistance(gamedata->playerx, gamedata->playery, gamedata->shipsx[i], gamedata->shipsy[i]) <= 8*8));
    if(finddistance(gamedata->playerx, gamedata->playery, gamedata->shipsx[i], gamedata->shipsy[i]) <= 8*8){
      //open up a menu
      if(gamedata->shipsorder[i] != 'a'){ //if ship is not attacking initialize the menu
        gamedata->playership = i;
        gamedata->gamemode = 'm';
        gamedata->menulayer = 3;
      }
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

void exitisland(GameData* gamedata){
  if(gamedata->playerx > gamedata->islandsx[gamedata->playerisland]){
    gamedata->playerx = gamedata->playerx + 10;
  }
  else{
    gamedata->playerx = gamedata->playerx - 10;
  }
  if(gamedata->playery > gamedata->islandsy[gamedata->playerisland]){
    gamedata->playery = gamedata->playery + 10;
  }
  else{
    gamedata->playery = gamedata->playery - 10;
  }
}

