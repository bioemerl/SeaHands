#include <pebble.h>
#include "game.h"
#include "shipbattle.h"
  
//prototypes
void draw_ships(Layer *this_layer, GContext *ctx);
void draw_gui(Layer *this_layer, GContext *ctx);
void draw_islands(Layer *this_layer, GContext *ctx);
void draw_menu_layer(Layer *this_layer, GContext *ctx, int menulayernumber, int x, int y);
void drawmaingame(Layer *this_layer, GContext *ctx);
void drawbattle(Layer *this_layer, GContext *ctx);
void draw_edge_points(Layer *this_layer, GContext *ctx);
void draw_map(Layer *this_layer, GContext *ctx);
void draw_wind_arrow(Layer *this_layer, GContext *ctx);
void drawmenuandbox(Layer *this_layer, GContext *ctx, int issquarelayer, int layernumber, int itemscount, int x, int y, int xdiff, int offset, char text[100]);
  
//define a number of constants
const uint8_t PEBBLEHEIGHT = 168;
const uint8_t PEBBLEWIDTH = 144;
const uint8_t NUMOFSHIPS = 0;
const uint8_t NUMOFISLANDS = 1;
//used for island AI to sell off materials at certain prices
const uint8_t TARGETVALUEMETAL = 100;
const uint8_t TARGETVALUEWOOD = 100;
const uint8_t TARGETVALUESTONE = 100;
const uint8_t TARGETVALUEFOOD = 100;
//values for the player stats
const uint8_t MINSHIPSPEED = 1;
const uint8_t MAXSHIPSPEED = 10;
const uint8_t SHIPMAXCARGO = 10; //per resource, not as a whole
const uint8_t NUMBEROFMENUITEMS = 5;

//create the window and canvas layer
static Window *window;
static Layer *graphics_canvas_layer;

GameData gamedata;
ShipBattleData shipbattledata;

static void canvas_update_proc(Layer *this_layer, GContext *ctx){
  //GRect bounds = layer_get_bounds(this_layer);
  //get the bounds of the layer
  //get center of screen
  //GPoint center = GPoint((bounds.size.w / 2), (bounds.size.h / 2));
  if(gamedata.gamemode != 'b')
    drawmaingame(this_layer, ctx);
  if(gamedata.gamemode == 'b')
    drawbattle(this_layer, ctx);
  if(gamedata.gamemode == 'w')
    draw_map(this_layer, ctx);
  
}

void drawbattle(Layer *this_layer, GContext *ctx){
  //graphics_fill_rect(ctx, GRect(x,y,54,90), 0, GCornerNone);
  
  graphics_context_set_fill_color(ctx, GColorBlack);
  GPoint bullet;
  //draw all bullets
  for(int i = 1; i <= shipbattledata.numberofshots; i++){
    bullet = GPoint(shipbattledata.shotsx[i],shipbattledata.shotsy[i]);
    graphics_fill_circle(ctx, bullet, 3);
  }
  //draw player and enemy
  
  //draw the player body
  graphics_fill_rect(ctx, GRect(shipbattledata.playerx - PLAYER_SHIP_SIZE/2, PLAYER_Y_LEVEL - PLAYER_SHIP_SIZE/4, PLAYER_SHIP_SIZE, (PLAYER_SHIP_SIZE/2) + 1), 0, GCornerNone);
  //draw player endpoints
  graphics_fill_circle(ctx, GPoint(shipbattledata.playerx - PLAYER_SHIP_SIZE/2 ,PLAYER_Y_LEVEL), PLAYER_SHIP_SIZE/4);
  graphics_fill_circle(ctx, GPoint(shipbattledata.playerx + PLAYER_SHIP_SIZE/2 ,PLAYER_Y_LEVEL), PLAYER_SHIP_SIZE/4);
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, GPoint(shipbattledata.playerx, PLAYER_Y_LEVEL), PLAYER_SHIP_SIZE/4 - PLAYER_SHIP_SIZE/8);
  graphics_context_set_fill_color(ctx, GColorBlack);
  
  //draw enemy body
  graphics_fill_rect(ctx, GRect(shipbattledata.enemyx - ENEMY_SHIP_SIZE/2, ENEMY_Y_LEVEL - ENEMY_SHIP_SIZE/4, ENEMY_SHIP_SIZE, (ENEMY_SHIP_SIZE/2) + 1), 0, GCornerNone);
  //draw enemy endpoints
  graphics_fill_circle(ctx, GPoint(shipbattledata.enemyx - ENEMY_SHIP_SIZE/2 ,ENEMY_Y_LEVEL), ENEMY_SHIP_SIZE/4);
  graphics_fill_circle(ctx, GPoint(shipbattledata.enemyx + ENEMY_SHIP_SIZE/2 ,ENEMY_Y_LEVEL), ENEMY_SHIP_SIZE/4);
}

