#ifndef CONSTANTE_H
#define CONSTANTE_H

//param�tres � modifier pour les tests
#define NBITERATIONS 1000       //sauf erreur je ne me sers plus actuellement de cette constante
#define SEUIL_INF -100          //seuil inferieur des differences de temps acceptables-inutile d�sormais je crois
#define SEUIL_SUP 100           //seuil au-dela duquel on considere les valeurs comme significatives-idem
#define TAILLE 10              //la taille de la deuxi�me dimension du tableau utilis� pour stocker les variables
                                //visant � remplir le cache
#define SIGNIFICATIF 150       //le seuil au-del� duquel la diff�rence de temps d'acc�s � une variable est jug�e significative
#define ESSAIS 10               //nb d'it�rations de l'algorithmes par le main
#define NBCRIBLE 10             //nombre d'�tapes dans la troisi�me phase
#define CHANCES_RETOUR 10       //� terme, devrait contenir en pourcent la probabilit� qu'on accorde pour faire des sauts
                                //dans la troisi�me phase
#endif
