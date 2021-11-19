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
 * @date updated: 20211119
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
    
  //edited by Mike, 20211118
  reset(iMyXPosAsPixel, iMyYPosAsPixel);
/*    
  //added by Mike, 20211117
  iCountAnimationFrame=0;
  //added by Mike, 20211118
  iDeathCountMaxDelayBeforeHidden=60; //TO-DO: add: sound
  iDeathCountDelayBeforeHidden=0;
  
  iRegenerateCountMaxDelayBeforeActive=60; 
	iRegenerateCountDelayBeforeActive=0; 
*/


  mySDLRenderer = mySDLRendererInput;
  
  texture = loadTexture((char*)"textures/ipis.png");
  
	//added by Mike, 20211118  
  isCollidable=true;  
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

//edited by Mike, 20211118
void Ipis::drawExplosion() {
		//Rectangles for drawing which will specify source (inside the texture)
  	//and target (on the screen) for rendering our textures.
  	SDL_Rect SrcR;
  	SDL_Rect DestR;

  	SrcR.x = 0+iMyWidthAsPixel*2 + iCountAnimationFrame*iMyWidthAsPixel;
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

		if (iCountAnimationFrame>=2) { //2 frames of animation only
			iCountAnimationFrame=0;			
		}
  	
		iCountAnimationFrame=iCountAnimationFrame+1;		
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
      	case HIDDEN_STATE:
        	break;
	}    
}

void Ipis::update()
{
    switch (currentState)
    {
        case INITIALIZING_STATE:                
        case MOVING_STATE:      
						break;
        case DYING_STATE:
        		if (iDeathCountDelayBeforeHidden>=iDeathCountMaxDelayBeforeHidden) {
							iDeathCountDelayBeforeHidden=0;
							changeState(HIDDEN_STATE);
        		}
        		else {
        			iDeathCountDelayBeforeHidden++;
        		}        		
			  		break;
      	case HIDDEN_STATE:
        		if (iRegenerateCountDelayBeforeActive>=iRegenerateCountDelayBeforeActive) {
							iRegenerateCountDelayBeforeActive=0;
							
/*							//TO-DO: -add: auto-identify regenerate location
							int iCount=0;
							reset(fGridSquareWidth*5+fGridSquareWidth*iCount, fGridSquareHeight*3);
*/

							reset(getXPos(), getYPos());							
						}
						else {
							iRegenerateCountDelayBeforeActive++;
						}
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
    setCollidable(true);
    iDeathAnimationCounter=0;
    
  	iCountAnimationFrame=0;
  	iDeathCountMaxDelayBeforeHidden=60;
  	iDeathCountDelayBeforeHidden=0;
  
  	iRegenerateCountMaxDelayBeforeActive=60; 
		iRegenerateCountDelayBeforeActive=0;     
}


void Ipis::hitBy(MyDynamicObject* mdo)
{
		iDeathCountDelayBeforeHidden=0;
		iCountAnimationFrame=0;
    setCollidable(false);	
    //TO-DO: -add: score
    changeState(DYING_STATE);
}

void Ipis::destroy()
{
}


