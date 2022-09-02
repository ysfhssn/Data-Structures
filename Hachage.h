#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Reseau.h"
#include "Chaine.h"

#define A 0.6180339887498949

/* 4.1 */ 
typedef struct TableHachage {
	int nE;
	int m;
	CellNoeud** T;
} TableHachage;

TableHachage* creer_TableHachage(int m);
unsigned int fonctionClef(double x, double y);
int fonctionHachage(unsigned int clef, int m);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M); 
void liberer_TableHachage(TableHachage* tH);

#endif 
