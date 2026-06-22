#include <stdbool.h>
#include <stdio.h>
#include "grafika.h"

/* ablak letrehozasa */
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

void sdl_vege(SDL_Texture* babuk) {
    /* nincs mar ra szukseg: felszabaditjuk a memoriat */
    SDL_DestroyTexture(babuk);
    /* ablak bezarasa */
    SDL_Quit();
}

/* kirajzol egy babut; a forras a betoltott png, a cel nevu kepre rajzol.
 * melyik babut, milyen koordinatakra: melyik, x, y. */
void babu_rajzol(SDL_Renderer *renderer, SDL_Texture *babuk, Babu melyik, int x, int y) {
    /* a forras kepbol ezekrol a koordinatakrol, ilyen meretu reszletet masolunk. */
    SDL_Rect src = { (melyik % 6) * 62 + 10, (melyik / 6) * 60 + 10, MERET, MERET };
    /* a cel kepre, ezekre a koordinatakra masoljuk */
    SDL_Rect dest = { x, y, MERET, MERET };
    /* kepreszlet masolasa */
    SDL_RenderCopy(renderer, babuk, &src, &dest);
}

void babu_helyez(SDL_Renderer *renderer, SDL_Texture *babuk, Babu babu, int x, int y){
    babu_rajzol(renderer, babuk, babu, 239+x*70, 54+y*70);
    //printf("babu kirak %d %d %d\n",babu,x,y);
}

void babu_torol(SDL_Renderer *renderer, int x, int y){
    int r, g, b;
    if (x%2==y%2) r=g=b=255;
    else r=g=b=0;
    boxRGBA(renderer, 230+x*70, 45+y*70, 300+x*70, 115+y*70, r, g, b, 255);
    //printf("babutorol %d %d \n",x,y);
}

void alapall(SDL_Renderer *renderer,SDL_Texture *babuk,Babu tabla[8][8]){
    boxRGBA(renderer, 0, 0, 1100, 650, 245,222,179, 255);
    roundedBoxRGBA(renderer, 200, 15, 820, 635, 30, 139, 69, 19, 255);
    boxRGBA(renderer, 935, 60, 1000, 90, 255, 255, 255, 255);
    stringRGBA(renderer, 940, 70, "Feladom", 0, 0, 0, 255);
    int r, g, b;
    for (int i=0; i<8; i+=1){
     for (int j=0; j<8; j+=1){
        if (i%2==j%2) r=g=b=255;
        else r=g=b=0;
        boxRGBA(renderer, 230+i*70, 45+j*70, 300+i*70, 115+j*70, r, g, b, 255);
        }
    }
    for (int i=0; i<8; i+=1){
     for (int j=0; j<8; j+=1){
        if (tabla[i][j]!=Semmi)
            babu_helyez(renderer,babuk,tabla[i][j],i,j);
     }
    }
    SDL_RenderPresent(renderer);
}

void kijelol(SDL_Renderer* renderer, int x, int y, bool kirak) {
    int r, g, b;
    if (kirak) {
        r=255;
        g=0;
        b=0;
    }
    else if (x%2==y%2) r=g=b=255;
    else r=g=b=0;
    circleRGBA(renderer, 265+x*70, 80+y*70, 30, r, g, b, 255);
    SDL_RenderPresent(renderer);
}

void lepes_info(SDL_Renderer* renderer,int lepesszam) {
    if (lepesszam%2==0) stringRGBA(renderer, 920, 30, "Sotet jon", 0, 0, 0, 255);
    else stringRGBA(renderer, 920, 30, "Vilagos jon", 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

void sakk_kiir(SDL_Renderer* renderer, bool sakk_e) {
    if (sakk_e)  stringRGBA(renderer, 920, 120, "Sakk!", 0, 0, 0, 255);
    else boxRGBA(renderer, 915, 115, 1100, 150, 245,222,179, 255);
    SDL_RenderPresent(renderer);
}

void gyalog_valaszto(SDL_Renderer *renderer) {
    boxRGBA(renderer, 935, 200, 1000, 230, 255, 255, 255, 255);
    stringRGBA(renderer, 940, 210, "Vezer", 0, 0, 0, 255);
    boxRGBA(renderer, 935, 240, 1000, 270, 255, 255, 255, 255);
    stringRGBA(renderer, 940, 250, "Futo", 0, 0, 0, 255);
    boxRGBA(renderer, 935, 280, 1000, 310, 255, 255, 255, 255);
    stringRGBA(renderer, 940, 290, "Huszar", 0, 0, 0, 255);
    boxRGBA(renderer, 935, 320, 1000, 350, 255, 255, 255, 255);
    stringRGBA(renderer, 940, 330, "Bastya", 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

Babu atvalt_valasztas(int lepesszam, int evx,int evy) {
    if (evx>=935&&evx<=1000&&evy>=200&&evy<=230){
        if (lepesszam%2==1) return VVezer;
        else return SVezer;
    }
    if (evx>=935&&evx<=1000&&evy>=240&&evy<=270){
        if (lepesszam%2==1) return VFuto;
        else return SFuto;
    }
    if (evx>=935&&evx<=1000&&evy>=280&&evy<=310){
        if (lepesszam%2==1) return VHuszar;
        else return SHuszar;
    }
    if (evx>=935&&evx<=1000&&evy>=320&&evy<=350){
        if (lepesszam%2==1) return VBastya;
        else return SBastya;
    }
    return Semmi;
}

void babu_vegrehajt(SDL_Renderer* renderer, SDL_Texture* babuk, Mozgatas* mozgatas, Babu tabla[8][8]) {
    babu_torol(renderer, mozgatas->kx, mozgatas->ky);
    babu_torol(renderer, mozgatas->ex, mozgatas->ey);
    babu_helyez(renderer, babuk, mozgatas->kezben, mozgatas->ex, mozgatas->ey);
    if (mozgatas->sanc_e) {
        //printf("babu vegrehajt sanc\n");
        if (mozgatas->ex==2) {
            babu_torol(renderer,0,mozgatas->ey);
            babu_helyez(renderer,babuk,tabla[3][mozgatas->ey],3,mozgatas->ey);
        }
        else {
            babu_torol(renderer,7,mozgatas->ey);
            babu_helyez(renderer,babuk,tabla[5][mozgatas->ey],5,mozgatas->ey);
        }
    }
    boxRGBA(renderer, 900, 10, 1100, 50, 245,222,179, 255);
    SDL_RenderPresent(renderer);
}

void gyalog_csere(SDL_Renderer* renderer, SDL_Texture* babuk, Babu uj_babu,int x, int y) {
    babu_torol(renderer,x, y);
    babu_helyez(renderer, babuk,uj_babu,x,y);
    boxRGBA(renderer,935, 200, 1000,350,245,222,179, 255);
}
