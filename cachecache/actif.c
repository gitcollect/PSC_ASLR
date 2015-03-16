#include "stats.h"
#include <time.h>
#include "constantes.h"

//la premi�re partie de l'algorithme : on cherche un bloc de lignes cons�cutives qui permet d'�jecter la variable du cache


//la fonction de mesure du temps - renvoie l'heure syst�me en battements d'horloge (sauf erreur de ma part... Guillaume ?)
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
        return( lo | (hi << 32) ); //Reponse sur 64 bits stock�e dans 2 registres, � reconstituer
}
#endif

//**********************************Premi�re phase********************************************************
//Il s'agit de d�terminer un bloc de lignes de la forme "ligne 0 jusqu'� ligne i" qui contienne des variables
//capables d'�jecter la variable myst�re du cache


//la fonction la plus importante en quelque sorte, � laquelle des am�liorations doivent encore pouvoir �tre apport�es
//elle prend en param�tre la taille d'une ligne du tableau, un tableau � double entr�e et l'indice de la derni�re
//ligne � consid�rer.
//cette fonction fait un peu n'importe quoi, en faisant appel aux emplacements m�moires des lignes du tableau jusqu'� l'indice i
//l'objectif est bien s�r de chasser la variable myst�re du cache
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

//le bloc de base : on se donne un pointeur sur la variable myst�re (a), le tableau avec la taille de ses lignes, et la derni�re
//ligne � consid�rer
//on effectue alors une mesure de dur�e d'acc�s � a juste apr�s y avoir acc�d� et juste apr�s avoir ex�cut� les instructions
//centrales (qui utilisent les i premi�res lignes du tableau), et on rend la diff�rence entre ces deux temps
long duree(long taille, long* t[], int* a, long i)
{
    unsigned long begin;
    unsigned long end;
    //premi�re mesure, juste apr�s un acc�s
    (*a)=0;
    begin= rdtsc();
    (*a)=1;
    end= rdtsc();
    long t1=end-begin;
    //deuxi�me mesure, avec instructions centrales
    instructionsCentrales(taille,t,i);
    begin=rdtsc();
    (*a)=2;
    end = rdtsc();
    long t2=end-begin;
    //on rend la diff�rence des deux
    return(t2-t1);
}

//***************************************Deuxi�me phase**************************************************************
//le principe est le m�me que pour la premi�re phase, mais on veut r�duire le bloc obtenu � un bloc de la forme
//"ligne j jusqu'� ligne i"  o� i est l'indice d�termin� dans la premi�re phase


//les nouvelles instructions centrales.
//m�me principe que dans la premi�re phase mais d�sormais on se donne la premi�re ligne (i) et la derni�re (imax)
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

//m�me principe que dans la premi�re phase, mais d�sormais on se donne la premi�re ligne (i) et la derni�re (imax)
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

