#include "Chaine.h"

/* ==========           TESTS CHAINES           ==========*/ 

int main(int argc, char** argv) {
    FILE *fi;
    FILE *fo;
    Chaines *chaines;

	if (argc != 2) {
		printf("il faut ecrire %s <fichier .cha>\n", argv[0]); 
		return 1; 
	}

    fi = fopen(argv[1], "r");
    assert(fi); 
    chaines = lectureChaines(fi);
    fclose(fi);

	afficheChainesSVG(chaines, "Chaines"); 

	printf("Nombre de points : %d\n", comptePointsTotal(chaines)); 
	CellChaine *cc = chaines->chaines; 
	printf("Lg chaine num %d  : %.3lf\n", cc->numero, longueurChaine(cc)); 
	printf("Lg totale chaines: %.3lf\n", longueurTotale(chaines)); 

    fo = fopen("newChaines.cha", "w");
    assert(fo); 
	ecrireChaines(chaines, fo);
    fclose(fo);

    libererChaines(chaines); 

    return 0; 
}