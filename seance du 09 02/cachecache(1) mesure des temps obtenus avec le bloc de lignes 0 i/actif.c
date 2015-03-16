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
    /*int* t[1024]={NULL};
    for(i=0;i<1024;i++)
    {
        t[i]=malloc(sizeof(int)*taille);
    }*/
    for(k=0; k < i; k++)
    {
        for(j=0;j<taille;j++)
        {
                long piment = (rand()% (5*taille));
                //printf(" piment : %d", piment);
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
    /*begin=rdtsc();
    (*a)=3;
    end = rdtsc();
    long t3=end-begin;*/
    //t[0]=t1;
    //t[1]=t2;
    //t[2]=t3;
//   if(t2-t1>SIGNIFICATIF)
//    {
//        printf("t1 : %ld    t2 : %ld\n",t1, t2);
//    }
    //printf("t1 : %ld    t2 : %ld\n",t1, t2);
    return(t2-t1);
}

long chercheEmplacement(int* a,int* echec)
{
    long i=0, tps=0,j=0, ok=0, t1=0, t2=0;
    int* t[1024]={NULL};
    long res[10]={0};
    for(i=0;i<1024;i++)
    {
        t[i]=malloc(sizeof(int)*TAILLE);
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
//        tps=duree(TAILLE,t,a,i);
//        if(tps>SIGNIFICATIF)
//        {
//            t1=tps;
//            tps=duree(TAILLE,t,a,i);
//            if(tps>SIGNIFICATIF)
//            {
//                t2=tps;
//               tps=duree(TAILLE,t,a,i);
//               if(tps>SIGNIFICATIF)
//               {
//                    ok=1;
//                    printf("temps 1 : %ld\ntemps 2 ; %ld\ntemps 3 : %ld\n", t1, t2,tps);
//               }
//            }
//        }
        if(tps>SIGNIFICATIF)
        {
            ok=1;
            //printf("temps 1 : %ld\ntemps 2 ; %ld\ntemps 3 : %ld\n", t1, t2,tps);
        }
        i++;
    }
    if(i==1024){
        (*echec)=1;
    }
    else{
        (*echec)=0;
    }
    //printf("temps obtenu : %ld\niteration : %ld\n", tps,i-1);
    //printf("deuxieme essai\n");
    //printf("%ld\n",i);
    tps=duree(TAILLE,t,a,i);
    //printf("temps obtenu : %ld\n\n", tps,i-1);
    return(tps);
}
long unEssai(int* echec)
{
    srand(time(NULL));
    int a=rand()%10;
    return(chercheEmplacement(&a,echec));
}

