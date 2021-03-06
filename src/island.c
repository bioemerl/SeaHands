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
    gamedata->islandsallegiance[i] = 0;
  }
  
  //initialize the storage island
  gamedata->islandsx[10] = MAX16BITVALUE;
  gamedata->islandsy[10] = MAX16BITVALUE;
  gamedata->islandscargo[10][0] = 0;
  gamedata->islandscargo[10][1] = 0;
  gamedata->islandscargo[10][2] = 0;
  gamedata->islandscargo[10][3] = 0;
  gamedata->storageexists = 0;
  

  //now set their cargos, and their types
  //1 - metal       2 - wood        3 - stone      4 - food
  //1 - loses wood. 2 - looses stone 3 - loses food 4 - loses metal
  initialize_island(gamedata, 0, 0, -200, 200, 20, 0, 100, 100);
  
  initialize_island(gamedata, 1, 1, 300, 340, 100, 20, 100, 0);
  
  initialize_island(gamedata, 2, 2, 440, 400, 100, 0, 20, 100);
  
  initialize_island(gamedata, 3, 3, 300, -512, 0, 100, 100, 20);
  
  initialize_island(gamedata, 4, 0, -250, -270, 20, 0, 100, 100);
  
  initialize_island(gamedata, 5, 1, -150, -170, 100, 20, 100, 0);
  
  initialize_island(gamedata, 6, 2, 380, 100, 100, 0, 20, 100);
  
  initialize_island(gamedata, 7, 1, 200, 0, 0, 100, 100, 20);
  
  initialize_island(gamedata, 8, 23, 50, -80, 100, 100, 100, 100);

  
  initialize_island(gamedata, 9, 2, -10, 15, 100, 100, 100, 100);
  /*gamedata->islandsx[9] = -10;
  gamedata->islandsy[9] = 15;
  gamedata->islandstypes[9] = 2;
  gamedata->islandscargo[9][0] = 100; //metal
  gamedata->islandscargo[9][1] = 100; //wood
  gamedata->islandscargo[9][2] = 100; //stone
  gamedata->islandscargo[9][3] = 100; //food*/
}

