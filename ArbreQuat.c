#include "ArbreQuat.h"

/* 5.1 */ 
void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax) {
    if (!C || !C->chaines) return;
    *xmin = INFINITY; *ymin = INFINITY; 
    *xmax = -INFINITY; *ymax = -INFINITY; 

    CellChaine *cc = C->chaines; 
    while (cc) {
        CellPoint *point = cc->points;
        while (point != NULL) {
            double x = point->x; 
            double y = point->y; 
            if (x < *xmin) *xmin = x; 
            if (y < *ymin) *ymin = y; 
            if (x > *xmax) *xmax = x; 
            if (y > *ymax) *ymax = y; 
            point = point->suiv; 
        } 
        cc = cc->suiv; 
    }
}

/* 5.2 */ 
ArbreQuat *creerArbreQuat(double xc, double yc, double coteX, double coteY) {
    ArbreQuat *aq = (ArbreQuat*) malloc(sizeof(ArbreQuat)); 
    assert(aq); 
    aq->xc = xc; 
    aq->yc = yc; 
    aq->coteX = coteX; 
    aq->coteY = coteY;

    aq->noeud = NULL; 
    aq->no = NULL; aq->ne = NULL;
    aq->so = NULL; aq->se = NULL; 
    return aq; 
}

/* 5.3 */ 
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent) {
    assert(n); assert(a);

    /* Arbre vide */ 
    if (*a == NULL) { 
        //printf("Arbre vide\n"); 
        double x = n->x, y = n->y; 
        double xc = parent->xc, yc = parent->yc; 
        double coteX = parent->coteX, coteY = parent->coteY; 
        ArbreQuat *aq; 
        /* NO */  
        if (x < xc && y > yc) {
            aq = creerArbreQuat(xc-coteX/4, yc+coteY/4, coteX/2, coteY/2); 
            parent->no = aq; 
        }
        /* NE */ 
        else if (x >= xc && y >= yc) {
            aq = creerArbreQuat(xc+coteX/4, yc+coteY/4, coteX/2, coteY/2); 
            parent->ne = aq;  
        }
        /* SE */ 
        else if (x >= xc && y < yc) {
            aq = creerArbreQuat(xc+coteX/4, yc-coteY/4, coteX/2, coteY/2); 
            parent->se = aq; 
        }
        /* SO */ 
        else { 
            aq = creerArbreQuat(xc-coteX/4, yc-coteY/4, coteX/2, coteY/2); 
            parent->so = aq; 
        }
        aq->noeud = n; 
        *a = aq; 
    }

    /* Feuille */ 
    else if ((*a)->noeud != NULL) {
        //printf("Feuille\n");
        Noeud *noeud = (*a)->noeud; 
        (*a)->noeud = NULL; // la feuille devient cellule interne
        // insertions de 2 nouvelles feuilles  
        insererNoeudArbre(n, a, parent); 
        insererNoeudArbre(noeud, a, parent); 
    }

    /* Cellule interne */ 
    else {
        //printf("Cellule interne\n"); 
        double x = n->x, y = n->y; 
        double xc = (*a)->xc, yc = (*a)->yc; 
        if (x < xc && y > yc) 
            insererNoeudArbre(n, &((*a)->no), *a); /* NO */ 
        else if (x >= xc && y >= yc) 
            insererNoeudArbre(n, &((*a)->ne), *a); /* NE */  
        else if (x >= xc && y < yc) 
            insererNoeudArbre(n, &((*a)->se), *a); /* SE */  
        else 
            insererNoeudArbre(n, &((*a)->so), *a); /* SO */ 
    }
}

/* 5.4 */ 
Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat* a, ArbreQuat* parent, double x, double y) {
    assert(R); 

    /* Arbre vide -> on a pas trouve le noeud dans une des feuilles */ 
    if (a == NULL) {
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
        insererNoeudArbre(nd, &a, parent);
        return nd; 
    }

    /* Feuille */ 
    else if (a->noeud != NULL) {
        Noeud *n = a->noeud;
        if (compareNoeud_xy(n, x, y))
            return n;  
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
        insererNoeudArbre(nd, &a, parent); 
        return nd; 
    }

    /* Cellule interne */ 
    else {
        double xc = a->xc, yc = a->yc; 
        if (x < xc && y > yc) 
            return rechercheCreeNoeudArbre(R, a->no, a, x, y); /* NO */ 
        else if (x >= xc && y >= yc) 
            return rechercheCreeNoeudArbre(R, a->ne, a, x, y); /* NE */  
        else if (x >= xc && y < yc) 
            return rechercheCreeNoeudArbre(R, a->se, a, x, y); /* SE */  
        else 
            return rechercheCreeNoeudArbre(R, a->so, a, x, y); /* SO */ 
    }
}

/* 5.5 */ 
Reseau* reconstitueReseauArbre(Chaines* C) {
    assert(C); 

    Reseau *res = (Reseau*) malloc(sizeof(Reseau));
    assert(res); 
    res->nbNoeuds = 0;
    res->gamma = C->gamma;
    res->noeuds = NULL;
    res->commodites = NULL;

    double xmin, ymin, xmax, ymax; 
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double xc = (xmax+xmin)/2, yc = (ymax+ymin)/2;  
    double coteX = (xmax-xmin), coteY = (ymax-ymin); 
    ArbreQuat *root = creerArbreQuat(xc, yc, coteX, coteY);

    Noeud *nd, *ndRec;
    CellNoeud *ndVoisin;

    CellChaine *chaine = C->chaines;
    CellPoint *p;

    while (chaine != NULL) {
        CellCommodite *comNew = (CellCommodite*) malloc(sizeof(CellCommodite));
        assert(comNew);
        p = chaine->points;
        nd = rechercheCreeNoeudArbre(res, root, NULL, p->x, p->y); 
        comNew->extrA = nd;

        p = p->suiv;
        while (p != NULL) {
            ndRec = nd;
            nd = rechercheCreeNoeudArbre(res, root, NULL, p->x, p->y);
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
    libererArbreQuat(root); 
    return res;
}

void libererArbreQuat(ArbreQuat *aq) {
    if (aq) {
        libererArbreQuat(aq->no);
        libererArbreQuat(aq->ne);
        libererArbreQuat(aq->se);
        libererArbreQuat(aq->so);
        //free(aq->noeud); deja efectue dans libererReseau 
        free(aq); 
    }
}