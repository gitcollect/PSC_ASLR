#include "stats.h"
#include <time.h>
#include "constantes.h"

//la première partie de l'algorithme : on cherche un bloc de lignes consécutives qui permet d'éjecter la variable du cache


//la fonction de mesure du temps - renvoie l'heure système en battements d'horloge (sauf erreur de ma part... Guillaume ?)
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

//**********************************Première phase********************************************************
//Il s'agit de déterminer un bloc de lignes de la forme "ligne 0 jusqu'à ligne i" qui contienne des variables
//capables d'éjecter la variable mystère du cache


//la fonction la plus importante en quelque sorte, à laquelle des améliorations doivent encore pouvoir être apportées
//elle prend en paramètre la taille d'une ligne du tableau, un tableau à double entrée et l'indice de la dernière
//ligne à considérer.
//cette fonction fait un peu n'importe quoi, en faisant appel aux emplacements mémoires des lignes du tableau jusqu'à l'indice i
//l'objectif est bien sûr de chasser la variable mystère du cache
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

//le bloc de base : on se donne un pointeur sur la variable mystère (a), le tableau avec la taille de ses lignes, et la dernière
//ligne à considérer
//on effectue alors une mesure de durée d'accès à a juste après y avoir accédé et juste après avoir exécuté les instructions
//centrales (qui utilisent les i premières lignes du tableau), et on rend la différence entre ces deux temps
long duree(long taille, long* t[], int* a, long i)
{
    unsigned long begin;
    unsigned long end;
    //première mesure, juste après un accès
    (*a)=0;
    begin= rdtsc();
    (*a)=1;
    end= rdtsc();
    long t1=end-begin;
    //deuxième mesure, avec instructions centrales
    instructionsCentrales(taille,t,i);
    begin=rdtsc();
    (*a)=2;
    end = rdtsc();
    long t2=end-begin;
    //on rend la différence des deux
    return(t2-t1);
}

//***************************************Deuxième phase**************************************************************
//le principe est le même que pour la première phase, mais on veut réduire le bloc obtenu à un bloc de la forme
//"ligne j jusqu'à ligne i"  où i est l'indice déterminé dans la première phase


//les nouvelles instructions centrales.
//même principe que dans la première phase mais désormais on se donne la première ligne (i) et la dernière (imax)
void instructionsCentralesRetour(long taille,long* t[], long i,long imax)
{
    long k=0, j=0;
    srand(time(NULL));

    for(k=i; k <= imax; k++)//k<=imax ?
    {
        for(j=0;j<taille;j++)
        {
                long piment = (rand()% (5*taille));
                t[k][j] = k*(j+32)-j*piment/(k+2);
        }
    }
    for(k=i;k<=imax;k++)     //k<=imax ?
    {
        tri(t[k],taille);
    }
}

//même principe que dans la première phase, mais désormais on se donne la première ligne (i) et la dernière (imax)
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

