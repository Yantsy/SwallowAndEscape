#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
// #include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

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
  std::vector<SDL_Rect> segments;
  SDL_Rect block = {x, y, 15, 15};
  segments.push_back(block);
  SDL_Rect food = {fx, fy, 14, 14};
  // event handler
  SDL_Event e;

  // create the snake body

  // control variable for the game loop
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

    // update the position of the snake's head and body

    block.x += pdir * speed / 10;
    block.y += ndir * speed / 10;

    segments.insert(segments.begin(), block);

    // check for collision with the boundary
    if (block.x < 0)
      block.x = 0;
    if (block.x > 665)
      block.x = 665;
    if (block.y < 0)
      block.y = 0;
    if (block.y > 465)
      block.y = 465;

    // check for collision with food
    if (std::abs(block.x - food.x) < 15 && std::abs(block.y - food.y) < 15) {

      fx = rand() % 67 * 10;
      fy = rand() % 48 * 10;
      food.x = fx;
      food.y = fy;

    } else {
      if (!segments.empty()) {
        segments.pop_back();
      } else {
      }
    }

    // check for collision with the boundary of the snake itself
    for (int seg; seg < segments.size(); seg++) {
      if (std::abs(block.x - segments[seg].x) < 15 &&
          std::abs(block.y - segments[seg].y) < 15) {
      }
    }

    // render and present
    SDL_SetRenderDrawColor(renderer01, 255, 248, 220, 255);
    SDL_RenderClear(renderer01);

    SDL_SetRenderDrawColor(renderer01, 34, 139, 34, 255);
    SDL_RenderFillRect(renderer01, &block);

    for (auto &segsheet : segments) {
      SDL_SetRenderDrawColor(renderer01, 60, 179, 173, 255);
      SDL_RenderFillRect(renderer01, &segsheet);
    };

    SDL_SetRenderDrawColor(renderer01, 220, 20, 60, 135);
    SDL_RenderFillRect(renderer01, &food);

    SDL_RenderPresent(renderer01);

    SDL_Delay(1000 / 60);
  }

  SDL_DestroyRenderer(renderer01);
  SDL_DestroyWindow(win1);

  SDL_Quit();

  return 0;
}
