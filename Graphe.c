#include "Graphe.h"

/* Undirected Unweighted Graph */ 

Arete *creerArete(int i, int j) {
	Arete *a = (Arete*) malloc(sizeof(Arete));
    assert(a);
	a->u = i;
	a->v = j;
	return a;
}

void insererTete_ListeA(Cellule_arete **LA, Arete *a) {
    assert(LA); 
	Cellule_arete *newA = (Cellule_arete*) malloc(sizeof(Cellule_arete));
    assert(newA); 
	newA->a = a;
	newA->suiv = *LA;
	*LA = newA;
}

void ajoutArete(Graphe *g, int i, int j) {
	Arete *a = creerArete(i, j);
	insererTete_ListeA(&(g->T_som[i]->L_voisin), a);
	insererTete_ListeA(&(g->T_som[j]->L_voisin), a);
}

/* 7.1 */ 
/* ATTENTION: nums des sommets entre 1 et |V| comme dans les instances .res */
Graphe *creerGraphe(Reseau *r) {
	assert(r); 
	Graphe *g = (Graphe*) malloc(sizeof(Graphe));
	assert(g); 

	/* Caracteristiques du graphe */  
	g->nbsom = r->nbNoeuds;
	g->nbcommod = nbCommodites(r);
	g->gamma = r->gamma; 
	
	/* Construction du tableau de sommets */ 
	g->T_som = (Sommet**) malloc((g->nbsom+1) * sizeof(Sommet*));
	assert(g->T_som); 
	for (int i = 1; i <= g->nbsom; i++) {
		g->T_som[i] = (Sommet*) malloc(sizeof(Sommet));
		g->T_som[i]->num = i; //numeros de 1 a |V|
		g->T_som[i]->L_voisin = NULL;
	}

	CellNoeud *cn = r->noeuds;  
	while (cn != NULL) { 
        /* Ajout des sommets dans le tableau de sommets */ 
		int i = cn->nd->num;
		g->T_som[i]->x = cn->nd->x;
		g->T_som[i]->y = cn->nd->y;

		/* Construction des listes d'adjacence d'aretes */ 
		CellNoeud *voisins = cn->nd->voisins; 
		while (voisins != NULL) {
			int j = voisins->nd->num; 
			if (i < j)
				ajoutArete(g, i, j); 
			voisins = voisins->suiv; 
		}
		cn = cn->suiv;
	}

	/* Construction du tableau de commodites */ 
	g->T_commod = (Commod*) malloc(g->nbcommod * sizeof(Commod));
	assert(g->T_commod); 
	CellCommodite *com = r->commodites; 
	int i = 0; 
	while (com != NULL) {
		g->T_commod[i++] = (Commod) {com->extrA->num, com->extrB->num};  
		com = com->suiv;
	}

	return g;
}

void afficher_ListeA(Cellule_arete *LA) { 
	while (LA != NULL) {
		printf("{%d, %d} | ", LA->a->u, LA->a->v);
		LA = LA->suiv ;
	}
}

void afficher_Graphe(Graphe *g) {
    if (!g) {
        printf("NULL\n"); 
        return; 
    }

	printf("Nombre de sommets : %d\n", g->nbsom);
	printf("Nombre de commodites : %d\n", g->nbcommod); 
	printf("Gamma : %d\n\n", g->gamma);
	
	for (int i = 1; i <= g->nbsom; i++) {
		printf ("%d : ", i);
		afficher_ListeA(g->T_som[i]->L_voisin);
		printf("\n");
	}

    printf("\nCommodites:\n"); 
    for (int i = 0; i < g->nbcommod; i++)
        printf("%d %d\n", g->T_commod[i].e1, g->T_commod[i].e2); 
    printf("\n"); 
}

void libererListeA(Cellule_arete *LA) {
    while (LA != NULL) {
        Cellule_arete *tmp = LA;
        LA = LA->suiv; 
        if (tmp->a->u != -1)
            tmp->a->u = -1;  /* Premiere rencontre de l'arete: on le note par -1 */
        else 
            free(tmp->a);    /* Deuxieme rencontre de l'arete: on la desalloue */
        free(tmp);
    }
}

void libererGraphe(Graphe *g) {
    if (!g) return; 
    for (int i = 1; i <= g->nbsom ; i++) {
        libererListeA(g->T_som[i]->L_voisin);
        free(g->T_som[i]); 
    }
    free(g->T_som); 
    free(g->T_commod); 
    free(g);  
}

            	/* Unweighted Graph Breadth First Search O(|V|+|E|) */ 
