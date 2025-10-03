#include "../include/quote.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
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

  // 创建窗口、渲染器（renderer/suface/texture）、事件队列、蛇和食物、地图和控制器等事物

  // Create a window
  SDL_Window *win1 =
      SDL_CreateWindow("SwallowAndEscape", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1920, 1080,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
                           SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
  if (win1 == nullptr) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    return 1;
  }
  int ww, wh;
  SDL_GetWindowSize(win1, &ww, &wh);

  // create the renderer for the window

  SDL_Renderer *renderer01 = SDL_CreateRenderer(
      win1, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer01 == nullptr) {
    std::cerr << "SDL_CreateRenderer Error:" << SDL_GetError() << std::endl;
  }

  // create the map

  int thickness = 15;
  ;
  SDL_Rect map1 = {
      ww / 8,
      wh / 16,
      ww * 3 / 4,
      wh * 3 / 4,
  };

  SDL_Rect map2 = {
      map1.x + thickness,
      map1.y + thickness,
      map1.w - thickness * 2,
      map1.h - thickness * 2,
  };

  // create the snake head and food

  // head direction
  int pdir = 1;
  int ndir = 0;
  int cpdir = 1;
  int cndir = 0;

  // food texture

  SDL_Surface *foodsuf = IMG_Load("../assets/food.png");

  SDL_Texture *foodtex = SDL_CreateTextureFromSurface(renderer01, foodsuf);

  SDL_FreeSurface(foodsuf);

  // food position

  int fx = (rand() % (map2.w - map2.x)) + map2.x;
  int fy = (rand() % (map2.h - map2.y)) + map2.y;
  // create the rectangles
  std::vector<SDL_Rect> segments;
  std::vector<SDL_Rect> csegments;
  std::vector<SDL_Rect> foods;
  SDL_Rect block = {ww / 2, wh / 2, 15, 15};
  int speed = block.w / 3;
  segments.push_back(block);
  SDL_Rect cblock = {map2.x, map2.y, 15, 15};
  csegments.push_back(cblock);
  SDL_Rect food = {fx, fy, 15, 15};
  foods.push_back(food);

  // bgm
  Mix_Music *bgm = Mix_LoadMUS("../assets/99._Red!.mp3");
  if (bgm == nullptr) {
    std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
  }
  // Mix_PlayMusic(bgm, -1);

  // gamecontroler

  SDL_GameController *controller = SDL_GameControllerOpen(0);

  // open the controller

  controlleropencheck(controller);

  // check and start the controller rumble

  controllerrumbleopencheck(controller);

  // map the controller
  SDL_GameControllerAddMappingsFromFile("../assets/gamecontrollerdb.txt");

  // 设置游戏主循环以及在玩家操作下，上述事物会如何变化

  // control variable for the game loop
  // event handler
  SDL_Event e;
  bool quit = false;

  int ld, rd, td, bd, md;

  // game loop
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {

      case SDL_QUIT: {
        quit = true;
        break;
      }

      case SDL_KEYDOWN: {
        keyboard(&pdir, &ndir, e);
        break;
      }

      case (SDL_CONTROLLERBUTTONDOWN): {
        pad(pdir, ndir, e);
        break;
      }
      }
    }
    ld = abs(cblock.x - speed - food.x) + abs(cblock.y - food.y);
    rd = abs(cblock.x + speed - food.x) + abs(cblock.y - food.y);
    td = abs(cblock.x - food.x) + abs(cblock.y - speed - food.y);
    bd = abs(cblock.x - food.x) + abs(cblock.y + speed - food.y);

    std::vector<int> neighbors = {ld, rd, td, bd};

    md = std::min({neighbors[0], neighbors[1], neighbors[2], neighbors[3]});
    if (md == neighbors[0]) {
      cpdir = -1;
      cndir = 0;
    }
    if (md == neighbors[1]) {
      cpdir = 1;
      cndir = 0;
    }
    if (md == neighbors[2]) {
      cpdir = 0;
      cndir = -1;
    }
    if (md == neighbors[3]) {
      cpdir = 0;
      cndir = 1;
    }
    // update the position of the player's head and body

    block.x += pdir * speed;
    block.y += ndir * speed;

    cblock.x += cpdir * speed;
    cblock.y += cndir * speed;

    segments.insert(segments.begin(), block);

    csegments.insert(csegments.begin(), cblock);

    // check for collision with the boundary
    boundarycheck(&block, map2);
    boundarycheck(&cblock, map2);

    // check for collision with food
    if (std::abs(((block.x + 15) / 2) - ((food.x + 15) / 2)) < 5 &&
        std::abs(((block.y + 15) / 2) - ((food.y + 15) / 2)) < 5) {
      // check and start the controller rumble
      if (controller != nullptr) {
        SDL_GameControllerRumble(controller, 0x4000, 0x4000, 300);
      } else {
      };
      fx = floor(((rand() % (map2.w)) + map2.x) / 15) * 15;
      fy = floor(((rand() % (map2.h)) + map2.y) / 15) * 15;
      food.x = fx;
      food.y = fy;

    } else {
      if (!segments.empty()) {
        segments.pop_back();
      } else {
      }
    }

    if (std::abs(((cblock.x + 15) / 2) - ((food.x + 15) / 2)) < 5 &&
        std::abs(((cblock.y + 15) / 2) - ((food.y + 15) / 2)) < 5) {
      // check and start the controller rumble
      fx = floor(((rand() % (map2.w)) + map2.x) / 15) * 15;
      fy = floor(((rand() % (map2.h)) + map2.y) / 15) * 15;
      food.x = fx;
      food.y = fy;

    } else {
      if (!csegments.empty()) {
        csegments.pop_back();
      } else {
      }
    }

    // check for collision with the boundary of the snake itself
    for (int seg = 0; seg < segments.size(); seg++) {
      if (std::abs(block.x - segments[seg].x) < 15 &&
          std::abs(block.y - segments[seg].y) < 15) {
      }
    }
    for (int seg = 0; seg < csegments.size(); seg++) {
      if (std::abs(block.x - csegments[seg].x) < 15 &&
          std::abs(block.y - csegments[seg].y) < 15) {
      }
    }

    // 将创建的事物以及玩家操作导致它产生的变化渲染出来
    //  render and present
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

    for (auto &csegsheet : csegments) {
      if (&csegsheet == &csegments[0]) {
        SDL_SetRenderDrawColor(renderer01, 47, 79, 79, 255);
        SDL_RenderFillRect(renderer01, &cblock);
        continue;
      };
      SDL_SetRenderDrawColor(renderer01, 192, 192, 192, 255);
      SDL_RenderFillRect(renderer01, &csegsheet);
    };

    SDL_RenderCopy(renderer01, foodtex, NULL, &food);
    SDL_RenderPresent(renderer01);

    SDL_Delay(1000 / 60);
  }

  // 清理程序运行过程中生成的资源，释放内存
  SDL_DestroyTexture(foodtex);
  SDL_GameControllerClose(controller);
  SDL_DestroyRenderer(renderer01);
  SDL_DestroyWindow(win1);

  SDL_Quit();

  return 0;
}