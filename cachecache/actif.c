#include "stats.h"
#include <time.h>
#include "constantes.h"
//ce fichier comporte le test lui-même - la fonction duree en est le point culminant, elle renvoie la mesure

#ifdef __i386
unsigned long rdtsc() {
  unsigned long lo, hi;
        asm( "rdtsc" : "=a" (lo), "=d" (hi) );
        return (lo);
}
#elif defined __amd64
unsigned long rdtsc() {
  unsigned long lo, hi;
        asm( "rdtsc" : "=a" (lo), "=d" (hi) );
        return( lo | (hi << 32) ); //Reponse sur 64 bits stockée dans 2 registres, à reconstituer
}
#endif

void instructionsCentrales(long taille,long* t[], long i)
{
    long k=0, j=0;
    srand(time(NULL));

    for(k=0; k < i; k++)
    {
        for(j=0;j<taille;j++)
        {
                long piment = (rand()% (5*taille));
                t[k][j] = k*(j+32)-j*piment/(k+2);
        }
    }
    for(k=0;k<i;k++)
    {
        tri(t[k],taille);
    }
}

long duree(long taille, long* t[], int* a, long i)
{
    unsigned long begin;
    unsigned long end;
    (*a)=0;
    begin= rdtsc();
    (*a)=1;
    end= rdtsc();
    long t1=end-begin;
    instructionsCentrales(taille,t,i);
    begin=rdtsc();
    (*a)=2;
    end = rdtsc();
    long t2=end-begin;
    return(t2-t1);
}
//Deuxième phase : il s'agit de revenir en arrière et de voir combien des dernières lignes utilisées
//sont nécessaires pour avoir un résultat concluant
void instructionsCentralesRetour(long taille,long* t[], long i,long imax)
{
    long k=0, j=0;
    srand(time(NULL));

    for(k=i; k < imax; k++)
    {
        for(j=0;j<taille;j++)
        {
                long piment = (rand()% (5*taille));
                t[k][j] = k*(j+32)-j*piment/(k+2);
        }
    }
    for(k=i;k<imax;k++)
    {
        tri(t[k],taille);
    }
}
long dureeRetour(long taille, long* t[], int* a, long i,long imax)
{
    unsigned long begin;
    unsigned long end;
    (*a)=0;
    begin= rdtsc();
    (*a)=1;
    end= rdtsc();
    long t1=end-begin;
    instructionsCentralesRetour(taille,t,i,imax);
    begin=rdtsc();
    (*a)=2;
    end = rdtsc();
    long t2=end-begin;
    return(t2-t1);
}
long chercheEmplacement(int* a,int* echec)
{
    long i=0, tps=0,j=0, ok=0, t1=0, t2=0,imax=0;
    long* t[1024]={NULL};
    long res[10]={0};
    for(i=0;i<1024;i++)
    {
        t[i]=malloc(sizeof(long)*TAILLE);
    }
    i=0;
    while(!ok && i<1024)
    {
        for(j=0;j<10;j++)
        {
            res[j]=duree(TAILLE,t,a,i);
        }
        tri(res,10);
        tps=res[5];

        if(tps>SIGNIFICATIF)
        {
            ok=1;
        }
        i++;
    }
    if(i==1024){
        (*echec)=1;
    }
    else{
        (*echec)=0;
        imax=i-1;
        i=0;
        while(!ok && i<=imax)
        {
            for(j=0;j<3;j++)
            {
                res[j]=dureeRetour(TAILLE,t,a,i,imax);
            }
            tri(res,3);
            tps=res[2];

            if(tps<SIGNIFICATIF)
            {
                ok=1;
            }
            i++;
        }

    }
    return(imax-i);
}
long unEssai(int* echec)
{
    srand(time(NULL));
    int a=rand()%10;
    return(chercheEmplacement(&a,echec));
}




