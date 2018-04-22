#include <cmath>
#include <random>
#include <functional>
#include "hud.h"
#include "gamedata.h"
#include "renderContext.h"

Vector2f Hud::makeVelocity(int vx, int vy) const {
  return Vector2f(vx, vy);
}

Hud::Hud(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img, const Image* imgh, const Image* imgf1):
  Drawable(n, pos, vel), 
  image( img ),
  imagehold(imgh),
  imagef1(imgf1),
  imagetwo(),
  imagethree(),
  imagefour(),
  player( new Player("SpinningStar") ),
  f1toggle(false),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud::Hud(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  image( RenderContext::getInstance()->getImage(name) ),
  imagehold(RenderContext::getInstance()->getImage(name)),
  imagef1(RenderContext::getInstance()->getImage(name+"f1")),
  imagetwo(RenderContext::getInstance()->getImage(name+"two")),
  imagethree(RenderContext::getInstance()->getImage(name+"three")),
  imagefour(RenderContext::getInstance()->getImage(name+"four")),
  player( new Player("SpinningStar") ),
  f1toggle(false),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud::Hud(const Hud& s) :
  Drawable(s), 
  image(s.image),
  imagehold(s.imagehold),
  imagef1(s.imagef1),
  imagetwo(s.imagetwo),
  imagethree(s.imagethree),
  imagefour(s.imagefour),
  player( new Player("SpinningStar") ),
  f1toggle(false),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud& Hud::operator=(const Hud& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  imagehold = rhs.image;
  imagef1 = rhs.imagef1;
  imagetwo = rhs.imagetwo;
  imagethree = rhs.imagethree;
  imagefour = rhs.imagefour;
  player = rhs.player;
  f1toggle = rhs.f1toggle;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Hud::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  if(f1toggle != 2) image->draw(getX(), getY(), getScale()); 
}

void Hud::controls(){
  if(f1toggle == 0){
    image = imagef1;
    f1toggle = 1;
  }
  else if(f1toggle == 1){
    image = imagehold;
    f1toggle = 2;
  }
  else{
    f1toggle = 0;
  }
}

void Hud::healthUpdate(int health){
  if(health == 2){
    image = imagetwo;
  }
  if(health == 1){
    image = imagethree;
  }
  if(health == 0){
    image = imagefour;
  }
}

void Hud::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
}
