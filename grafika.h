#ifndef GRAFIKA_H_INCLUDED
#define GRAFIKA_H_INCLUDED

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include "sakktypedef.h"

/* a pieces.png fajlban levo figurak merete */
enum { MERET = 52 };

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);

void sdl_vege(SDL_Texture* babuk);

void babu_rajzol(SDL_Renderer *renderer, SDL_Texture *babuk, Babu melyik, int x, int y);

void babu_helyez(SDL_Renderer *renderer, SDL_Texture *babuk, Babu babu, int x, int y);

void babu_torol(SDL_Renderer *renderer, int x, int y);

void alapall(SDL_Renderer *renderer,SDL_Texture *babuk,Babu tabla[8][8]);

void kijelol(SDL_Renderer* renderer, int x, int y, bool kirak);

void lepes_info(SDL_Renderer* renderer,int lepesszam);

void sakk_kiir(SDL_Renderer* renderer, bool sakk_e);

void gyalog_valaszto(SDL_Renderer *renderer);

Babu atvalt_valasztas(int lepesszam, int evx,int evy);

void gyalog_csere(SDL_Renderer* renderer, SDL_Texture* babuk, Babu uj_babu,int x, int y);

void babu_vegrehajt(SDL_Renderer *renderer, SDL_Texture* babuk, Mozgatas* mozgatas, Babu tabla[8][8]);

#endif // GRAFIKA_H_INCLUDED
