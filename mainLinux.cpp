/*
 * Copyright 2021 SYSON, MICHAEL B.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @company: USBONG
 * @author: SYSON, MICHAEL B.
 * @date created: 20211111
 * @date updated: 20211121
 * @website address: http://www.usbong.ph
 *
 * Notes:
 * 1) used: Simple Directmedia Layer (SDL) + SDL Image, instead of OpenGL
 * objective: output application CAN execute even on recycled machines,
 * e.g. with 32-bit Operating Systems and no add-on 3D Graphics Card
 * 
 * 2) reused: select component parts from Usbong Pagong
 * https://github.com/usbong/pagong;
 * last accessed: 20211111
 *
 */

/**************************
 * Includes
 *
 **************************/

#ifdef _WIN32 //Windows machine
	#include <SDL.h>
	#include <SDL_image.h>
#elif defined(__APPLE__)
    #include <SDL2/SDL.h>
    #include <SDL2_image/SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

#include <time.h>
#include <unistd.h>
#include <iostream>

#include "Ipis.h"
#include "Unit.h"

//added by Mike, 20211120
#include "Sdlwav.h"

#define MAX_IPIS 22 //6
//added by Mike, 20211118
#define GRASS_TILE 1
#define WATER_TILE 2
#define TREE_TILE 3

//added by Mike, 20211119
#define ROAD_RIGHT_TILE 4
#define ROAD_LEFT_TILE 5
#define ROAD_UP_TILE 6
#define ROAD_DOWN_TILE 7
#define ROAD_RIGHT_DOWN_TILE 8
#define ROAD_DOWN_LEFT_TILE 9
#define ROAD_LEFT_UP_TILE 10
#define ROAD_UP_RIGHT_TILE 11

//added by Mike, 20211121
#define IPIS_START_INDEX 0

//added by Mike, 20211117
class Ipis; 
class Unit; 

static int myWindowWidthAsPixel=640;
static int myWindowHeightAsPixel=640;

SDL_Window *mySDLWindow = NULL;
SDL_Renderer *mySDLRenderer = NULL;

int iPilotX;
int iPilotY;
int iCountTaoAnimationFrame;
int iCountTileAnimationFrame; //added by Mike, 20211113

int myKeysDown[10]; //note: includes KEY_J, KEY_L, KEY_I, KEY_K,

//added by Mike, 20211112
int iRowCountMax;
int iColumnCountMax;

float fGridSquareWidth;
float fGridSquareHeight;

int iBaseOffsetWidth;
int iBaseOffsetHeight;

int iCurrentOffsetWidth;
int iCurrentOffsetHeight;

int iDestroyBugShakeDelayCount;
int iDestroyBugShakeDelayMax;

bool bIsExecutingDestroyBug;

int iStepX;
int iStepY;

//added by Mike, 20211121
int iCountIpisDestroyed;

//added by Mike, 20211116
Ipis *myIpis[MAX_IPIS];

//added by Mike, 20211117
Unit *myUnit;

/*
//added by Mike, 20211119
int myLevelWeakBeat[MAX_IPIS];
int myLevelMediumBeat[MAX_IPIS];
int myLevelStrongBeat[MAX_IPIS];
*/

//added by Mike, 20211120
char **myArrayOfInputStringsBeatSound;

SDL_Texture *texture;

#define TRUE 1
#define FALSE 0

enum Keys
{
	KEY_W = 0,
	KEY_S,
	KEY_D,
	KEY_A,

	KEY_J,
	KEY_L,
	KEY_I,
	KEY_K,
	KEY_H,
	KEY_U,

	iNumOfKeyTypes
};

