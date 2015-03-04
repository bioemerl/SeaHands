#include <pebble.h>
#include "ship.h"
#include "types.h"
#include "game.h"


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
    gamedata->islandscargo[gamedata->shipsisland[shipnumber]][1] += 1;
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
    APP_LOG(APP_LOG_LEVEL_ERROR, "ATTEMPTED TO DELETE NONEXISTANT SHIP");
    gamedata->totalships = -1;
  }
}