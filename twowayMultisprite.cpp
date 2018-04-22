#include <iostream>
#include <cmath>
#include "twowayMultisprite.h"
#include "gamedata.h"
#include "renderContext.h"
//#include "explodingSprite.h"

void TwowayMultisprite::advanceImage(Uint32 ticks){
	timeSinceLastImage += ticks;
	if(timeSinceLastImage > frameInterval){
		currentImage = (currentImage + 1) % numberOfImages;
		timeSinceLastImage = 0;
	}
}

TwowayMultisprite::TwowayMultisprite(const std::string& name) :
	Drawable(name,
		Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
				 Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
		Vector2f(Gamedata::getInstance().getXmlInt(name+"/speed/x"),
				 Gamedata::getInstance().getXmlInt(name+"/speed/y"))
		),
	imagesRight(ImageFactory::getInstance()->getImages(name)),
	imagesLeft(ImageFactory::getInstance()->getImages(name+"Left")),
	imagesUp(ImageFactory::getInstance()->getImages(name+"Up")),
	imagesDown(ImageFactory::getInstance()->getImages(name+"Down")),
	slashRight(ImageFactory::getInstance()->getImages(name+"SlashR")),
	slashLeft(ImageFactory::getInstance()->getImages(name+"SlashL")),
	slashUp(ImageFactory::getInstance()->getImages(name+"SlashU")),
	slashDown(ImageFactory::getInstance()->getImages(name+"SlashD")),
	images(imagesRight),
	hasSlashed(false),//new
	ultimatecount(0),
	frameWidth(imagesLeft[0]->getWidth()),
	frameHeight(imagesLeft[0]->getHeight()),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	worldTop(Gamedata::getInstance().getXmlInt(name+"/worldTop")),
	worldBottom(Gamedata::getInstance().getXmlInt(name+"/worldBottom")),
	dt(0),
	currentImage(0),
	numberOfImages(Gamedata::getInstance().getXmlInt(name+"/frames")),
	frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameSpeed")),
	timeSinceLastImage(0)
{ }

TwowayMultisprite::TwowayMultisprite(const TwowayMultisprite& s) :
	Drawable(s.getName(), s.getPosition(), s.getVelocity()),
	imagesRight(s.imagesRight),
	imagesLeft(s.imagesLeft),
	imagesUp(s.imagesUp),
	imagesDown(s.imagesDown),
	slashRight(s.slashRight),
	slashLeft(s.slashLeft),
	slashUp(s.slashUp),
	slashDown(s.slashDown),
	images(s.imagesRight),
	hasSlashed(s.hasSlashed),
	ultimatecount(s.ultimatecount),
	//explosion(s.explosion),
	frameWidth(s.getImage()->getWidth()),
	frameHeight(s.getImage()->getHeight()),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
	worldTop(s.worldTop),
	worldBottom(s.worldBottom),
	dt(s.dt),
	currentImage(s.currentImage),
	numberOfImages(s.numberOfImages),
	frameInterval(s.frameInterval),
	timeSinceLastImage(s.timeSinceLastImage)
{ }

/*void TwowayMultisprite::explode(){
  if(!explosion){
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentImage]);
    explosion = new ExplodingSprite(sprite);
  }
}*/

void TwowayMultisprite::draw() const {
	Uint32 x = static_cast<Uint32>(getX());
	Uint32 y = static_cast<Uint32>(getY());
	images[currentImage]->draw(x, y);
}

void TwowayMultisprite::update(Uint32 ticks){

	Vector2f incr = getVelocity() * static_cast<float>(ticks) * .001;
	setPosition(getPosition() + incr);

// slash stuff
if(hasSlashed){
	ultimatecount++;

	if((images == imagesDown || images == slashDown) ){
		images = slashDown;
		advanceImage(ticks);
		if(ultimatecount == 50){ 
			hasSlashed = false;
			ultimatecount = 0;
		}
	}
	else if((images == imagesUp || images == slashUp) ){
		images = slashUp;
		advanceImage(ticks);
		if(ultimatecount == 50) {
			hasSlashed = false;
			ultimatecount = 0;
		}
	}
	else if((images == imagesRight || images == slashRight) ){
		images = slashRight;
		advanceImage(ticks);
		if(ultimatecount == 50) {
			hasSlashed = false;
			ultimatecount = 0;
		}
	}
	else if((images == imagesLeft || images == slashLeft) ){
		images = slashLeft;
		advanceImage(ticks);
		if(ultimatecount == 50) {
			hasSlashed = false;
			ultimatecount = 0;
		}
	}
}
//slash stuff
else{
	if(getVelocityY() > 0){
		images = imagesDown;
		advanceImage(ticks);
	}
	else if(getVelocityY() < 0){
		images = imagesUp;
		
		advanceImage(ticks);
	}

	else if(getVelocityX() > 0){
		images = imagesRight;
		
		advanceImage(ticks);
	}
	else if(getVelocityX() < 0){
		images = imagesLeft;
		
		advanceImage(ticks);
	}
}
}

void TwowayMultisprite::slash(){
	hasSlashed = true;
}