void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	//includes window title bar	
	SDL_DisplayMode mySDLDisplayMode;
	SDL_GetCurrentDisplayMode(0, &mySDLDisplayMode);
	
	myWindowWidthAsPixel=mySDLDisplayMode.w;
	myWindowHeightAsPixel=mySDLDisplayMode.h;
	
	
	mySDLWindow = SDL_CreateWindow("Tugon Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, myWindowWidthAsPixel, myWindowHeightAsPixel, windowFlags | SDL_WINDOW_BORDERLESS);
	

	if (!mySDLWindow )
	{
		printf("Failed to open %d x %d window: %s\n", myWindowWidthAsPixel, myWindowHeightAsPixel, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	mySDLRenderer = SDL_CreateRenderer(mySDLWindow, -1, rendererFlags);

	if (!mySDLRenderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
	
	SDL_GL_CreateContext(mySDLWindow);
}

//note: super family computer controller with button colors, 
//OK with children in learning color's name identification
//additional note: SNK's arcade buttons also use colors
void keyDown(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
/* //removed by Mike, 20211120	
		if (event->keysym.scancode == SDL_SCANCODE_W)
		{
			myKeysDown[KEY_W] = TRUE;		
		}
		
		if (event->keysym.scancode == SDL_SCANCODE_S)
		{
			myKeysDown[KEY_S] = TRUE;					
		}

		if (event->keysym.scancode == SDL_SCANCODE_A)
		{
			myKeysDown[KEY_A] = TRUE;					
		}

		if (event->keysym.scancode == SDL_SCANCODE_D)
		{
			myKeysDown[KEY_D] = TRUE;					
		}
*/		
    //note: one button press only; beat, cadence; skipping stone?
    
    //can be pressed simultaneously with directional button
    if (event->keysym.scancode == SDL_SCANCODE_K)
    {
    		//edited by Mike, 20211115
        //myKeysDown[KEY_K] = TRUE;

printf(">> KEY_K!!\n");

	executeSDLWaveSound(2,myArrayOfInputStringsBeatSound);

/*
				if (bIsExecutingDestroyBug) {
				  myKeysDown[KEY_K] = FALSE;
    			bIsExecutingDestroyBug = false;
				}
				else {
        	myKeysDown[KEY_K] = TRUE;
    			bIsExecutingDestroyBug = true;	
				}
*/				
				//note: already executed once, despite pressed and hold
        myKeysDown[KEY_K] = TRUE;
    		bIsExecutingDestroyBug = true;	
    }
	}
}

void keyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
/*	//removed by Mike, 20211120
		if (event->keysym.scancode == SDL_SCANCODE_W)
		{
			myKeysDown[KEY_W] = FALSE;		
		}

		if (event->keysym.scancode == SDL_SCANCODE_S)
		{
			myKeysDown[KEY_S] = FALSE;					
		}

		if (event->keysym.scancode == SDL_SCANCODE_A)
		{
			myKeysDown[KEY_A] = FALSE;					
		}

		if (event->keysym.scancode == SDL_SCANCODE_D)
		{
			myKeysDown[KEY_D] = FALSE;					
		}
*/

    //note: one button press only; beat, cadence; skipping stone?
        
    //added by Mike, 20210905
    if (event->keysym.scancode == SDL_SCANCODE_K)
    {
        myKeysDown[KEY_K] = FALSE;
        bIsExecutingDestroyBug=false;
    }        
	}
}

void doInput(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;

			case SDL_KEYDOWN:
				keyDown(&event.key);
				break;

			case SDL_KEYUP:
				keyUp(&event.key);
				break;

			default:
				break;
		}
	}
}

void prepareScene(void)
{
	//note: SDL color max 255; GIMP color max 100
	//edited by Mike, 20211111
//	SDL_SetRenderDrawColor(mySDLRenderer, 0, 255*0.667, 255*0.494, 255); //blue green
	//edited by Mike, 20211119
//	SDL_SetRenderDrawColor(mySDLRenderer, 0, 0, 0, 255); //black
//	SDL_SetRenderDrawColor(mySDLRenderer, 193, 246, 253, 255); //sky blue
	SDL_SetRenderDrawColor(mySDLRenderer, 233, 214, 146, 255); //sand
	
	SDL_RenderClear(mySDLRenderer);
}

void presentScene(void)
{
	SDL_RenderPresent(mySDLRenderer);
}

SDL_Texture *loadTexture(char *filename)
{
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

	texture = IMG_LoadTexture(mySDLRenderer, filename);

	return texture;
}

