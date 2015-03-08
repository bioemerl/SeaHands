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
  //draw the player
  GPoint player = GPoint(72, 82);
  graphics_context_set_fill_color(ctx, GColorBlack);
  //draw the player
  graphics_fill_circle(ctx, player, 5);
  
  //draw the direction, if it is not zero
  if((gamedata.playerxvelocity != 0) || (gamedata.playeryvelocity !=0)){
    if( (abs(gamedata.playerxvelocity) <= 5) && (abs(gamedata.playeryvelocity) <= 5 ))
      graphics_context_set_fill_color(ctx, GColorWhite);
    //72 and 82 are the center of pebble's screen.
    GPoint playervector = GPoint((72 + gamedata.playerxvelocity), (82 + gamedata.playeryvelocity));
    graphics_fill_circle(ctx, playervector, 2);
  }
  
  draw_islands(this_layer, ctx);
  draw_ships(this_layer, ctx);
  draw_gui(this_layer, ctx);
  
  //draw the menus
  if(gamedata.gamemode == 'm'){
    //always draw the base menu
    if(gamedata.menulayer == 0 || gamedata.menulayer == 1 || gamedata.menulayer == 2)
      draw_menu_layer(this_layer, ctx, 0, 0, 15); //the base menu layer
    if(gamedata.menulayer == 1)//buysellmenu
      draw_menu_layer(this_layer, ctx, 1, 54, 15);
    if(gamedata.menulayer == 2) // the islands upgrade menu
      draw_menu_layer(this_layer, ctx, 2, 45, 15);
    if(gamedata.menulayer == 3) //the ship pillage menu
      draw_menu_layer(this_layer, ctx, 3, 0, 15);
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
    graphics_fill_rect(ctx, GRect(x,y,54,90), 0, GCornerNone); //background box
    graphics_draw_rect(ctx, GRect(x, y+gamedata.currentmenu[0]*15, 54, 15 )); //highlighting box
    
    //text prep
    GRect textbox = GRect(x, y, 54, 105);
    char totalmenu[60];
    //create the char array for the menu    
    snprintf(totalmenu, sizeof(totalmenu), "Metal:%d\nWood:%d\nStone:%d\nFood:%d\nIsland:\n-Exit-",
           gamedata.islandscargo[gamedata.playerisland][0],
           gamedata.islandscargo[gamedata.playerisland][1],
           gamedata.islandscargo[gamedata.playerisland][2],
           gamedata.islandscargo[gamedata.playerisland][3]);
    //draw text
    graphics_draw_text(ctx, totalmenu, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), textbox, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  }
  //menu layer for buying and selling resources
  if(menulayernumber == 1){
    
    graphics_fill_rect(ctx, GRect(x,y,46,50), 0, GCornerNone);
    graphics_draw_rect(ctx, GRect(x, y+gamedata.currentmenu[1]*15, 46, 15));
      
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
    snprintf(firstmenulayer, sizeof(firstmenulayer), "Back\nBuy:%i\nSell", resourcevalue);
    //print the text
    graphics_draw_text(ctx, firstmenulayer, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), layer2text, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  }
  //menu layer for upgrade purchases
  if(menulayernumber == 2){
    graphics_fill_rect(ctx, GRect(x,y,80,65), 0, GCornerNone);
    graphics_draw_rect(ctx, GRect(x, y+gamedata.currentmenu[2]*15, 80, 15));
    GRect layer3text = GRect(x,y,80, 65);
    //get needed data
    //find current upgrade level, and change the price for that upgrade by a function on the constant
    //BASE_VALUE_UPGRADENAME
    int cargoprice = 1;
    int speedprice = 1;
    
    char secondmenulayer[45];
    int upgradeprices[3];
    for(int i = 0; i < 2; i++)
      upgradeprices[i] = check_player_upgrade_price(&gamedata, i);
    cargoprice = upgradeprices[0];
    speedprice = upgradeprices[1];
    snprintf(secondmenulayer, sizeof(secondmenulayer), "BuySu:%i\nUpCrgo:%i\nUpSpd: %i\nBack", BASE_PRICE_SUPPLIES, cargoprice, speedprice);
    graphics_draw_text(ctx, secondmenulayer, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), layer3text, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  }
  if(menulayernumber == 3){
    graphics_fill_rect(ctx, GRect(x,y,40,35), 0, GCornerNone);
    graphics_draw_rect(ctx, GRect(x, y+gamedata.currentmenu[3]*15, 40, 15));
    GRect layer4text = GRect(x,y,40, 35);
    
    char thirdmenulayer[17];
    snprintf(thirdmenulayer, sizeof(thirdmenulayer), "Pillage:\nBack:");
    graphics_draw_text(ctx, thirdmenulayer, fonts_get_system_font(FONT_KEY_FONT_FALLBACK), layer4text, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  }
}

void draw_ships(Layer *this_layer, GContext *ctx){
  //draw any ships that exist, and are on screen
  if(gamedata.totalships >= 0){
    for(int i = 0; i <= gamedata.totalships; i++){
      if(abs(gamedata.shipsx[i]-gamedata.playerx) < 146 && abs(gamedata.shipsy[i] - gamedata.playery) < 165)
      graphics_context_set_fill_color(ctx, GColorBlack);
      GPoint shippoint = GPoint(gamedata.shipsx[i] - gamedata.playerx + 72, gamedata.shipsy[i] - gamedata.playery + 84);
      graphics_fill_circle(ctx, shippoint, 3);
    }
  }
}

void draw_islands(Layer *this_layer, GContext *ctx){
  //draw any islands that are currently on screen.
  for(int i = 0; i < 10; i++){
    if(gamedata.islandsx[i] == -1 && gamedata.islandsy[i] == -1){
      //do nothing, island does not exist
    }
    else{ //if(abs(gamedata.islandsx[i]-gamedata.playerx) < 146 && abs(gamedata.islandsy[i] - gamedata.playery) < 165){
      graphics_context_set_fill_color(ctx, GColorBlack);
      GPoint islandpoint = GPoint(gamedata.islandsx[i] - gamedata.playerx + 72, gamedata.islandsy[i] - gamedata.playery + 84);
      graphics_fill_circle(ctx, islandpoint, 25);
      if(gamedata.islandstypes[i] == 1){
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_fill_circle(ctx, islandpoint, 3);
      }
      if(gamedata.islandstypes[i] == 2){
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_fill_circle(ctx, islandpoint, 8);
      }
      if(gamedata.islandstypes[i] == 3){
        graphics_context_set_fill_color(ctx, GColorWhite);
        graphics_fill_circle(ctx, islandpoint, 15);
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

static void click_config_provider(void *context) {
  window_raw_click_subscribe(BUTTON_ID_UP, up_click_handler, up_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_DOWN, down_click_handler, down_release_handler, NULL);
  window_raw_click_subscribe(BUTTON_ID_SELECT, select_click_handler, select_release_handler, NULL);
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