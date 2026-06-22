#ifndef LOGIKA_H_INCLUDED
#define LOGIKA_H_INCLUDED

void tabla_kitolt(Babu tabla[8][8]);

bool sanc(Mozgatas* mozgatas, Babu tabla[8][8], bool balsanc[2], bool jobbsanc[2]);

bool sakk_e(int lepesszam,Babu tabla[8][8]);

bool vegrehajt(Mozgatas* mozgatas, Babu tabla[8][8], bool balsanc[2], bool jobbsanc[2]);

bool nemtudlepni(int lepesszam,Babu tabla[8][8]);

void vegere_beszur(Babu babu, Akcio akcio, Mozgatas* mozgatas);

bool felveheti(int ex, int ey,int lepesszam, Babu tabla[8][8]);







#endif // LOGIKA_H_INCLUDED
