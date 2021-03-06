#include <pebble.h>
#include "game.h"
#include "player.h"
#include "island.h"
#include "ship.h"
#include "shipbattle.h"
#include "worldevent.h"

//main character's name is bill
//bill because he looks for money all the time.

static unsigned long long upcounter = 1;

void initialize_game(GameData* gamedata){
  gamedata->playership = -1;
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
  gamedata->buttonrelease = 1;
  //set up the tutorial
  gamedata->tutorialcounter = -1;
  if(persist_exists(DATA_KEY))
     gamedata->tutorialcounter = 7;
}

void update_game(GameData* gamedata){
  
  if(gamedata->tutorialcounter <= 6)
      attempt_tutorial(gamedata);
  
  //set up a timer as to not update everything every frame
  upcounter++;
  //what to do on each timer final
  if(upcounter%200 == 0){
    burn_player_cargo(gamedata);
    update_islands(gamedata);
  }
  if(upcounter%2000 == 0){
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    int currenthour = tick_time->tm_hour;
    int currentday = tick_time->tm_yday;
    if(currenthour != gamedata->eventhour || currentday != gamedata->eventday){
      selectrandomevent(gamedata);
      gamedata->eventhour = currenthour;
      gamedata->eventday = currentday;
    }
    //always do this on the largest upcounter
    upcounter = 1;
  }
    updateevent(gamedata);
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
  if(currentisland >= 10){
    ResourceValues empty;
    empty.metalvalue = 0;
    empty.stonevalue = 0;
    empty.woodvalue = 0;
    empty.foodvalue = 0;
    return empty;
  }
  //APP_LOG(APP_LOG_LEVEL_INFO, "CURRENTISLAND IS: %i", currentisland);
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
  for(int i = 0; i < 15; i++){
    saving.shipsowner[i] = gamedata->shipsowner[i];
    saving.shipstype[i] = gamedata->shipstype[i];
    saving.shipscargo[i] = gamedata->shipscargo[i];
    saving.shipsorder[i] = gamedata->shipsorder[i];
    saving.shipsx[i] = gamedata->shipsx[i];
    saving.shipsy[i] = gamedata->shipsy[i];
    for(int j = 0; j < 3; j++)
      saving.shipsorderinfo[i][j] = gamedata->shipsorderinfo[i][j];
  }
  for(int i = 0; i < 10; i++)
    saving.islandsallegiance[i] = gamedata->islandsallegiance[i];
  for(int i = 0; i < 11; i++){
    for(int j = 0; j < 4; j++)
      saving.islandscargo[i][j] = gamedata->islandscargo[i][j];
  }
  for(int i = 0; i < 5; i++)
    saving.playercargo[i] = gamedata->playercargo[i];
  saving.playerwallet = gamedata->playerwallet;
  saving.maxplayercargo = gamedata->maxplayercargo;
  saving.currentspeed = gamedata->currentspeed;
  saving.cargolevel = gamedata->cargolevel;
  saving.speedlevel = gamedata->speedlevel;
  saving.metalmultiplier = gamedata->metalmultiplier;
  saving.stonemultiplier = gamedata->stonemultiplier;
  saving.woodmultiplier = gamedata->woodmultiplier;
  saving.foodmultiplier = gamedata->foodmultiplier;
  saving.metaldivisor = gamedata->metaldivisor;
  saving.stonedivisor = gamedata->stonedivisor;
  saving.wooddivisor = gamedata->wooddivisor;
  saving.fooddivisor = gamedata->fooddivisor;
  saving.currentevent = gamedata->currentevent;
  saving.eventhour = gamedata->eventhour;
  saving.eventday = gamedata->eventday;
  saving.totalships = gamedata->totalships;
  
  saving.storagewallet = gamedata->storagewallet;
  saving.storagex = gamedata->islandsx[10];
  saving.storagey = gamedata->islandsy[10];
  saving.storageexists = gamedata->storageexists;
  saving.numberofplayerships = gamedata->numberofplayerships;
  
  //save the object into memory
  persist_write_int(DATA_VERSION_KEY, CURRENT_SAVE_VERSION);
  persist_write_data(DATA_KEY, &saving, sizeof(saving));
}

