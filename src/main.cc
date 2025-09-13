#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
// #include <cstddef>
#include <cstdlib>
#include <iostream>

int main() {

  // initialize SDL video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  // Create a window
  SDL_Window *win1 =
      SDL_CreateWindow("SwallowAndEscape", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 680, 480,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
                           SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
  if (win1 == nullptr) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // create the renderer for the window

  SDL_Renderer *renderer01 = SDL_CreateRenderer(
      win1, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer01 == nullptr) {
    std::cerr << "SDL_CreateRenderer Error:" << SDL_GetError() << std::endl;
  }

  // create the snake head and food

  // head position
  int x = 10;
  int y = 10;
  // head direction
  int pdir = 1;
  int ndir = 0;
  // food position
  int fx = rand() % 67 * 10;
  int fy = rand() % 48 * 10;
  // create the rectangles
  SDL_Rect block = {x, y, 15, 15};
  SDL_Rect food = {fx, fy, 10, 10};
  // event handler
  SDL_Event e;

  bool quit = false;

  int speed = 30;

  // game loop
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {

      case SDL_QUIT: {
        quit = true;
        break;
      }

      case SDL_KEYDOWN: {
        switch (e.key.keysym.scancode) {

        case SDL_SCANCODE_Q: {
          ndir = 0;
          pdir = 0;
          break;
        }

        case SDL_SCANCODE_E: {
          ndir = 0;
          pdir = 1;
          break;
        }

        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
          ndir = -1;
          pdir = 0;
          break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
          ndir = 1;
          pdir = 0;
          break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
          pdir = -1;
          ndir = 0;
          break;
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
          pdir = 1;
          ndir = 0;
          break;
        default:
          break;
        }
      }
      }
    }

    // update the position of the snake head
    block.x += pdir * speed / 10;
    block.y += ndir * speed / 10;
    // boundary check
    if (block.x < 0)
      block.x = 0;
    if (block.x > 665)
      block.x = 665;
    if (block.y < 0)
      block.y = 0;
    if (block.y > 465)
      block.y = 465;
    // render and present
    SDL_SetRenderDrawColor(renderer01, 114, 159, 207, 255);
    SDL_RenderClear(renderer01);

    SDL_SetRenderDrawColor(renderer01, 76, 44, 255, 0);
    SDL_RenderFillRect(renderer01, &block);

    SDL_SetRenderDrawColor(renderer01, 255, 0, 0, 135);
    SDL_RenderFillRect(renderer01, &food);

    if (std::abs(block.x - food.x) < 15 && std::abs(block.y - food.y) < 15) {
      fx = rand() % 67 * 10;
      fy = rand() % 48 * 10;
      food.x = fx;
      food.y = fy;
    } else {
    }
    SDL_RenderPresent(renderer01);

    SDL_Delay(1000 / 50);
  }

  SDL_DestroyRenderer(renderer01);
  SDL_DestroyWindow(win1);

  SDL_Quit();

  return 0;
}
