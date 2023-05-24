/* TANASIE Daria-Maria - 312CD */

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    /*STRUCTURA CE NE VA AJUTA SA STOCAM PIXELII RED, GREEN, BLUE INTR O
            MATRICE ALOCATA DINAMIC DE STRUCTURI*/

typedef struct pixels {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} grid;

    /*STRUCTURA PENTRU UN NOD DIN ARBORE CE CONTINE TIPUL (0 DACA E NOD INTERN,
            1 DACA E FRUNZA), PIXELII DE CULORI(DACA E FRUNZA) SI CEI 4 COPII DACA 
                    ESTE INTERN*/

typedef struct node {
    unsigned char type;
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    struct node* t_left;
    struct node* t_right;
    struct node* b_left;
    struct node* b_right;
} Tree;

#endif
