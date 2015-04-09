#include <pebble.h>
#include "island.h"
#include "types.h"
#include "game.h"
#include "ship.h"
  
const int METALPRODUCTIONVALUE = 2;
const int WOODPRODUCTIONVALUE = 3;
const int STONEPRODUCTIONVALUE = 2;
const int FOODPRODUCTIONVALUE = 4;


void initialize_islands(GameData* gamedata){
  //initialize multipliers for the islands
  gamedata->metalmultiplier = 1;
  gamedata->stonemultiplier = 1;
  gamedata->woodmultiplier = 1;
  gamedata->foodmultiplier = 1;
  gamedata->metaldivisor = 1;
  gamedata->stonedivisor = 1;
  gamedata->wooddivisor = 1;
  gamedata->fooddivisor = 1;
  //initialize island positions to a default value
  for(int i = 0; i < 10; i++){
    gamedata->islandsx[i] = -1;
    gamedata->islandsy[i] = -1;
  }
  //now initialize them to actual island positions
  gamedata->islandsx[0] = -200;
  gamedata->islandsy[0] = 200;
  //now set their cargos, and their types
  //1 - metal       2 - wood        3 - stone      4 - food
  //1 - loses wood. 2 - looses stone 3 - loses food 4 - loses metal
  gamedata->islandstypes[0] = 0;
  //now set the islands initial cargo
  gamedata->islandscargo[0][0] = 20; //metal
  gamedata->islandscargo[0][1] = 0; //wood
  gamedata->islandscargo[0][2] = 100; //stone
  gamedata->islandscargo[0][3] = 100; //food
  
  gamedata->islandsx[1] = 300;
  gamedata->islandsy[1] = 340;
  gamedata->islandstypes[1] = 1;
  gamedata->islandscargo[1][0] = 100; //metal
  gamedata->islandscargo[1][1] = 20; //wood
  gamedata->islandscargo[1][2] = 100; //stone
  gamedata->islandscargo[1][3] = 0; //food
  
  gamedata->islandsx[2] = 440;
  gamedata->islandsy[2] = 400;
  gamedata->islandstypes[2] = 2;
  gamedata->islandscargo[2][0] = 100; //metal
  gamedata->islandscargo[2][1] = 0; //wood
  gamedata->islandscargo[2][2] = 20; //stone
  gamedata->islandscargo[2][3] = 100; //food
  
  gamedata->islandsx[3] = 300;
  gamedata->islandsy[3] = -512;
  gamedata->islandstypes[3] = 3;
  gamedata->islandscargo[3][0] = 0; //metal
  gamedata->islandscargo[3][1] = 100; //wood
  gamedata->islandscargo[3][2] = 100; //stone
  gamedata->islandscargo[3][3] = 20; //food
  
  gamedata->islandsx[4] = -250;
  gamedata->islandsy[4] = -270;
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
  gamedata->islandstypes[7] = 1;
  gamedata->islandscargo[7][0] = 0; //metal
  gamedata->islandscargo[7][1] = 100; //wood
  gamedata->islandscargo[7][2] = 100; //stone
  gamedata->islandscargo[7][3] = 20; //food

  gamedata->islandsx[8] = 50;
  gamedata->islandsy[8] = -80;
  gamedata->islandstypes[8] = 3;
  gamedata->islandscargo[8][0] = 100; //metal
  gamedata->islandscargo[8][1] = 100; //wood
  gamedata->islandscargo[8][2] = 100; //stone
  gamedata->islandscargo[8][3] = 100; //food
  
  gamedata->islandsx[9] = -10;
  gamedata->islandsy[9] = 15;
  gamedata->islandstypes[9] = 2;
  gamedata->islandscargo[9][0] = 100; //metal
  gamedata->islandscargo[9][1] = 100; //wood
  gamedata->islandscargo[9][2] = 100; //stone
  gamedata->islandscargo[9][3] = 100; //food
}

void initialize_island(GameData* gamedata, int islandnumber, int islandtype, int x, int y, int overridemetal, int overridewood,
                       int overridestone, int overridefood){
  gamedata->islandsx[9] = -10;
  gamedata->islandsy[9] = 15;
  gamedata->islandstypes[9] = 2;
  gamedata->islandscargo[9][0] = 100; //metal
  gamedata->islandscargo[9][1] = 100; //wood
  gamedata->islandscargo[9][2] = 100; //stone
  gamedata->islandscargo[9][3] = 100; //food
}