void drawmaingame(Layer *this_layer, GContext *ctx){
  //draw the ocean
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorBlueMoon);
  #else
    graphics_context_set_fill_color(ctx, GColorWhite);
  #endif
  graphics_fill_rect(ctx, GRect(0,0,200,200), 0, GCornerNone);
    
  //draw the player
  
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorWindsorTan);
  #else
    graphics_context_set_fill_color(ctx, GColorBlack);
  #endif
    
  GPoint player = GPoint(72, 82);
  //draw the player
  graphics_fill_circle(ctx, player, 5);
  
  //draw the direction, if it is not zero
  
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorArmyGreen);
  #else
    graphics_context_set_fill_color(ctx, GColorBlack);
  #endif
    
  if((gamedata.playerxvelocity != 0) || (gamedata.playeryvelocity !=0)){
    if( (abs(gamedata.playerxvelocity) <= 5) && (abs(gamedata.playeryvelocity) <= 5 )){
      #ifdef PBL_COLOR
        graphics_context_set_fill_color(ctx, GColorArmyGreen);
      #else
        graphics_context_set_fill_color(ctx, GColorWhite);
      #endif
    }
    //72 and 82 are the center of pebble's screen.
    GPoint playervector = GPoint((72 + gamedata.playerxvelocity), (82 + gamedata.playeryvelocity));
    graphics_fill_circle(ctx, playervector, 2);
  }
  draw_edge_points(this_layer, ctx);
  draw_islands(this_layer, ctx);
  draw_ships(this_layer, ctx);
  draw_gui(this_layer, ctx);
  draw_wind_arrow(this_layer, ctx);
  
  //draw the menus
  if(gamedata.gamemode == 'm'){
    //always draw the base menu
    if(gamedata.menulayer == 0 || gamedata.menulayer == 1 || gamedata.menulayer == 2)
      draw_menu_layer(this_layer, ctx, 0, 0, 15); //the base menu layer
    if(gamedata.menulayer == 1)//buysellmenu
      draw_menu_layer(this_layer, ctx, 1, 57, 15);
    if(gamedata.menulayer == 2) // the islands upgrade menu
      draw_menu_layer(this_layer, ctx, 2, 57, 15);
    if(gamedata.menulayer == 3) //the ship pillage menu
      draw_menu_layer(this_layer, ctx, 3, 0, 15);
    if(gamedata.menulayer == 4)//the notifications menu
      draw_menu_layer(this_layer, ctx, 4, 0, 0);
    if(gamedata.menulayer == 5) //the player start menu
      draw_menu_layer(this_layer, ctx, 5, 0, 15);
    if(gamedata.menulayer == 6 || gamedata.menulayer == 7)
      draw_menu_layer(this_layer, ctx, 6, 0, 15);
    if(gamedata.menulayer == 7 || gamedata.menulayer == 8)
      draw_menu_layer(this_layer, ctx, 7, 0, 15);
    if(gamedata.menulayer == 8)
      draw_menu_layer(this_layer, ctx, 8, 57, 15);
    if(gamedata.menulayer == 9)
      draw_menu_layer(this_layer, ctx, 9, 0, 15);
    if(gamedata.menulayer == 10)
      draw_menu_layer(this_layer, ctx, 10, 0, 15);
    if(gamedata.menulayer == 11)
      draw_menu_layer(this_layer, ctx, 11, 57, 15);
    if(gamedata.menulayer == 12)
      draw_menu_layer(this_layer, ctx, 12, 57, 15);
    if(gamedata.menulayer == 13)
      draw_menu_layer(this_layer, ctx, 13, 57, 15);
  }
  
  
}

