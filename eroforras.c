#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sakktypedef.h"
#include "eroforras.h"
#include "debugmalloc.h"

void mentes(Lepes* eleje){
    char fajlnev[255];
    FILE* fp;
    printf("Milyen neven legyen mentve a jatek? \n");
    scanf("%s", fajlnev);
    fp = fopen(fajlnev, "w");
    if (fp==NULL) {
        perror("Nem sikerult megnyitni a fajlt, megszakit.");
        return;
    }
    Lepes* temp = eleje;
    while (temp!=NULL) {
        //printf("eleje es temp egyenlosege: %d\n",temp==eleje);
        char b;
        switch(temp->babu) {
            case VKiraly:
            case SKiraly:
                b='K';
                break;
            case VVezer:
            case SVezer:
                b='V';
                break;
            case VBastya:
            case SBastya:
                b='B';
                break;
            case VFuto:
            case SFuto:
                b='F';
                break;
            case VHuszar:
            case SHuszar:
                b='H';
                break;
            case VGyalog:
            case SGyalog:
                b='G';
                break;
            default:
                break;
        }
        char a;
        switch (temp->akcio) {
        case Lep:
            a='L';
            break;
        case Ut:
            a='U';
            break;
        case Sancol:
            a='S';
            break;
        case Atvalt:
            a='A';
            break;
        }
        fprintf(fp, "%c %d %d %d %d %c\n", b,temp->kx, temp->ky, temp->ex, temp->ey,a);
        temp=temp->kov;
    }
    fclose(fp);
    printf("Mentes sikeres.\n");
}

bool betolt(FILE* fp, Mozgatas* mozgatas, Babu tabla[8][8]) {
    while (!feof(fp)) {
        Lepes *uj;
        uj = (Lepes*) malloc(sizeof(Lepes));
        if (mozgatas->eleje==NULL){
            mozgatas->eleje=uj;
        }
        else {
            (mozgatas->vege)->kov=uj;
        }
        mozgatas->vege=uj;
        uj->kov=NULL;
        char a,b;
        int kx,ky,ex,ey;
        if (fscanf(fp,"%c %d %d %d %d %c\n",&b,&kx,&ky,&ex,&ey,&a)!=6) {
            return false;
        }
        //printf("beolv %c %d %d %d %d %c\n",b,kx,ky,ex,ey,a);
        switch(b) {
        case 'K':
            uj->babu=mozgatas->lepesszam%2==1?VKiraly:SKiraly;
            break;
        case 'B':
            uj->babu=mozgatas->lepesszam%2==1?VBastya:SBastya;
            break;
        case 'H':
            uj->babu=mozgatas->lepesszam%2==1?VHuszar:SHuszar;
            break;
        case 'F':
            uj->babu=mozgatas->lepesszam%2==1?VFuto:SFuto;
            break;
        case 'V':
            uj->babu=mozgatas->lepesszam%2==1?VVezer:SVezer;
            break;
        case 'G':
            uj->babu=mozgatas->lepesszam%2==1?VGyalog:SGyalog;
            break;
        }
        uj->kx=kx;
        uj->ky=ky;
        uj->ex=ex;
        uj->ey=ey;
    //    Babu sajat_gyalog;
        switch(a) {
        case 'L':
            uj->akcio=Lep;
            break;
        case 'U':
            uj->akcio=Ut;
            break;
        case 'S':
            uj->akcio=Sancol;
            break;
        case 'A':
            uj->akcio=Atvalt;
    //        sajat_gyalog=lepesszam%2==1?VGyalog:SGyalog;
            break;
        }
    //    if (tabla[kx][ky]!=uj->babu && (uj->akcio!=Atvalt || tabla[kx][ky]!=sajat_gyalog)) {
    //        perror("Hibas a leiro fajl! Uresrol indul->\n");
    //        return false;
    //    }
        tabla[kx][ky]=Semmi;
        tabla[ex][ey]=uj->babu;
        if (uj->akcio==Sancol) {
            Babu sajat_bastya=mozgatas->lepesszam%2==1?VBastya:SBastya;
            if (ex==2) {
    //            if (tabla[0][ey]!=sajat_bastya) {
    //                perror("Hibas a leiro fajl! Uresrol indul->\n");
    //                return false;
    //            }
                tabla[0][ey]=Semmi;
                tabla[3][ey]=sajat_bastya;
            }
            else {
    //            if (tabla[7][ey]!=sajat_bastya) {
    //                perror("Hibas a leiro fajl! Uresrol indul->\n");
    //                return false;
    //            }
                tabla[7][ey]=Semmi;
                tabla[5][ey]=sajat_bastya;
            }
        }
        mozgatas->lepesszam++;
    }
    return true;
}

void kezdokepernyo(Mozgatas* mozgatas, Babu tabla[8][8]) {
    printf("Uj(u) vagy regi(r) jatek? ");
    char c;
    scanf("%c",&c);
    if (c=='r'||c=='R'){
        char fajlnev[255];
        printf("Milyen neven mentette a jatekot? ");
        scanf("%s", fajlnev);

        FILE* fp;
        fp = fopen(fajlnev, "r");
        if (fp == NULL) {
            perror("Nem sikerült megnyitni a fájlt, alapallasrol indul.\n");
        }
        else {
            bool sikeres=betolt(fp,mozgatas,tabla);
            fclose(fp);
            if (!sikeres)
                perror("Hibas a leiro fajl! Alapallasrol indul.\n");
            else {
                printf("Sikeres betoltes.\n");
                return;
            }
        }
    }
}

void felszabadit_kilep(Mozgatas* mozgatas) {
    mentes(mozgatas->eleje);
    Lepes* iter = mozgatas->eleje;
    while (iter != NULL) {
        Lepes *kov = iter->kov;
        //printf("free iter %d %d %d %d %d\n",iter->kx,iter->ky,iter->ex,iter->ey,iter->babu);
        free(iter);
        iter = kov;
    }
    free(mozgatas);
    exit(0);
}

