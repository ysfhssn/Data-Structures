#include "Graphe.h"

/* ==========           TESTS GRAPHE          ==========*/

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("il faut ecrire %s <fichier .cha>\n", argv[0]);
        return 1;
    }

    FILE *fi = fopen(argv[1], "r");
    assert(fi);
    Chaines *chaines = lectureChaines(fi);
    fclose(fi);

    Reseau *r = reconstitueReseauListe(chaines);

    Graphe *g = creerGraphe(r); 
    afficher_Graphe(g);

    /* Test shortestPath_bfs */ 
    int nbAretes; 
    ListeEntier PATH; 
    
    /* Plus courte chaine entre 1 et v appartenant a V */ 
    for (int i = 1; i <= g->nbsom; i++) {
        PATH = shortestPath_bfs(g, 1, i, &nbAretes);
        afficherPATH(g, PATH); 
        desalloue(&PATH);
        printf("\n"); 
    }

    printf("---------------------------------------------------------------\n\n"); 
    
    /* Test reorganiseReseau */ 
    int reorg = reorganiseReseau(r); 
    printf("γ respecte : %d\n", reorg); 

    libererGraphe(g); 
    libererReseau(r);
    libererChaines(chaines); 
    
    return 0;
}

/* 7.5 */ 
/* On obtient 0 pour les instances burma, usa et pla. Cela veut dire que si on prend la plus courte chaine pour chaque commodité, le nombre maximal de fibres optiques (gamma) dans un câble n'est plus respecté ...   
Pour améliorer la fonction, au lieu d'utiliser une matrice qui prend beaucoup d'espace mémoire (nbSommet²/2 * sizeof(int)) inutilement, on peut ajouter un champ de type int pour chaque arête. En effet, la densité des graphes pour chacune des instances étant très faible ( densité = nbLiaisons/(nbNoeuds*(nbNoeuds-1)/2) ), on obtient des matrices creuses. 
*/ 