void draw_wind_arrow(Layer *this_layer, GContext *ctx){
  //is a number 0 through 8
  //want to put the icon next to this: GRect(0, PEBBLEHEIGHT - 29, 55, 15);
  //so, about at point -29 -> -19
  //so at Y = PEBBLEHEIGHT - 24 X = 60
  uint8_t linestartx = 62, linestarty = PEBBLEHEIGHT - 24;
  uint8_t lineendx = 62, lineendy = PEBBLEHEIGHT - 24;
  int8_t multiplier = 2;
  
  //determine the start and end points for the arrow, depending on the wind direction
  if(gamedata.currentwindspeed == 1){
    lineendx += 0 * multiplier;
    lineendy += 2 * multiplier;
    linestartx -= 0 * multiplier;
    linestarty -= 2 * multiplier;
  }
  if(gamedata.currentwindspeed == 2){
    lineendx += 1 * multiplier;
    lineendy += 1 * multiplier;
    linestartx -= 1 * multiplier;
    linestarty -= 1 * multiplier;
  }
  if(gamedata.currentwindspeed == 3){
    lineendx += 2 * multiplier;
    lineendy += 0 * multiplier;
    linestartx -= 2 * multiplier;
    linestarty -= 0 * multiplier;
  }
  if(gamedata.currentwindspeed == 4){
    lineendx += 1 * multiplier;
    lineendy += -1 * multiplier;
    linestartx -= 1 * multiplier;
    linestarty -= -1 * multiplier;
  }
  if(gamedata.currentwindspeed == 5){
    lineendx += 0 * multiplier;
    lineendy += -2 * multiplier;
    linestartx -= 0 * multiplier;
    linestarty -= -2 * multiplier;
  }
  if(gamedata.currentwindspeed == 6){
    lineendx += -1 * multiplier;
    lineendy += -1 * multiplier;
    linestartx -= -1 * multiplier;
    linestarty -= -1 * multiplier;
  }
  if(gamedata.currentwindspeed == 7){
    lineendx += -2 * multiplier;
    lineendy += 0 * multiplier;
    linestartx -= -2 * multiplier;
    linestarty -= 0 * multiplier;
  }
  if(gamedata.currentwindspeed == 8){
    lineendx += -1 * multiplier;
    lineendy += 1 * multiplier;
    linestartx -= -1 * multiplier;
    linestarty -= 1 * multiplier;
  }
  //draw the arrow end point as a dot at the end point, and the line from start to end
  //draw the background
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(56, PEBBLEHEIGHT - 29, 12, 12), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, GColorBlack); //draw the box
  graphics_draw_rect(ctx, GRect(56, PEBBLEHEIGHT - 29, 12, 12));
  graphics_draw_line(ctx, GPoint(lineendx, lineendy), GPoint(linestartx, linestarty)); //draw arrow
  graphics_fill_circle(ctx, GPoint(lineendx, lineendy), 1); //draw point
}

