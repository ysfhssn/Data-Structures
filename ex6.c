#include "Chaine.h"
#include "ArbreQuat.h"
#include "Hachage.h"
#include "Reseau.h"
#include <time.h>

/* 6.2 */ 
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
	Chaines *chaines = (Chaines*) malloc(sizeof(Chaines));
	assert(chaines);
	CellChaine *headChaine = NULL, *nextChaine;
	CellPoint *headPoint, *nextPoint;
	int i, p;

	chaines->gamma = 0;
	chaines->nbChaines = 0;
	chaines->chaines = NULL;

	for (i = 0; i < nbChaines; i++) {
		nextChaine = (CellChaine*) malloc(sizeof(CellChaine));
		assert(nextChaine);
		nextChaine->numero = i;
		headPoint = NULL;
		nextChaine->points = NULL;

		for (p = 0; p < nbPointsChaine; p++) {
			nextPoint = (CellPoint*) malloc(sizeof(CellPoint));
			assert(nextPoint);

			nextPoint->x = rand()%xmax;
			nextPoint->y = rand()%ymax;
			nextPoint->suiv = headPoint;
			headPoint = nextPoint;
		}
		nextChaine->points = headPoint;
		nextChaine->suiv = headChaine;
		headChaine = nextChaine;
		chaines->nbChaines++;
	}

	assert(nbChaines == chaines->nbChaines);
	chaines->chaines = headChaine;
	return chaines;
}

/* Ne pas oublier de commenter liberer_TableHachage/libererArbreQuat dans reconstitue pour avoir des comparaisons equitables */ 
int main(int argc, char **argv) {
	if (argc != 2) {
		printf("il faut ecrire %s <fichier .cha>\n", argv[0]);
		return 1;
	}

	srand(time(NULL));
	clock_t dbt, fin;
	double time1, time2, time3;
	Reseau *r;

	/*   6.1   */
	FILE *test = fopen("ex6_1.txt", "w");
	assert(test);
	FILE *fi = fopen(argv[1], "r");
	assert(fi);
	Chaines *chaines = lectureChaines(fi);
	fclose(fi);

	/* Liste chainee */
	dbt = clock();
	r = reconstitueReseauListe(chaines);
	fin = clock();
	time1 = ((double)(fin - dbt)) / CLOCKS_PER_SEC;
	libererReseau(r);

	/* Arbre Quat */
	dbt = clock();
	r = reconstitueReseauArbre(chaines);
	fin = clock();
	time2 = ((double)(fin - dbt)) / CLOCKS_PER_SEC;
	libererReseau(r);

	/* Table H */
	for (int i = 1; i <= 30; i++) {
		dbt = clock();
		r = reconstitueReseauHachage(chaines, i);
		fin = clock();
		time3 = ((double)(fin - dbt)) / CLOCKS_PER_SEC;
		fprintf(test, "%d %lf %lf %lf\n", i, time1, time2, time3);
		libererReseau(r);
	}

	fclose(test);
	libererChaines(chaines);



	/*   6.3   */ 
	/* Decommenter libererReseau/libererChaines si on ne veut pas de fuites memoires */ 
	FILE *test2 = fopen("ex6_3.txt", "w");
	assert(test2);
	Chaines *chaines2;
	int nbPointsChaine = 10, xmax = 5000, ymax = 5000;

	for (int n = 500; n < 5001; n+=500) {
		printf("%d\n", n); 
		chaines2 = generationAleatoire(n, nbPointsChaine, xmax, ymax);

		/* Liste chainee */
		dbt = clock();
		r = reconstitueReseauListe(chaines2);
		fin = clock();
		time1 = ((double)(fin - dbt)) / CLOCKS_PER_SEC;

		//libererReseau(r);

		/* Arbre Quat */
		dbt = clock();
		r = reconstitueReseauArbre(chaines2);
		fin = clock();
		time2 = ((double)(fin - dbt)) / CLOCKS_PER_SEC;

		//libererReseau(r);

		fprintf(test2, "%d %lf %lf", n, time1, time2);

		/* Table H */
		int nbPoints = n*nbPointsChaine;  
		for (int i = nbPoints/10; i < 3*nbPoints; i=2*i) {
			dbt = clock();
			r = reconstitueReseauHachage(chaines2, i);
			fin = clock();
			time3 = ((double)(fin - dbt)) / CLOCKS_PER_SEC;
			fprintf(test2, " %lf", time3);
			//libererReseau(r);
		}
		fprintf(test2, "\n");
		//libererChaines(chaines2);
	}

	fclose(test2);

	return 0;
}