//added by Mike, 20211112
//Reference: https://stackoverflow.com/questions/28346989/drawing-and-filling-a-circle
//last accessed: 20211112
//answer by: Edgar Luque, 20170127T2024
//note: excess space; due to 1366 x 768; width x height?
void drawShieldPrev()
{
  int iRowCountMax=10;
  int iColumnCountMax=18;
  
  float fGridSquareWidth = (myWindowWidthAsPixel)/iColumnCountMax; //example: 136.60
  float fGridSquareHeight = (myWindowHeightAsPixel)/iRowCountMax; //example: 76.80

	SDL_Point center = {myWindowWidthAsPixel/2, myWindowHeightAsPixel/2};

	int radius = fGridSquareHeight; //fGridSquareWidth
	
  //note: SDL color max 255; GIMP color max 100
	SDL_SetRenderDrawColor(mySDLRenderer, 255*1, 0, 0, 255); //red
    
	for (int w = 0; w < radius * 2; w++)
  {
      for (int h = 0; h < radius * 2; h++)
      {
          int dx = radius - w; //horizontal offset
          int dy = radius - h; //vertical offset
          if ((dx*dx + dy*dy) <= (radius * radius))
          {
//              SDL_RenderDrawPoint(mySDLRenderer, center.x + dx, center.y + dy);
              SDL_RenderDrawPoint(mySDLRenderer, center.x + dx - w/2, center.y + dy - h/2);
          }
      }
  }
}

