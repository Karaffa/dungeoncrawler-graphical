#ifndef PLAYER__H
#define PLAYER__H

#include "twowayMultisprite.h"
#include "smartSprite.h"
#include <list>


// In this example the player is derived from MultiSprite.
// However, there are many options.
class Player : public TwowayMultisprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();

  void attach( SmartSprite* o ) { observers.push_back(o); } 
  void detach( SmartSprite* o );

  int healthHit();

  void slash();
  bool isSlashing();

private:
  bool collision;
  Vector2f initialVelocity;
  std::list<SmartSprite*> observers;
  int health;
};
#endif
