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
 * @date created: 20211117
 * @date updated: 20211118
 * @website address: http://www.usbong.ph
 *
 */

#include "MyDynamicObject.h"

class Unit: public MyDynamicObject
{
private:	
	int iMyScoreValue; 
        
	int iMyStartXPos;
	int iMyStartYPos;

	int iDeathAnimationCounter;

  void setup();
	void drawUnit();
	void drawExplosion();

public:
  Unit(SDL_Renderer* mySDLRenderer, int xPos, int yPos, int zPos,int windowWidth,int windowHeight);
	~Unit();

  virtual void draw();
    
	void update();
	
	void changeState(int s); //moving or dead
	int getState();

	void reset(int iXPosInput, int iYPosInput);     
	void move(int key);	
	
  virtual void hitBy(MyDynamicObject* mdo);
  virtual void destroy();
};