//added by Mike, 20211112
void init() {
/* //edited by Mike, 20211113
  iRowCountMax=10;
  iColumnCountMax=18;
  
  //note: SDL and SDL_Image use integers, i.e. whole numbers,
  //instead of floating-point numbers; result: incorrect size of fGridSquare
  fGridSquareWidth = (myWindowWidthAsPixel)/iColumnCountMax; //example: 136.60
  fGridSquareHeight = (myWindowHeightAsPixel)/iRowCountMax; //example: 42.66
*/  
  iRowCountMax=10;
  iColumnCountMax=iRowCountMax;//18; 
  
  //note: SDL and SDL_Image use integers, i.e. whole numbers,
  //instead of floating-point numbers; result: incorrect size of fGridSquare
/* //edited by Mike, 20211114  
  fGridSquareWidth = 64;
  fGridSquareHeight = 64;
*/  
  fGridSquareHeight = (myWindowHeightAsPixel)/iRowCountMax;
  fGridSquareWidth = fGridSquareHeight;
  
  //wide screen; portrait mode
  iBaseOffsetWidth=(myWindowWidthAsPixel-myWindowHeightAsPixel)/2;
  //TO-DO: -add: this  
  iBaseOffsetHeight=0;
  
  iCurrentOffsetWidth=iBaseOffsetWidth;
	iCurrentOffsetHeight=iBaseOffsetHeight;
	bIsExecutingDestroyBug=false;

	iDestroyBugShakeDelayCount=0;
	iDestroyBugShakeDelayMax=5;
	
	//added by Mike, 20211119
	//TO-DO: -update: this based on Level
	//added by Mike, 20211115
	iStepX=1; //2;//1;
	iStepY=1; //2;//1;

	iCountIpisDestroyed=0;
	    
  iCountTileAnimationFrame=0;
  
  for (int iCount=0; iCount<4; iCount++) { //directional keys only
		myKeysDown[iCount]=FALSE;
	}		
	myKeysDown[KEY_D] = TRUE;  	

/*myLevelWeakBeat
	//edited by Mike, 20211119	
	for (int iCount=0; iCount<MAX_IPIS; iCount++) {
		myIpis[iCount] = new Ipis(mySDLRenderer,fGridSquareWidth*5+fGridSquareWidth*iCount,fGridSquareHeight*3,0,myWindowWidthAsPixel,myWindowHeightAsPixel);
		myIpis[iCount]->setGridTileWidthHeight(fGridSquareWidth,fGridSquareHeight);
	}
*/
		//added by Mike, 20211119
		//TO-DO: -add: in a reusable function		
//		int myLevelWeakBeat[MAX_IPIS] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
		//start from tile immediately after left corner; clockwise

/*	//edited by Mike, 20211120
		int myLevelWeakBeat[MAX_IPIS] = {1,0,1,0,1,0, //RIGHT
																		 1,1,0,1,1, //DOWN; corner included
																		 0,1,1,0,1,1, //LEFT
																		 0,1,1,1,0}; //UP; corner included
*/
		int myLevelWeakBeat[MAX_IPIS] = {1,0,1,0,1,0, //RIGHT
																		 1,1,0,1,1, //DOWN; corner included
																		 0,1,1,0,1,1, //LEFT
																		 0,1,1,1,0}; //UP; corner included

/*
		for (int iCount=0; iCount<MAX_IPIS; iCount++) {
			if (iCount%2==0) {
				myLevelWeakBeat[iCount]=0;
			}
			else {
				myLevelWeakBeat[iCount]=1;
			}
		}
*/
//		printf(">>myLevelWeakBeat[21]: %i\n",myLevelWeakBeat[21]); //max
		printf(">>myLevelWeakBeat[1]: %i\n",myLevelWeakBeat[1]);		
		
		//TO-DO: -update: this

		int iIpisCount=0;
		
		//ROAD_RIGHT_TILE
		for (int iColumnCount=2; iColumnCount<8; iColumnCount++) {
//			if (myLevelWeakBeat[iIpisCount]!=0) {
				myIpis[iIpisCount] = new Ipis(mySDLRenderer,iColumnCount*fGridSquareWidth+iCurrentOffsetWidth,
																									3*fGridSquareHeight,0,
																		myWindowWidthAsPixel,myWindowHeightAsPixel);
				myIpis[iIpisCount]->setGridTileWidthHeight(fGridSquareWidth,fGridSquareHeight);	
//			}
			iIpisCount++;
		}
		
			//ROAD_DOWN_TILE
		for (int iRowCount=3; iRowCount<8; iRowCount++) {
//			if (myLevelWeakBeat[iIpisCount]!=0) {
				myIpis[iIpisCount] = new Ipis(mySDLRenderer,8*fGridSquareWidth+iCurrentOffsetWidth,
																										iRowCount*fGridSquareHeight,0,
																			myWindowWidthAsPixel,myWindowHeightAsPixel);
				myIpis[iIpisCount]->setGridTileWidthHeight(fGridSquareWidth,fGridSquareHeight);	
//			}
			iIpisCount++;
		}
				
		//ROAD_LEFT_TILE
		//edited by Mike, 20211120
//		for (int iColumnCount=2; iColumnCount<8; iColumnCount++) {
		for (int iColumnCount=7; iColumnCount>=2; iColumnCount--) {

//			if (myLevelWeakBeat[iIpisCount]!=0) {
				myIpis[iIpisCount] = new Ipis(mySDLRenderer,iColumnCount*fGridSquareWidth+iCurrentOffsetWidth,
																									7*fGridSquareHeight,0,
																		myWindowWidthAsPixel,myWindowHeightAsPixel);
				myIpis[iIpisCount]->setGridTileWidthHeight(fGridSquareWidth,fGridSquareHeight);	
//			}
			iIpisCount++;
		}
		
		//ROAD_UP_TILE
		//edited by Mike, 20211120
//		for (int iRowCount=3; iRowCount<8; iRowCount++) {
		for (int iRowCount=7; iRowCount>=3; iRowCount--) {
			myIpis[iIpisCount] = new Ipis(mySDLRenderer,1*fGridSquareWidth+iCurrentOffsetWidth,
														iRowCount*fGridSquareHeight,0,
														myWindowWidthAsPixel,myWindowHeightAsPixel);																		
			myIpis[iIpisCount]->setGridTileWidthHeight(fGridSquareWidth,fGridSquareHeight);	

/*
			printf(">>iIpisCount: %i\n",iIpisCount);
			printf(">>myLevelWeakBeat[iIpisCount]: %i\n",myLevelWeakBeat[iIpisCount]);
			
			if (myLevelWeakBeat[iIpisCount]==0) {
				myIpis[iIpisCount]->changeState(HIDDEN_STATE);
			}
*/
			iIpisCount++;			
		}
		
		printf(">>iIpisCount: %i\n",iIpisCount);

		//TO-DO: -update: this

		//added by Mike, 20211119
		for (int iCount=0; iCount<MAX_IPIS; iCount++) {
			if (myLevelWeakBeat[iCount]==0) {
				myIpis[iCount]->setToInactiveState();
			}			
		}

	myUnit = new Unit(mySDLRenderer,fGridSquareWidth*5,fGridSquareHeight*3,0,myWindowWidthAsPixel,myWindowHeightAsPixel);

	//added by Mike, 20211120
	myArrayOfInputStringsBeatSound = (char **)malloc(2 * sizeof(char *)); //for only 1 item
 	myArrayOfInputStringsBeatSound [0] = (char *)"./sdlwav"; //add this, albeit NOT used; arg count 1 as filename used
 	myArrayOfInputStringsBeatSound [1] = (char *)"sounds/tugonBeat1.mp3"; //OK
	
}

