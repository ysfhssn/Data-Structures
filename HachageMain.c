#include "Hachage.h"

/* ==========           TESTS TABLEHACHAGE          ==========*/ 

int main(int argc, char** argv) {

    /* 4.2 */ 
    /* N² -> N */ 
    int i = 0; 
    unsigned int tab[100]; 
    for (int x = 1; x <= 10; x++) {
        for (int y = 1; y <= 10; y++) {
            printf("f(%d,%d) = %u\n", x, y, fonctionClef(x, y));
            tab[i++] = fonctionClef(x, y);  
        }
    }

    int nbCollisions = 0; 
    for (int i = 0; i < 99; i++) {
        for (int j = i+1; j < 100; j++) { 
            if (tab[i] == tab[j])
                nbCollisions++; 
        }
    }

    printf("Nombre de collisions : %d\n", nbCollisions); 

    /* 
    Cette fonction clef semble appropriee ! Les clefs generees sont differentes pour x et y entiers allant de 1 a 10. 
    La fonction f est en fait une bijection de N² -> N (demonstration simple, voir maths discretes)

    Les instances 07397_pla.res et 05000_USA-road-d-NY.res ne contiennent que des entiers !! Et meme pour les reels (burma.res), on a jamais la meme clef generee. 

    Remarque: R+² n'est pas denombrable et il n'existe pas de bijection de R+² -> N. Si on a des points tres tres proches, alors on peut avoir la meme clef generee mais ici on considere des reseaux de fibres optiques (les clients/concentrateurs sont suffisamment eloignes les uns des autres pour ne pas avoir la meme clef generee)  
    */ 

    return 0; 
}