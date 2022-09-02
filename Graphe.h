#ifndef __GRAPHE_H__
#define __GRAPHE_H__
#include <stdlib.h>
#include <stdio.h>
#include "Struct_File.h"
#include "Struct_Liste.h"
#include "Reseau.h"

typedef struct {
  int u, v;        /* Numeros des sommets extremité */
} Arete;


typedef struct cellule_arete {
  Arete *a;     /* pointeur sur l'arete */
  struct cellule_arete *suiv;
} Cellule_arete;


typedef struct {
  int num;                  /* Numero du sommet (le meme que dans T_som) */
  double x, y;
  Cellule_arete *L_voisin;  /* Liste chainee des voisins */
} Sommet;


typedef struct {
  int e1, e2;    /* Les deux extremites de la commodite */
} Commod;


typedef struct {
  int nbsom;        /* Nombre de sommets */
  Sommet **T_som;   /* Tableau de pointeurs sur sommets */
  int gamma;     
  int nbcommod;     /* Nombre de commodites */
  Commod *T_commod; /* Tableau des commodites */
} Graphe;


Arete *creerArete(int u, int v);
void insererTete_ListeA(Cellule_arete **LA, Arete *a);
void ajoutArete(Graphe *g, int i, int j);
Graphe *creerGraphe(Reseau *r);
void afficher_ListeA(Cellule_arete *LA);
void afficher_Graphe(Graphe *g);
void libererListeA(Cellule_arete *LA);
void libererGraphe(Graphe *g);

/* Unweighted Graph Breadth First Search O(|V|+|E|) */ 
ListeEntier shortestPath_bfs(Graphe *g, int src, int dest, int *nbAretes); 
void afficherPATH(Graphe *g, ListeEntier PATH); 
int reorganiseReseau(Reseau* r); 

#endif