/* //edited by Mike, 20211119
//added by Mike, 20211113
void drawMovementTile(int x, int y)
{
	int iTileWidth=fGridSquareWidth;
	int iTileHeight=fGridSquareHeight;

  //Rectangles for drawing which will specify source (inside the texture)
  //and target (on the screen) for rendering our textures.
  SDL_Rect SrcR;
  SDL_Rect DestR;
  	    
  SrcR.x = 0; //x;
  SrcR.y = 0; //y;

  SrcR.w = 64; //iTileWidth;
  SrcR.h = 64; //iTileHeight;

  DestR.x = x+iCurrentOffsetWidth;
  DestR.y = y;
  
  DestR.w = iTileWidth;
  DestR.h = iTileHeight;

	SDL_RenderCopy(mySDLRenderer, texture, &SrcR, &DestR);
}
*/

//added by Mike, 20211119
void drawMovementTile(int iDirection, int x, int y)
{
/*
	int iTileWidth=fGridSquareWidth;
	int iTileHeight=fGridSquareHeight;
*/
	int iTileWidth=64;
	int iTileHeight=64;
	
  //Rectangles for drawing which will specify source (inside the texture)
  //and target (on the screen) for rendering our textures.
  SDL_Rect SrcR;
  SDL_Rect DestR;

/*  	    
  SrcR.x = 0; //x;
  SrcR.y = 0; //y;
*/
	
	if ((iDirection==ROAD_RIGHT_TILE)||(iDirection==ROAD_LEFT_TILE)) {
  	SrcR.x = 0; //x;
  	SrcR.y = 0; //y;	
	}
	else if ((iDirection==ROAD_UP_TILE)||(iDirection==ROAD_DOWN_TILE)) {
  	SrcR.x = 0+iTileWidth; //x;
  	SrcR.y = 0; //y;  	
	}
	//corner
	else {
			if (iDirection==ROAD_RIGHT_DOWN_TILE) {
  			SrcR.x = 0+iTileWidth*3;
  			SrcR.y = 0;
			}
			else if (iDirection==ROAD_DOWN_LEFT_TILE) {
  			SrcR.x = 0+iTileWidth*3; 
  			SrcR.y = 0+iTileHeight; 			
			}
			else if (iDirection==ROAD_LEFT_UP_TILE) {
  			SrcR.x = 0+iTileWidth*2; 
  			SrcR.y = 0+iTileHeight;  			
			}
			else if (iDirection==ROAD_UP_RIGHT_TILE) {
  			SrcR.x = 0+iTileWidth*2;
  			SrcR.y = 0;
			}
	}	

  SrcR.w = iTileWidth;
  SrcR.h = iTileHeight;

  DestR.x = x+iCurrentOffsetWidth;
  DestR.y = y;
  
  DestR.w = fGridSquareWidth;
  DestR.h = fGridSquareHeight;

	SDL_RenderCopy(mySDLRenderer, texture, &SrcR, &DestR);
}

//added by Mike, 20211118
void drawBackgroundTile(int iTileId, int x, int y)
{
/*

	int iTileWidth=fGridSquareWidth;
	int iTileHeight=fGridSquareHeight;
*/
	int iTileWidth=64;
	int iTileHeight=64;

  //Rectangles for drawing which will specify source (inside the texture)
  //and target (on the screen) for rendering our textures.
  SDL_Rect SrcR;
  SDL_Rect DestR;
  
  //note: iTileId=2 //river
	    
//  printf(">>iCountTileAnimationFrame: %i\n",iCountTileAnimationFrame);

	if (iTileId==2) { //water
	  iCountTileAnimationFrame=iCountTileAnimationFrame+1;		               																				
	  if (iCountTileAnimationFrame>=2) { //2 frames of animation only
			iCountTileAnimationFrame=0;
	  }
	    
  	  SrcR.x = 0+iTileWidth*iCountTileAnimationFrame; //x;
  	  SrcR.y = 0+iTileHeight*(iTileId); //y;
   }
   else if (iTileId==3) { //tree
  	  SrcR.x = 0+iTileWidth*1; //x;
  	  SrcR.y = 0+iTileHeight*(1); //y;
   }

   else {
  	  SrcR.x = 0+iTileWidth*0; //x;
  	  SrcR.y = 0+iTileHeight*(iTileId); //y;
   }


  SrcR.w = iTileWidth;
  SrcR.h = iTileHeight;

  DestR.x = x+iCurrentOffsetWidth;
  DestR.y = y;
    
  DestR.w = fGridSquareWidth;
  DestR.h = fGridSquareHeight;


  SDL_RenderCopy(mySDLRenderer, texture, &SrcR, &DestR);

}