//la fonction charg�e des deux premi�res phases
//lavariable myst�re et le tableau t lui sont donn�s en param�tres
//on rajoute un pointeur vers �chec, qu'on modifie si les r�sultats � l'ici de cette phase sont d'ores et d�j� insatisfaisants
//les bornes i et j d�finies dans les 2 premi�res phases sont rentr�es dans le tableau bornes
void chercheEmplacement(int* a,int* echec, long* t[],long bornes[])
{
    long i=0, tps=0,j=0, ok=0, t1=0, t2=0,imax=0;
    long res[10]={0};
    i=0;

    //premi�re phase
    //tant qu'on n'a pas des r�sultats satisfaisants et qu'on a pas utilis� toutes les lignes du tableau
    while(!ok && i<1024)
    {
        //on effectue 10 tests avec la fonction dur�e, utilis�es sur les i premi�res lignes du taleau
        for(j=0;j<10;j++)
        {
            res[j]=duree(TAILLE,t,a,i);
        }
        //on regarde si au moins la moiti� des r�sultats sont concluants (�jection du cache de la variable myst�re,
        //mise en �vidence par une diff�rence de temps importante (et positive) dans la fonction dur�e
        tri(res,10);
        tps=res[5];

        if(tps>SIGNIFICATIF)
        {
            ok=1;//quand c'est le cas (r�sultats concluants), on d�cr�te qu'on a trouv� une borne sup�rieure satisfaisante
        }
        i++;
    }
    //si on a parcouru tout le tableau en n'obtenant toujours pas de r�sultat, on a un �chec et on le signale au main
    // en modifiant la valeur de echec
    if(i==1024){
        (*echec)=1;
    }

    //sinon, on peut passer � la deuxi�me phase
    //m�me principe en gros
    else{
        //on signale qu'il n'y a pas eu �chec
        (*echec)=0;
        imax=i-1;//la boucle pr�c�dente s'est termin�e par i++ ...
        i=0;
        ok=0;
        //tant qu'on a toujours des r�sultats satisfaisants et su'on ne s'est pas ramen� � une seule ligne
        while(!ok && i<=imax)
        {
            //on n'effectue plus que trois tests (efficacit�), en regardant si les 3 sont concluants
            //bien s�r, on fait appel � dur�e retour car on a d�j� d�termin�e la derni�re ligne du bloc, c'est la premi�re que l'on
            //cherche d�sormais
            for(j=0;j<3;j++)
            {
                res[j]=dureeRetour(TAILLE,t,a,i,imax);
            }
            tri(res,3);
            tps=res[2];

            //quand les r�sultats ne sont plus significatifs, on a trouv� la premi�re ligne du bloc
            if(tps<SIGNIFICATIF)
            {
                ok=1;
            }
            i++;
        }

    }
    //on prend note dans bornes des r�sultats obtenus : les limites du bloc de lignes pertinent
    bornes[0]=i;
    bornes[1]=imax;
}

//la fonction principale, appel�e directement par le main
//effectue l'encemble du test et renvoie un code indiquant :
// 0 : test concluant avec r�sultat contraire aux attentes
//1 : test concluant, r�sultat attendu
//2 : probl�me dans les derni�res phases
long unEssai(int* echec)
{
    long i;
    //cr�ation du tableau � double entr�e
    long* t[1024]={NULL};
    for(i=0;i<1024;i++)
    {
        t[i]=malloc(sizeof(long)*TAILLE);
    }

    //cr�ation de la variable myst�re
    srand(time(NULL));
    int a=rand()%10;
    //cr�ation du tableau destin� � contenir les bornes du bloc de lignes d�termin� dans les 2 premi�res phases
    long bornes[2];
    //d�termination de ces bornes
    //echec est modifi� pour indiqu� s'il y a eu �chec d�s la premi�re phase ou non
    chercheEmplacement(&a,echec,t,bornes);
    //printf("fin cherche emplacement\n");

    //s'il n'y a pas eu �chec, on peut passer � la suite de l'algorithme
    if(!(*echec))
    {
        //printf("pas echec");

        //cr�ation d'un tableau destin� � contenir l'ensemble de lignes consid�r�
        //les lignes sont renum�rot�es � partir de 0 (au lieu de bornes[0]
        //etat[i] contient 1 si la ligne i fait partie du sous-ensemble consid�r�, 0 sinon
        int etat[bornes[1]-bornes[0]+1];

        //fin de l'algorithme : d�termination d'un sous-ensemble de lignes aussi bon que possible (en un certain sens)
        //le tableau etat contient alors une repr�sentation de cet �tat
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

        //afichage du r�sultat (nb de lignes et ensemble de lignes retenu)
        impressionResultat(bornes[0],bornes[1],etat,nbTrue);

        //on renvoie 0 si la ligne d'indice bornes[1] a �t� retenue, 0 sinon
        return(etat[bornes[1]-bornes[0]]);
    }
    //s'il y a eu �chec, on renvoie 2
    else
    {
        return(2);
    }


}