void update_islands(GameData* gamedata){
  
  /*DEBUG CODE
  int metalval, stoneval, foodval, woodval;
  ResourceValues currentvalues; 
  currentvalues = getmoneyvalue(gamedata, -1);
  metalval = currentvalues.metalvalue;
  woodval = currentvalues.woodvalue;
  stoneval = currentvalues.stonevalue;
  foodval = currentvalues.foodvalue;
  APP_LOG(APP_LOG_LEVEL_INFO, "\nMETAL VALUE: %i\nWOOD VALUE: %i\nSTONE VALUE: %i\nFOOD VALUE: %i\n", metalval, woodval, stoneval, foodval);
  END DEBUG CODE*/
  for(int i = 0; i < TOTALISLANDS; i++){
    int resourceincrease = 0;
    if(gamedata->islandstypes[i] == 0){
      resourceincrease = ((METALPRODUCTIONVALUE*gamedata->metalmultiplier) / gamedata->metaldivisor);
      updateislandresource(gamedata, i, resourceincrease, 0, -1, -1);
    }
    if(gamedata->islandstypes[i] == 1){
      resourceincrease = ((WOODPRODUCTIONVALUE*gamedata->woodmultiplier) / gamedata->stonedivisor);
      updateislandresource(gamedata, i, 0, resourceincrease, 0, -2);
    }
    if(gamedata->islandstypes[i] == 2){
      resourceincrease = ((STONEPRODUCTIONVALUE*gamedata->stonemultiplier) / gamedata->wooddivisor);
      updateislandresource(gamedata, i, 0, -1, resourceincrease, -1);
    }
    if(gamedata->islandstypes[i] == 3){
      resourceincrease = ((FOODPRODUCTIONVALUE*gamedata->foodmultiplier) / gamedata->fooddivisor);
      updateislandresource(gamedata, i, -2, 0, 0, resourceincrease);
    }
    
    //check if the island has a ship
    int currentship = find_owned_ship(gamedata, i);
    if(currentship == -1){ //if no ship is found
      create_ship(gamedata, i); //make a ship for the island
      currentship = find_owned_ship(gamedata, i); //set the value again to the ship
    }
    if(gamedata->shipsorderinfo[currentship][1] == -1 || (gamedata->islandscargo[gamedata->shipsorderinfo[currentship][1]][gamedata->islandstypes[i]] >= gamedata->islandscargo[i][gamedata->islandstypes[i]])){
      //if the current island has fewer of it's resource than the island it is delivering to
      int8_t ordervalues[3] = {-1, -1, -1};
      give_ship_order(gamedata, i, currentship, 'n', ordervalues); //set the ship to do nothing
      
      //search for a valid island
      int searchtype = gamedata->islandstypes[i];
      int possibleislands[10];
      int picounter = -1;
      for(int j = 0; j < TOTALISLANDS; j++){ //search through each existing island
        //if the island needs more of the type being sent, and the island sending has more than 10 to send, and the island being checked is not itself
        if(gamedata->islandscargo[j][searchtype] < 80 && gamedata->islandscargo[i][gamedata->islandstypes[i]] >= 10 && gamedata->islandscargo[i][gamedata->islandstypes[i]] >= gamedata->islandscargo[j][searchtype] && j != i){
          //add the island to an array to be selected from later  
          ++picounter;
          possibleislands[picounter] = j;
        }  
      }
      //pi counter is the total number of islands that are valid
      //need to randomly select a number between 0 and picounter
      //then create a ship for that counter
      if(picounter >= 0){ //if there were any valid targets
        int randomvalue = random(picounter);
        picounter = 0;
        //APP_LOG(APP_LOG_LEVEL_INFO, "RANDOM INT IS %i PICOUNTER IS %i", randomvalue, picounter);
        //create_ship(gamedata, i, possibleislands[randomvalue]);
        int8_t ordervalues[3] = {i, possibleislands[randomvalue], gamedata->islandstypes[i]};
        give_ship_order(gamedata, i, currentship, 'd', ordervalues);
      }
      
    }    
    
    
    
    //based on island type, check if each island has enough of their excess resource
      
    
  
  
  }
}

//will cause the island to change values, if and only if all value changes are valid (do not go over max, or under 0)
void updateislandresource(GameData* gamedata, int islandnumber, int res1adjust, int res2adjust, int res3adjust, int res4adjust){
  int8_t failadjustments = 0;
  if((gamedata->islandscargo[islandnumber][0] + res1adjust > 100 || gamedata->islandscargo[islandnumber][0] + res1adjust < 0))
    failadjustments = 1;
  if((gamedata->islandscargo[islandnumber][1] + res2adjust > 100 || gamedata->islandscargo[islandnumber][1] + res2adjust < 0))
    failadjustments = 1;
  if((gamedata->islandscargo[islandnumber][2] + res3adjust > 100 || gamedata->islandscargo[islandnumber][2] + res3adjust < 0))
    failadjustments = 1;
  if((gamedata->islandscargo[islandnumber][3] + res4adjust > 100 || gamedata->islandscargo[islandnumber][3] + res4adjust < 0))
    failadjustments = 1;
    
  if(failadjustments == 0){
    gamedata->islandscargo[islandnumber][0] += res1adjust;
    gamedata->islandscargo[islandnumber][1] += res2adjust;
    gamedata->islandscargo[islandnumber][2] += res3adjust;
    gamedata->islandscargo[islandnumber][3] += res4adjust;
  }
}