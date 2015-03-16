#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
#include "actif.h"
#include "constantes.h"


int main()
{
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
    return 0;
}

