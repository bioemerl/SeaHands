#include <pebble.h>
#include "worldevent.h"
#include "game.h"


const char eventnames[10] = {'m', 's', 'w', 'f', 'z', 'z', 'z', 'z', 'z', 'z'};
const int eventweights[10] = {5, 5, 5, 5, 5, 6, 7, 8, 9, 10};
const int NUMOFEVENTS = 4;

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
  if(event == 'm'){
    displaynotification(gamedata, "Severe shortages\nof metal have\n been reported.");
  }
  if(event == 's'){
    displaynotification(gamedata, "Severe shortages\nof stone have\n been reported.");
  }
  if(event == 'w'){
    displaynotification(gamedata, "Severe shortages\nof wood have\n been reported.");
  }
  if(event == 'f'){
    displaynotification(gamedata, "Severe shortages\nof food have\n been reported.");
  }
}
void cancelevent(GameData* gamedata, char event){ //clean up or trigger things that occur at the end
  if(event == 'm'){
    //do thing
  }
  if(event == 's'){
    //do other thing
  }
  if(event == 'w'){
    //do other thing
  }
  if(event == 'f'){
    //do other thing
  }
}

void selectrandomevent(GameData* gamedata){
  //int random(int max);
  char returnvalue = 'w';
  int indavidualweights[10];
  int totalweights = gettotalweights(indavidualweights);
  int pointer = random(totalweights);
  for(int i = 0; i < NUMOFEVENTS; i++){
    if(pointer <= indavidualweights[i])
      returnvalue = eventnames[i];
    if(pointer > indavidualweights[i])
      break;
  }
  cancelevent(gamedata, gamedata->currentevent);
  triggerevent(gamedata, returnvalue);
  gamedata->currentevent = returnvalue;
}
