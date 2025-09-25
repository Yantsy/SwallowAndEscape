#ifndef QUOTE_H
#define QUOTE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
void keyboard(int ndir, int pdir, SDL_Event e);
void pad(int ndir, int pdir, SDL_Event e);
void controlleropencheck(SDL_GameController *controller);
void controllerrumbleopencheck(SDL_GameController *controller);
void boundarycheck(SDL_Rect *object, SDL_Rect map);
#endif