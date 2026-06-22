#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sakktypedef.h"
#include "logika.h"
#include "debugmalloc.h"

static int gcd(int a, int b){
    int x=a>b?b:a;
    int y=a>b?a:b;
    int t;
    while (x!=0){
        t=y%x;
        y=x;
        x=t;
    }
    return y;
}

static bool vilagos(Babu babu){
    switch (babu){
        case VKiraly:
        case VVezer:
        case VBastya:
        case VFuto:
        case VHuszar:
        case VGyalog:
            return true;
            break;
        default:
            return false;
            break;
        }
}

static bool nincs_kozte(int kx, int ky, int ex, int ey,Babu tabla[8][8]){
    //printf("nincs_kozte %d %d %d %d \n",kx,ky,ex,ey);
    int g=gcd(abs(kx-ex),abs(ky-ey));
    int dx=(ex-kx)/g;
    int dy=(ey-ky)/g;
    int tx=kx+dx;
    int ty=ky+dy;
    while ((tx!=ex||ty!=ey)&&tabla[tx][ty]==Semmi){
        tx+=dx;
        ty+=dy;
        }
    return tx==ex&&ty==ey;
}

static bool lephet_kiraly(int kx, int ky, int ex, int ey){
    if (abs(ex-kx)<=1&&abs(ey-ky)<=1) return true;
    else return false;
}

static bool lephet_vezer(int kx, int ky, int ex, int ey,Babu tabla[8][8]){
    if ((kx==ex||ky==ey||(abs(ex-kx)==abs(ey-ky)))&&nincs_kozte(kx, ky, ex, ey,tabla)) return true;
    else return false;
}

static bool lephet_bastya(int kx, int ky, int ex, int ey,Babu tabla[8][8]){
    if ((kx==ex||ky==ey)&&nincs_kozte( kx, ky, ex, ey,tabla)) return true;
    else return false;
}

static bool lephet_futo(int kx, int ky, int ex, int ey,Babu tabla[8][8]){
    if((abs(ex-kx)==abs(ey-ky))&&nincs_kozte(kx, ky, ex, ey,tabla)) return true;
    else return false;
}

static bool lephet_huszar(int kx, int ky, int ex, int ey){
    if((abs(ex-kx)==2&&abs(ey-ky)==1)||(abs(ex-kx)==1&&abs(ey-ky)==2)) return true;
    else return false;
}

static bool lephet_Sgyalog(int kx, int ky, int ex, int ey,Babu tabla[8][8]){
    if (ky==1 && kx==ex && ey==3 && tabla[ex][2]==Semmi && tabla[ex][3]==Semmi)
      return true;
    if (ey-ky==1&&((kx==ex&&tabla[ex][ey]==Semmi)||(abs(ex-kx)==1&&tabla[ex][ey]!=Semmi && vilagos(tabla[ex][ey])))) return true;
    return false;
}

static bool lephet_Vgyalog(int kx, int ky, int ex, int ey,Babu tabla[8][8]){
    if (ky==6 && kx==ex && ey==4 && tabla[ex][5]==Semmi && tabla[ex][4]==Semmi)
      return true;
    if (ky-ey==1&&((kx==ex&&tabla[ex][ey]==Semmi)||(abs(ex-kx)==1&&tabla[ex][ey]!=Semmi && !vilagos(tabla[ex][ey])))) return true;
    return false;
}

static bool lephet_e(Babu kezben, int kx, int ky, int ex, int ey,Babu tabla[8][8]){
    if (kx==ex && ky==ey)
        return false;
    if (tabla[ex][ey]!=Semmi&&(vilagos(tabla[ex][ey])==vilagos(kezben))) return false;
    //printf("lephet_e %d %d %d %d %d\n",kezben,kx,ky,ex,ey);
    switch (kezben){
        case VKiraly:
        case SKiraly:
            return lephet_kiraly(kx, ky, ex, ey);
            break;
        case VVezer:
        case SVezer:
            return lephet_vezer(kx, ky, ex, ey,tabla);
            break;
        case VBastya:
        case SBastya:
            return lephet_bastya(kx, ky, ex, ey,tabla);
            break;
        case VFuto:
        case SFuto:
            return lephet_futo(kx, ky, ex, ey,tabla);
            break;
        case VHuszar:
        case SHuszar:
            return lephet_huszar(kx, ky, ex, ey);
            break;
        case SGyalog:
            return lephet_Sgyalog(kx, ky, ex, ey,tabla);
            break;
        case VGyalog:
            return lephet_Vgyalog(kx, ky, ex, ey,tabla);
            break;
        default:
            break;
        }
        return false;
}