/*	//edited by Mike, 20211119
void drawLevel()
{
	//note: count starts at zero	
	//drawMovementTile(5*fGridSquareWidth,5*fGridSquareHeight);
	for (int iRowCount=3; iRowCount<8; iRowCount++) {
//  		drawMovementTile(1*fGridSquareWidth,iRowCount*fGridSquareHeight);
  		drawMovementTile(ROAD_UP_TILE, 1*fGridSquareWidth,iRowCount*fGridSquareHeight);
	}

	for (int iRowCount=3; iRowCount<8; iRowCount++) {
//  		drawMovementTile(8*fGridSquareWidth,iRowCount*fGridSquareHeight);
  		drawMovementTile(ROAD_DOWN_TILE, 8*fGridSquareWidth,iRowCount*fGridSquareHeight);
	}
	
	for (int iColumnCount=2; iColumnCount<8; iColumnCount++) {
//  		drawMovementTile(iColumnCount*fGridSquareWidth,3*fGridSquareHeight);
  		drawMovementTile(ROAD_RIGHT_TILE, iColumnCount*fGridSquareWidth,3*fGridSquareHeight);
	}
	
	for (int iColumnCount=2; iColumnCount<8; iColumnCount++) {
//  		drawMovementTile(iColumnCount*fGridSquareWidth,7*fGridSquareHeight);
  		drawMovementTile(ROAD_LEFT_TILE, iColumnCount*fGridSquareWidth,7*fGridSquareHeight);
	}
	
	//added by Mike, 20211118; edited by Mike, 20211119
	drawBackgroundTile(GRASS_TILE,0*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,1*fGridSquareWidth,8*fGridSquareHeight);
//	drawBackgroundTile(GRASS_TILE,2*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,3*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,7*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,8*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,9*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,10*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,8*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,9*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,10*fGridSquareWidth,9*fGridSquareHeight);

	//note: water tile count, if NOT correct, no animation sequence
	drawBackgroundTile(WATER_TILE,0*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,1*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,2*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,3*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,4*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,5*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,6*fGridSquareWidth,9*fGridSquareHeight);

	drawBackgroundTile(TREE_TILE,0*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,2*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,4*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,5*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,6*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,7*fGridSquareWidth,9*fGridSquareHeight);
}
*/

void drawLevel()
{
	//note: count starts at zero	
  drawMovementTile(ROAD_UP_RIGHT_TILE, 1*fGridSquareWidth,3*fGridSquareHeight);
	for (int iRowCount=4; iRowCount<7; iRowCount++) {
  		drawMovementTile(ROAD_UP_TILE, 1*fGridSquareWidth,iRowCount*fGridSquareHeight);
	}
  drawMovementTile(ROAD_LEFT_UP_TILE, 1*fGridSquareWidth,7*fGridSquareHeight);

  drawMovementTile(ROAD_RIGHT_DOWN_TILE, 8*fGridSquareWidth,3*fGridSquareHeight);
	for (int iRowCount=4; iRowCount<8; iRowCount++) {
  		drawMovementTile(ROAD_DOWN_TILE, 8*fGridSquareWidth,iRowCount*fGridSquareHeight);
	}
  drawMovementTile(ROAD_DOWN_LEFT_TILE, 8*fGridSquareWidth,7*fGridSquareHeight);

	for (int iColumnCount=2; iColumnCount<8; iColumnCount++) {
  		drawMovementTile(ROAD_RIGHT_TILE, iColumnCount*fGridSquareWidth,3*fGridSquareHeight);
	}
	
	for (int iColumnCount=2; iColumnCount<8; iColumnCount++) {
  		drawMovementTile(ROAD_LEFT_TILE, iColumnCount*fGridSquareWidth,7*fGridSquareHeight);
	}
	
	//added by Mike, 20211118; edited by Mike, 20211119
	drawBackgroundTile(GRASS_TILE,0*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,1*fGridSquareWidth,8*fGridSquareHeight);
//	drawBackgroundTile(GRASS_TILE,2*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,3*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,7*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,8*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,9*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,10*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,8*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,9*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(GRASS_TILE,10*fGridSquareWidth,9*fGridSquareHeight);

	//note: water tile count, if NOT correct, no animation sequence
	drawBackgroundTile(WATER_TILE,0*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,1*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,2*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,3*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,4*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,5*fGridSquareWidth,9*fGridSquareHeight);
	drawBackgroundTile(WATER_TILE,6*fGridSquareWidth,9*fGridSquareHeight);

	drawBackgroundTile(TREE_TILE,0*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,2*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,4*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,5*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,6*fGridSquareWidth,8*fGridSquareHeight);
	drawBackgroundTile(TREE_TILE,7*fGridSquareWidth,9*fGridSquareHeight);

}


