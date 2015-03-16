#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
#include "actif.h"
#include "constantes.h"

//�videmment, change d'une version � une autre.
//actuellement : on effectue un certain nombre de fois l'exp�rience suivante : on se donne une variable enti�re,
//on essaye d'en apprendre plus sur son emplacement � l'aide d'un tableau fix�
//le test est d�clar� en �chec si la zone du tableau identifi�e par le test ne fournit pas un r�sultat satisfaisant
//lors d'un dernier essai
//sinon, on afiche l'ensemble des lignes retenues. Le r�sultat est d�clar� coh�rent quand la derni�re ligne retenue par la
//premi�re partie de l'algorithme figure dans cet ensemble
int main()
{
    int i=0;long j=0;long echec=0; long nbreussites=0;
    long nbCoherents=0;
    int bigProblem=0;
    //ESSAIS exp�riences
    for(i=0;i<ESSAIS;i++)
    {
        //on effectue le test
        //l'affichege des r�sultats est g�r� par les fonctions constituant le test
        //�chec est modifi�e � true si le test est un �chec (cf plus haut)
        j=unEssai(&echec);
        //actualisation des statistiques globales, pour l'ensemble des tests :
        if(!echec){
            //actualisation du nombre de tests men�s � terme
            nbreussites++;
            //actualisation du nombre de r�sultats coh�rents
            //j prend 3 valeurs : 0=r�sultat incoh�rent, 1= r�sultat coh�rent 2=probl�me dans les derni�res phases de l'algorithme
            if(j==1)
            {
                nbCoherents++;
            }
            //pour le d�buggage : s'il y a eu un probl�me en fin d'algorithme on en prend note
            else if(j==2)
            {
                bigProblem=1;
            }
        }
    }
    //affichage des statistiques globales sur l'ensemble des tests
    printf("Nombre d'essais : %ld\nNombre de reussites : %ld\n",ESSAIS,nbreussites);
    //signalement du probl�me mentionn� ci-dessus si besoin
    if(bigProblem)
    {
        printf("BigProblem : rendu d'une valeur d'echec sans modification de echec");
    }
    //fin de l'affichage des statistiques globales sur l'ensemble des tests
    printf("Nombre de resultats coherents : %ld",nbCoherents);
    return 0;
}
