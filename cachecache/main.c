#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
#include "actif.h"
#include "constantes.h"

//évidemment, change d'une version à une autre.
//actuellement : on effectue un certain nombre de fois l'expérience suivante : on se donne une variable entière,
//on essaye d'en apprendre plus sur son emplacement à l'aide d'un tableau fixé
//le test est déclaré en échec si la zone du tableau identifiée par le test ne fournit pas un résultat satisfaisant
//lors d'un dernier essai
//sinon, on afiche l'ensemble des lignes retenues. Le résultat est déclaré cohérent quand la dernière ligne retenue par la
//première partie de l'algorithme figure dans cet ensemble
int main()
{
    int i=0;long j=0;long echec=0; long nbreussites=0;
    long nbCoherents=0;
    int bigProblem=0;
    //ESSAIS expériences
    for(i=0;i<ESSAIS;i++)
    {
        //on effectue le test
        //l'affichege des résultats est géré par les fonctions constituant le test
        //échec est modifiée à true si le test est un échec (cf plus haut)
        j=unEssai(&echec);
        //actualisation des statistiques globales, pour l'ensemble des tests :
        if(!echec){
            //actualisation du nombre de tests menés à terme
            nbreussites++;
            //actualisation du nombre de résultats cohérents
            //j prend 3 valeurs : 0=résultat incohérent, 1= résultat cohérent 2=problème dans les dernières phases de l'algorithme
            if(j==1)
            {
                nbCoherents++;
            }
            //pour le débuggage : s'il y a eu un problème en fin d'algorithme on en prend note
            else if(j==2)
            {
                bigProblem=1;
            }
        }
    }
    //affichage des statistiques globales sur l'ensemble des tests
    printf("Nombre d'essais : %ld\nNombre de reussites : %ld\n",ESSAIS,nbreussites);
    //signalement du problème mentionné ci-dessus si besoin
    if(bigProblem)
    {
        printf("BigProblem : rendu d'une valeur d'echec sans modification de echec");
    }
    //fin de l'affichage des statistiques globales sur l'ensemble des tests
    printf("Nombre de resultats coherents : %ld",nbCoherents);
    return 0;
}
