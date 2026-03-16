#include "quote.h"

#include <iostream>

void controlleropencheck(SDL_GameController *controller) {
  for (int counter = 0; counter < 1; counter++) {
    if (controller == nullptr) {
      std::cerr << "SDL_GameControllerOpen Error: " << SDL_GetError()
                << std::endl;
    }
  }
};
void controllerrumbleopencheck(SDL_GameController *controller) {
  for (int counter = 0; counter < 1; counter++) {
    if (controller != nullptr) {

      if (SDL_GameControllerHasRumble(controller) == false) {

        std::cerr << "SDL_GameControllerHasRumble Error: " << SDL_GetError()
                  << std::endl;
      }
    };
  }
};
void keyboard(int *xdir, int *ydir, SDL_Event e) {

  switch (e.key.keysym.scancode) {

  case SDL_SCANCODE_Q: {
    *xdir = 0, *ydir = 0;
    break;
  }

  case SDL_SCANCODE_W:
  case SDL_SCANCODE_UP:
    *ydir = -1, *xdir = 0;
    break;

  case SDL_SCANCODE_S:
  case SDL_SCANCODE_DOWN:
    *ydir = 1, *xdir = 0;
    break;

  case SDL_SCANCODE_A:
  case SDL_SCANCODE_LEFT:
    *xdir = -1, *ydir = 0;
    break;

  case SDL_SCANCODE_D:
  case SDL_SCANCODE_RIGHT:
    *xdir = 1, *ydir = 0;
    break;

  default:
    break;
  }
}
void pad(int &xdir, int &ydir, SDL_Event e) {
  switch (e.cbutton.button) {

  case SDL_CONTROLLER_BUTTON_START:
    xdir = 0, ydir = 0;
    break;
  case SDL_CONTROLLER_BUTTON_DPAD_UP:
    ydir = -1, xdir = 0;
    break;
  case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
    ydir = 1, xdir = 0;
    break;
  case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
    xdir = -1, ydir = 0;
    break;
  case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
    xdir = 1, ydir = 0;
    break;
  default:
    break;
  }
}

void boundarycheck(SDL_Rect *object, SDL_Rect map) {
  if (object->x < map.x)
    object->x = map.x;
  if (object->x > map.x + map.w - object->w)
    object->x = map.x + map.w - object->w;
  if (object->y < map.y)
    object->y = map.y;
  if (object->y > map.y + map.h - object->w)
    object->y = map.y + map.h - object->w;
};