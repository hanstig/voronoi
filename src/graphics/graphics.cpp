#include "graphics.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <cstdint>

void graphics::init(uint16_t width, uint16_t height, const char *title) {

  int rendererFlags, windowFlags;
  rendererFlags = SDL_RENDERER_ACCELERATED;
  windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  window =
      SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, windowFlags);

  if (!window) {
    printf("Failed to open %d x %d window: %s\n", width, height,
           SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  renderer = SDL_CreateRenderer(window, -1, rendererFlags);
  if (!renderer) {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }
}
void graphics::doInput(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button != SDL_BUTTON_LEFT) {
        break;
      }

      points.push_back(
          point{(uint16_t)event.button.x, (uint16_t)event.button.y});

    default:
      break;
    }
  }
}

void graphics::drawCircle(int16_t centreX, int32_t centreY, uint16_t radius)
{
   const int16_t diameter = (radius * 2);

   int16_t x = (radius - 1);
   int16_t y = 0;
   int16_t tx = 1;
   int16_t ty = 1;
   int16_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void graphics::drawPoints() {
  for (const point &p : points) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    drawCircle(p.x, p.y, 3); 

    SDL_RenderDrawPoint(renderer, p.x, p.y);
  }
}


void graphics::drawLines() {
  for(const point &p1 : points) {
    for(const point &p2 : points) {
      if(p1.x != p2.x && p1.y != p2.y) {
        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
      }
    }
  }
}

void graphics::prepareScene(void) {
  SDL_SetRenderDrawColor(renderer, 23, 42, 48, 255);
  SDL_RenderClear(renderer);
  drawPoints();
  drawLines();
}

void graphics::presentScene(void) { SDL_RenderPresent(renderer); }

int main(int argc, char *argv[]) {

  graphics g;

  g.init(600, 600, "SDL");

  // atexit(cleanup);

  while (1) {
    g.prepareScene();

    g.doInput();

    g.presentScene();

    SDL_Delay(16);
  }

  return 0;
}
