#ifndef TWOWAY_MULTISPRITE_H
#define TWOWAY_MULTISPRITE_H

#include <string>
#include <iostream>
#include <vector>
#include "drawable.h"

//class ExplodingSprite;

class TwowayMultisprite : public Drawable{
public:
	TwowayMultisprite(const std::string&);
	TwowayMultisprite(const TwowayMultisprite& s);
	virtual ~TwowayMultisprite() { }
	virtual const Image* getImage() const {return images[currentImage];}

	int getScaledWidth() const {
		return getScale()*images[currentImage]->getWidth();
	}
	int getScaledHeight() const{
		return getScale()*images[currentImage]->getHeight();
	}
	virtual const SDL_Surface* getSurface() const{
		return images[currentImage]->getSurface();
	}

	
	virtual void draw() const;
	virtual void update(Uint32 ticks);
	virtual void incrImage(){
		currentImage = (currentImage + 1) % numberOfImages;
	}
	void setImagesLeft() { images = imagesLeft;}
	void setImagesRight() {images = imagesRight;}
	void setImagesUp() {images = imagesUp;}
	void setImagesDown() {images = imagesDown;}

	void slash();
	bool isSlashing(){return hasSlashed;}

	//virtual void explode();

private:
	std::vector<Image *> imagesRight;
	std::vector<Image *> imagesLeft;
	std::vector<Image *> imagesUp;
	std::vector<Image *> imagesDown;
	std::vector<Image *> slashRight;
	std::vector<Image *> slashLeft;
	std::vector<Image *> slashUp;
	std::vector<Image *> slashDown;
	std::vector<Image *> images;

	bool hasSlashed;
	int ultimatecount;

	//ExplodingSprite* explosion;
protected:
	int frameWidth;
	int frameHeight;
	int worldWidth;
	int worldHeight;
	int worldTop;
	int worldBottom;
	float dt;
	unsigned currentImage;
	unsigned numberOfImages;
	unsigned frameInterval;
	float timeSinceLastImage;
	void advanceImage(Uint32 ticks);
	TwowayMultisprite& operator=(const TwowayMultisprite&);
};
#endif