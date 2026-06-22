#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "debugmalloc.h"
#include "grafika.h"
#include "logika.h"
#include "eroforras.h"

int main(int argc, char *argv[]) {
    /* allapotot koveto valtozok */
    Babu tabla[8][8];
    Mozgatas* mozgatas=(Mozgatas*)malloc(sizeof(Mozgatas));
    mozgatas->lepesszam=1;
    mozgatas->eleje=NULL;
    mozgatas->vege=NULL;
    mozgatas->kezben=Semmi;
    mozgatas->utott=Semmi;
    mozgatas->sanc_e=false;
    mozgatas->atvaltozas=false;
    bool balsanc[2]={true,true};
    bool jobbsanc[2]={true,true};

    /* logika es grafika beallitasa (uj vagy betoltes) */
    tabla_kitolt(tabla);
    kezdokepernyo(mozgatas, tabla);

    /* ablak letrehozasa */
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Sakk", 1100, 650, &window, &renderer);

    /* kep betoltese */
    SDL_Texture *babuk = IMG_LoadTexture(renderer, "pieces.png");
    if (babuk == NULL) {
        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
        exit(1);
    }

    alapall(renderer,babuk,tabla);
    lepes_info(renderer,mozgatas->lepesszam);


    SDL_Event event;
    /* esemenyek feldolgozasa */
    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT) { // várunk a következő eseményre
        switch (event.type) {   /* esemény típusa szerinti esetszétválasztás */
            case SDL_MOUSEBUTTONUP:{
                boxRGBA(renderer, 0, 0, 110, 65, 245,222,179, 255);
                int evx=event.motion.x;
                int evy=event.motion.y;
                if (evx>=935&&evx<=1000&&evy>=60&&evy<=90) {// feladom gomb
                    if (mozgatas->lepesszam%2==1) printf("Sotet gyozott\n");
                    else printf("Vilagos gyozott\n");
                    sdl_vege(babuk);
                    felszabadit_kilep(mozgatas);
                }
                if (mozgatas->atvaltozas) { // az atvaltozasra jelol ki babut
                    //printf("atvaltozas true\n");
                    Babu uj_babu = atvalt_valasztas(mozgatas->lepesszam,evx,evy);
                    //printf("valasztott babu, %d\n",uj_babu);
                    if (uj_babu!=Semmi) {
                        tabla[mozgatas->gyx][mozgatas->gyy]=uj_babu;
                        if (nemtudlepni(mozgatas->lepesszam,tabla)) {
                            if (sakk_e(mozgatas->lepesszam,tabla)) {
                                printf("Matt!\n");
                                if (mozgatas->lepesszam%2==1) printf("Sotet gyozott\n");
                                else printf("Vilagos gyozott\n");
                            }
                            else printf ("Patt!\n");
                            sdl_vege(babuk);
                            felszabadit_kilep(mozgatas);
                        }
                        bool sakk=sakk_e(mozgatas->lepesszam, tabla);
                        sakk_kiir(renderer,sakk);
                        gyalog_csere(renderer,babuk,uj_babu, mozgatas->gyx, mozgatas->gyy);
                        vegere_beszur(uj_babu, Atvalt, mozgatas);
                        lepes_info(renderer,mozgatas->lepesszam+1);
                        SDL_RenderPresent(renderer);
                        mozgatas->kezben=Semmi;
                        mozgatas->utott=Semmi;
                        mozgatas->atvaltozas=false;
                        mozgatas->lepesszam++;
                    }
                    break; // kovetkezo esemenyre varunk
                }
                // tablan tortent kattintast keresunk
                mozgatas->ex=(evx-230)/70;
                mozgatas->ey=(evy-45)/70;
                //printf("x: %d, y: %d \n",mozgatas->ex, mozgatas->ey );
                if (mozgatas->ex<0||mozgatas->ex>7||mozgatas->ey<0||mozgatas->ey>7)
                    break; // nem a tablan tortent kattintas, varunk egy masikat
                //printf("palyan %d %d\n",mozgatas->ex,mozgatas->ey);
                if (mozgatas->kezben==Semmi) { // ha nincs kezben babu
                    if (felveheti(mozgatas->ex,mozgatas->ey,mozgatas->lepesszam,tabla)) { // ezt felveheti: megjeloljuk
                        mozgatas->kezben=tabla[mozgatas->ex][mozgatas->ey];
                        tabla[mozgatas->ex][mozgatas->ey]=Semmi;
                        mozgatas->kx=mozgatas->ex;
                        mozgatas->ky=mozgatas->ey;
                        kijelol(renderer,mozgatas->ex,mozgatas->ey,true);
                        //printf("kezbevett %d %d %d utott %d\n",mozgatas->kezben,mozgatas->kx,mozgatas->ky,mozgatas->utott);
                    }
                    break; // kovetkezo esemenyre varunk
                }
                // lepes masodik kattintasa: lerakas
                if (mozgatas->kx==mozgatas->ex&&mozgatas->ey==mozgatas->ky) { // ugyanarra a helyre rakja vissza
                    kijelol(renderer,mozgatas->ex,mozgatas->ey,false);
                    tabla[mozgatas->ex][mozgatas->ey]=mozgatas->kezben;
                    mozgatas->kezben=Semmi;
                    break; // kovetkezo esemenyre varunk
                }
                // lerakas mashova
                if (vegrehajt(mozgatas,tabla,balsanc,jobbsanc)) {
                    babu_vegrehajt(renderer, babuk, mozgatas ,tabla);
                    //printf("vegrehajtva %d %d %d %d kezben %d utott %d sanc %d\n",mozgatas->kx,mozgatas->ky,mozgatas->ex,mozgatas->ey,mozgatas->kezben,mozgatas->utott,mozgatas->sanc_e);
                    bool sakk=sakk_e(mozgatas->lepesszam+1,tabla);
                    sakk_kiir(renderer,sakk);
                    // gyalog az utolso sorban
                    if ((mozgatas->ey==0 && mozgatas->kezben==VGyalog)||(mozgatas->ey==7&&mozgatas->kezben==SGyalog)) {
                        //printf("atvaltozas-eszleles\n");
                        mozgatas->atvaltozas=true;
                        mozgatas->gyx=mozgatas->ex;
                        mozgatas->gyy=mozgatas->ey;
                        gyalog_valaszto(renderer);
                        break; // kovetkezo esemenyre varunk
                    }
                    // szabalyos lepest talaltunk
                    vegere_beszur(mozgatas->kezben,mozgatas->sanc_e?Sancol:((mozgatas->utott==Semmi)?Lep:Ut),mozgatas);
                    mozgatas->lepesszam+=1;
                    if (nemtudlepni(mozgatas->lepesszam,tabla)) {
                        if (sakk_e(mozgatas->lepesszam,tabla)) {
                            printf("Matt!\n");
                            if (mozgatas->lepesszam%2==1) printf("Sotet gyozott\n");
                            else printf("Vilagos gyozott\n");
                        }
                        else printf ("Patt!\n");
                        sdl_vege(babuk);
                        felszabadit_kilep(mozgatas);
                    }
                    mozgatas->utott=Semmi;
                    mozgatas->kezben=Semmi;
                    mozgatas->sanc_e=false;
                    lepes_info(renderer,mozgatas->lepesszam);
                }
                // nem volt szabalyos lepes
                else stringRGBA(renderer, 10, 10, "Rossz lepes", 0, 0, 0, 255);
                break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    /* ablak bezarasa */
    felszabadit_kilep(mozgatas);
    return 0;
}