//la fonction chargée des deux premières phases
//lavariable mystère et le tableau t lui sont donnés en paramètres
//on rajoute un pointeur vers échec, qu'on modifie si les résultats à l'ici de cette phase sont d'ores et déjà insatisfaisants
//les bornes i et j définies dans les 2 premières phases sont rentrées dans le tableau bornes
void chercheEmplacement(int* a,int* echec, long* t[],long bornes[])
{
    long i=0, tps=0,j=0, ok=0, t1=0, t2=0,imax=0;
    long res[10]={0};
    i=0;

    //première phase
    //tant qu'on n'a pas des résultats satisfaisants et qu'on a pas utilisé toutes les lignes du tableau
    while(!ok && i<1024)
    {
        //on effectue 10 tests avec la fonction durée, utilisées sur les i premières lignes du taleau
        for(j=0;j<10;j++)
        {
            res[j]=duree(TAILLE,t,a,i);
        }
        //on regarde si au moins la moitié des résultats sont concluants (éjection du cache de la variable mystère,
        //mise en évidence par une différence de temps importante (et positive) dans la fonction durée
        tri(res,10);
        tps=res[5];

        if(tps>SIGNIFICATIF)
        {
            ok=1;//quand c'est le cas (résultats concluants), on décrète qu'on a trouvé une borne supérieure satisfaisante
        }
        i++;
    }
    //si on a parcouru tout le tableau en n'obtenant toujours pas de résultat, on a un échec et on le signale au main
    // en modifiant la valeur de echec
    if(i==1024){
        (*echec)=1;
    }

    //sinon, on peut passer à la deuxième phase
    //même principe en gros
    else{
        //on signale qu'il n'y a pas eu échec
        (*echec)=0;
        imax=i-1;//la boucle précédente s'est terminée par i++ ...
        i=0;
        ok=0;
        //tant qu'on a toujours des résultats satisfaisants et su'on ne s'est pas ramené à une seule ligne
        while(!ok && i<=imax)
        {
            //on n'effectue plus que trois tests (efficacité), en regardant si les 3 sont concluants
            //bien sûr, on fait appel à durée retour car on a déjà déterminée la dernière ligne du bloc, c'est la première que l'on
            //cherche désormais
            for(j=0;j<3;j++)
            {
                res[j]=dureeRetour(TAILLE,t,a,i,imax);
            }
            tri(res,3);
            tps=res[2];

            //quand les résultats ne sont plus significatifs, on a trouvé la première ligne du bloc
            if(tps<SIGNIFICATIF)
            {
                ok=1;
            }
            i++;
        }

    }
    //on prend note dans bornes des résultats obtenus : les limites du bloc de lignes pertinent
    bornes[0]=i;
    bornes[1]=imax;
}

//la fonction principale, appelée directement par le main
//effectue l'encemble du test et renvoie un code indiquant :
// 0 : test concluant avec résultat contraire aux attentes
//1 : test concluant, résultat attendu
//2 : problème dans les dernières phases
long unEssai(int* echec)
{
    long i;
    //création du tableau à double entrée
    long* t[1024]={NULL};
    for(i=0;i<1024;i++)
    {
        t[i]=malloc(sizeof(long)*TAILLE);
    }

    //création de la variable mystère
    srand(time(NULL));
    int a=rand()%10;
    //création du tableau destiné à contenir les bornes du bloc de lignes déterminé dans les 2 premières phases
    long bornes[2];
    //détermination de ces bornes
    //echec est modifié pour indiqué s'il y a eu échec dès la première phase ou non
    chercheEmplacement(&a,echec,t,bornes);
    //printf("fin cherche emplacement\n");

    //s'il n'y a pas eu échec, on peut passer à la suite de l'algorithme
    if(!(*echec))
    {
        //printf("pas echec");

        //création d'un tableau destiné à contenir l'ensemble de lignes considéré
        //les lignes sont renumérotées à partir de 0 (au lieu de bornes[0]
        //etat[i] contient 1 si la ligne i fait partie du sous-ensemble considéré, 0 sinon
        int etat[bornes[1]-bornes[0]+1];

        //fin de l'algorithme : détermination d'un sous-ensemble de lignes aussi bon que possible (en un certain sens)
        //le tableau etat contient alors une représentation de cet état
        crible(bornes[0],bornes[1],t,TAILLE,&a,etat);//cf crible.c

        //calcul du nombre de lignes contenues dans la solution
        long nbTrue =0, i=0;
        for(i=0;i<bornes[1]-bornes[0]+1;i++)
        {
            if(etat[i])
            {
                nbTrue++;
            }
        }

        //printf("fin crible");

        //afichage du résultat (nb de lignes et ensemble de lignes retenu)
        impressionResultat(bornes[0],bornes[1],etat,nbTrue);

        //on renvoie 0 si la ligne d'indice bornes[1] a été retenue, 0 sinon
        return(etat[bornes[1]-bornes[0]]);
    }
    //s'il y a eu échec, on renvoie 2
    else
    {
        return(2);
    }


}




