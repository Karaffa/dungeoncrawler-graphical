#include "player.h"

Player::Player( const std::string& name) :
  TwowayMultisprite(name),
  collision(false),
  initialVelocity(getVelocity()),
  observers(),
  health(3)
{ }

Player::Player(const Player& s) :
  TwowayMultisprite(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  observers(s.observers),
  health(s.health)
  { }

Player& Player::operator=(const Player& s) {
  //TwowayMultisprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  health = s.health;
  return *this;
}

void Player::stop() { 
  setVelocityX(0);
  setVelocityY(0);
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
  if ( !collision ) TwowayMultisprite::update(ticks);
  stop();
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

int Player::healthHit(){
  //std::cout << health << std::endl;
  if(health > 0) health--;
  return health;
}

void Player::slash(){
  TwowayMultisprite::slash();
}

bool Player::isSlashing(){
  return TwowayMultisprite::isSlashing();
}