void draw_edge_points(Layer *this_layer, GContext *ctx){
  for(int i = 0; i < TOTALISLANDS; i++){
    int fullx = gamedata.islandsx[i] - gamedata.playerx;
    int fully = gamedata.islandsy[i] - gamedata.playery;
    int pointx, pointy;
    pointx = 0;
    pointy = 0;
    pointx = pointy + pointx; //because you have to use pointx, apparently.
    //check if the line is straight
    if(abs(fullx) < 72 && abs(fully) < 84){
      
    }
    else if(fullx == 0){
      pointx = 72;
      if(fully > 0)
        pointy = 138;
      if(fully < 0)
        pointy = 15;
    }
    else if(fully == 0){
      pointy = (77);
      if(fullx > 0)
        pointx = 144;
      if(fully < 0)
        pointx = 0;
    }
    else{
      //create values
      if(fullx > 0){
        pointx = 144;
        pointy = (77) + ((fully * 77)/fullx);
      }
      if(fullx < 0){
        pointx = 0;
        pointy = (77) - ((fully * 77)/fullx);
      }

      if(pointy >= 138 || pointy <= 15){
        if(fully < 0){
          pointy = 15;
          pointx = (72) - ((fullx * 72)/fully);
        }
        if(fully > 0){
          pointy = 138;
          pointx = (72) + ((fullx * 72)/fully);
        }
        
      }
    }
    //draw the point here
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_circle(ctx, GPoint(pointx, pointy), 4);
    graphics_context_set_fill_color(ctx, GColorWhite);
    if(gamedata.islandstypes[i] == 0)
      graphics_fill_circle(ctx, GPoint(pointx, pointy), 0);
    if(gamedata.islandstypes[i] == 1)
      graphics_fill_circle(ctx, GPoint(pointx, pointy), 1);
    if(gamedata.islandstypes[i] == 2)
      graphics_fill_circle(ctx, GPoint(pointx, pointy), 2);
    if(gamedata.islandstypes[i] == 3)
      graphics_fill_circle(ctx, GPoint(pointx, pointy), 3);
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
}

//system for menus will be as such:
//I am going to have menu "layers" which I should probably rename to something else
//Each layer is a different set of options for a menu.  0 is the base menu, 1 is the buy/sell, 2 is the upgrades
//this function should draw the specific menu it is requested, at the point that menu is typically at.  It should draw the
//menu at an X/Y coordinate given to it.  The selected item in the menu is based off the corresponding int inside of menulayer[]
//the currentmenu int value tells the function that calls this one which layers to draw, and where
//currentmenu should also probably be renamed to "menuformat" or something of that sort
void draw_menu_layer(Layer *this_layer, GContext *ctx, int menulayernumber, int x, int y){
  
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  
  //any layer: draw the background box, then draw the highlighting box, then draw the text
  //base menu layer
  if(menulayernumber == 0){
    
    graphics_fill_rect(ctx, GRect(x,y,57, 4 + y+(MENUITEMSCNT - 1)*15 - MENUITEMSCNT), 0, GCornerNone); //background box
    graphics_draw_rect(ctx, GRect(x, 2 + y+gamedata.currentmenu[0]*15 - gamedata.currentmenu[0], 57, 15 )); //highlighting box
    
    //text prep
    GRect textbox = GRect(x, y, 57, 105);
    char totalmenu[60];
    //create the char array for the menu    
    snprintf(totalmenu, sizeof(totalmenu), " Metal:%d\n Wood:%d\n Stone:%d\n Food:%d\n Island:\n -Exit-",
           gamedata.islandscargo[gamedata.playerisland][0],
           gamedata.islandscargo[gamedata.playerisland][1],
           gamedata.islandscargo[gamedata.playerisland][2],
           gamedata.islandscargo[gamedata.playerisland][3]);
    //draw text
    graphics_draw_text(ctx, totalmenu, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), textbox, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
    
    //draw two white bars to display which resource is being bought or sold
    int producerectpos, consumerectpos;
    //find where the island produce is on the menu
    producerectpos = 19 + (gamedata.islandstypes[gamedata.playerisland] * 15) - gamedata.islandstypes[gamedata.playerisland];
    //find where island consumption is on the menu
    if(gamedata.islandstypes[gamedata.playerisland] == 0){
      consumerectpos = 19 + (2 * 15) - 2; //2 + 2 + 15 + (2 * 15) - 2;
    }
    if(gamedata.islandstypes[gamedata.playerisland] == 1){
      consumerectpos = 19 + (3 * 15) - 3; //2 + 2 + 15 + (3 * 15) - 3;
    }
    if(gamedata.islandstypes[gamedata.playerisland] == 2){
      consumerectpos = 19 + (1 * 15) - 1; //2 + 2 + 15 + (1 * 15) - 1;
    }
    if(gamedata.islandstypes[gamedata.playerisland] == 3){
      consumerectpos = 19 + (0 * 15) - 0; //2 + 2 + 15 + (0 * 15) - 0;
    }
    //draw the icons for the two
    graphics_context_set_fill_color(ctx, GColorWhite);
    GRect producerect = GRect(53, producerectpos, 2, 11);
    GRect consumerect = GRect(54, consumerectpos, 1, 11);
    graphics_fill_rect(ctx, producerect, 0, GCornerNone);
    graphics_fill_rect(ctx, consumerect, 0, GCornerNone);
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
  //menu layer for buying and selling resources
  if(menulayernumber == 1){
    
    graphics_fill_rect(ctx, GRect(x,y,46,4 + y+(MENU2ITEMSCNT - 1)*15 - MENU2ITEMSCNT), 0, GCornerNone);
    graphics_draw_rect(ctx, GRect(x, 2 + y+gamedata.currentmenu[1]*15 - gamedata.currentmenu[1], 46, 15));
    //2 + y+gamedata.currentmenu[0]*15 - gamedata.currentmenu[0]
      
    //needs to be made better later, shouldn't calculate for all at once, should do one at a time
    //this change will increase performance
    ResourceValues buysellvals = getmoneyvalue(&gamedata, gamedata.playerisland);
    int resourcevalue = 0;
    if(gamedata.currentmenu[0] == 0)
      resourcevalue = buysellvals.metalvalue;
    if(gamedata.currentmenu[0] == 1)
      resourcevalue = buysellvals.woodvalue;
    if(gamedata.currentmenu[0] == 2)
      resourcevalue = buysellvals.stonevalue;
    if(gamedata.currentmenu[0] == 3)
      resourcevalue = buysellvals.foodvalue;
    gamedata.currentcosts = resourcevalue;
    //end area that needs changing 
    //prepare the text
    GRect layer2text = GRect(x,y,46, 50);
    char firstmenulayer[30];
    snprintf(firstmenulayer, sizeof(firstmenulayer), " -Back-\n Buy:%i\n Sell:", resourcevalue);
    //print the text
    graphics_draw_text(ctx, firstmenulayer, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), layer2text, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  }
  //menu layer for upgrade purchases
  if(menulayernumber == 2){
    graphics_fill_rect(ctx, GRect(x,y,80,15 + 4 + y+(MENU3ITEMSCNT - 1)*15 - MENU3ITEMSCNT), 0, GCornerNone);
    graphics_draw_rect(ctx, GRect(x, 2 + y+(gamedata.currentmenu[2]+1)*15 - gamedata.currentmenu[2], 80, 15));
    GRect layer3text = GRect(x,y,80, 80);
    //get needed data
    //find current upgrade level, and change the price for that upgrade by a function on the constant
    //BASE_VALUE_UPGRADENAME
    int cargoprice = 1;
    int speedprice = 1;
    
    char secondmenulayer[67];
    int upgradeprices[3];
    for(int i = 0; i < 2; i++)
      upgradeprices[i] = check_player_upgrade_price(&gamedata, i);
    cargoprice = upgradeprices[0];
    speedprice = upgradeprices[1];
    snprintf(secondmenulayer, sizeof(secondmenulayer), "Faction:%i\n BuySu:%i\n UpCrgo:%i\n UpSpd: %i\n DnSpd: %i -Back-", manageislandallegiance(&gamedata, gamedata.playerisland, 0), BASE_PRICE_SUPPLIES, cargoprice, speedprice, speedprice);
    graphics_draw_text(ctx, secondmenulayer, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), layer3text, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  }
  //menu layer for ship interactions
  if(menulayernumber == 3){
    char thirdmenulayer[25];
    char shipcontents[10] = "none";
    //check what the ship has in it's cargo hold
    if(gamedata.shipstype[gamedata.playership] == 0)
      snprintf(shipcontents, sizeof(shipcontents), "Metal");
    if(gamedata.shipstype[gamedata.playership] == 1)
      snprintf(shipcontents, sizeof(shipcontents), "Wood");
    if(gamedata.shipstype[gamedata.playership] == 2)
      snprintf(shipcontents, sizeof(shipcontents), "Stone");
    if(gamedata.shipstype[gamedata.playership] == 3)
      snprintf(shipcontents, sizeof(shipcontents), "Food");
    if(gamedata.shipscargo[gamedata.playership] == 0)
      snprintf(shipcontents, sizeof(shipcontents), "None");
    snprintf(thirdmenulayer, sizeof(thirdmenulayer), " %s\n -Back-:\n Pillage", shipcontents);
    drawmenuandbox(this_layer, ctx, 0, 3, MENU4ITEMSCNT, x, y, 50, 1, thirdmenulayer);
  }
  //menu layer for notification pop ups
  if(menulayernumber == 4){
    GRect layer5area = GRect(8, 10, 128, 134);
    graphics_fill_rect(ctx, layer5area, 0, GCornerNone);
    GRect layer5text = GRect(8, 10, 128, 180);
    graphics_draw_text(ctx, gamedata.notificationtext, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), layer5text, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  }
  if(menulayernumber == 5){ //player "start menu"
    drawmenuandbox(this_layer, ctx, 0, 5, MENU6ITEMSCNT, x, y, 54, 0, " Map:\n Event:\n Tutorial\n Build\n -Back-");
  }
  if(menulayernumber == 6){
    drawmenuandbox(this_layer, ctx, 0, 6, MENU7ITEMSCNT, x, y, 54, 0, " Resource:\n Ships:\n -Back-");
  }
  if(menulayernumber == 7){
    char menu7text[100];
    snprintf(menu7text, sizeof(menu7text), " Metal: %i\n Wood: %i\n Stone%i\n Food %i\n -Back-", gamedata.islandscargo[10][0], gamedata.islandscargo[10][1], gamedata.islandscargo[10][2], gamedata.islandscargo[10][3]);
    drawmenuandbox(this_layer, ctx, 0, 7, MENU8ITEMSCNT, x, y, 54, 0, menu7text);
  }
  if(menulayernumber == 8){
    drawmenuandbox(this_layer, ctx, 0, 8, MENU9ITEMSCNT, x, y, 54, 0, " -Back-\n Store:\n Take");
  }
  if(menulayernumber == 9){
    drawmenuandbox(this_layer, ctx, 0, 9, MENU10ITEMSCNT, x, y, 54, 0, " Order:\n Buy:\n Sell:\n -Back-");
  }
  if(menulayernumber == 10){
    int numberofitems = gamedata.numberofplayerships + 1; //only draw the number of slots necessary based on how many player ships there are
    drawmenuandbox(this_layer, ctx, 0, 10, numberofitems, x, y, 54, 0, " -Back-\n Ship1:\n Ship2:\n Ship3:\n Ship4:\n");
  }
  if(menulayernumber == 11){
    drawmenuandbox(this_layer, ctx, 1, 11, MENU12ITEMSCNT, x, y, 46, 0, " 1      2\n 3      4\n 5      6 \n 7      8\n 9      10\n -Back-");
  }
  if(menulayernumber == 12){
    drawmenuandbox(this_layer, ctx, 0, 12, MENU13ITEMSCNT, x, y, 54, 0, " Metal:\n Stone:\n Wood:\n Food:\n -Back-");
  }
  if(menulayernumber == 13){
    drawmenuandbox(this_layer, ctx, 0, 13, MENU14ITEMSCNT, x, y, 54, 0, " Buy:\n Sell:\n -Back-");
  }
}

void drawmenuandbox(Layer *this_layer, GContext *ctx, int issquarelayer, int layernumber, int itemscount, int x, int y, int xdiff, int offset, char text[100]){
  itemscount = itemscount + offset;
  GRect layertext;
  GRect highlightbox;
  if(issquarelayer == 0){
    highlightbox = (GRect(x, 2 + y+(gamedata.currentmenu[layernumber] + offset)*15 - gamedata.currentmenu[layernumber], xdiff, 15 )); //highlighting box
    layertext = GRect(x, y, xdiff, 4 + y+(itemscount - 1)*15 - itemscount);
  }
  else if(issquarelayer == 1){ //if you want to draw a number of boxes
    if((gamedata.currentmenu[layernumber]%2) == 0 && gamedata.currentmenu[layernumber] == itemscount - 1){ //draw the last box as large, if number of items is odd
      highlightbox = (GRect(x, 2 + y+((gamedata.currentmenu[layernumber] + offset)/ 2)*15 - (gamedata.currentmenu[layernumber]/2), xdiff, 15 )); //highlighting box
    }
    else if(((gamedata.currentmenu[layernumber] + 1)%2) == 0) //draw box around the second colm of entries
      highlightbox = (GRect(x  + (xdiff/2), 2 + y+((gamedata.currentmenu[layernumber] + offset) / 2)*15 - (gamedata.currentmenu[layernumber]/2), (xdiff / 2), 15 ));
    else if(((gamedata.currentmenu[layernumber] + 1)%2 != 0)) //draw box around the first colm of entires
      highlightbox = (GRect(x, 2 + y+((gamedata.currentmenu[layernumber] + offset) / 2)*15 - (gamedata.currentmenu[layernumber]/2), (xdiff / 2), 15 ));
    layertext = GRect(x, y, xdiff, 4 + y+((itemscount/2 + 1) - 1)*15 - (itemscount/2));
  }
  graphics_fill_rect(ctx, layertext, 0, GCornerNone); //background box
  graphics_draw_rect(ctx, highlightbox); //the box around selected area
  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), layertext, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
}

void draw_ships(Layer *this_layer, GContext *ctx){
  //draw any ships that exist, and are on screen
  
  #ifdef PBL_COLOR
    graphics_context_set_fill_color(ctx, GColorWindsorTan);
  #else
    graphics_context_set_fill_color(ctx, GColorBlack);
  #endif
    
  if(gamedata.totalships >= 0){
    for(int i = 0; i <= gamedata.totalships; i++){
      GPoint shippoint = GPoint(gamedata.shipsx[i] - gamedata.playerx + 72, gamedata.shipsy[i] - gamedata.playery + 84);
      graphics_fill_circle(ctx, shippoint, 3);
      if(gamedata.shipsorder[i] == 'a'){ //draw a smaller circle to identify attack ships
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_fill_circle(ctx, shippoint, 2);
        graphics_context_set_fill_color(ctx, GColorBlack);
      }
    }
  }
}

void draw_islands(Layer *this_layer, GContext *ctx){
  //draw any islands that are currently on screen.
  for(int i = 0; i < 10; i++){
    if(gamedata.islandsx[10] != MAX16BITVALUE && gamedata.islandsy[10] != MAX16BITVALUE){
      graphics_context_set_fill_color(ctx, GColorBlack);
      GPoint storagepoint = GPoint(gamedata.islandsx[10] - gamedata.playerx + 72, gamedata.islandsy[10] - gamedata.playery + 84);
      graphics_fill_circle(ctx, storagepoint, 10);
    }
    if(gamedata.islandsx[i] == -1 && gamedata.islandsy[i] == -1){
      //do nothing, island does not exist
    }
    else{ //if(abs(gamedata.islandsx[i]-gamedata.playerx) < 146 && abs(gamedata.islandsy[i] - gamedata.playery) < 165){
      
      #ifdef PBL_COLOR
        graphics_context_set_fill_color(ctx, GColorLimerick);
      #else
        graphics_context_set_fill_color(ctx, GColorBlack);
      #endif
    
      GPoint islandpoint = GPoint(gamedata.islandsx[i] - gamedata.playerx + 72, gamedata.islandsy[i] - gamedata.playery + 84);
      graphics_fill_circle(ctx, islandpoint, 25);
      graphics_context_set_fill_color(ctx, GColorWhite);
      if(gamedata.islandstypes[i] == 1){
        #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorDarkGreen);
        #endif
        graphics_fill_circle(ctx, islandpoint, 8);
      }
      if(gamedata.islandstypes[i] == 2){
        #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorLightGray);
        #endif
        graphics_fill_circle(ctx, islandpoint, 13);
      }
      if(gamedata.islandstypes[i] == 3){
        #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorYellow);
        #endif
        graphics_fill_circle(ctx, islandpoint, 18);
      }
    }
  }
}

void draw_gui(Layer *this_layer, GContext *ctx){
  //draw the GUI that will go atop the screen
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_text_color(ctx, GColorWhite);
  char playerGUI[50];
  snprintf(playerGUI, sizeof(playerGUI), "Mt:%d Wd:%d St:%d Fd:%d",
           gamedata.playercargo[0], gamedata.playercargo[1], 
           gamedata.playercargo[2], gamedata.playercargo[3]);
  GRect guibox = GRect(0,0,148, 15);
  graphics_fill_rect(ctx, guibox, 0, GCornerNone);
  graphics_draw_text(ctx, playerGUI, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), guibox, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  
  //draw the GUI for the bottom of the screen to show player money
  char moneyGUIRight[10];
  char moneyGUILeft[4];
  snprintf(moneyGUIRight, sizeof(moneyGUIRight), "%i", gamedata.playerwallet);
  snprintf(moneyGUILeft, sizeof(moneyGUILeft), "$:");
  GRect moneyguibox = GRect(0, PEBBLEHEIGHT - 29, 55, 15);
  GRect moneytextbox = GRect(0, PEBBLEHEIGHT - 32, 55, 12);
  graphics_fill_rect(ctx, moneyguibox, 0, GCornerNone);
  graphics_draw_text(ctx, moneyGUILeft, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), moneytextbox, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  graphics_draw_text(ctx, moneyGUIRight, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), moneytextbox, GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
  
  //draw the GUI to show the number of player supplies
  char suppliesGUI[7];
  snprintf(suppliesGUI, sizeof(suppliesGUI), "Su:%i", gamedata.playercargo[4]);
  GRect suppliesbox = GRect(PEBBLEWIDTH - 33, PEBBLEHEIGHT - 29, 33, 15);
  GRect suppliestextbox = GRect(PEBBLEWIDTH - 33, PEBBLEHEIGHT - 32, 33, 12);
  graphics_fill_rect(ctx, suppliesbox, 0, GCornerNone);
  graphics_draw_text(ctx, suppliesGUI, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), suppliestextbox, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
}

void draw_map(Layer *this_layer, GContext *ctx){
  int islandcoords[10][2], shipcoords[2], factorycoords[2], storagecoords[2];
  //want to:
  //get whole area I can draw in
  //find the edge points in terms of X and y, by way of which thing is the furthest out.
  //scale down all X and y coords of islands, player, factory, and storage.
  //draw all these points on the map.
  shipcoords[0] = gamedata.playerx; //accidentally used these for comparisons
  shipcoords[1] = gamedata.playery; //these will be adjusted later.
  factorycoords[0] = 0;
  factorycoords[1] = 0;
  storagecoords[0] = 0;
  storagecoords[1] = 0;
  int largestx = 0, largesty = 0, lowestx = 0, lowesty = 0;
  for(int i = 0; i < 10; i++){ //find the largest/lowest X and Y of the islands
    if(largestx < gamedata.islandsx[i])
      largestx = gamedata.islandsx[i];
    if(lowestx > gamedata.islandsx[i])
      lowestx = gamedata.islandsx[i];
    if(largesty < gamedata.islandsy[i])
      largesty = gamedata.islandsy[i];
    if(lowesty > gamedata.islandsy[i])
      lowesty = gamedata.islandsy[i];
    islandcoords[i][0] = gamedata.islandsx[i];
    islandcoords[i][1] = gamedata.islandsy[i];
  }
  if(largestx < shipcoords[0])
    largestx = shipcoords[0];
  if(lowestx > shipcoords[0])
    lowestx = shipcoords[0];
  if(largestx < factorycoords[0])
    largestx = factorycoords[0];
  if(lowestx > factorycoords[0])
    lowestx = factorycoords[0];
  if(largestx < storagecoords[0])
    largestx = storagecoords[0];
  if(lowestx > storagecoords[0])
    lowestx = storagecoords[0];
  
  if(largesty < shipcoords[1])
    largesty = shipcoords[1];
  if(lowesty > shipcoords[1])
    lowesty = shipcoords[1];
  if(largesty < factorycoords[1])
    largesty = factorycoords[1];
  if(lowesty > factorycoords[1])
    lowesty = factorycoords[1];
  if(largesty < storagecoords[1])
    largesty = storagecoords[1];
  if(lowesty > storagecoords[1])
    lowesty = storagecoords[1];
  
  //adjust the coordinates to fit on the pebble screen:
  //adjustcoords(int coords[2], int largestx, int lowestx, int largesty, int lowesty)
  for(int i = 0; i < 10; i++){
    adjustminimapcoords(islandcoords[i], largestx, lowestx, largesty, lowesty);
  }
  adjustminimapcoords(shipcoords, largestx, lowestx, largesty, lowesty);
  adjustminimapcoords(factorycoords, largestx, lowestx, largesty, lowesty);
  adjustminimapcoords(storagecoords, largestx, lowestx, largesty, lowesty);
  
  //draw the map on the screen.
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(0,0,144,148), 0, GCornerNone);//background
  graphics_context_set_fill_color(ctx, GColorBlack);
  for(int i = 0; i < 10; i++){
    graphics_context_set_fill_color(ctx, GColorBlack);
    GPoint islandpoint = GPoint(islandcoords[i][0], islandcoords[i][1]);
    graphics_fill_circle(ctx, islandpoint, 4);
    //if the island has a negative affinity, indicate it on the minimap
    if(manageislandallegiance(&gamedata, i, 0) != 0){
      graphics_draw_line(ctx, GPoint(islandcoords[i][0] - 5, islandcoords[i][1] - 5), GPoint(islandcoords[i][0] - 3, islandcoords[i][1] - 5));
    }
    if(manageislandallegiance(&gamedata, i, 0) > 0){
      graphics_draw_line(ctx, GPoint(islandcoords[i][0] - 4, islandcoords[i][1] - 4), GPoint(islandcoords[i][0] - 4, islandcoords[i][1] - 6));
    }
    graphics_context_set_fill_color(ctx, GColorWhite);
    if(gamedata.islandstypes[i] == 0)
      graphics_fill_circle(ctx, islandpoint, 0);
    if(gamedata.islandstypes[i] == 1)
      graphics_fill_circle(ctx, islandpoint, 1);
    if(gamedata.islandstypes[i] == 2)
      graphics_fill_circle(ctx, islandpoint, 2);
    if(gamedata.islandstypes[i] == 3)
      graphics_fill_circle(ctx, islandpoint, 3);
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
  GPoint playerpoint = GPoint(shipcoords[0], shipcoords[1]);
  GPoint storagepoint = GPoint(storagecoords[0], storagecoords[1]);
  GPoint factorypoint = GPoint(factorycoords[0], factorycoords[1]);
  //draw factory and storage later
  graphics_fill_circle(ctx, playerpoint, 3);
  graphics_fill_circle(ctx, GPoint(shipcoords[0] + gamedata.playerxvelocity, shipcoords[1] + gamedata.playeryvelocity), 1);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  gamedata.downhit = 1;
  gamedata.uphit = 1;
  
  //text_layer_set_text(text_layer, "Select");
}

static void select_release_handler(ClickRecognizerRef recognizer, void *context) {
  gamedata.downhit = 0;
  gamedata.uphit = 0;
  gamedata.buttonrelease = 1;
  //text_layer_set_text(text_layer, "XSelect");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  gamedata.uphit = 1;
  //gamedata.buttonrelease = 0;
  //text_layer_set_text(text_layer, "Up");
}

static void up_release_handler(ClickRecognizerRef recognizer, void *context) {
  gamedata.uphit = 0;
  gamedata.buttonrelease = 1;
  //text_layer_set_text(text_layer, "XUp");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  gamedata.downhit = 1;
  //gamedata.buttonrelease = 0;
  //text_layer_set_text(text_layer, "Down");
}

static void down_release_handler(ClickRecognizerRef recognizer, void *context) {
  gamedata.downhit = 0;
  gamedata.buttonrelease = 1;
  //text_layer_set_text(text_layer, "XDown");
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context){
  //going to be a "use anywhere" menu for the player, to save the game, or to see info, so on and
  //so forth
  if(gamedata.gamemode == 'p'){
    exitmenus(&gamedata);
    gamedata.gamemode = 'm';
    gamedata.menulayer = 5;
  }
}
  
static void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context){
  
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context){
  //I want to make this have the player exit whatever island it is on
  //to do this I am going to need to make a function that makes the player leave islands and other
  //menus
  exitmenus(&gamedata);
}
  
