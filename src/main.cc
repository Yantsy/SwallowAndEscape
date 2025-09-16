#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
// #include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>

int main() {

  // initialize SDL video subsystem
  if (SDL_Init(SDL_INIT_VIDEO) != 0 && SDL_Init(SDL_INIT_AUDIO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  // initialize SDL_mixer audio subsystem
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
    std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
    return 1;
  }
  // initialize the SDL game controller subsystem
  if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
    std::cerr << "SDL_InitSubSystem Error: " << SDL_GetError() << std::endl;
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
  // food texture

  SDL_Surface *foodsuf = IMG_Load("../assets/food.png");

  SDL_Texture *foodtex = SDL_CreateTextureFromSurface(renderer01, foodsuf);

  SDL_FreeSurface(foodsuf);

  int fx = rand() % 67 * 10;
  int fy = rand() % 48 * 10;
  // create the rectangles
  std::vector<SDL_Rect> segments;
  SDL_Rect block = {x, y, 15, 15};
  segments.push_back(block);
  SDL_Rect food = {fx, fy, 17, 17};

  // create the map
  int ww, wh;
  SDL_GetWindowSize(win1, &ww, &wh);
  SDL_Rect map1 = {
      (ww / 2) - 300,
      (wh / 2) - 200,
      600,
      400,
  };
  SDL_Rect map2 = {
      (ww / 2) - 285,
      (wh / 2) - 185,
      570,
      370,
  };
  // event handler
  SDL_Event e;

  // bgm
  Mix_Music *bgm = Mix_LoadMUS("../assets/99._Red!.mp3");
  if (bgm == nullptr) {
    std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
  }
  Mix_PlayMusic(bgm, -1);

  // gamecontroler

  SDL_GameController *controller = SDL_GameControllerOpen(0);

  // controller button
  if (controller == nullptr) {
    std::cerr << "SDL_GameControllerOpen Error: " << SDL_GetError()
              << std::endl;
  }

  // check and start the controller rumble
  if (controller != nullptr) {
    if (SDL_GameControllerHasRumble(controller) != false) {
      SDL_GameControllerRumble(controller, 0x4000, 0x4000, 300);
    }
  } else {
    std::cerr << "SDL_GameControllerHasRumble Error: " << SDL_GetError()
              << std::endl;
  };

  SDL_GameControllerAddMappingsFromFile("../assets/gamecontrollerdb.txt");

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

      case (SDL_CONTROLLERBUTTONDOWN): {
        switch (e.cbutton.button) {

        case SDL_CONTROLLER_BUTTON_START:
          ndir = 0;
          pdir = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
          ndir = -1;
          pdir = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
          ndir = 1;
          pdir = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
          pdir = -1;
          ndir = 0;
          break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
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
    if (block.x < 55)
      block.x = 55;
    if (block.x > 610)
      block.x = 610;
    if (block.y < 55)
      block.y = 55;
    if (block.y > 410)
      block.y = 410;

    // check for collision with food
    if (std::abs(((block.x + 15) / 2) - ((food.x + 14) / 2)) < 5 &&
        std::abs(((block.y + 15) / 2) - ((food.y + 14) / 2)) < 5) {
      // check and start the controller rumble
      if (controller != nullptr) {
        if (SDL_GameControllerHasRumble(controller) != false) {
          SDL_GameControllerRumble(controller, 0x4000, 0x4000, 300);
        }
      } else {
        std::cerr << "SDL_GameControllerHasRumble Error: " << SDL_GetError()
                  << std::endl;
      };

      fx = rand() % 67 * 10;
      fy = rand() % 48 * 10;
      food.x = fx;
      food.y = fy;
      if (food.x < 55)
        food.x = 55;
      if (food.x > 610)
        food.x = 610;
      if (food.y < 55)
        food.y = 55;
      if (food.y > 410)
        food.y = 410;

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
    SDL_SetRenderDrawColor(renderer01, 165, 222, 229, 255);
    SDL_RenderClear(renderer01);

    SDL_SetRenderDrawColor(renderer01, 254, 253, 202, 200);
    SDL_RenderFillRect(renderer01, &map1);

    SDL_SetRenderDrawColor(renderer01, 224, 249, 181, 255);
    SDL_RenderFillRect(renderer01, &map2);

    for (auto &segsheet : segments) {
      if (&segsheet == &segments[0]) {
        SDL_SetRenderDrawColor(renderer01, 255, 69, 69, 255);
        SDL_RenderFillRect(renderer01, &block);
        continue;
      };
      SDL_SetRenderDrawColor(renderer01, 255, 207, 223, 255);
      SDL_RenderFillRect(renderer01, &segsheet);
    };

    SDL_RenderCopy(renderer01, foodtex, NULL, &food);

    SDL_RenderPresent(renderer01);

    SDL_Delay(1000 / 60);
  }
  SDL_GameControllerClose(controller);
  SDL_DestroyRenderer(renderer01);
  SDL_DestroyWindow(win1);

  SDL_Quit();

  return 0;
}
