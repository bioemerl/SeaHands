#include <pebble.h>
#include "game.h"
#include "player.h"
#include "island.h"
#include "ship.h"
//main character's name is bill
//bill because he looks for money all the time.

static int twohundredcountdown = 200;

void initialize_game(GameData* gamedata){
  gamedata->gamemode = 'p';
  gamedata->playerisland = 1;
  gamedata->menulayer = 0;
  for(int i = 0; i < 5; i++)
    gamedata->currentmenu[i] = 0;
  gamedata->maxplayercargo = BASE_PLAYER_CARGO;
  gamedata->currentspeed = BASE_PLAYER_SPEED;
  gamedata->speedlevel = 0;
  gamedata->cargolevel = 0;
  initialize_islands(gamedata);
  initialize_player(gamedata);
  initialize_ships(gamedata);
}

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


int random(int max){
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

//takes the gamedata, and figures out how much of each resource exists
//returns the total value of each resource based on this, based on how much of each mineral
//exists in relation to the others
// 0 - 100 results in a high price for the 0
// 0 - 0 results in average price for both
//currentisland can either be a number 1 - max, or will default to "average" values with -1
//returns values for all 4 inside of int8_t data
ResourceValues getmoneyvalue(GameData* gamedata, int currentisland){
  APP_LOG(APP_LOG_LEVEL_INFO, "CURRENTISLAND IS: %i", currentisland);
  int totalmetal = 0, totalwood = 0, totalstone = 0, totalfood = 0, average = 0;
  for(int i = 0; i < TOTALISLANDS; i++){
    totalmetal += gamedata->islandscargo[i][0];
    totalwood += gamedata->islandscargo[i][1];
    totalstone  += gamedata->islandscargo[i][2];
    totalfood += gamedata->islandscargo[i][3];
  }
  average = totalmetal + totalwood + totalstone + totalfood;
  average = average / 4;
  
  ResourceValues returnvalue;
  
  //find initial values based on currency
  if(totalmetal > 0)
    returnvalue.metalvalue = ((10 * average)/totalmetal);
  else
    returnvalue.metalvalue = 1;
  if(totalstone > 0)
    returnvalue.stonevalue = ((10 * average)/totalstone);
  else
    returnvalue.stonevalue = 1;
  if(totalwood > 0)
    returnvalue.woodvalue = ((10 * average)/totalwood);
  else
    returnvalue.woodvalue = 1;
  if(totalfood > 0)
    returnvalue.foodvalue = ((10 * average)/totalfood);
  else
    returnvalue.foodvalue = 1;
  
  if(returnvalue.metalvalue > 200)
    returnvalue.metalvalue = 200;
  if(returnvalue.woodvalue > 200)
    returnvalue.woodvalue = 200;
  if(returnvalue.stonevalue > 200)
    returnvalue.stonevalue = 200;
  if(returnvalue.foodvalue > 200)
    returnvalue.foodvalue = 200;
  //adjust values based on island cargo levels if an island was selected
  if(currentisland != -1){
    returnvalue.metalvalue = returnvalue.metalvalue + (returnvalue.metalvalue* (50-gamedata->islandscargo[currentisland][0])/100);
    returnvalue.woodvalue = returnvalue.woodvalue + (returnvalue.woodvalue* (50-gamedata->islandscargo[currentisland][1])/100);
    returnvalue.stonevalue = returnvalue.stonevalue + (returnvalue.stonevalue* (50-gamedata->islandscargo[currentisland][2])/100);
    returnvalue.foodvalue =  returnvalue.foodvalue + (returnvalue.foodvalue* (50-gamedata->islandscargo[currentisland][3])/100);
  }
  
  
  //ten credits is on average worth 1 resource
  //islands value resources they have less than average of
  //islands do not value resources they have more than average of
  //a resource that has more than the average number of units, is less valuable
  //a resources that has less than the average number of units is more valuable
  
  return returnvalue;
}


void save_data(GameData* gamedata){
  //Object object;
  //object.field = 2;
  //persist_write_data(OBJECT_KEY, &object, sizeof(object));
  
  //set up the save values object with corresponding values from gamedata
  SaveValues saving;
  for(int i = 0; i < 10; i++){
    saving.shipsx[i] = gamedata->shipsx[i];
    saving.shipsy[i] = gamedata->shipsy[i];
    
    
    
    //these are hardcoded... for now.
    //islandsx[i] = gamedata->islandsx[i];
    //islandsy[i] =  = gamedata->islandsy[i];
    //islandstypes[i];
    for(int j = 0; j < 4; j++)
      saving.islandscargo[i][j] = gamedata->islandscargo[i][j];
    saving.shipsisland[i] = gamedata->shipsisland[i];
    saving.shipstype[i] = gamedata->shipstype[i];
  }
  for(int i = 0; i < 5; i++)
    saving.playercargo[i] = gamedata->playercargo[i];
  saving.playerwallet = gamedata->playerwallet;
  saving.maxplayercargo = gamedata->maxplayercargo;
  saving.currentspeed = gamedata->currentspeed;
  saving.cargolevel = gamedata->cargolevel;
  saving.speedlevel = gamedata->speedlevel;
  
  //save the object into memory
  persist_write_data(DATA_KEY, &saving, sizeof(saving));
}

void load_data(GameData* gamedata){
  SaveValues loading;
  //Object read_object;
  //persist_read_data(OBJECT_KEY, &read_object, sizeof(read_object));
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "read_object.field = %d", read_object.field);
  if(persist_exists(DATA_KEY)){
    persist_read_data(DATA_KEY, &loading, sizeof(loading));
    //load the values into gamedata
    for(int i = 0; i < 10; i++){
      gamedata->shipsx[i] = loading.shipsx[i];
      gamedata->shipsy[i] = loading.shipsy[i];
      //these are hardcoded... for now.
      //islandsx[i] = gamedata->islandsx[i];
      //islandsy[i] =  = gamedata->islandsy[i];
      //islandstypes[i];
      for(int j = 0; j < 4; j++)
        gamedata->islandscargo[i][j] = loading.islandscargo[i][j];
      gamedata->shipsisland[i] = loading.shipsisland[i];
      gamedata->shipstype[i] = loading.shipstype[i];
    }
    for(int i = 0; i < 5; i++)
      gamedata->playercargo[i] = loading.playercargo[i];
    gamedata->playerwallet = loading.playerwallet;
    gamedata->maxplayercargo = loading.maxplayercargo;
    gamedata->currentspeed = loading.currentspeed;
    gamedata->playeryvelocity = loading.currentspeed;
    gamedata->cargolevel = loading.cargolevel;
    gamedata->speedlevel = loading.speedlevel;
  }
}

int check_for_player_upgrade(GameData* gamedata, int upgradetype){
  return 0;
}

int check_player_upgrade_price(GameData* gamedata, int upgradetype){
  if(upgradetype == 0){
    return (gamedata->cargolevel + 1)*100;
  }
  if(upgradetype == 1){
    return (gamedata->speedlevel + 1)*500;
  }
  return 0;
}
