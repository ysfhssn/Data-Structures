#include "Reseau.h"

int compareNoeud_xy(Noeud *nd, double x, double y) {
	assert(nd);
	return nd->x == x && nd->y == y;
}

/* 2.1 */ 
Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {
	assert(R);
	CellNoeud *curr = R->noeuds;

	while (curr != NULL) {
		Noeud *nd = curr->nd; 
		if (compareNoeud_xy(nd, x, y))
			return nd;
		curr = curr->suiv;
	}

	Noeud *nd = (Noeud*) malloc(sizeof(Noeud));
	assert(nd);
	nd->x = x;
	nd->y = y;
	nd->num = R->nbNoeuds+1;
	nd->voisins = NULL;

	CellNoeud *cnd = (CellNoeud*) malloc(sizeof(CellNoeud));
	assert(cnd);
	cnd->nd = nd;
	cnd->suiv = R->noeuds;
	R->noeuds = cnd;
	R->nbNoeuds++;
	return nd;
}

/* 2.2 */ 
Reseau* reconstitueReseauListe(Chaines *C) {
    assert(C); 
	CellNoeud *ndVoisin;

	Reseau *res = (Reseau*) malloc(sizeof(Reseau));
	assert(res); 
	res->nbNoeuds = 0;
	res->gamma = C->gamma;
	res->noeuds = NULL;
	res->commodites = NULL;

	Noeud *nd, *ndRec;

	CellChaine *chaine = C->chaines;
	CellPoint *p;

	while (chaine != NULL) {
		CellCommodite *comNew = (CellCommodite*) malloc(sizeof(CellCommodite));
		assert(comNew);
		p = chaine->points;
		nd = rechercheCreeNoeudListe(res, p->x, p->y);
		comNew->extrA = nd;

		p = p->suiv;
		while (p != NULL) {
			ndRec = nd;
			nd = rechercheCreeNoeudListe(res, p->x, p->y);
			ndVoisin = nd->voisins;

			while (ndVoisin != NULL) {
				if (compareNoeud_xy(ndRec, ndVoisin->nd->x, ndVoisin->nd->y))
					break;
				ndVoisin = ndVoisin->suiv;
			}
			if (ndVoisin == NULL) {
				CellNoeud *ndNew = (CellNoeud*) malloc(sizeof(CellNoeud));
				assert(ndNew);
				ndNew->nd = nd;
				ndNew->suiv = ndRec->voisins;
				ndRec->voisins = ndNew;

				CellNoeud *ndNew2 = (CellNoeud*) malloc(sizeof(CellNoeud));
				assert(ndNew2);
				ndNew2->nd = ndRec;
				ndNew2->suiv = nd->voisins;
				nd->voisins = ndNew2;
			}

			if (p->suiv == NULL) {
				comNew->extrB = nd;
				comNew->suiv = res->commodites;
				res->commodites = comNew;
			}
			p = p->suiv;
		}
		chaine = chaine->suiv;
	}
	return res;
}

/* 3.1 */ 
int nbLiaisons(Reseau *R) {
    if (!R || !R->noeuds) return 0;  
	int nbLiaisons = 0;
	CellNoeud *cnd = R->noeuds, *voisins;
	int extrA;

	while (cnd != NULL) {
		extrA = cnd->nd->num;
		voisins = cnd->nd->voisins;
		while (voisins != NULL) {
			if (voisins->nd->num < extrA)
				nbLiaisons++;
			voisins = voisins->suiv;
		}
		cnd = cnd->suiv;
	}
	return nbLiaisons;
}

/* 3.1 */ 
int nbCommodites(Reseau *R) {
    if (!R || !R->commodites) return 0;
	int i = 0;
	CellCommodite *com = R->commodites;
	while (com != NULL) {
		i++;
		com = com->suiv;
	}
	return i;
}

/* 3.2 */ 
void ecrireReseau(Reseau *R, FILE *f){
    assert(R);
	fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
	fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
	fprintf(f, "NbCommodites: %d\n", nbCommodites(R));
	fprintf(f, "Gamma: %d\n\n", R->gamma);

	CellNoeud *cnd = R->noeuds;
	CellCommodite *com = R->commodites;
	Noeud *nd;

	/* Ecriture des Noeuds */
	while (cnd != NULL) {
		nd = cnd->nd;
		fprintf(f, "v %d %.2lf %.2lf\n", nd->num, nd->x, nd->y);
		cnd = cnd->suiv;
	}
	fprintf(f, "\n");

	/* Ecriture des Liaisons */
	cnd = R->noeuds;
	CellNoeud *voisins;
	int extrA, extrB;

	while (cnd != NULL) {
		extrA = cnd->nd->num;
		voisins = cnd->nd->voisins;
		while (voisins != NULL) {
			extrB = voisins->nd->num;
			if (extrA < extrB)
				fprintf(f, "l %d %d\n", extrA, extrB);
			voisins = voisins->suiv;
		}
		cnd = cnd->suiv;
	}
	fprintf(f, "\n");

	/* Ecriture des Commodites */
	while (com != NULL) {
		fprintf(f, "k %d %d\n", com->extrA->num, com->extrB->num);
		com = com->suiv;
	}
}

/* 3.3 */ 
void afficheReseauSVG(Reseau *R, char* nomInstance) {
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

void libererNoeud(Noeud* noeud) {
    if (noeud) {
        CellNoeud *tmp; 
        CellNoeud *curr = noeud->voisins; 
        while (curr) {
            tmp = curr;
            curr = curr->suiv;
            free(tmp);  
        }
        free(noeud);
    }
}

void libererNoeuds(CellNoeud* noeuds) {
    CellNoeud *tmp; 
    CellNoeud *curr = noeuds; 
    while (curr) {
        tmp = curr;
        curr = curr->suiv;
        libererNoeud(tmp->nd); 
        free(tmp);  
    }
}

void libererCommodites(CellCommodite *coms) {
    CellCommodite *tmp; 
    CellCommodite *curr = coms; 
    while (curr) {
        tmp = curr; 
        curr = curr->suiv;  
        free(tmp);  
    }
}

void libererReseau(Reseau *res) {
    if (res) {
        libererNoeuds(res->noeuds); 
        libererCommodites(res->commodites); 
        free(res);
    }
}