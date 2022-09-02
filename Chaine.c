#include "Chaine.h"

/* 1.1 */
Chaines *lectureChaines(FILE *f) {
	int gamma = 0;
    int nbChain = 0;
    int num = 0;
    int nbPoints = 0;
	double x, y;

  	Chaines *chaines = (Chaines*) malloc(sizeof(Chaines));
  	assert(chaines);
	chaines->gamma = 0;
	chaines->nbChaines = 0;
	chaines->chaines = NULL;

	assert(fscanf(f, " %*s %d", &nbChain) == 1);
    chaines->nbChaines = nbChain;
	assert(fscanf(f, " %*s %d", &gamma) == 1);
	chaines->gamma = gamma;

	CellChaine *headChaine = NULL; 
	for (int i = 0; i < nbChain; i++) {
		CellChaine *cc = (CellChaine*) malloc(sizeof(CellChaine));
		assert(cc);
		assert(fscanf(f, " %d %d", &num, &nbPoints) == 2);
		cc->numero = num;

		CellPoint *headPoint = NULL;
		for (int i = 0; i < nbPoints; i++) {
			CellPoint *cp = (CellPoint*) malloc(sizeof(CellPoint));
			assert(cp);
			assert(fscanf(f, " %lf %lf", &x, &y) == 2);
			cp->x = x;
			cp->y = y;
			cp->suiv = headPoint;
			headPoint = cp;
		}
		cc->points = headPoint;
		cc->suiv = headChaine;
		headChaine = cc;
	}
	chaines->chaines = headChaine;
    return chaines;
}

int len(CellPoint *points) {
	int res = 0;
	while (points != NULL) {
		res++;
		points = points->suiv;
	}
	return res;
}

/* 1.2 */
void ecrireChaines(Chaines *C, FILE *f) {
    assert(C);
    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n\n", C->gamma);

	CellChaine *cc = C->chaines;
	while (cc != NULL) {
		fprintf(f, "%d %d", cc->numero, len(cc->points));
		CellPoint *cp = cc->points;
		while (cp != NULL) {
			fprintf(f, " %.2lf %.2lf", cp->x, cp->y);
			cp = cp->suiv;
		}
		fprintf(f, "\n");
		cc = cc->suiv;
	}
}

/* 1.4 */
double longueurChaine(CellChaine *c) {
	double res = 0;
	if (c == NULL || c->points == NULL)
		return res;

	CellPoint *pA = c->points;
	CellPoint *pB = pA->suiv;
	if (pB == NULL)
		return res;
	while (pB != NULL) {
		double dx = pB->x - pA->x;
		double dy = pB->y - pA->y;
		double dist = sqrt(dx*dx + dy*dy);
		res += dist;
		pA = pA->suiv;
		pB = pB->suiv;
	}
	return res;
}

/* 1.4 */
double longueurTotale(Chaines *C) {
	double res = 0;
	if (C == NULL || C->nbChaines == 0)
		return res;

	CellChaine *cc = C->chaines;
	while (cc != NULL) {
		res += longueurChaine(cc);
		cc = cc->suiv;
	}
	return res;
}

/* 1.5 */
int comptePointsTotal(Chaines *C) {
    int res = 0;
    if (C == NULL || C->nbChaines == 0)
        return res;

    CellChaine *chaine = C->chaines;
    while (chaine != NULL) {
        res += len(chaine->points);
		chaine = chaine->suiv;
	}
    return res;
}

/* 1.3 */
void afficheChainesSVG(Chaines *C, char* nomInstance) {
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
        precx=pcour->x;
        precy=pcour->y;
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

void libererPoints(CellPoint *points) {
    if (points) {
        CellPoint *tmp;
        CellPoint *curr = points;
        while (curr) {
            tmp = curr;
            curr = curr->suiv;
            free(tmp);
        }
    }
}

void libererChaines(Chaines *chaines) {
    if (chaines) {
        CellChaine *tmp;
        CellChaine *curr = chaines->chaines;
        while (curr) {
            tmp = curr;
            curr = curr->suiv;
            libererPoints(tmp->points);
            free(tmp);
        }
        free(chaines);
    }
}