void initialize_island(GameData* gamedata, int8_t islandnumber, int8_t islandtype, int x, int y, int8_t overridemetal, int8_t overridewood,
                       int8_t overridestone, int8_t overridefood){
  gamedata->islandsx[islandnumber] = x;
  gamedata->islandsy[islandnumber] = y;
  gamedata->islandstypes[islandnumber] = islandtype;
  gamedata->islandscargo[islandnumber][0] = overridemetal; //metal
  gamedata->islandscargo[islandnumber][1] = overridewood; //wood
  gamedata->islandscargo[islandnumber][2] = overridestone; //stone
  gamedata->islandscargo[islandnumber][3] = overridefood; //food
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
    
    //increase negative islands affinity by one occasionally
    int8_t rand = random(7);
    if(rand > 3 && manageislandallegiance(gamedata, i, 0) < 0){
      manageislandallegiance(gamedata, i, 1);
    }
    
    
    //for each resource the current island has zero of, check each island that makes that resource
    //if the island has that resource, go towards the opposite affinity of that island
    //one in three chance to do so
    int8_t randomnumber = 0;
    int8_t attackislandnumber = -1;
    for(int j = 0; j < TOTALISLANDS; j++){
      if(j != i){
        int8_t islandallegiance = manageislandallegiance(gamedata, i, 0);
        int8_t enemyallegiance = manageislandallegiance(gamedata, j, 0);
        if(gamedata->islandstypes[j] == 0 && gamedata->islandscargo[i][0] == 0 && gamedata->islandscargo[j][0] > 0){
          randomnumber = random(15);
          if(randomnumber == 2){
            if(enemyallegiance > 0){
              enemyallegiance = manageislandallegiance(gamedata, i, -1);
            }
            if(enemyallegiance < 0){
              enemyallegiance = manageislandallegiance(gamedata, i, 1);
            }
            if(abs(enemyallegiance + islandallegiance) != (abs(enemyallegiance) + abs(islandallegiance))) //if the two values are the same + or -
              attackislandnumber = j;
          }
          
        }
        if(gamedata->islandstypes[j] == 1 && gamedata->islandscargo[i][1] == 0 && gamedata->islandscargo[j][1] > 0){
          randomnumber = random(3);
          if(randomnumber == 2){
            if(enemyallegiance > 0){
              enemyallegiance = manageislandallegiance(gamedata, i, -1);
            }
            if(enemyallegiance < 0){
              enemyallegiance = manageislandallegiance(gamedata, i, 1);
            }
            if(abs(enemyallegiance + islandallegiance) != (abs(enemyallegiance) + abs(islandallegiance))) //if the two values are the same + or -
              attackislandnumber = j;
          }
        }
        if(gamedata->islandstypes[j] == 2 && gamedata->islandscargo[i][2] == 0 && gamedata->islandscargo[j][2] > 0){
          randomnumber = random(3);
          if(randomnumber == 2){
            if(enemyallegiance > 0){
              enemyallegiance = manageislandallegiance(gamedata, i, -1);
            }
            if(enemyallegiance < 0){
              enemyallegiance = manageislandallegiance(gamedata, i, 1);
            }
            if(abs(enemyallegiance + islandallegiance) != (abs(enemyallegiance) + abs(islandallegiance))) //if the two values are the same + or -
              attackislandnumber = j;
          }
        }
        if(gamedata->islandstypes[j] == 3 && gamedata->islandscargo[i][3] == 0 && gamedata->islandscargo[j][3] > 0){
          randomnumber = random(3);
          if(randomnumber == 2){
            if(enemyallegiance > 0){
              enemyallegiance = manageislandallegiance(gamedata, i, -1);
            }
            if(enemyallegiance < 0){
              enemyallegiance = manageislandallegiance(gamedata, i, 1);
            }
            if(abs(enemyallegiance + islandallegiance) != (abs(enemyallegiance) + abs(islandallegiance))) //if the two values are the same + or -
              attackislandnumber = j;
          }
        }
      }
    }
    
    
    
    //check if the island has a ship.  If not, make a ship for it
    //NOTE: could optimize here by making create ship return the index of it's created ship
    //that way I wouldn't have to do find owned ship after making one
    int currentship = find_owned_ship(gamedata, i);
    if(currentship == -1 && gamedata->islandscargo[i][1] >= 20 && gamedata->islandscargo[i][2] >= 20){ //if no ship is found, and island has enough resources
      create_ship(gamedata, i); //make a ship for the island
      currentship = find_owned_ship(gamedata, i); //set the value again to the ship
    }
    
    //check faction, and if it is extreme enough, order the creation of an attack ship:
    //then figure out where that attack ship should attack.  First choose an island, then have a 1 in 4
    //chance to instead attack the player
    randomnumber = random(30);
    int8_t ordervalues[3] = {0, 0, 0};
    if(randomnumber == 25 && manageislandallegiance(gamedata, i, 0) <= -30){
      give_ship_order(gamedata, i, currentship, 'a', ordervalues);
    }
    if(randomnumber == 25){
      if(attackislandnumber != -1){ //if there is an island the current one hates, attack that one instead
        ordervalues[0] = attackislandnumber + 1;
        ordervalues[1] = gamedata->islandstypes[attackislandnumber];
        give_ship_order(gamedata, i, currentship, 'a', ordervalues);
      }
    }
    
    //check orders for standard shipping
    if(gamedata->shipsorder[currentship] != 'a' && currentship != -1 && (gamedata->shipsorderinfo[currentship][1] == -1 || (gamedata->islandscargo[gamedata->shipsorderinfo[currentship][1]][gamedata->islandstypes[i]] - 10 > gamedata->islandscargo[i][gamedata->islandstypes[i]] + 10))){
      //if the current island has fewer of it's resource than the island it is delivering to, and has a ship
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
  if(gamedata->islandsx[10] != MAX16BITVALUE && gamedata->islandsy[10] != MAX16BITVALUE){ //update the storage island
        
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

