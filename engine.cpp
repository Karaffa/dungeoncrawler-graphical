#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "smartSprite.h"
#include "subjectSprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "twowayMultisprite.h"

const SDL_Color yellow = {255, 255, 0, 255};

Engine::~Engine() { 
  delete player;
  delete hud;
  for ( Drawable* sprite : sprites ) {
    delete sprite;
  }
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  lava("backlava", Gamedata::getInstance().getXmlInt("backlava/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("SpinningStar")),
  hud(new Hud("hud")),
  sprites(),
  strategies(),
  currentStrategy(0),
  collision(false),
  makeVideo(false),
  didSlash(false)
{
  int n = Gamedata::getInstance().getXmlInt("numberOfStars");
  sprites.reserve(n);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < n; ++i) {
    sprites.push_back( new SmartSprite("YellowStar", pos, w, h) );
    player->attach( sprites[i] );
  }

  strategies.push_back( new PerPixelCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  lava.draw();
  world.draw();

  for ( const Drawable* sprite : sprites ) {
    sprite->draw();
  }
  std::stringstream strm;
  //strm << sprites.size() << " Smart Sprites Remaining";
  //IOmod::getInstance().writeText(strm.str(), yellow, 30, 60);

  if (collision) {
    IOmod::getInstance().writeText("Oops: Collision", 500, 90);
  }
  player->draw();
  hud->draw();
  SDL_Color color = {0, 0, 255, 255};
  IOmod::getInstance().writeText("Pierre Karaffa", color, 0, 453);
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    SmartSprite* doa = *it;
    if (doa->exploded() /*&& player->isSlashing()*/){
      if(!didSlash){
       hud->healthUpdate(player->healthHit());
      }
      didSlash = false;
      //player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    }
    if (doa != NULL && strategies[currentStrategy]->execute(*player, **it)) {
      doa->explode();
      if(player->isSlashing()) didSlash = true;
      player->detach(doa);
      ++it;
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  player->update(ticks);
  hud->setPlayer(player); //Yikes
  hud->update(ticks); // testing
  for ( Drawable* sprite : sprites ) {
    sprite->update( ticks );
  }
  world.update();
  viewport.update(); // always update viewport last
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while (!done) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if(keystate[SDL_SCANCODE_SPACE]){
          player->slash();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDL_SCANCODE_F1]) {
          hud->controls();
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