/* 7.2 et 7.3 */  
ListeEntier shortestPath_bfs(Graphe *g, int src, int dest, int *nbAretes) {
	if (!g) {
		*nbAretes = 0; 
		return NULL; 
	} 
    
	assert(1 <= src && src <= g->nbsom && 1 <= dest && dest <= g->nbsom); 

	ListeEntier PATH; 
	Init_Liste(&PATH); 
	
	if (src == dest) {
		*nbAretes = 0; 
		ajoute_en_tete(&PATH, src); 
        printf("Il existe une chaine reliant %d a %d !\n", src, dest);
        printf("Nb min aretes: %d\n", *nbAretes); 
		return PATH; 
	}

	/* theta(|V|) */ 
	int *D = (int*) calloc(g->nbsom+1, sizeof(int)); /* Tableau des distances (en nb d'aretes) par rapport a src */  
	int *visit = (int*) calloc(g->nbsom+1, sizeof(int)); /* Tableau des sommets visites */ 
	int *prev = (int*) calloc(g->nbsom+1, sizeof(int)); /* Tableau de backtracking */ 

	visit[src] = 1;
	prev[src] = -1;

	/* FIFO */ 
	S_file *queue = (S_file*) malloc(sizeof(S_file)); 
	assert(queue); 

	Init_file(queue); 
	enfile(queue, src); // theta(1)

	while (!estFileVide(queue)) { // O(|V|)
		int u = defile(queue); // theta(1)

		Cellule_arete *voisinsA = g->T_som[u]->L_voisin;
		while (voisinsA != NULL) { // O(d(u))
			int v = (voisinsA->a->u != u) ? voisinsA->a->u : voisinsA->a->v;
			if (!visit[v]) {
				visit[v] = 1;
				prev[v] = u; 
				D[v] = D[u] + 1; 
				enfile(queue, v);
			}
			if (v == dest) {
				/* Reconstruction de la chaine la plus courte (en nb d'aretes) entre src et dest */
				for (int i = dest; i != -1; i = prev[i])
					ajoute_en_tete(&PATH, i); 
				*nbAretes = D[v];
				free(D); 
				free(visit); 
				free(prev);
				libererFile(queue); 
                printf("Il existe une chaine reliant %d a %d !\n", src, dest);
                printf("Nb min aretes: %d\n", *nbAretes); 
				return PATH; 
			}
			voisinsA = voisinsA->suiv ;
		}
	}
	printf("Graphe non connexe: il n'existe pas de chaine reliant %d a %d !\n", src, dest); 
	*nbAretes = -1;
	free(D); 
	free(visit); 
	free(prev);
	libererFile(queue);  
	return NULL; 
}

void afficherPATH(Graphe *g, ListeEntier PATH) {
    printf("PATH: "); 
    if (!PATH) return; 
    while (PATH != NULL) {
        if (PATH->suiv != NULL)
            printf("%d --> ", PATH->u);
        else 
            printf("%d", PATH->u); 
        PATH = PATH->suiv; 
    }
    printf("\n");  
}

/* 7.4 */ 
int reorganiseReseau(Reseau* r) {
	/* Creer le graphe correspondant au reseau */ 
	Graphe *g = creerGraphe(r); 

	/* Initialisation matrice triangulaire sommet-sommet */ 
	int **matrix = (int**) malloc(g->nbsom * sizeof(int*)); 
	for (int i = 0; i < g->nbsom; i++) 	
		matrix[i] = (int*) calloc(i, sizeof(int)); 

	/* Calculer la plus courte chaine pour chaque commodite */
	int nbAretes;  
	for (int i = 0; i < g->nbcommod; i++) {
        ListeEntier PATH = shortestPath_bfs(g, g->T_commod[i].e1, g->T_commod[i].e2, &nbAretes); 
        afficherPATH(g, PATH); 
        printf("\n");
		/* Comptage du nombre de courtes chaines passant par {u, v} */
		ListeEntier curr = PATH; 
		ListeEntier next = PATH->suiv; 
		while (next != NULL) {
			int u, v; 
			if (curr->u > next->u) {
				u = curr->u - 1; 
				v = next->u - 1; 
			} 
			else {
				u = next->u - 1; 
				v = curr->u - 1; 
			}
			matrix[u][v]++; 
			if (matrix[u][v] > r->gamma) {
				for (int i = 0; i < g->nbsom; i++) 	
					free(matrix[i]); 
				free(matrix); 
				libererGraphe(g); 
				desalloue(&PATH);
				return 0; 
			}
			curr = curr->suiv; 
			next = next->suiv; 
		} 
        desalloue(&PATH);
    }

    for (int i = 0; i < g->nbsom; i++) 	
		free(matrix[i]); 
    free(matrix); 
    libererGraphe(g); 

    return 1; 
}





/* Difference Between BFS and Dijkstra’s Algorithms: 
   https://www.baeldung.com/cs/graph-algorithms-bfs-dijkstra */ 