void load_data(GameData* gamedata){
  if(persist_exists(DATA_VERSION_KEY)){
    if(persist_exists(DATA_KEY)){
      int versionnumber = 0;
      if(persist_exists(DATA_VERSION_KEY))
        versionnumber = persist_read_int(DATA_VERSION_KEY);
      if(versionnumber == 3){
        SaveValues versiontwoloading;
        persist_read_data(DATA_KEY, &versiontwoloading, sizeof(versiontwoloading));
        for(int i = 0; i < 15; i++){
          gamedata->shipsowner[i] = versiontwoloading.shipsowner[i];
          gamedata->shipstype[i] = versiontwoloading.shipstype[i];
          gamedata->shipscargo[i] = versiontwoloading.shipscargo[i];
          gamedata->shipsorder[i] = versiontwoloading.shipsorder[i];
          gamedata->shipsx[i] = versiontwoloading.shipsx[i];
          gamedata->shipsy[i] = versiontwoloading.shipsy[i];
          for(int j = 0; j < 3; j++)
           gamedata->shipsorderinfo[i][j] = versiontwoloading.shipsorderinfo[i][j];
        }
        for(int i = 0; i < 10; i++)
          gamedata->islandsallegiance[i] = versiontwoloading.islandsallegiance[i];
        for(int i = 0; i < 11; i++){
          for(int j = 0; j < 4; j++)
            gamedata->islandscargo[i][j] = versiontwoloading.islandscargo[i][j];
        }
        for(int i = 0; i < 5; i++)
          gamedata->playercargo[i] = versiontwoloading.playercargo[i];
        gamedata->playerwallet = versiontwoloading.playerwallet;
        gamedata->maxplayercargo = versiontwoloading.maxplayercargo;
        gamedata->currentspeed = versiontwoloading.currentspeed;
        gamedata->cargolevel = versiontwoloading.cargolevel;
        gamedata->speedlevel = versiontwoloading.speedlevel;
        gamedata->metalmultiplier = versiontwoloading.metalmultiplier;
        gamedata->stonemultiplier = versiontwoloading.stonemultiplier;
        gamedata->woodmultiplier = versiontwoloading.woodmultiplier;
        gamedata->foodmultiplier = versiontwoloading.foodmultiplier;
        gamedata->metaldivisor = versiontwoloading.metaldivisor;
        gamedata->stonedivisor = versiontwoloading.stonedivisor;
        gamedata->wooddivisor = versiontwoloading.wooddivisor;
        gamedata->fooddivisor = versiontwoloading.fooddivisor;
        gamedata->currentevent = versiontwoloading.currentevent;
        gamedata->eventhour = versiontwoloading.eventhour;
        gamedata->eventday = versiontwoloading.eventday;
        gamedata->totalships = versiontwoloading.totalships;
  
        gamedata->storagewallet = versiontwoloading.storagewallet;
        gamedata->islandsx[10] = versiontwoloading.storagex;
        gamedata->islandsy[10] = versiontwoloading.storagey;
        gamedata->storageexists = versiontwoloading.storageexists;
        gamedata->numberofplayerships = versiontwoloading.numberofplayerships;
      }
      if(versionnumber == 2){
        APP_LOG(APP_LOG_LEVEL_INFO, "USING OLD SYSTEM");
        SaveValuesOne realloading;
        persist_read_data(DATA_KEY, &realloading, sizeof(realloading));
        
        //load the values into gamedata
        for(int i = 0; i < 10; i++){
          gamedata->shipsx[i] = realloading.shipsx[i];
          gamedata->shipsy[i] = realloading.shipsy[i];
          gamedata->islandsallegiance[i] = realloading.islandsallegiance[i];
          //these are hardcoded... for now.
          //islandsx[i] = gamedata->islandsx[i];
          //islandsy[i] =  = gamedata->islandsy[i];
          //islandstypes[i];
          for(int j = 0; j < 4; j++)
            gamedata->islandscargo[i][j] = realloading.islandscargo[i][j];
          gamedata->shipsowner[i] = realloading.shipsowner[i];
          gamedata->shipstype[i] = realloading.shipstype[i];
          gamedata->shipscargo[i] = realloading.shipscargo[i];
          gamedata->shipsorder[i] = realloading.shipsorder[i];
          for(int j = 0; j < 3; j++)
            gamedata->shipsorderinfo[i][j] = realloading.shipsorderinfo[i][j];
        }
        for(int i = 0; i < 5; i++)
          gamedata->playercargo[i] = realloading.playercargo[i];
        gamedata->playerwallet = realloading.playerwallet;
        gamedata->maxplayercargo = realloading.maxplayercargo;
        gamedata->currentspeed = realloading.currentspeed;
        gamedata->playeryvelocity = realloading.currentspeed;
        gamedata->cargolevel = realloading.cargolevel;
        gamedata->speedlevel = realloading.speedlevel;
        gamedata->metalmultiplier = realloading.metalmultiplier;
        gamedata->stonemultiplier = realloading.stonemultiplier;
        gamedata->woodmultiplier = realloading.woodmultiplier;
        gamedata->foodmultiplier = realloading.foodmultiplier;  
        gamedata->metaldivisor = realloading.metaldivisor;
        gamedata->stonedivisor = realloading.stonedivisor;
        gamedata->wooddivisor = realloading.wooddivisor;
        gamedata->fooddivisor = realloading.fooddivisor;
        gamedata->currentevent = realloading.currentevent;
        gamedata->eventhour = realloading.eventhour;
        gamedata->eventday = realloading.eventday;
        gamedata->totalships = realloading.totalships;
      }
    }
    else{
      //if no previous data values, set default values
      gamedata->playerwallet = 500;
    }
  }
  else{
    //if no previous data values, set default values
    gamedata->playerwallet = 500;
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

//returns 1 for up 2 for down, and 3 for select
int check_current_button(GameData* gamedata){
  if(gamedata->uphit == 1 && gamedata->downhit == 0)
    return 1;
  if(gamedata->uphit == 0 && gamedata->downhit == 1)
    return 2;
  if(gamedata->uphit == 1 && gamedata->downhit == 1)
    return 3;
  return 0;
}

//will setup the display of notifications, including the changing gamemode, loading the data in gamedata
//it should eventually manage data for variables in notifications
void displaynotification(GameData* gamedata, char notificationtext[150]){
  //here is where I would process and insert variable values into the string
  //however, that is a bit fancy and tough to do, and I don't need it right now
  //so do that later, future me.
  gamedata->gamemode = 'm';
  gamedata->menulayer = 4;
  for(int i = 0; i < 150; i++){
    gamedata->notificationtext[i] = notificationtext[i];
  }
}

//will show a notification for the tutorial
//if forcetutorial is one, it will always show the tutorial.
void attempt_tutorial(GameData* gamedata){
  if(gamedata->buttonrelease == 1){
    if(gamedata->gamemode != 'm'){
      gamedata->tutorialcounter++;
      gamedata->buttonrelease = 0;
    }
    if(gamedata->tutorialcounter == 0)
      displaynotification(gamedata, "WELCOME TO SEA HANDS!\nTutorial:\nMt = Metal\nWd = Wood\nSt = Stone\nFd = Food\nSu = Supplies");
    if(gamedata->tutorialcounter == 1)
      displaynotification(gamedata, "Bottom right corner\nshows your supplies.\nrestock at any island.\nSupplies are used over time.\n\nSupplies cost $10\nand one wood\nand one food.");
    if(gamedata->tutorialcounter == 2)
      displaynotification(gamedata, "Up/Down keys to turn.\n\nRun into islands\nor ships\nto bring up menu.\n\nRemember to use\n-exit- instead of\nthe back button.");
    if(gamedata->tutorialcounter == 3)
      displaynotification(gamedata, "Long press select\nwhen not at an island\nto bring up the player menu.\n\nLong press down\nto leave menus\nat any time.");
    if(gamedata->tutorialcounter == 4)
      displaynotification(gamedata, "Make money by\npillaging ships\nor by\nselling resources high\nand buying them low");
    if(gamedata->tutorialcounter == 5)
      displaynotification(gamedata, "Build your HQ\n using the\n long press menu.\nStore resources\nOnce you load 40.\nHQ will consume resources.\nLet you build ships.");
    if(gamedata->tutorialcounter == 6)
      displaynotification(gamedata, "Wind effects\nyour ship.\nPay attention\nto the time.\n\nGood Luck!");
    
  }
}

void exitmenus(GameData* gamedata){
  if(gamedata->gamemode == 'm'){
    if((gamedata->menulayer == 0 || gamedata->menulayer == 1 || gamedata->menulayer == 2 || gamedata->menulayer == 6)){
      exitisland(gamedata);
      gamedata->currentmenu[0] = 0;
      gamedata->currentmenu[1] = 0;
      gamedata->currentmenu[2] = 0;
    }
    if(gamedata->menulayer == 3){
      gamedata->playerisland = gamedata->shipsorderinfo[gamedata->playership][2];
      exitisland(gamedata);
      gamedata->playership = -1;
    }
    gamedata->currentmenu[gamedata->menulayer] = 0;
    gamedata->menulayer = 0;
    gamedata->gamemode = 'p';
  }
}

void adjustminimapcoords(int coords[2], int largestx, int lowestx, int largesty, int lowesty){
  int totalxrange = 1170;
  int totalyrange = 1170;
  //height of pebble screen: 148
  //width of pebble screen: 144
  coords[0] = (coords[0] * 144)/totalxrange;
  coords[1] = (coords[1] * 148)/totalyrange;
  //adjust for 0,0 being the top left
  coords[0] = coords[0] + 72;
  coords[1] = coords[1] + 74;
}

int manageislandallegiance(GameData* gamedata, int8_t islandnumber, int8_t adjustnumber){
  if(adjustnumber != 0 && (gamedata->islandsallegiance[islandnumber] < 100 && gamedata->islandsallegiance[islandnumber] > -100)){
    gamedata->islandsallegiance[islandnumber] += adjustnumber;
    if(gamedata->islandsallegiance[islandnumber] > 100)
      gamedata->islandsallegiance[islandnumber] = 100;
    if(gamedata->islandsallegiance[islandnumber] < -100)
      gamedata->islandsallegiance[islandnumber] = -100;
  }
  return gamedata->islandsallegiance[islandnumber];
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