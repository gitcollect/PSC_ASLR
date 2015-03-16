long duree(long taille,long* t[], int* a, long i);
unsigned long rdtsc();
void instructionsCentrales(long taille,long* t[], long i);
long duree(long taille, long* t[], int* a, long i);
void chercheEmplacement(int* a,int* echec,long* t[],long bornes[]);
long unEssai(int* echec);
//la deuxième phase
void instructionsCentralesRetour(long taille,long* t[], long i,long imax);
long dureeRetour(long taille, long* t[], int* a, long i,long imax);
