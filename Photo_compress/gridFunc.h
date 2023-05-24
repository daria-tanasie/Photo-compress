/* TANASIE Daria-Maria - 312CD */

#ifndef _GRIDFUNC_H_
#define _GRIDFUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

    /* FUNCTIA getMat() VA CREEA MATRICEA DE PIXELI DIN ARBORE
        PARCURGAND ARBORELE RECURSIV PANA VA AJUNGE LA 0 FRUNZA
            PENTRU A SCRIE VALORILE IN MATRICE*/

void getMat(Tree *root, grid **grid_mat, int lPos, int cPos, int width)
{
    if(root->type == 0) {
        width = width/2;
        getMat(root->t_left, grid_mat, lPos, cPos, width);
        getMat(root->t_right, grid_mat, lPos, cPos + width, width);
        getMat(root->b_right, grid_mat, lPos + width, cPos + width, width);
        getMat(root->b_left, grid_mat, lPos + width, cPos, width);
    
    } else {
        for(int i = lPos; i < lPos + width; i++)
            for(int j = cPos; j < cPos + width; j++) {
                grid_mat[i][j].red = root->red;
                grid_mat[i][j].green = root->green;
                grid_mat[i][j].blue = root->blue;
            }
    }
}

    /*FUNCTIA freeGrid() VA ELIBERA MEMORIA ALOCATA DINAMIC PENTRU 
        MATRICEA DE PIXELI, DAR NE VA AJUTA SI PT VECTORUL DE PONTERI
            LA STRUCTURI **vecDec CREAT DIN FISIERUL DE COMPRESIE*/

void freeGrid(grid **grid_mat, int height)
{
    for(int i = 0; i < height; i++)
        free(grid_mat[i]);
    free(grid_mat);
}

#endif
