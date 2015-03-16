#include <stdio.h>
#include <stdlib.h>

//Ce fichier comprend les fonctions permettant de traiter sommairement les données d'un tableau de long-la fonction
//synthese en est le point culminant, elle affiche un récapitulatif des données du tableau - éventuelement on pourra
//modifier cette fonction pour qu'elle n'affiche pas en console mais écrive dans un fichier

long compare (void const *a, void const *b)
{
    return ( *(long*)a - *(long*)b );
}

void tri(long tab[], long length)
{
    qsort(tab,length,sizeof(long),compare);
}

//les fonctions suivantes supposent le tableau trié
long nbNegatifs(long tab[], long length)
{
    long i=0;
    while((i<length) && (tab[i]<0))
    {
        //printf("%ld ",i);
        i++;
    }
    //printf("%ld ",i+1);
    return (i);
}

long nbPositifs(long tab[], long length)
{
    return (length-nbNegatifs(tab,length));
}

long quartile(long tab[],long length,long q)
{
    if (q>length)
    {
        printf("quartile d'ordre superieur a 1");
        return(-1);
    }
    else if(q<1)
    {
        printf("quartile d'ordre inferieur a 0");
        return (-1);
    }
    else
    {
        return(tab[q-1]);
    }
}

//q est cette fois un nombre de %
long quartileRelatif(long tab[],long length,long q)
{
    long n=q*length;
    n=n/100;
    if(n==0)
    {
        n=1;
    }
    return(quartile(tab,length,n));
}

long moyenne(long tab[],long length)
{
    long i=0, res=0;
    for(i=0;i<length;i++)
    {
        res=res+tab[i];
    }
    return(res/length);
}
long moyennePositive(long tab[],long length)
{
    long i=length-1, res=0;
    if(tab[i]<0)
    {
        return -1;
    }
    else
    {
        while(i>=0 && tab[i]>=0)
        {
            res=tab[i]+res;
            i--;
        }
        return(res/(length-i));
    }
}

long moyenneNegative(long tab[],long length)
{
    long i=0, res=0;
    if(tab[i]>=0)
    {
        return 1;
    }
    else
    {
        while(i<length && tab[i]<0)
        {
            res=tab[i]+res;
            i++;
        }
        return(res/i);
    }
}

long nbSup(long tab[],long length,long M)
{
    long i=length-1;
    while(i>=0 && tab[i]>=M)
    {
        i--;
    }
    return(length-1-i);
}
long nbInf(long tab[],long length,long m)
{
    long i=0;
    while(i<length && tab[i]<=m)
    {
        i++;
    }
    return(i);
}
//le récapitulatif
void synthese(long tab[],long length, long seuilInf, long seuilSup)
{
    tri(tab,length);
    printf("Moyenne globale : %ld\n",moyenne(tab,length));
    printf("Nombre de negatifs : %ld\n",nbNegatifs(tab,length));
    printf("Nombre de positifs : %ld\n",nbPositifs(tab,length));
    printf("Moyenne des negatifs : %ld\n",moyenneNegative(tab,length));
    printf("Moyenne des positifs : %ld\n",moyennePositive(tab,length));
    printf("Nombre de valeurs inferieures a %ld : %ld\n",seuilInf,nbInf(tab,length,seuilInf));
    printf("Nombre de valeurs superieures a %ld : %ld\n",seuilSup,nbSup(tab,length,seuilSup));
    printf("Valeur minimale : %ld\n",tab[0]);
    if(length>=1000)
    {
        printf("Centile 1 : %ld\n",quartileRelatif(tab,length,1));
    }
    long i;
    for(i=1;i<10;i++)
    {
        printf("Decile %ld : %ld\n",i,quartileRelatif(tab,length,10*i));
    }
    if(length>=1000)
    {
        printf("Centile 99 : %ld\n",quartileRelatif(tab,length,99));
    }
    printf("Valeur maximale : %ld\n",tab[length-1]);
}
