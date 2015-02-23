
#include <time.h>
#include "constantes.h"

long randomIndice(long taille, int etat[],int ilFautSupprimer, long* nbTrue)
{
    long r=rand()%taille;
    long res=0;
    while(r>=0){
        res++;
        while(etat[res]!=ilFautSupprimer){
            res++;
            if(res>=taille){
                res=0;
            }
        }
    r--;
    }
    if(ilFautSupprimer)
    {
        etat[res]=0;
        (*nbTrue)--;
    }
    else
    {
       etat[res]=1;
       return res;
       (*nbTrue)++;
    }
}

void initCrible(long min, long max, int etatOptimal[],int etatCourant[])
{
    int i=0;
    for(i=0;i<max-min+1;i++)
    {
        etatCourant[i]=1;
        etatOptimal[i]=1;
    }
}
void crible(long min,long max, int* tab[],long taille, int* a, int etatOptimal[])
{
    int etatCourant[max-min+1];
    initCrible(min,max,etatOptimal,etatCourant);
    int compteur=0;
    int ilFautSupprimer=1;
    long nbTrue=max-min+1;
    long bestNbTrue=nbTrue;
    long longueur=nbTrue;
    long i,n;
    for(compteur=0;compteur<NBCRIBLE;compteur++)
    {
        i=randomIndice(longueur,etatCourant,ilFautSupprimer,&nbTrue);
        n=uneIteration(taille,tab,a,etatCourant);
        if(n>SIGNIFICATIF)
        {
            ilFautSupprimer=1;
            if(nbTrue==1)
            {
                int j=0;
                while(etatCourant[j]==0)
                {
                    j++;
                }
                printf("Il ne reste qu'une ligne, la numero %ld",j);
                return;
            }
        }
        else
        {
            ilFautSupprimer=0;
            if(nbTrue<bestNbTrue)
            {
                copie(longueur,etatCourant,etatOptimal);
                bestNbTrue=nbTrue;
            }
        }
        if(nbTrue==n)
        {
            ilFautSupprimer=1;
        }
    }
}
void copie(long taille,int to[], int td[])
{
    int i=0;
    for(i=0;i<taille;i++)
    {
        td[i]=to[i];
    }
}
long uneIteration(long taille, int* tab[],int* a, int etat[])
{

}
