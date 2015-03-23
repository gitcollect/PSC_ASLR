#ifndef CONSTANTE_H
#define CONSTANTE_H

//paramètres à modifier pour les tests
#define NBITERATIONS 1000       //sauf erreur je ne me sers plus actuellement de cette constante
#define SEUIL_INF -100          //seuil inferieur des differences de temps acceptables-inutile désormais je crois
#define SEUIL_SUP 100           //seuil au-dela duquel on considere les valeurs comme significatives-idem
#define TAILLE 10              //la taille de la deuxième dimension du tableau utilisé pour stocker les variables
                                //visant à remplir le cache
#define SIGNIFICATIF 150       //le seuil au-delà duquel la différence de temps d'accès à une variable est jugée significative
#define ESSAIS 10               //nb d'itérations de l'algorithmes par le main
#define NBCRIBLE 10             //nombre d'étapes dans la troisième phase
#define CHANCES_RETOUR 10       //à terme, devrait contenir en pourcent la probabilité qu'on accorde pour faire des sauts
                                //dans la troisième phase
#endif
