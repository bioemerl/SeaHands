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
    gamedata->shipsowner[i] = 0;
    gamedata->shipsorder[i] = 'n'; //no order
    gamedata->shipsorderinfo[i][0] = 0;
    gamedata->shipsorderinfo[i][1] = 0;
    gamedata->shipsorderinfo[i][2] = 0;
    gamedata->totalships = 0;
    
  }
}

void create_ship(GameData* gamedata, int8_t ownernumber){
  //add another ship if there is room
  if(gamedata->totalships < 9){
    gamedata->totalships++;
    //APP_LOG(APP_LOG_LEVEL_INFO, "Created Ship %i", gamedata->totalships);
    //APP_LOG(APP_LOG_LEVEL_INFO, "at X =  %i", gamedata->islandsx[islandnumber]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "at Y =  %i", gamedata->islandsy[islandnumber]);
    //APP_LOG(APP_LOG_LEVEL_INFO, "Destination being at island: %i", destinationnumber);
    //set new ship x and y coords to coords of spawning island
    gamedata->shipsx[gamedata->totalships] = gamedata->islandsx[ownernumber];
    gamedata->shipsy[gamedata->totalships] = gamedata->islandsy[ownernumber];
    gamedata->shipsowner[gamedata->totalships] = ownernumber;
    gamedata->shipsorderinfo[gamedata->totalships][0] = -1;
    gamedata->shipsorderinfo[gamedata->totalships][1] = -1;
    gamedata->shipsorderinfo[gamedata->totalships][2] = -1;
    //APP_LOG(APP_LOG_LEVEL_INFO, "Destination Stored at index %i: %i", gamedata->totalships, gamedata->shipsisland[gamedata->totalships]);
    //remove building resources from the island
    //gamedata->islandscargo[ownernumber][2] += -2; //remove 2 wood
    //gamedata->islandscargo[ownernumber][1] += -2; //remove 2 stone
    //set cargo, and remove cargo from spawning island
  }
}

void give_ship_order(GameData* gamedata, int8_t ownernumber, int8_t shipnumber, char ordercharacter, int8_t orderinfo[3]){
  if(ownernumber == gamedata->shipsowner[shipnumber]){
    gamedata->shipsorder[shipnumber] = ordercharacter;
    gamedata->shipsorderinfo[shipnumber][0] = orderinfo[0];
    gamedata->shipsorderinfo[shipnumber][1] = orderinfo[1];
    gamedata->shipsorderinfo[shipnumber][2] = orderinfo[2];
  }
}

void update_ships(GameData* gamedata){
  //old code, but useful later
  /*if((gamedata->shipsx[i] == gamedata->islandsx[gamedata->shipsisland[i]]) && (gamedata->shipsy[i] == gamedata->islandsy[gamedata->shipsisland[i]])){
        //APP_LOG(APP_LOG_LEVEL_INFO, "Attempting to destroy ship %i", i);
        destroy_ship(gamedata, i, 1);
      }
  */
  //APP_LOG(APP_LOG_LEVEL_INFO, "Updating Ships");
  for(int i = 0; i <= gamedata->totalships; i++){ //for each ship
    if(i != gamedata->playership){ //if the current ship is not under capture by player
      if(gamedata->shipsorder[i] == 'a'){ //attack
        //move to host island to offload any cargo
        //move to target(currently, only the player)
        //initiate attack on the target, force the attack.
        //first offload cargo
        if(gamedata->shipscargo[i] > 0){
          //move to island to offload cargo
          int8_t arrived = move_ship(gamedata, gamedata->shipsowner[i], i);
          if(arrived == 1){
            ship_give_cargo(gamedata, i, gamedata->shipsowner[i], gamedata->shipstype[i], gamedata->shipscargo[i]); //offload whole inventory
          }
        }
        else{
          //move to the player
          int8_t arrived = move_ship(gamedata, 11 /*the player*/, i);
          if(arrived == 1){
            
          }
          //I am unsure of how to force an attack
        }
      }
      if(gamedata->shipsorder[i] == 'd'){ //deliver
        //if there is something in cargo, move to the second island in the array
        //else move to the first island in the array
        //when the island the ship is moving to is reached, either remove or put supplies 
        //into that island's inventory
        //use third array item to denote what resource is being moved.
        if(gamedata->shipscargo[i] > 0){ //if there is something in the cargo
          //move to second island in array, offload cargo
          //if the ship is at the island, move cargo onto the island
          APP_LOG(APP_LOG_LEVEL_INFO, "updaing for D, moving to %i", gamedata->shipsorderinfo[i][1]);
          int8_t arrived = move_ship(gamedata, gamedata->shipsorderinfo[i][1], i);
          if(arrived == 1){
            APP_LOG(APP_LOG_LEVEL_INFO, "trying to give cargo");
            ship_give_cargo(gamedata, i, gamedata->shipsorderinfo[i][1], gamedata->shipscargo[i], gamedata->shipsorderinfo[i][2]);
          }
        }
        if(gamedata->shipscargo[i] <= 0){ //if cargo is empty
          //move to the first island in array
          //if the ship is at the island, take cargo from the island
          APP_LOG(APP_LOG_LEVEL_INFO, "Attempting to deliver to %i", gamedata->shipsorderinfo[i][0]);
          int8_t arrived = move_ship(gamedata, gamedata->shipsorderinfo[i][0], i);
          if(arrived == 1){
            APP_LOG(APP_LOG_LEVEL_INFO, "Attempting to take cargo from %i", gamedata->shipsorderinfo[i][0]);
            ship_take_cargo(gamedata, i, gamedata->shipsorderinfo[i][0], MAX_SHIP_CARGO, gamedata->shipsorderinfo[i][2]);
          }
        }
      }
      if(gamedata->shipsorder[i] == 's'){ //shop
        //same as 'd', but instead, also deduct funds.  No need for this until later with the storage 
        //building the player can build
      }
      if(gamedata->shipsorder[i] == 'n'){ //nothing
        //move to the host island.  Stay there
        //offload any held cargo
        int8_t arrived = move_ship(gamedata, gamedata->shipsowner[i], i);
      }
    }
  }//end for
}