//added by Mike, 20211114
void drawGrid()
{
  //note: SDL color max 255; GIMP color max 100
	SDL_SetRenderDrawColor(mySDLRenderer, 0, 255*1, 0, 255); //green
    
  // Draw a Green Line
  //rows
  for (int iRowCount=0; iRowCount<=iRowCountMax; iRowCount++) {
			SDL_RenderDrawLine(mySDLRenderer,
        0+iCurrentOffsetWidth, iRowCount*fGridSquareHeight, iColumnCountMax*fGridSquareWidth+iCurrentOffsetWidth, iRowCount*fGridSquareHeight);
   }

  //columns
  for (int iColumnCount=0; iColumnCount<=iColumnCountMax; iColumnCount++) {
			SDL_RenderDrawLine(mySDLRenderer,
        iColumnCount*fGridSquareWidth+iCurrentOffsetWidth, 0, iColumnCount*fGridSquareWidth+iCurrentOffsetWidth, iRowCountMax*fGridSquareHeight);
   }
}


//edited by Mike, 20211118
void draw(int x, int y)
{

	SDL_RenderClear(mySDLRenderer);
				
	//added by Mike, 20211113
	drawLevel();

	//added by Mike, 20211111
	//note: excess drawn pixel due to drawGrid()...
//	drawGrid();

	//added by Mike, 20211117; edited by Mike, 20211118
//	myIpis[0]->draw();
	for (int iCount=0; iCount<MAX_IPIS; iCount++) {
		myIpis[iCount]->draw();
	}

	//added by Mike, 20211117
	myUnit->draw();
	
	//added by Mike, 20211112
//	drawShield();
}