static bool lephet_nemkiralyra(Mozgatas* mozgatas,Babu tabla[8][8]) {
    return lephet_e(mozgatas->kezben,mozgatas->kx,mozgatas->ky,mozgatas->ex,mozgatas->ey,tabla)
        &&(tabla[mozgatas->ex][mozgatas->ey]!=VKiraly)&&(tabla[mozgatas->ex][mozgatas->ey]!=SKiraly);
}

void tabla_kitolt(Babu tabla[8][8]) {
    for (int i=0; i<8; i+=1){
     for (int j=0; j<8; j+=1){
        tabla[i][j]=Semmi;
        //helyen[i][j]=true;
     }
    }
    tabla[0][0]=SBastya;
    tabla[1][0]=SHuszar;
    tabla[2][0]=SFuto;
    tabla[3][0]=SVezer;
    tabla[4][0]=SKiraly;
    tabla[5][0]=SFuto;
    tabla[6][0]=SHuszar;
    tabla[7][0]=SBastya;
    tabla[0][7]=VBastya;
    tabla[1][7]=VHuszar;
    tabla[2][7]=VFuto;
    tabla[3][7]=VVezer;
    tabla[4][7]=VKiraly;
    tabla[5][7]=VFuto;
    tabla[6][7]=VHuszar;
    tabla[7][7]=VBastya;
    for (int i=0;i<8;i++) {
        tabla[i][1]=SGyalog;
        tabla[i][6]=VGyalog;
    }
}

bool felveheti(int ex, int ey,int lepesszam, Babu tabla[8][8]){
    return (tabla[ex][ey]!=Semmi)&&(vilagos(tabla[ex][ey])==(lepesszam%2==1));
}

bool sanc(Mozgatas* mozgatas, Babu tabla[8][8], bool balsanc[2], bool jobbsanc[2]){
    //printf("sanc %d %d %d %d %d\n",mozgatas->kezben,mozgatas->kx,mozgatas->ky,mozgatas->ex,mozgatas->ey);
    if (tabla[mozgatas->ex][mozgatas->ey]!=Semmi) return false;
    if (mozgatas->kezben==SKiraly && mozgatas->ey==0 && mozgatas->ky==0){
        if (balsanc[0] && mozgatas->ex==2 && nincs_kozte(0,0,4,0,tabla))
            return true;
        if (jobbsanc[0] && mozgatas->ex == 6 && nincs_kozte(4,0,7,0,tabla))
            return true;
    }
    if (mozgatas->kezben==VKiraly && mozgatas->ey==7 && mozgatas->ky==7){
        if (balsanc[1] && mozgatas->ex==2 && nincs_kozte(0,7,4,7,tabla))
            return true;
        if (jobbsanc[1] && mozgatas->ex == 6 && nincs_kozte(4,7,7,7,tabla))
            return true;
    }
    return false;
}

bool sakk_e(int lepesszam,Babu tabla[8][8]){
    //printf("sakk_e %d\n",lepesszam);
    Babu sajat;
    if (lepesszam%2==0) sajat=SKiraly;
    else sajat=VKiraly;
    int kx=0,ky=0;
    while (tabla[kx][ky]!=sajat){
        if (ky==7) {
            ky=0;
            kx+=1;
        }
        else ky+=1;
    }
    for (int i=0; i<8; i+=1){
        for (int j=0; j<8; j+=1){
            if (tabla[i][j]!=Semmi&&(vilagos(tabla[i][j])!=vilagos(sajat))){
                if (lephet_e(tabla[i][j], i,j,kx,ky,tabla)) {
                    //printf("Sakk!\n");
                    return true;
                }
            }
        }
    }
    return false;
}

