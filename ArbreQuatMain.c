#include "ArbreQuat.h"

/* ==========           TESTS QUADTREE          ==========*/

int main(int argc, char** argv) {

    if (argc != 2) {
		printf("il faut ecrire %s <fichier .cha>\n", argv[0]);
		return 1;
	}

    FILE *fi = fopen(argv[1], "r");
    assert(fi);
    Chaines *chaines = lectureChaines(fi);
    fclose(fi);

    /* test chaineCoordMinMax */
    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(chaines, &xmin, &ymin, &xmax, &ymax);
    printf("Coords min : (%lf, %lf)\n", xmin, ymin);
    printf("Coords max : (%lf, %lf)\n", xmax, ymax);

    /* ========= test insertions =========*/
    double xc = (xmax+xmin)/2, yc = (ymax+ymin)/2;
    double coteX = (xmax-xmin), coteY = (ymax-ymin);
    ArbreQuat *root = creerArbreQuat(xc, yc, coteX, coteY); //cellule interne

    /* Reseau a 4 noeuds */
    Noeud *n_no = (Noeud*) malloc(sizeof(Noeud));
    n_no->x = xmin; n_no->y = ymax; /* NO */
    Noeud *n_ne = (Noeud*) malloc(sizeof(Noeud));
    n_ne->x = xmax; n_ne->y = ymax; /* NE */
    Noeud *n_se = (Noeud*) malloc(sizeof(Noeud));
    n_se->x = xmax; n_se->y = ymin; /* SE */
    Noeud *n_so = (Noeud*) malloc(sizeof(Noeud));
    n_so->x = xmin; n_so->y = ymin; /* SO */

    /* insertions */
    insererNoeudArbre(n_no, &root, NULL);
    insererNoeudArbre(n_ne, &root, NULL);
    insererNoeudArbre(n_se, &root, NULL);
    insererNoeudArbre(n_so, &root, NULL);

    assert(root->no->noeud == n_no);
    assert(root->ne->noeud == n_ne);
    assert(root->se->noeud == n_se);
    assert(root->so->noeud == n_so);

    /* Insertion 5e et 6e noeud */ 
    Noeud *n5 = (Noeud*) malloc(sizeof(Noeud));
    Noeud *n6 = (Noeud*) malloc(sizeof(Noeud));
    n5->x = xc-1; n5->y = yc+1; /* NO puis SE */
    n6->x = xc-1; n6->y = yc-1; /* SO puis NE */
    insererNoeudArbre(n5, &root, NULL);
    insererNoeudArbre(n6, &root, NULL);

    /*
    printf("root: cotes %lf %lf\n", coteX, coteY);
    printf("root: xc yc %lf %lf\n", xc, yc);
    printf("root->no: xc yc %lf %lf\n", root->no->xc, root->no->yc);
    printf("noeud n_no: x y %lf %lf\n", n_no->x, n_no->y);
    printf("noeud n5: x y %lf %lf\n", n5->x, n5->y);
    */

    assert(root->no->noeud == NULL); // feuille devenue cellule interne
    assert(root->so->noeud == NULL); // feuille devenue cellule interne
    assert(root->no->no->noeud == n_no); // fils NO de root a 2 feuilles: NO et SE
    assert(root->so->so->noeud == n_so); // fils SO de root a 2 feuilles: SO et NE
    assert(root->no->se->noeud == n5);
    assert(root->so->ne->noeud == n6);

    free(n_no); free(n_ne); free(n_se); free(n_so);
    free(n5); free(n6);
    libererArbreQuat(root);
    libererChaines(chaines);

    return 0;
}