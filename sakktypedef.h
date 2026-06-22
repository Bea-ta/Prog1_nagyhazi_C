#ifndef SAKKENUM_H_INCLUDED
#define SAKKENUM_H_INCLUDED

/* mezon allo figura. ugyanolyan sorrendben vannak, mint a kepen,
 * igy a kapott egesz szamok megegyeznek a png-beli indexekkel */
typedef enum Babu {
    VKiraly, VVezer, VBastya, VFuto, VHuszar, VGyalog,
    SKiraly, SVezer, SBastya, SFuto, SHuszar, SGyalog,
    Semmi
} Babu;

typedef enum Akcio { Lep, Ut, Sancol, Atvalt} Akcio;

typedef struct Lepes {
    Babu babu;
    int kx;
    int ky;
    int ex;
    int ey;
    Akcio akcio;
   struct Lepes *kov;
} Lepes;

typedef struct Mozgatas {
    int lepesszam;
    Babu kezben;
    Babu utott;
    int kx,ky;
    int ex,ey;
    int gyx,gyy;
    bool atvaltozas;
    bool sanc_e;
    Lepes* eleje;
    Lepes* vege;
} Mozgatas;

#endif // SAKKENUM_H_INCLUDED
