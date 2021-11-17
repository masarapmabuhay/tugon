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
 * @date updated: 20211117
 * @website address: http://www.usbong.ph
 *
 */

#include <stdio.h>
#include <math.h>

#include "Ipis.h"

#include <string>

//Reference: https://stackoverflow.com/questions/34152424/autodetection-of-os-in-c-c;
//answer by: Jeegar Patel, 20151208T0940
//auto-identify if Windows Machine
#ifdef _WIN32
	#include <windows.h> //Windows Machine
#endif
		
Ipis::Ipis(SDL_Renderer* mySDLRendererInput, int xPos, int yPos, int zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,zPos, windowWidth, windowHeight)
{ 
  currentState=MOVING_STATE;

	iMyWidthAsPixel=64; 
  iMyHeightAsPixel=64; 
    
  iMyXPosAsPixel=xPos;
  iMyYPosAsPixel=yPos;
  iMyZPosAsPixel=zPos;

	iStepXAsPixel=1;
	iStepYAsPixel=1;
	iStepZAsPixel=1;

	fMyWindowWidth=windowWidth;
	fMyWindowHeight=windowHeight;
			
  iMyStartXPos=iMyXPosAsPixel;
	iMyStartYPos=iMyYPosAsPixel;
    
  iMyScoreValue=200;
    
  //added by Mike, 20211117
  iCountAnimationFrame=0;

  mySDLRenderer = mySDLRendererInput;
  
  texture = loadTexture((char*)"textures/ipis.png");
}

Ipis::~Ipis()
{
}


void Ipis::drawIpis() {	
  	//Rectangles for drawing which will specify source (inside the texture)
  	//and target (on the screen) for rendering our textures.
  	SDL_Rect SrcR;
  	SDL_Rect DestR;
  	
		iCountAnimationFrame=iCountAnimationFrame+1;		               																				

		if (iCountAnimationFrame>=2) { //2 frames of animation only
			iCountAnimationFrame=0;
		}
	    	
  	SrcR.x = 0+iCountAnimationFrame*iMyWidthAsPixel;
  	SrcR.y = 0;
	
  	SrcR.w = iMyWidthAsPixel; 
  	SrcR.h = iMyHeightAsPixel; 
	
  	DestR.x = getXPos();
  	DestR.y = getYPos();
  	
  	DestR.w = iMyWidthAsPixel;
  	DestR.h = iMyHeightAsPixel;
	
	
  	//note: SDL color max 255; GIMP color max 100
//		SDL_SetRenderDrawColor(mySDLRenderer, 255*1, 255*1, 255*1, 255); //white
		
		SDL_RenderCopy(mySDLRenderer, texture, &SrcR, &DestR);
}

void Ipis::drawExplosion() {
}

void Ipis::draw()
{
    switch (currentState)
    {
        case INITIALIZING_STATE:
        case MOVING_STATE:
          drawIpis();
          break;
      	case DYING_STATE:
					drawExplosion();
        	break;
	}    
}

void Ipis::update(float dt)
{
    switch (currentState)
    {
           case INITIALIZING_STATE:                
           case MOVING_STATE:      
			  break;
           case DYING_STATE:
			  break;
            default: //STANDING STATE
              break;//do nothing    
    }
}

void Ipis::changeState(int s)
{
  currentState=s;                  
}

int Ipis::getState()
{
    return currentState;
}


void Ipis::reset(int iXPosInput, int iYPosInput)
{
    iMyXPosAsPixel=iXPosInput;
    iMyYPosAsPixel=iYPosInput;  

    changeState(INITIALIZING_STATE);
    setCollidable(false);
    iDeathAnimationCounter=0;
}


void Ipis::hitBy(MyDynamicObject* mdo)
{
    setCollidable(false);	
    //TO-DO: -add: score
    changeState(DYING_STATE);
}

void Ipis::destroy()
{
}