int8_t move_ship(GameData* gamedata, int8_t destination, int8_t shipnumber){
  //set the destinations depending on the number given.
  int destx = 0, desty = 0;
  if(destination < 10){ //islands
    destx = gamedata->islandsx[destination];
    desty = gamedata->islandsy[destination];
  }
  if(destination == 11){ //the player
    destx = gamedata->playerx;
    desty = gamedata->playery;
  }
  
  //update the ship to move to said destinations
  if(gamedata->shipsx[shipnumber] < destx){
    gamedata->shipsx[shipnumber]++;
  }
  if(gamedata->shipsx[shipnumber] > destx){
    gamedata->shipsx[shipnumber]--;
  }
  if(gamedata->shipsy[shipnumber] < desty){
    gamedata->shipsy[shipnumber]++;
  }
  if(gamedata->shipsy[shipnumber] > desty){
    gamedata->shipsy[shipnumber]--;
  }
  if(gamedata->shipsx[shipnumber] == destx && gamedata->shipsy[shipnumber] == desty){
    return 1;
  }
  return 0;
}

void destroy_ship(GameData* gamedata, int shipnumber){
  //APP_LOG(APP_LOG_LEVEL_INFO, "Destroying ship - reached %i", reacheddestination);
  if(shipnumber < gamedata->playership){ //ensure that the ships frozen in player encounter stay frozen
    gamedata->playership--;
  }
  //reset the ship's x and y to the default -1 vals
  gamedata->shipsx[shipnumber] = -1;
  gamedata->shipsy[shipnumber] = -1;
  //reset the ship's cargo
  gamedata->shipsowner[shipnumber] = 0;
  gamedata->shipscargo[shipnumber] = 0;
  gamedata->shipstype[shipnumber] = 0;
  //adjust all the ships back one unit in the array to make sure the array has no gaps
  for(int i = shipnumber; i < gamedata->totalships; i++){
    gamedata->shipsx[i] = gamedata->shipsx[i+1];
    gamedata->shipsy[i] = gamedata->shipsy[i+1];
    gamedata->shipscargo[i] = gamedata->shipscargo[i+1];
    gamedata->shipstype[i] = gamedata->shipstype[i+1];
    gamedata->shipsowner[i] = gamedata->shipsowner[i+1];
    gamedata->shipsorder[i] = gamedata->shipsorder[i+1];
    gamedata->shipsorderinfo[i][0] = gamedata->shipsorderinfo[i+1][0];
    gamedata->shipsorderinfo[i][1] = gamedata->shipsorderinfo[i+1][1];
    gamedata->shipsorderinfo[i][2] = gamedata->shipsorderinfo[i+1][2];
  }
  gamedata->totalships--;
  if(gamedata->totalships < -1){
    APP_LOG(APP_LOG_LEVEL_ERROR, "ATTEMPTED TO DELETE NONEXISTANT SHIP");
    gamedata->totalships = -1;
  }
}


void ship_take_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource){
  //if the ship either has no resources, or the ship type is the same as the type it is picking up:
  if((gamedata->shipscargo[shipnumber] + amount) <= MAX_SHIP_CARGO && (gamedata->shipscargo[shipnumber] <= 0 || gamedata->shipstype[shipnumber] == resource)){ //if the ship can hold this cargo
    
    if(gamedata->islandscargo[target][resource] <= amount) //if there is not enough, take what is left
      amount = gamedata->islandscargo[target][resource];
    
    gamedata->islandscargo[target][resource] += -amount; //take resource from island
    gamedata->shipscargo[shipnumber] = gamedata->shipscargo[shipnumber] + amount; //add cargo to ship
    gamedata->shipstype[shipnumber] = resource; //set ship resource to be what resource it took
  
  }
}

void ship_give_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource){
  //if the ship has cargo, and is putting the correct resource to the island
  APP_LOG(APP_LOG_LEVEL_INFO, "Cargo: %i.  Type: %i.  ExpType: %i", gamedata->shipscargo[shipnumber], gamedata->shipstype[shipnumber], resource );
  if(gamedata->shipscargo[shipnumber] > 0 && gamedata->shipstype[shipnumber] == resource){
    if(gamedata->islandscargo[target][resource] + amount <= MAX_ISLAND_CARGO){ //if the island cannot hold the amount
      amount = MAX_ISLAND_CARGO - gamedata->islandscargo[target][resource]; //put what can fit
      if(amount < 0) //always be secure.  If island somehow had more than the max, be sure not to get negative numbers
        amount = 0;
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "GOT HERE!");
    gamedata->islandscargo[target][resource] += +amount; //add resource to island
    gamedata->shipscargo[shipnumber] = gamedata->shipscargo[shipnumber] - amount; //add cargo from ship
  }
}
void ship_buy_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource){
  
}
void ship_sell_cargo(GameData* gamedata, int8_t shipnumber, int8_t target, int8_t amount, int8_t resource){
  
}

int8_t find_owned_ship(GameData* gamedata, int8_t owner){
  //if a ship with the given owner exists, return said index
  //otherwise, return -1;
  for(int i = 0; i <= gamedata->totalships; i++){
    if(gamedata->shipsowner[i] == owner){
      return i;
    }
  }
  return -1;
}