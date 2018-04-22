#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void SmartSprite::goLeft()  { setVelocityX( -abs(getVelocityX()) );  }
void SmartSprite::goRight() { setVelocityX( fabs(getVelocityX()) );  }
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) );  }


SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, 
  int w, int h) :
  Sprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  safeDistance(Gamedata::getInstance().getXmlFloat(name+"/safeDistance")),
  showExplode(false),
  showExploded(0)
{}


SmartSprite::SmartSprite(const SmartSprite& s) : 
  Sprite(s),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  safeDistance(s.safeDistance),
  showExplode(s.showExplode),
  showExploded(s.showExploded)
{}

void SmartSprite::update(Uint32 ticks) { 
  Sprite::update(ticks);
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) goRight();
      if ( x > ex ) goLeft();
      if ( y < ey ) goUp();
      if ( y > ey ) goDown();
    }
  }
}

void SmartSprite::explode(){
  showExplode = true;
  Sprite::explode();
}

bool SmartSprite::exploded(){
  if(!Sprite::didExplode()){
    showExplode = false;
  }

  if(showExplode == true){
    showExploded = 1;
  }
  if(showExplode == false && showExploded == 1)
  {
    showExploded = 2;
  }

  if(showExploded == 2){
    return true;
  }
  else return false;
}
