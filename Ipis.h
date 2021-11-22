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
 * @date updated: 20211122
 * @website address: http://www.usbong.ph
 *
 */

#include "MyDynamicObject.h"


class Ipis: public MyDynamicObject
{
private:	
	int iMyScoreValue; 
        
	int iMyStartXPos;
	int iMyStartYPos;
	
	//added by Mike, 20211120	
	int iIpisLevel;
	int iIpisLevelMax;

	//added by Mike, 20211122
	bool bIsHit;
	bool bIsHitAgain;

	int iDeathAnimationCounter;

  void setup();
	void drawIpis();
	void drawExplosion();

public:
    Ipis(SDL_Renderer* mySDLRenderer, int xPos, int yPos, int zPos,int windowWidth,int windowHeight);
	~Ipis();

    virtual void draw();
    
	void update();
	
	void changeState(int s); //moving or dead
	int getState();

	void reset(int iXPosInput, int iYPosInput);     
	void move(int key);	
	
	int executeRegenerate(); //added by Mike, 20211120; edited by Mike, 20211121
	
    virtual void hitBy(MyDynamicObject* mdo);
    virtual void destroy();
};
