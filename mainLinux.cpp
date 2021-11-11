/*
 * Copyright 2020~2021 SYSON, MICHAEL B.
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
 * @date updated: 20211111
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

static int myWindowWidthAsPixel=640;
static int myWindowHeightAsPixel=640;


SDL_Window *mySDLWindow = NULL;
SDL_Renderer *mySDLRenderer = NULL;

int iPilotX;
int iPilotY;
int iCountTaoAnimationFrame;

int myKeysDown[10]; //note: includes KEY_J, KEY_L, KEY_I, KEY_K,

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
    
    //can be pressed simultaneously with directional button
    if (event->keysym.scancode == SDL_SCANCODE_K)
    {
        myKeysDown[KEY_K] = TRUE;
    }
	}
}

void keyUp(SDL_KeyboardEvent *event)
{
	if (event->repeat == 0)
	{
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
        
    //added by Mike, 20210905
    if (event->keysym.scancode == SDL_SCANCODE_K)
    {
        myKeysDown[KEY_K] = FALSE;
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
	SDL_SetRenderDrawColor(mySDLRenderer, 0, 255*0.667, 255*0.494, 255); //blue green
	
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

//Reference: http://wiki.libsdl.org/SDL_RenderCopy;
//last accessed: 20211111
void draw(SDL_Texture *texture, int x, int y)
{
	int iPilotWidth=64;
	int iPilotHeight=64;

	
  //Rectangles for drawing which will specify source (inside the texture)
  //and target (on the screen) for rendering our textures.
  SDL_Rect SrcR;
  SDL_Rect DestR;
  
	iCountTaoAnimationFrame=(iCountTaoAnimationFrame)%3;                    																				    

  SrcR.x = 0+ iCountTaoAnimationFrame*iPilotWidth;
  SrcR.y = 0;
  SrcR.w = iPilotWidth;
  SrcR.h = iPilotHeight;

  DestR.x = x;
  DestR.y = y;
  DestR.w = iPilotWidth;
  DestR.h = iPilotHeight;
  	
  int iCount;
  for (iCount=0; iCount<iNumOfKeyTypes; iCount++) {
		if (myKeysDown[iCount]==TRUE) {
 			iCountTaoAnimationFrame=iCountTaoAnimationFrame+1;																				
 			break;
		}
  }
  if (iCount==iNumOfKeyTypes) {
 			iCountTaoAnimationFrame=0;																				
  }

	SDL_RenderClear(mySDLRenderer);
	SDL_RenderCopy(mySDLRenderer, texture, &SrcR, &DestR);
}

void update() {
		if (myKeysDown[KEY_W])
		{
			iPilotY-=4;
		}

		if (myKeysDown[KEY_S])
		{
			iPilotY+=4;
		}

		if (myKeysDown[KEY_A])
		{
			iPilotX-=4;
		}

		if (myKeysDown[KEY_D])
		{
			iPilotX+=4;
		}
}

int main(int argc, char *argv[])
{
	initSDL();
	
	//solution to problem: ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
	SDL_Texture *texture = loadTexture((char*)"textures/imageSpriteExampleMikeWithoutBG.png");

	iPilotX=myWindowWidthAsPixel/2;
	iPilotY=myWindowHeightAsPixel/2;
	
	printf("myWindowWidthAsPixel: %i\n",myWindowWidthAsPixel);
	printf("myWindowHeightAsPixel: %i\n",myWindowHeightAsPixel);
	
	iCountTaoAnimationFrame=0;

	while (1)
	{
		prepareScene();

		doInput();
		
		update();
				
		draw(texture, iPilotX, iPilotY);

		presentScene();

		//TO-DO: add: auto-identify delay input count
		SDL_Delay(8);
	}

	return 0;
}

