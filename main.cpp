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

#include <windows.h>

#ifdef _WIN32 //Windows machine
	#include <SDL.h>
	#include <SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

#include <time.h>
#include <unistd.h>
#include <iostream>

//note: "static" in C/C++ = "final" in java
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

bool pause;

/**************************
 * Function Declarations
 *
 **************************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);

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
}

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

/**************************
 * WinMain
 *
 **************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    //register window class
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "Halimbawa";
    RegisterClass (&wc);


	//reference: https://stackoverflow.com/questions/60119473/how-to-get-physical-screen-size-on-windows-i-get-strange-values;
	//last accessed: 20211111
	//answer by: Horst Walter, 20200207T1854; edited 20200208T0032
	myWindowWidthAsPixel  = static_cast<int>(GetSystemMetrics(SM_CXSCREEN));
	myWindowHeightAsPixel = static_cast<int>(GetSystemMetrics(SM_CYSCREEN));


	hWnd = CreateWindow(
		"Halimbawa", "Usbong Tugon System", 
      	WS_POPUPWINDOW | WS_VISIBLE,
		0, 0, myWindowWidthAsPixel, myWindowHeightAsPixel, 
		NULL, NULL, hInstance, NULL); 


   	RECT desktop;
    //Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();

    //Get the size of screen to the variable desktop
    GetWindowRect(hDesktop, &desktop);

    //The top left corner will have coordinates (0,0)
    //and the bottom right corner will have coordinates
    //(horizontal, vertical)
    myWindowWidthAsPixel = desktop.right;
    myWindowHeightAsPixel = desktop.bottom;

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

		SDL_Delay(1);
	}
	
    //destroy the window explicitly
    DestroyWindow (hWnd);

    return msg.wParam;
}

/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
	    case WM_CREATE:
	        return 0;
	
	    case WM_CLOSE:
	    	SDL_Quit();    	
	        PostQuitMessage (0);
	        return 0;

	    case WM_DESTROY:
	        return 0;
        
    	//Reference: https://social.msdn.microsoft.com/Forums/vstudio/en-US/fb4de52d-66d4-44da-907c-0357d6ba894c/swmaximize-is-same-as-fullscreen?forum=vcgeneral;
    	//last accessed: 20211111
    	//answer by: Espen Ruud Schultz Tuesday, 20090303T0743
	 	case WM_GETMINMAXINFO:  
	         RECT WorkArea; SystemParametersInfo( SPI_GETWORKAREA, 0, &WorkArea, 0 );  
	 
	         ( ( MINMAXINFO * )lParam )->ptMaxSize.x = ( WorkArea.right - WorkArea.left );  
	         ( ( MINMAXINFO * )lParam )->ptMaxSize.y = ( WorkArea.bottom - WorkArea.top );  
	         ( ( MINMAXINFO * )lParam )->ptMaxPosition.x = WorkArea.left;  
	         ( ( MINMAXINFO * )lParam )->ptMaxPosition.y = WorkArea.top;  
	 
	         return 0;  
 
	    case WM_KEYDOWN:
	        switch (wParam)
	        {
		        case VK_ESCAPE:
		            PostQuitMessage(0);
		            return 0;
	
	            //reference: 
	            //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	            //last accessed: 20211111
	   	        case 0x41: //A key
	                return 0;     
	   	       case 0x44: //D key
	                return 0;
	   	       case 0x57: //W key
	                return 0;
	   	       case 0x53: //S key
	                return 0;     
	   	       case 0x4A: //J key
                return 0;     
	   	       case 0x4C: //L key
	                return 0;     
	   	       case 0x49: //I key
	                return 0;     
	   	       case 0x4B: //K key
	                return 0;     
	   	       case 0x48: //H key
	                return 0;
	   	       case 0x55: //U key
	                return 0;	
	        }
	        return 0;

	    case WM_KEYUP:
	        switch (wParam)
	        {
		           //added by Mike, 20201013
		           //reference: 
		           //https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		           //last accessed: 20211111
		   	       case 0x41: //A key
	                    return 0;
		   	       case 0x44: //D key
		                return 0;     
		   	       case 0x57: //W key
		                return 0;     
		   	       case 0x53: //S key
		                return 0;     	
		   	       case 0x4A: //J key
		                return 0;     
		   	       case 0x4C: //L key
		                return 0;     
		   	       case 0x49: //I key
		                return 0;     
		   	       case 0x4B: //K key
		                return 0;
		   	       case 0x48: //H key
		                return 0;
		   	       case 0x55: //U key
		                return 0;
	        }
	        return 0;

	    default:
	        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}

