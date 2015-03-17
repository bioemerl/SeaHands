#pragma once
#include "types.h"

extern const char eventnames[10];
extern const int eventweights[10];
extern const int NUMOFEVENTS;

int gettotalweights(int listofints[10]);
void updateevent(GameData* gamedata);
void triggerevent(GameData* gamedata, char event);
void cancelevent(GameData* gamedata, char event);
void selectrandomevent(GameData* gamedata);