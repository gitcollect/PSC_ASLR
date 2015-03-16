#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
#include "actif.h"
#include "constantes.h"


int main()
{
    /*long tab[NBITERATIONS]={0};
    long i;long t[3]={0};
    FILE* fichier=NULL;
    fichier=fopen("test.csv","w");
    if(fichier!=NULL)
    {
        for(i=0;i<NBITERATIONS;i++)
        {
            duree(TAILLE,t);
            fprintf(fichier,"%ld;%ld;%ld\n",t[0],t[1],t[2]);
            tab[i]=t[1]-t[0];
        }
        synthese(tab,NBITERATIONS,SEUIL_INF,SEUIL_SUP);
        fclose(fichier);
    }*/
    int i=0;long j=0;long echec=0;long k=0; long nbreussites=0;
    long tab[ESSAIS]={0};
    for(i=0;i<ESSAIS;i++)
    {
        printf("%i\n",i);
        j=unEssai(&echec);
        if(!echec){
            tab[k]=j;
            nbreussites++;
            k++;
        }
    }
    printf("Nombre d'essais : %ld\nNombre de reussites : %ld\n",ESSAIS,nbreussites);
    synthese(tab,nbreussites,30,100);
    //printf("Nombre d'essais satisfaisants sur 50 : %d", res);
    return 0;
}

