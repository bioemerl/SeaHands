#pragma once
#include "types.h"

extern const char eventnames[10];
extern const int eventweights[10];

int gettotalweights(int listofints[10]);
void updateevent(GameData* gamedata);
void triggerevent(GameData* gamedata);
void cancelevent(GameData* gamedata);
char selectrandomevent();