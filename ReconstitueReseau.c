#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

/* 2.3 */ 
int main(int argc, char **argv) {

	if (argc != 3) {
		printf("il faut ecrire %s <fichier .cha> <1-3>\n", argv[0]);
		return 1;
	}

	int choix = atoi(argv[2]);
	if (choix < 1 || choix > 3) {
		printf("le deuxieme argument (choix) doit etre compris entre 1 et 3");
		return 1;
	}

	FILE *fi = fopen(argv[1], "r");
    assert(fi); 
	Chaines *chaines = lectureChaines(fi);
	fclose(fi);

    /* Liste chainee */ 
	if (choix == 1) {
		Reseau *r = reconstitueReseauListe(chaines);
		FILE *fo = fopen("newReseau.res", "w"); 
        assert(fo);
		ecrireReseau(r, fo);
		afficheReseauSVG(r, "Reseau");
		fclose(fo);
        /* tests */ 
        printf("Nb liaisons   : %d\n", nbLiaisons(r)); 
        printf("Nb commodites : %d\n", nbCommodites(r)); 
        libererReseau(r); 
	}

    /* Table H */ 
    else if (choix == 2) { 
        int M; 
        printf("Taille de la table de hachage: "); 
        scanf(" %d", &M); 
        Reseau *r = reconstitueReseauHachage(chaines, M); 
        FILE *fo = fopen("newHachage.res", "w"); 
        assert(fo);
		ecrireReseau(r, fo);
		afficheReseauSVG(r, "Hachage");
		fclose(fo);
        /* tests */ 
        printf("Nb liaisons   : %d\n", nbLiaisons(r)); 
        printf("Nb commodites : %d\n", nbCommodites(r)); 
        libererReseau(r);
    }

    /* Arbre Quat */ 
    else {
        Reseau *r = reconstitueReseauArbre(chaines);
		FILE *fo = fopen("newArbreQuat.res", "w"); 
        assert(fo);
		ecrireReseau(r, fo);
		afficheReseauSVG(r, "ArbreQuat");
		fclose(fo);
        /* tests */ 
        printf("Nb liaisons   : %d\n", nbLiaisons(r)); 
        printf("Nb commodites : %d\n", nbCommodites(r)); 
        libererReseau(r);
    }

    libererChaines(chaines); 
	return 0;
}

/*                           burma.cha
                          *** Choix 1 ***  
==525== HEAP SUMMARY:
==525==     in use at exit: 0 bytes in 0 blocks
==525==   total heap usage: 109 allocs, 109 frees, 17,168 bytes allocated
==525== 
==525== All heap blocks were freed -- no leaks are possible
==525== 
==525== For counts of detected and suppressed errors, rerun with: -v
==525== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
---------------------------------------------------------------------------------
                            *** Choix 2, M = 10 ***
==135== HEAP SUMMARY:
==135==     in use at exit: 0 bytes in 0 blocks
==135==   total heap usage: 124 allocs, 124 frees, 18,480 bytes allocated
==135== 
==135== All heap blocks were freed -- no leaks are possible
==135== 
==135== For counts of detected and suppressed errors, rerun with: -v
==135== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
---------------------------------------------------------------------------------
                            *** Choix 3 *** 
==397== HEAP SUMMARY:
==397==     in use at exit: 0 bytes in 0 blocks
==397==   total heap usage: 130 allocs, 130 frees, 18,680 bytes allocated
==397== 
==397== All heap blocks were freed -- no leaks are possible
==397== 
==397== For counts of detected and suppressed errors, rerun with: -v
==397== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/ 