static void down_long_click_release_handler(ClickRecognizerRef recognizer, void *context){
  
}

static void up_long_click_handler(ClickRecognizerRef recognizer, void *context){
  //same as down click, unless I think of another reason to use this
}
  
static void up_long_click_release_handler(ClickRecognizerRef recognizer, void *context){
  
}
  
static void click_config_provider(void *context) {
  window_raw_click_subscribe(BUTTON_ID_UP, up_click_handler, up_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_click_handler, down_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_click_handler, select_release_handler, NULL);
  // long click config:
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
  window_long_click_subscribe(BUTTON_ID_DOWN, 700, down_long_click_handler, down_long_click_release_handler);
  window_long_click_subscribe(BUTTON_ID_UP, 700, up_long_click_handler, up_long_click_release_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  //create and set the update protocol for the canvas to draw on
  graphics_canvas_layer = layer_create(GRect(0,0,bounds.size.w, bounds.size.h));
  layer_add_child(window_layer, graphics_canvas_layer);
  layer_set_update_proc(graphics_canvas_layer, canvas_update_proc);

  //remove tutorial things later, when I understand them
  //text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  //text_layer_set_text(text_layer, "Press a button");
  //text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  //layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  //text_layer_destroy(text_layer);
  layer_destroy(graphics_canvas_layer);
}

void handleTimer(){
  //tell the watch to run this again and again and again, X amount of time apart
  app_timer_register(GAMELOOP_TIMER_INTERVALL, handleTimer, NULL);
  //function that is called to update the values in gamedata
  if(gamedata.gamemode != 'b')
    update_game(&gamedata);
  if(gamedata.gamemode == 'b')
    updatebattle(&shipbattledata, &gamedata);
  //make sure the layer gets redrawn
  layer_mark_dirty(graphics_canvas_layer);
}

static void init(void) {
  //do required window things
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
  initializebattle(&shipbattledata);
  initialize_game(&gamedata);
  load_data(&gamedata);

  //start the game loop
  app_timer_register(GAMELOOP_TIMER_INTERVALL, handleTimer, NULL);
}

static void deinit(void) {
  save_data(&gamedata);
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}