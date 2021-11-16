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
 * @date updated: 20211116
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
		
Ipis::Ipis(int xPos, int yPos, int zPos, int windowWidth, int windowHeight): MyDynamicObject(xPos,yPos,zPos, windowWidth, windowHeight)
{ 
    invincibleCounter=0;

    currentState=MOVING_STATE;
    currentStatus=status;

	iMyWidthAsPixel=64; 
    iMyHeightAsPixel=64; 
    
    iMyXPosAsPixel=xPos;
    iMyYPosAsPixel=yPos;
    iMyZPosAsPixel=zPos;

	iStepX=1;
	iStepY=1;
	iStepZ=1;

	fMyWindowWidth=windowWidth;
	fMyWindowHeight=windowHeight;
			
    iMyStartXPos=iMyXPosAsPixel;
	iMyStartYPos=iMyYPosAsPixel;
    
    iMyScoreValue=200;
}

Ipis::~Ipis()
{
}


void Ipis::drawIpis() {	
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
        case DEATH_STATE:
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
           case DEATH_STATE:
			  break;
            default: //STANDING STATE
              break;//do nothing    
    }
}

void Ipis::changeStatus(int s)
{
  currentStatus=s;                  
}

void Ipis::changeState(int s)
{
  currentState=s;                  
}

int Ipis:getState()
{
    return currentState;
}


void Ipis::reset(float xPos, float yPos, float zPos)
{
    iMyXPos=xPos;
    iMyYPos=yPos;  
    iMyZPos=zPos;  

    changeState(INITIALIZING_STATE);
    setCollidable(false);
    iDeathAnimationCounter=0;
}


void Ipis::hitBy(MyDynamicObject* mdo)
{
    setCollidable(false);	
    //TO-DO: -add: score
    changeState(DEATH_STATE);
}

void Ipis::destroy()
{
}


