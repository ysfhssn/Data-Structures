#include "Hachage.h"

/* 4.1 */ 
TableHachage* creer_TableHachage(int m) {
    TableHachage *tH = (TableHachage*) malloc(sizeof(TableHachage));
    assert(tH); 
	tH->nE = 0;
	tH->m = m;
	tH->T = (CellNoeud**) malloc(sizeof(CellNoeud*) * m);
    assert(tH->T); 
	for (int i = 0; i < m; i++)
		tH->T[i] = NULL;
	return tH;
}

/* 4.2 */ 
unsigned int fonctionClef(double x, double y) {
    return (unsigned int)(y + (x + y)*(x + y + 1)/2); 
}

/* 4.3 */ 
int fonctionHachage(unsigned int clef, int m) {
	return (int)(m * (clef*A - (unsigned int)(clef*A))); 
}

/* 4.4 */ 
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y) {
    assert(H);   
    int index = fonctionHachage(fonctionClef(x, y), H->m);  
    CellNoeud *cn = H->T[index]; 
    while (cn != NULL) {
        Noeud *nd = cn->nd; 
        if (compareNoeud_xy(nd, x, y))
            return nd; 
        cn = cn->suiv; 
    }
    /* Insertion Reseau */ 
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

    /* Insertion TableHachage */ 
    CellNoeud *cnd2 = (CellNoeud*) malloc(sizeof(CellNoeud));
    assert(cnd2);
    cnd2->nd = nd;
    cnd2->suiv = H->T[index];
    H->T[index] = cnd2;
    H->nE++; 

    return nd; 
}

/* 4.5 */ 
Reseau* reconstitueReseauHachage(Chaines *C, int M) {
    assert(C); 

	Reseau *res = (Reseau*) malloc(sizeof(Reseau));
	assert(res); 
	res->nbNoeuds = 0;
	res->gamma = C->gamma;
	res->noeuds = NULL;
	res->commodites = NULL;

    TableHachage *tH = creer_TableHachage(M); 

	Noeud *nd, *ndRec;
	CellNoeud *ndVoisin;

	CellChaine *chaine = C->chaines;
	CellPoint *p;

	while (chaine != NULL) {
		CellCommodite *comNew = (CellCommodite*) malloc(sizeof(CellCommodite));
		assert(comNew);
		p = chaine->points;
		nd = rechercheCreeNoeudHachage(res, tH, p->x, p->y);  
		comNew->extrA = nd;

		p = p->suiv;
		while (p != NULL) {
			ndRec = nd;
			nd = rechercheCreeNoeudHachage(res, tH, p->x, p->y);
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
    liberer_TableHachage(tH);  
	return res;
}

void liberer_TableHachage(TableHachage* tH) {
	if (!tH) return; 
    /* Attention les noeuds sont deja liberes dans libererReseau */
    for (int i = 0; i < tH->m; i++) {
        CellNoeud *cn = tH->T[i], *tmp;
        while (cn != NULL) {
            tmp = cn; 
            cn = cn->suiv;
            free(tmp);  
        } 
    } 
    free(tH->T); 
    free(tH); 
}