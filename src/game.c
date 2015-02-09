#include <pebble.h>
#include "game.h"
#include "types.h"

static int twohundredcountdown = 200;
const int TOTALISLANDS = 8;
const int MENUITEMSCNT = 5;
const int MENU2ITEMSCNT = 3;

void update_game(GameData* gamedata){ 
  //set up a timer as to not update everything every frame
  twohundredcountdown--;
  if(twohundredcountdown <= 0)
    twohundredcountdown = 200;
  
  //adjust player's Y velocity depending on where they are on the screen.

  //APP_LOG(APP_LOG_LEVEL_INFO, "Updating Game %i", framespassed);
  //APP_LOG(APP_LOG_LEVEL_INFO, "PlayerX %i", gamedata->playerx);
  
  if(twohundredcountdown == 100){
    burn_player_cargo(gamedata);
    update_islands(gamedata);
  }
  update_ships(gamedata);
  update_player(gamedata);
      
}



void initialize_player(GameData* gamedata){
  //initiate player values
  gamedata->playerxvelocity = 0;
  gamedata->playeryvelocity = 5;
  gamedata->playerx = 50;
  gamedata->playery = 50;
  //initiate player cargo
  gamedata->playercargo[0] = 0;
  gamedata->playercargo[1] = 10;
  gamedata->playercargo[2] = 0;
  gamedata->playercargo[3] = 10;
  
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
  //UP TRIGGER
  if(gamedata->uphit == 1 && gamedata -> downhit == 0 && gamedata->gamemode == 'p'){    
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
  if(gamedata->downhit == 1 && gamedata->uphit == 0 && gamedata->gamemode == 'p'){
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
  if((gamedata->uphit) == 1 && (gamedata->downhit == 1) && gamedata->gamemode == 'p'){
    //APP_LOG(APP_LOG_LEVEL_INFO, "Select");
    //sqrt((x2-x1)^2 - (y2 - y1)^2)
    //APP_LOG(APP_LOG_LEVEL_INFO, "Distances: %i", finddistance(gamedata->playerx, gamedata->playery, gamedata->islandsx[1], gamedata->islandsy[1]));
    gamedata->playerx += gamedata->playerxvelocity;
    gamedata->playery += gamedata->playeryvelocity;
  }
  
  if(gamedata->gamemode == 'm'){ //if gamemode is menus
    
    //take up down left and right to adjust things
    //take back to leave menu
    
    //DOWN PRESSED IN MENU LAYER ZERO
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
         && gamedata->playercargo[0] < 10 && gamedata->islandscargo[gamedata->playerisland][0] > 0){
        gamedata->islandscargo[gamedata->playerisland][0] += -1;
        gamedata->playercargo[0] += 1;
      } // sell item back
      if(gamedata->currentmenu[0] == 0  && gamedata->currentmenu[1] == 1 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[0] > 0 && gamedata->islandscargo[gamedata->playerisland][0] < 100){
        gamedata->islandscargo[gamedata->playerisland][0] += 1;
        gamedata->playercargo[0] += -1;
      }
      //IF SECOND IS SELECTED
      if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[1] < 10 && gamedata->islandscargo[gamedata->playerisland][1] > 0){
        gamedata->islandscargo[gamedata->playerisland][1] += -1;
        gamedata->playercargo[1] += 1;
      } // sell item back
      if(gamedata->currentmenu[0] == 1  && gamedata->currentmenu[1] == 1 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[1] > 0 && gamedata->islandscargo[gamedata->playerisland][1] < 100){
        gamedata->islandscargo[gamedata->playerisland][1] += 1;
        gamedata->playercargo[1] += -1;
      }
      //IF THIRD IS SELETED
      if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[2] < 10 && gamedata->islandscargo[gamedata->playerisland][2] > 0){
        gamedata->islandscargo[gamedata->playerisland][2] += -1;
        gamedata->playercargo[2] += 1;
      } // sell item back
      if(gamedata->currentmenu[0] == 2  && gamedata->currentmenu[1] == 1 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[2] > 0 && gamedata->islandscargo[gamedata->playerisland][2] < 100){
        gamedata->islandscargo[gamedata->playerisland][2] += 1;
        gamedata->playercargo[2] += -1;
      }
      //IF FOURTH IS SELECTED buy item
      if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[3] < 10 && gamedata->islandscargo[gamedata->playerisland][3] > 0){
        gamedata->islandscargo[gamedata->playerisland][3] += -1;
        gamedata->playercargo[3] += 1;
      } // sell item back
      if(gamedata->currentmenu[0] == 3  && gamedata->currentmenu[1] == 0 && gamedata->uphit == 1 && gamedata->downhit == 1 
         && gamedata->playercargo[3] > 0 && gamedata->islandscargo[gamedata->playerisland][3] < 100){
        gamedata->islandscargo[gamedata->playerisland][3] += 1;
        gamedata->playercargo[3] += -1;
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

void initialize_islands(GameData* gamedata){
  //initialize island positions to a default value
  for(int i = 0; i < 10; i++){
    gamedata->islandsx[i] = -1;
    gamedata->islandsy[i] = -1;
  }
  //now initialize them to actual island positions
  gamedata->islandsx[0] = 100;
  gamedata->islandsy[0] = 100;
  //now set their cargos, and their types
  //1 - metal       2 - wood        3 - stone      4 - food
  //1 - loses wood. 2 - looses stone 3 - loses food 4 - loses metal
  gamedata->islandstypes[0] = 0;
  //now set the islands initial cargo
  gamedata->islandscargo[0][0] = 20; //metal
  gamedata->islandscargo[0][1] = 0; //wood
  gamedata->islandscargo[0][2] = 100; //stone
  gamedata->islandscargo[0][3] = 100; //food
  
  gamedata->islandsx[1] = 150;
  gamedata->islandsy[1] = 170;
  gamedata->islandstypes[1] = 1;
  gamedata->islandscargo[1][0] = 100; //metal
  gamedata->islandscargo[1][1] = 20; //wood
  gamedata->islandscargo[1][2] = 100; //stone
  gamedata->islandscargo[1][3] = 0; //food
  
  gamedata->islandsx[2] = 220;
  gamedata->islandsy[2] = 170;
  gamedata->islandstypes[2] = 2;
  gamedata->islandscargo[2][0] = 100; //metal
  gamedata->islandscargo[2][1] = 0; //wood
  gamedata->islandscargo[2][2] = 20; //stone
  gamedata->islandscargo[2][3] = 100; //food
  
  gamedata->islandsx[3] = 230;
  gamedata->islandsy[3] = 100;
  gamedata->islandstypes[3] = 3;
  gamedata->islandscargo[3][0] = 0; //metal
  gamedata->islandscargo[3][1] = 100; //wood
  gamedata->islandscargo[3][2] = 100; //stone
  gamedata->islandscargo[3][3] = 20; //food
  
  gamedata->islandsx[4] = 250;
  gamedata->islandsy[4] = 270;
  gamedata->islandstypes[4] = 0;
  gamedata->islandscargo[4][0] = 20; //metal
  gamedata->islandscargo[4][1] = 0; //wood
  gamedata->islandscargo[4][2] = 100; //stone
  gamedata->islandscargo[4][3] = 100; //food
  
  gamedata->islandsx[5] = -150;
  gamedata->islandsy[5] = -170;
  gamedata->islandstypes[5] = 1;
  gamedata->islandscargo[5][0] = 100; //metal
  gamedata->islandscargo[5][1] = 20; //wood
  gamedata->islandscargo[5][2] = 100; //stone
  gamedata->islandscargo[5][3] = 0; //food
  
  gamedata->islandsx[6] = 380;
  gamedata->islandsy[6] = 100;
  gamedata->islandstypes[6] = 2;
  gamedata->islandscargo[6][0] = 100; //metal
  gamedata->islandscargo[6][1] = 0; //wood
  gamedata->islandscargo[6][2] = 20; //stone
  gamedata->islandscargo[6][3] = 100; //food
  
  gamedata->islandsx[7] = 200;
  gamedata->islandsy[7] = 0;
  gamedata->islandstypes[7] = 3;
  gamedata->islandscargo[7][0] = 0; //metal
  gamedata->islandscargo[7][1] = 100; //wood
  gamedata->islandscargo[7][2] = 100; //stone
  gamedata->islandscargo[7][3] = 20; //food
}

void update_islands(GameData* gamedata){
  for(int i = 0; i < TOTALISLANDS; i++){
    //later on, update all 10 islands.  However, only one for now
    if(gamedata->islandstypes[i] == 0){
       if(gamedata->islandscargo[i][0] < 100 && gamedata->islandscargo[i][1] > 0){
        gamedata->islandscargo[i][1]--; //remove one wood
        gamedata->islandscargo[i][0] += 1; //add 1 metal
       }
       if(gamedata->islandscargo[i][3] >= 10){ //if the island still has food
         gamedata->islandscargo[i][3]--;
       }
    }
    if(gamedata->islandstypes[i] == 1){
      if(gamedata->islandscargo[i][1] < 100 && gamedata->islandscargo[i][3] > 0){
        gamedata->islandscargo[i][3]--; //remove one food
        gamedata->islandscargo[i][1] += 3; //add 3 wood
       }
      if(gamedata->islandscargo[i][3] >= 10){ //if the island still has food
         gamedata->islandscargo[i][3]--;
       }
    }
    if(gamedata->islandstypes[i] == 2){
      if(gamedata->islandscargo[i][2] < 100 && gamedata->islandscargo[i][1] > 0){
        gamedata->islandscargo[i][1]--; //remove one wood
        gamedata->islandscargo[i][2] += 3; //add 3 stone
       }
      if(gamedata->islandscargo[i][3] >= 10){ //if the island still has food
         gamedata->islandscargo[i][3]--;
       }
    }
    if(gamedata->islandstypes[i] == 3){
      if(gamedata->islandscargo[i][3] < 100 && gamedata->islandscargo[i][0] > 0){
        gamedata->islandscargo[i][0]--; //remove one iron
        gamedata->islandscargo[i][3] += 4; //add 4 food
       }
      if(gamedata->islandscargo[i][3] >= 10){ //if the island still has food
         gamedata->islandscargo[i][3]--; 
       }
    }
    //APP_LOG(APP_LOG_LEVEL_INFO, "Metal %i", gamedata->islandscargo[i][0]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "Wood %i", gamedata->islandscargo[i][1]);
      //based on island type, check if each island has enough of their excess resource
    int searchtype = gamedata->islandstypes[i];
    int possibleislands[10];
    int picounter = -1;
    for(int j = 0; j < TOTALISLANDS; j++){ //search through each existing island
      //if the island needs more of the type being sent, and the island sending has more than 10 to send, and the island being checked is not itself
      if(gamedata->islandscargo[j][searchtype] < 100 && gamedata->islandscargo[i][gamedata->islandstypes[i]] >= 10 && j != i){
        //add the island to an array to be selected from later  
        ++picounter;
        possibleislands[picounter] = j;
        //create_ship(gamedata, i, j);
      }  
    }  
    //pi counter is the total number of islands that are valid
    //need to randomly select a number between 0 and picounter
    //then create a ship for that counter
    if(picounter >= 0){ //if there were any valid targets
      int randomvalue = random(picounter);
      //APP_LOG(APP_LOG_LEVEL_INFO, "RANDOM INT IS %i PICOUNTER IS %i", randomvalue, picounter);
      if(gamedata->islandscargo[i][1] >= 10 && gamedata->islandscargo[i][2] >= 10)
        create_ship(gamedata, i, possibleislands[randomvalue]);
    }
  
  }
  
}

void initialize_ships(GameData* gamedata){
  gamedata->totalships = -1;
  for(int i = 0; i < 10; i++){ //from 0 to max size of ship array
    gamedata->shipsx[i] = 0;
    gamedata->shipsy[i] = 0;
    gamedata->shipscargo[i] = 0;
    gamedata->shipstype[i] = 0;
    gamedata->shipsisland[i] = 0;
  }
}
void create_ship(GameData* gamedata, int8_t islandnumber, int8_t destinationnumber){
  //add another ship if there is room
  if(gamedata->totalships < 10){
    gamedata->totalships++;
    //APP_LOG(APP_LOG_LEVEL_INFO, "Created Ship %i", gamedata->totalships);
    //APP_LOG(APP_LOG_LEVEL_INFO, "at X =  %i", gamedata->islandsx[islandnumber]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "at Y =  %i", gamedata->islandsy[islandnumber]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "Destination being at island: %i", destinationnumber);
    //set new ship x and y coords to coords of spawning island
    gamedata->shipsx[gamedata->totalships] = gamedata->islandsx[islandnumber];
    gamedata->shipsy[gamedata->totalships] = gamedata->islandsy[islandnumber];
    gamedata->shipsisland[gamedata->totalships] = destinationnumber;
    //APP_LOG(APP_LOG_LEVEL_INFO, "Destination Stored at index %i: %i", gamedata->totalships, gamedata->shipsisland[gamedata->totalships]);
    //remove building resources from the island
    gamedata->islandscargo[islandnumber][2] += -2; //remove 2 wood
    gamedata->islandscargo[islandnumber][1] += -2; //remove 2 stone
    //set cargo, and remove cargo from spawning island
    gamedata->islandscargo[islandnumber][gamedata->islandstypes[islandnumber]] += -10;
    gamedata->shipscargo[gamedata->totalships] = 10;
    gamedata->shipstype[gamedata->totalships] = gamedata->islandstypes[islandnumber];
  }
}

void update_ships(GameData* gamedata){
  //APP_LOG(APP_LOG_LEVEL_INFO, "Updating Ships");
  for(int i = 0; i <= gamedata->totalships; i++){
    //calculate movment, simple algorithm to do so
    if(i == 1){
    //APP_LOG(APP_LOG_LEVEL_INFO, "SHIP AT X: %i", gamedata->shipsx[i]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "SHIP AT Y: %i", gamedata->shipsy[i]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "SHIP INDEX %i", i);
    //APP_LOG(APP_LOG_LEVEL_INFO, "ISLAND NUMBER: %i", gamedata->shipsisland[i]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "ISLAND NUMBER0: %i", gamedata->shipsisland[0]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "ISLAND NUMBER1: %i", gamedata->shipsisland[1]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "ISLAND AT X: %i",  gamedata->islandsx[gamedata->shipsisland[i]]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "ISLAND AT Y: %i", gamedata->islandsy[gamedata->shipsisland[i]]);
    }
    
    if(gamedata->shipsx[i] < gamedata->islandsx[ gamedata->shipsisland[i] ]){
      gamedata->shipsx[i]++;
    }
    if(gamedata->shipsx[i] > gamedata->islandsx[gamedata->shipsisland[i]]){
      gamedata->shipsx[i]--;
    }
    if(gamedata->shipsy[i] < gamedata->islandsy[gamedata->shipsisland[i]]){
      gamedata->shipsy[i]++;
    }
   if(gamedata->shipsy[i] > gamedata->islandsy[gamedata->shipsisland[i]]){
      gamedata->shipsy[i]--;
    }
   if((gamedata->shipsx[i] == gamedata->islandsx[gamedata->shipsisland[i]]) && (gamedata->shipsy[i] == gamedata->islandsy[gamedata->shipsisland[i]])){
     //APP_LOG(APP_LOG_LEVEL_INFO, "Attempting to destroy ship %i", i);
     destroy_ship(gamedata, i, 1);
   }
  }//end for
}

void destroy_ship(GameData* gamedata, int shipnumber, int reacheddestination){
  //APP_LOG(APP_LOG_LEVEL_INFO, "Destroying ship - reached %i", reacheddestination);
  if(reacheddestination == 0){
    gamedata->shipsx[shipnumber] = -1;
    gamedata->shipsy[shipnumber] = -1;
  }
  else{
    //reset the ship's x and y to the default -1 vals
    gamedata->shipsx[shipnumber] = -1;
    gamedata->shipsy[shipnumber] = -1;
    //add the ships cargo and type to the islands cargo and type
    //APP_LOG(APP_LOG_LEVEL_INFO, "Ship Cargo: %i", gamedata->shipscargo[shipnumber]);
    gamedata->islandscargo[gamedata->shipsisland[shipnumber]][gamedata->shipstype[shipnumber]] += gamedata->shipscargo[shipnumber];
    //reset the ship's cargo
    gamedata->shipsisland[shipnumber] = 0;
    gamedata->shipscargo[shipnumber] = 0;
    gamedata->shipstype[shipnumber] = 0;
  }
  //adjust all the ships back one unit in the array to make sure the array has no gaps
  for(int i = shipnumber; i < gamedata->totalships; i++){
    gamedata->shipsx[i] = gamedata->shipsx[i+1];
    gamedata->shipsy[i] = gamedata->shipsy[i+1];
    gamedata->shipscargo[i] = gamedata->shipscargo[i+1];
    gamedata->shipstype[i] = gamedata->shipstype[i+1];
    gamedata->shipsisland[i] = gamedata->shipsisland[i+1];
  }
  gamedata->totalships--;
  if(gamedata->totalships < -1){
    //APP_LOG(APP_LOG_LEVEL_ERROR, "ATTEMPTED TO DELETE NONEXISTANT SHIP");
    gamedata->totalships = -1;
  }
}

int random(int max)
{
if(max == 0)
  return 0;
time_t temp = time(NULL);
struct tm *tick_time = localtime(&temp);
int minuite = 0;
int second = 0;
second = tick_time->tm_sec;
minuite = tick_time->tm_min;
  
static long seed = 100;
seed += minuite + second;
seed = (((seed * 214013L + 2531011L) >> 16) & 32767);

return ((seed % (max+1)));
}

int finddistance(int x1, int y1, int x2, int y2){
  int xminus;
  int yminus;
  xminus = x2 - x1;
  yminus = y2 - y1;
  long long int xsquared = xminus * xminus;
  long long int ysquared = yminus * yminus;
  return xsquared + ysquared;
}