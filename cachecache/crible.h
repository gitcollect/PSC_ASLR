#ifndef CRIBLE_H
#define CRIBLE_H

long randomIndice(long taille, int etat[],int ilFautSupprimer, long* nbTrue);
void initCrible(long min, long max, int etatOptimal[],int etatCourant[]);
void crible(long min,long max, int* tab[],long taille, int* a, int etatOptimal[]);
long uneIteration(long taille, int* tab[],int* a, int etat[],long min, long max);
void copie(long taille,int to[], int td[]);
void instructionsCentralesMelangees(long taille,long* t[], int etatCourant[],long min, long max);
void impressionResultat(long min, long max, int etat[],long nbTrue);

#endif