void update() {
			//added by Mike, 20211118
    	for (int iCount=0; iCount<MAX_IPIS; iCount++) {
    		myIpis[iCount]->update();
    		    		
    		//added by Mike, 20211120
    		if (myIpis[iCount]->isHiddenState()) {
//    				myIpis[iCount]->reset(myIpis[iCount]->getXPos(),myIpis[iCount]->getYPos());
				myIpis[iCount]->executeRegenerate();

				//TO-DO: -reverify: this
				//iCountIpisDestroyed++;
				
				//after 1 loop based on destroyed ipis start index, increase speed
				//TO-DO: -fix: Unit stuck at right-down corner when with shake, et cetera
				if (iCount==IPIS_START_INDEX) {
//				if (iCountIpisDestroyed>=14) {
					iStepX=2;
					iStepY=2;
				}
    		}
		}						


		//note: clock-wise movement
		//rectangle top side
		if (myKeysDown[KEY_D] == TRUE) {
			//edited by Mike, 20211115
//			if (iPilotY==3*fGridSquareHeight) {
			if (iPilotY==(3*fGridSquareHeight+iCurrentOffsetHeight)) {
				myKeysDown[KEY_D] = TRUE;
								
				if (iPilotX<(8*fGridSquareWidth+iCurrentOffsetWidth)) {
						//edited by Mike, 20211115
//					iPilotX+=2;
					iPilotX+=iStepX;
				}	
				else {
					//iPilotY+=2;
					myKeysDown[KEY_S] = TRUE;			
					myKeysDown[KEY_D] = FALSE;
				}
			}
		}
	
		//rectangle right side
		if (myKeysDown[KEY_S] == TRUE) {
			if (iPilotX==(8*fGridSquareWidth+iCurrentOffsetWidth)) {
				myKeysDown[KEY_S] = TRUE;
	
				if (iPilotY<7*fGridSquareHeight) {
					//edited by Mike, 20211115
//					iPilotY+=2;
					iPilotY+=iStepY;
				}	
				else {
					myKeysDown[KEY_A] = TRUE;			
					myKeysDown[KEY_S] = FALSE;
				}
			}
		}
				
		//rectangle bottom side		
		if (myKeysDown[KEY_A] == TRUE) {		
			//edited by Mike, 20211115
//			if (iPilotY==7*fGridSquareHeight) {
			if (iPilotY==7*(fGridSquareHeight+iCurrentOffsetHeight)) {

				myKeysDown[KEY_A] = TRUE;

				if (iPilotX>(1*fGridSquareWidth+iCurrentOffsetWidth)) {
					//edited by Mike, 20211115
//					iPilotX-=2;
					iPilotX-=iStepX;
				}	
				else {
					myKeysDown[KEY_W] = TRUE;			
					myKeysDown[KEY_A] = FALSE;			
				}
			}
		}

		//rectangle left side		
		if (myKeysDown[KEY_W] == TRUE) {		
			if (iPilotX==(1*fGridSquareWidth+iCurrentOffsetWidth)) {
				myKeysDown[KEY_W] = TRUE;

				if (iPilotY>3*fGridSquareHeight) {
					//edited by Mike, 20211115
//					iPilotY-=2;
					iPilotY-=iStepY;
				}	
				else {
					myKeysDown[KEY_D] = TRUE;			
					myKeysDown[KEY_W] = FALSE;			
				}
			}
		}
		
		//added by Mike, 2021117
		myUnit->setXPosAsPixel(iPilotX);
		myUnit->setYPosAsPixel(iPilotY);
		
		for (int iCount=0; iCount<4; iCount++) { //directional keys only
			if (myKeysDown[iCount]==TRUE) {
				myUnit->move(iCount);
				break;
			}
		}		

		
			if (bIsExecutingDestroyBug) {
  				iCurrentOffsetWidth+=2;
					iCurrentOffsetHeight-=2;			

					//added by Mike, 20211120
					//TO-DO: -add: miss count; excess destroy ipis action
					//TO-DO: -add: hit ipis, but NOT yet destroyed notification
					//TO-DO: -add: sound beat
					

    			//added by Mike, 20211118
    			for (int iCount=0; iCount<MAX_IPIS; iCount++) {
						myUnit->collideWith(myIpis[iCount]);
					}

					if (iDestroyBugShakeDelayCount==iDestroyBugShakeDelayMax) {
				  	myKeysDown[KEY_K] = FALSE;
    				bIsExecutingDestroyBug = false;					
    				iDestroyBugShakeDelayCount=0;
    			}
    			else {
						iDestroyBugShakeDelayCount+=1;					
    			}    			
			}		
			else {
  			iCurrentOffsetWidth=iBaseOffsetWidth;
				iCurrentOffsetHeight=iBaseOffsetHeight;				
			}

/* //removed by Mike, 20211121			
			//added by Mike, 20211118
    	for (int iCount=0; iCount<MAX_IPIS; iCount++) {
    		myIpis[iCount]->update();
    		    		
    		//added by Mike, 20211120
    		if (myIpis[iCount]->isHiddenState()) {
//    				myIpis[iCount]->reset(myIpis[iCount]->getXPos(),myIpis[iCount]->getYPos());
				myIpis[iCount]->executeRegenerate();
						
				//after 1 loop based on destroyed ipis start index, increase speed
				if (iCount==IPIS_START_INDEX) {
					iStepX=2;
					iStepY=2;
				}
    		}
		}						
*/		
}

int main(int argc, char *argv[])
{
	initSDL();
	
	//added by Mike, 20211112
	init();
	
	//solution to problem: ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
	//edited by Mike, 20211113
//	SDL_Texture *texture = loadTexture((char*)"textures/hq.png");
	texture = loadTexture((char*)"textures/hq.png");

/*	//edited by Mike, 20211113
	iPilotX=myWindowWidthAsPixel/2;
	iPilotY=myWindowHeightAsPixel/2;
*/
/*	//edited by Mike, 20211114
	iPilotX=fGridSquareWidth*5;
	iPilotY=fGridSquareHeight*3;
*/
	iPilotX=fGridSquareWidth*1+iCurrentOffsetWidth;
	iPilotY=fGridSquareHeight*3;

	printf("myWindowWidthAsPixel: %i\n",myWindowWidthAsPixel);
	printf("myWindowHeightAsPixel: %i\n",myWindowHeightAsPixel);
	
	iCountTaoAnimationFrame=0;

	while (1)
	{
		prepareScene();

		doInput();
		
		update();
				
		//edited by Mike, 20211112
//		draw(texture, iPilotX, iPilotY);
		draw(iPilotX, iPilotY);

		presentScene();

		//TO-DO: add: auto-identify delay input count
		SDL_Delay(8);
	}

	return 0;
}