bool vegrehajt(Mozgatas* mozgatas, Babu tabla[8][8], bool balsanc[2], bool jobbsanc[2]) {
    mozgatas->sanc_e=sanc(mozgatas,tabla,balsanc,jobbsanc);
    //printf("sanc eredmeny: %d\n",mozgatas->sanc_e);
    if (!mozgatas->sanc_e && !lephet_nemkiralyra(mozgatas,tabla)) {
        //printf("Nem is lephet ide\n");
        return false;
    }
    if (mozgatas->kezben==VKiraly) {
        balsanc[1]=false;
        jobbsanc[1]=false;
    }
    if (mozgatas->kezben==SKiraly) {
        balsanc[0]=false;
        jobbsanc[0]=false;
    }
    if (mozgatas->kx==0 && mozgatas->ky==0) balsanc[0]=false;
    if (mozgatas->kx==0 && mozgatas->ky==7) jobbsanc[0]=false;
    if (mozgatas->kx==7 && mozgatas->ky==0) balsanc[1]=false;
    if (mozgatas->kx==7 && mozgatas->ky==7) jobbsanc[1]=false;
    if (mozgatas->sanc_e) {
        tabla[mozgatas->ex][mozgatas->ey]=mozgatas->kezben;
        if (mozgatas->ex==2) {
            tabla[0][mozgatas->ey]=Semmi;
            tabla[3][mozgatas->ey]=mozgatas->ey==7?VBastya:SBastya;
        }
        else {
            tabla[7][mozgatas->ey]=Semmi;
            tabla[5][mozgatas->ey]=mozgatas->ey==7?VBastya:SBastya;
        }
        mozgatas->utott=Semmi;
    }
    else {
        mozgatas->utott=tabla[mozgatas->ex][mozgatas->ey];
        //printf("utott beallitva %d %d %d\n",mozgatas->utott, mozgatas->ex, mozgatas->ey);
        tabla[mozgatas->ex][mozgatas->ey]=mozgatas->kezben;
        bool sakk = sakk_e(mozgatas->lepesszam, tabla);
        //printf("sakk eredmeny: %d\n",sakk);
        if (sakk) {
            tabla[mozgatas->ex][mozgatas->ey]=mozgatas->utott;
            mozgatas->utott=Semmi;
            //printf("sakkba kerulne/maradna, nem lephet\n");
            return false;
        }
    }
    return true;
}

bool nemtudlepni(int lepesszam,Babu tabla[8][8]){
    //printf("tudlepni %d\n",lepesszam);
    Babu sajat;
    if (lepesszam%2==1) sajat=VKiraly;
    else sajat=SKiraly;
    //printf("lepesszam %d kir %d\n",lepesszam,sajat);
    for (int i=0; i<8; i+=1){
        for (int j=0; j<8; j+=1){
            Babu kezben = tabla[i][j];
            tabla[i][j]=Semmi;
            if (vilagos(kezben)==vilagos(sajat)){
                for (int k=0; k<8; k+=1){
                    for (int l=0; l<8; l+=1){
                        if (lephet_e(kezben, i, j, k, l,tabla)) {
                            Babu utott = tabla[k][l];
                            tabla[k][l] = kezben;
                            if (!sakk_e(lepesszam,tabla)) {
                                tabla[i][j]=kezben;
                                tabla[k][l]=utott;
                                //printf("talalt lepest %d %d %d %d\n",i,j,k,l);
                                return false;
                            }
                            tabla[k][l]=utott;
                        }
                    }
                }
            }
            tabla[i][j]=kezben;
        }
    }
    //printf("Nem tud lepni %d\n",lepesszam);
    return true;
}

void vegere_beszur(Babu babu, Akcio akcio, Mozgatas* mozgatas) {
    Lepes* uj;
    uj = (Lepes*) malloc(sizeof(Lepes));
    if (mozgatas->eleje!=NULL) {
        (mozgatas->vege)->kov=uj;
    }
    else {
        mozgatas->eleje=uj;
    }
    mozgatas->vege=uj;
    uj->babu = babu;
    uj->kx = mozgatas->kx;
    uj->ky = mozgatas->ky;
    uj->ex = mozgatas->ex;
    uj->ey = mozgatas->ey;
    uj->akcio = akcio;
    uj->kov = NULL;
}
