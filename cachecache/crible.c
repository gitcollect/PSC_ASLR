
#include <time.h>
#include "constantes.h"
#include "crible.h"

//******************************Troisième phase******************************************************
//L'idée : on va considérer le sous-ensemble de lignes déterminé en phases 1 et 2, et essayer d'en extraire un sous-ensemble
//qui donne des résultats satisfaisants et qui contienne aussi peu de lignes que possible
//on adopte une heuristique comme en INF 421 amphi 8

//un outil technique : on a besoin de déterminer (aléatoirement) l'indice de la prochaine ligne qu'on va ajouter/supprimer
//la fonction se charge de l'ajout/suppression, et renvoie l'indice de la ligne choisie
//attention, ici taille désigne le nombre de lignes dans la sélection
long randomIndice(long taille, int etat[],int ilFautSupprimer, long* nbTrue)
{
    //printf("debut randomIndice\n");
    //printf("taille : %ld\n",taille);

    //détermination d'un numéro de ligne au hasard
    long r=rand()%taille;//il faudra, peut-être mettre %(*nbTrue) pour que ce soit uniforme
    //on recherche le r-ième true dans le tableau
    long res=0;
    long mem=0;

    //on fait décoître r à chaque fois qu'on atteint un true, et on recommence jusqu'à ce que r vaille 0
    while(r>=0){
        //printf("%ld\n",r);
        //on note dans mem le numero de la dernière ligne où on a trouvé un true
        mem=res;
        //on va commencer la recherche du prochain true à la case suivante, modulo taille bien sûr
        res++;
        if(res>=taille)
        {
            res=0;
        }

        //on cherhce le prochain true si on cherche une ligne à supprimer, le prochain false sinon
        while(etat[res]!=ilFautSupprimer){
                //si on est revenu à mem c'est qu'il y a un problème
            if(mem==res)
            {
                printf("Bouclage sans fin\n");
                printf("Valeur de ilFautSupprimer : %d\n",ilFautSupprimer);
                printf("taille : %ld\n",taille);
                printf("nbTrue : %ld\n",(*nbTrue));
                clock();
                return(-1);
            }
            //printf("%ld\n",res);
            res++;
            if(res>=taille){
                res=0;
            }
        }
    r--;
    }
    //printf("sortie de boucle\n");
    //on actualise l'état et le nombre de true
    if(ilFautSupprimer)
    {
        etat[res]=0;
        (*nbTrue)--;
    }
    else
    {
       etat[res]=1;
       (*nbTrue)++;
    }
    //on renvoie le numéro de la ligne
    return(res);
}

//initialisation de la troisième phase : toutes les lignes sont dans l'état courant
//de plus, l'état optimal est identique pour l'instant
void initCrible(long min, long max, int etatOptimal[],int etatCourant[])
{
    int i=0;
    for(i=0;i<max-min+1;i++)
    {
        etatCourant[i]=1;
        etatOptimal[i]=1;
    }
}

