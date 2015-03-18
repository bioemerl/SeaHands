#include <pebble.h>
#include "worldevent.h"
#include "game.h"


const char eventnames[10] = {'m', 's', 'w', 'f', 'n', 'z', 'z', 'z', 'z', 'z'};
const int eventweights[10] = {5, 5, 5, 5, 25, 6, 7, 8, 9, 10};
const int NUMOFEVENTS = 5;

/*
m - metal shortage
s - stone shortage
w - wood shortage
f - food shortage
*/

int gettotalweights(int listofints[10]){
  int totalvalue = 0;
  for(int i = 0; i < NUMOFEVENTS; i++){
    totalvalue += eventweights[i];
    listofints[i] = totalvalue;
  }
  return totalvalue;
}
void updateevent(GameData* gamedata){ //change things around during game updates.
  
}

void triggerevent(GameData* gamedata, char event){ //display notifications, set values, and so on
  if(event == 'n'){
    displaynotification(gamedata, "Everything is\nnormal");
  }
  if(event == 'm'){
    displaynotification(gamedata, "Severe shortages\nof metal have\n been reported.");
    gamedata->metaldivisor = 2;
  }
  if(event == 's'){
    displaynotification(gamedata, "Severe shortages\nof stone have\n been reported.");
    gamedata->stonedivisor = 2;
  }
  if(event == 'w'){
    displaynotification(gamedata, "Severe shortages\nof wood have\n been reported.");
    gamedata->wooddivisor = 2;
  }
  if(event == 'f'){
    displaynotification(gamedata, "Severe shortages\nof food have\n been reported.");
    gamedata->foodmultiplier = 2;
    gamedata->fooddivisor = 3;
  }
}
void cancelevent(GameData* gamedata, char event){ //clean up or trigger things that occur at the end
  if(event == 'n'){
    
  }
  if(event == 'm'){
    gamedata->metaldivisor = 1;
    //do thing
  }
  if(event == 's'){
    gamedata->stonedivisor = 1;
    //do other thing
  }
  if(event == 'w'){
    gamedata->wooddivisor = 1;
    //do other thing
  }
  if(event == 'f'){
    gamedata->foodmultiplier = 1;
    gamedata->fooddivisor = 1;
    //do other thing
  }
  
}

void selectrandomevent(GameData* gamedata){
  //int random(int max);
  char returnvalue = 'z';
  int indavidualweights[10];
  int totalweights = gettotalweights(indavidualweights);
  int pointer = random(totalweights);
  for(int i = 0; i < NUMOFEVENTS; i++){
    if(pointer <= indavidualweights[i]){
      returnvalue = eventnames[i];
      i = NUMOFEVENTS;
    }
  }
  cancelevent(gamedata, gamedata->currentevent);
  triggerevent(gamedata, returnvalue);
  gamedata->currentevent = returnvalue;
}
