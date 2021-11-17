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
 * @date created: 20211112
 * @date updated: 20211117
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

//added by Mike, 20211117; prevents redefinition of header file
#ifndef _MYDYNAMICOBJECT_H
#define _MYDYNAMICOBJECT_H

#include <math.h>

//Reference: https://stackoverflow.com/questions/34152424/autodetection-of-os-in-c-c;
//answer by: Jeegar Patel, 20151208T0940
//auto-identify if Windows Machine
#ifdef _WIN32
	#include <windows.h> //Windows Machine
#endif

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

#include <stdlib.h>

#define IDLE_MOVING_STATE 0
#define WALKING_MOVING_STATE 1
#define ATTACKING_MOVING_STATE 2

/*	//edited by Mike, 20211117
const int INITIALIZING_STATE = 0;
const int MOVING_STATE = 1;
const int IN_TITLE_STATE = 2;
const int DYING_STATE = 3;

const int HIDDEN_STATE = 4,
					ACTIVE_STATE = 5;
*/
#define INITIALIZING_STATE 0
#define MOVING_STATE 1
#define IN_TITLE_STATE 2
#define DYING_STATE 3

#define HIDDEN_STATE 4
#define ACTIVE_STATE 5

#define FACING_UP 0
#define FACING_DOWN 1
#define FACING_LEFT 2
#define FACING_RIGHT 3

//TO-DO: -add: this
//class UsbongUtils;

class MyDynamicObject
{               
public:
		//note: OpenGL uses floating-point numbers, instead of integer numbers, i.e. whole numbers
    int iMyXPosAsPixel;
    int iMyYPosAsPixel;
    int iMyZPosAsPixel;
    
    int iMyWidthAsPixel;
    int iMyHeightAsPixel;
        
    int iStepXAsPixel;
    int iStepYAsPixel;
    int iStepZAsPixel;
    
    int iOffsetXPosAsPixel;
    int iOffsetYPosAsPixel;   
    
    //added by Mike, 20211117 
		int iCurrentKeyInput;
								
    bool isCollidable;
        
    float fMyWindowWidth;
    float fMyWindowHeight;
    
    float fGridSquareWidth;
    float fGridSquareHeight;

		SDL_Renderer *mySDLRenderer;
		SDL_Texture *texture;
		
		int iCountTotalFrames;
		int iCountAnimationFrame; //added by Mike, 20211117
    
    //TO-DO: -add: this
//    UsbongUtils *myUsbongUtils;
    
    int currentState;
    int currentMovingState;
    int currentFacingState;

    MyDynamicObject(int xPos=0, int yPos=0, int zPos=0, int windowWidth=0, int windowHeight=0): iMyXPosAsPixel(xPos), iMyYPosAsPixel(yPos), iMyZPosAsPixel(zPos), fMyWindowWidth(windowWidth), fMyWindowHeight(windowHeight)
    {
        currentState=ACTIVE_STATE;
        iCountTotalFrames=1;
    }
    
    ~MyDynamicObject();
    
    bool isActive(){
        if (currentState==ACTIVE_STATE) {
            return true;
        }
        else {
        	return false;
        }
    }
  	
  	//TO-DO: -add: this  
//    void setUsbongUtils(UsbongUtils* c);
    
    virtual void hitBy(MyDynamicObject* mdo);
    
    bool checkIsCollidable();
    void setCollidable(bool c);

		virtual int getXPos()
    {
        return iMyXPosAsPixel;
    }

    virtual float getYPos()
    {
        return iMyYPosAsPixel;
    }

    virtual float getZPos()
    {
        return iMyZPosAsPixel;
    }
    
    virtual float getWidth()
    {
        return iMyWidthAsPixel;
    }

    virtual float getHeight()
    {
        return iMyHeightAsPixel;
    }
            
    virtual float getStepX()
    {
        return iStepXAsPixel;
    }
    virtual float getStepY()
    {
        return iStepYAsPixel;
    }
    virtual float getStepZ()
    {
        return iStepZAsPixel;
    }

		SDL_Texture *loadTexture(char *filename);

    virtual void draw();
    
    virtual void setXPos(int iX) {
        iMyXPosAsPixel=iX;
    }

    virtual void setZPos(float iZ) {
        iMyZPosAsPixel=iZ;
    }
    virtual void setYPos(float iY) {
        iMyYPosAsPixel=iY;
    }
        
    virtual void setXPosAsPixel(int iX) {
        iMyXPosAsPixel=iX;
    }

    virtual void setZPosAsPixel(int iZ) {
        iMyZPosAsPixel=iZ;
    }

    virtual void setYPosAsPixel(int iY) {
        iMyYPosAsPixel=iY;
    }

    void move(int iKeyInput);
    
    bool checkCollision(MyDynamicObject* mdo1, MyDynamicObject* mdo2);
    void collideWith(MyDynamicObject* mdo);   
    virtual bool isIntersectingRect(MyDynamicObject* mdo1, MyDynamicObject* mdo2);        
    virtual void setCurrentMovingState(int iMovingState) {
        currentMovingState = iMovingState;
    }        
        
    virtual void destroy();
};

//added by Mike, 20211117
#endif