//le coeur de cette troisième phase
//détermine un sous-ensemble de lignes aussi bon que possible en temps raisonnable, et le copie dans etatOptimal
//prend en entrée les deux bornes du bloc à considérer, le tableau, la taille des lignes, la variable mystère et
// l'emplacement où stocker l'état optimal
void crible(long min,long max, int* tab[],long taille, int* a, int etatOptimal[])
{
    //printf("debut crible\n");
    printf("min : %ld\nmax : %ld\n",min,max);

    //création d'un tableau pour stocker l'état courant du sous-ensemble de lignes
    //les lignes sont réindicées à partir de 0 au lieu de min
    int etatCourant[max-min+1];
    //initialisation de etatOptimal et de etatCourant
    initCrible(min,max,etatOptimal,etatCourant);
    //printf("fin initCrible\n");

    //quelques variables intermédiaires :
    long compteur=0;//compteur de boucle
    int ilFautSupprimer=1;//vaut 1 si on est en train de supprimer des lignes, 0 si on en rajoute
    long nbTrue=max-min+1;//nombre de lignes dans le sous-ensemble de lignes courant
    long bestNbTrue=nbTrue;//le nombre de lignes dans le meilleur sous-ensemble trouvé
    long longueur=nbTrue;//la longueur du bloc de lignes sur lequel on travaille (ne sera pas modifié ensuite)
    long i,n,indice;
    //on effectue NBCRIBLES ajouts/suppressions
    for(compteur=0;compteur<NBCRIBLE;compteur++)
    {
        //printf("compteur : %ld\n",compteur);
        //on choisit la prochaine ligne à ajouter/supprimer, on effectue l'ajout/suppression, et on actualise nbTrue
        indice=randomIndice(longueur,etatCourant,ilFautSupprimer,&nbTrue);
        //printf("fin randomIndice");
        //on regarde si le résultat obtenu est satisfaisant (variable chassée du cache)-ck plus bas
        n=uneIteration(taille,tab,a,etatCourant,min,max);
        //printf("n : %ld\n",n);

        //si le résultat est satisfaisant :
        if(n>SIGNIFICATIF)
        {
            ilFautSupprimer=1;//on va donc continuer de supprimer des lignes
            if(nbTrue==1)//sauf s'il ne reste qu'une ligne, auquel cas on s'arrête en affichant le numéro de la ligne
            {
                long j=0;
                while(etatCourant[j]==0)
                {
                    j++;
                }
                printf("Il ne reste qu'une ligne, la numero %ld",j);
                return;
            }
        }

        //si le résultat n'est pas satisfaisant
        else
        {
            ilFautSupprimer=0;//in va donc rajouter des lignes
            if(nbTrue<bestNbTrue-1) //si on a atteint un nouvel optimum en terme de nombre de lignes
            {
                copie(longueur,etatCourant,etatOptimal);//on actualise la solution optimale
                bestNbTrue=nbTrue+1;//onn actualise la taille de la solution optimale
                etatOptimal[indice]=1;//on n'oublie pas que la ligne qu'on vient de supprimer faisait partie de la solution optimale
            }
        }

        if(nbTrue==longueur)//si on est revenu à l'ensemble total, on doit supprimer des lignes
        {
            ilFautSupprimer=1;
        }
    }
}

//d'autres fonctions utilisées par le crible

//copie du tableau to dans le tableau td, de tailles taille
void copie(long taille,int to[], int td[])
{
    int i=0;
    for(i=0;i<taille;i++)
    {
        td[i]=to[i];
    }
}

//il faut adapter la fonction durée pour qu'elle ne travaile qu'avec le sous-ensemble de lignes voulus
//à part ça, même principe que dans les deux premières phases
long uneIteration(long taille, int* tab[],int* a, int etat[],long min, long max)
{
    unsigned long begin;
    unsigned long end;
    (*a)=0;
    begin= rdtsc();
    (*a)=1;
    end= rdtsc();
    long t1=end-begin;
    instructionsCentralesMelangees(taille,tab,etat,min,max);
    begin=rdtsc();
    (*a)=2;
    end = rdtsc();
    long t2=end-begin;
    return(t2-t1);
}

//il faut adapter la fonction instructionsCentrales pour qu'elle ne travaile qu'avec le sous-ensemble de lignes voulus
//à part ça, même principe que dans les deux premières phases
void instructionsCentralesMelangees(long taille,long* t[], int etatCourant[],long min, long max)
{
    long k=0, j=0;
    srand(time(NULL));

    for(k=min; k <=max; k++)
    {
        if(etatCourant[k-min])
        {
            for(j=0;j<taille;j++)
            {
                    long piment = (rand()% (5*taille));
                    t[k][j] = k*(j+32)-j*piment/(k+2);
            }
        }

    }
    for(k=min;k<=max;k++)
    {
        if(etatCourant[k-min])
        {
            tri(t[k],taille);
        }
    }
}

//affichage du bilan pour le test
void impressionResultat(long min, long max, int etat[],long nbTrue)
{
    printf("Etat optimal trouve :\n");
    printf("indice minimal de la recherche : %ld\n",min);
    printf("indice maximal de la recherche : %ld\n",max);
    printf("nombre d'etats conserves dans la solution optimale : %ld\n",nbTrue);
    printf("etats conserves dans la solution optimale :\n");
    long i=0;
    for(i=0;i<max-min+1;i++)
    {
        if(etat[i])
        {
            printf("%ld  ",i+min);
        }
    }
    printf("\nLe dernier indice etait dans la solution optimale : ");
    if(etat[max-min])
    {
        printf("oui\n");
    }
    else
    {
        printf("non\n");
    }
}
