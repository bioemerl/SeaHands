#include <pebble.h>
#include "island.h"
#include "types.h"
#include "game.h"
#include "ship.h"

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