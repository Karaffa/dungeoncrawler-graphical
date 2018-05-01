// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

ImageFactory*  ImageFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   bool keepplaying = true;
   bool menuOn = true;
   try {
      while(keepplaying){
         Engine* engine = new Engine;
         while(menuOn){
            menuOn = engine->Menu();
         }
         keepplaying = engine->play();
         delete engine;
      }
      delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
