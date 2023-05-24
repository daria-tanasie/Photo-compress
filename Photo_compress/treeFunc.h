/* TANASIE Daria-Maria - 312CD */

#ifndef _TREEFUNC_H_
#define _TREEFUNCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

    /* AM DEFINIT ECUATIA DUPA CARE SE FAC CALCULA VALOAREA MEAN */

#define ecuation ((red-grid_mat[i][j].red)*(red-grid_mat[i][j].red) + \
(green-grid_mat[i][j].green)*(green-grid_mat[i][j].green) + \
(blue-grid_mat[i][j].blue)*(blue-grid_mat[i][j].blue));

    /*FUNCTIA createNode() VA ALOCA MEMORIE PENTRU FIECARE NOD SI VA
            INITIALIZA CULORILE DIN NOD CU 0, RETURNAND POINTER CATRE NOD*/

Tree *createNode()
{
    Tree *root = (Tree *) malloc(sizeof(struct node));
    root->b_left = NULL;
    root->b_right = NULL;
    root->t_left = NULL;
    root->t_right = NULL;
    root->type = 0;
    root->red = 0;
    root->green = 0;
    root->blue = 0;
    return root;
}

    /* FUNCTIA  createTree() VA CREEA ARBORELE ASOCIAT IMAGINI, FIIND O
        FUNCTIE RECURSIVA CARE DE FIECARE DATA CAND MEAN UL VA FI MAI MARE
            DECAT FACTORUL VA MERGE IN ADANCIME SA MAI CREEZE NODURI */

void createTree(Tree **root, grid **grid_mat, int lPos, int cPos, int block_size, int factor)
{
    unsigned long long red = 0, green = 0, blue = 0;
    unsigned long long mean = 0;

    for(int i = lPos; i < (block_size + lPos); i++) {
        for(int j = cPos; j < (block_size + cPos); j++) {
            red =  red + grid_mat[i][j].red;
            green =  green + grid_mat[i][j].green;
            blue =  blue + grid_mat[i][j].blue;
        }
    }

    *root = createNode();

    red = (int)(red/(block_size * block_size));
    green = (int)(green/(block_size * block_size));
    blue = (int)(blue/(block_size * block_size));

    for(int i = lPos; i < (block_size + lPos); i++) {
        for(int j = cPos; j < (block_size + cPos); j++) {
            /* "ecuation" reprezinta ecuatia prin care determinam valoarea lui mean
                pe care am definit o la inceput */
            mean = mean + ecuation;
        }
    }

    mean = (int)(mean/(3*block_size*block_size));

    if(mean > factor) {
        block_size = block_size/2;
        createTree(&(*root)->t_left, grid_mat, lPos, cPos, block_size, factor);
        createTree(&(*root)->t_right, grid_mat, lPos, cPos + block_size, block_size, factor);
        createTree(&(*root)->b_right, grid_mat, lPos + block_size, cPos + block_size, block_size, factor);
        createTree(&(*root)->b_left, grid_mat, lPos + block_size, cPos, block_size, factor);
    } else {
        (*root)->red = red;
        (*root)->green = green;
        (*root)->blue = blue;
    }
}

    /*FUNCTIA getMaxHeight() VA CALCULA CATE NIVELURI ARE ARBORELE PT A 
        FI SCRISE IN FISIER */

int getMaxHeight(Tree *root)
{   
    int top_max = 0, bottom_max = 0;
    if(root == NULL)
        return 0;
    int TLHeight = getMaxHeight(root->t_left);
    int TRHeight = getMaxHeight(root->t_right);
    int BRHeight = getMaxHeight(root->b_right);
    int BLHeight = getMaxHeight(root->b_left);

    if(TLHeight > TRHeight)
        top_max = TLHeight;
        else
        top_max = TRHeight;

    if(BRHeight > BLHeight)
        bottom_max = BRHeight;
        else 
        bottom_max = BLHeight;
    if(top_max > bottom_max) {
        top_max++;
        return top_max;
    }else {
        bottom_max++;
        return bottom_max;
    }
}

    /* FUNCTIA getMinHeight() VA RETURN CEL MAI MIC NIVEL PE CARE SE AFLA O
        FRUNZA, FIIND NECESAR PT A SCRIE LATURA PATRATULUI CARE DESCRIE
            CEA MAI MARE ZONA NEDIVIZATA*/

int getMinHeight(Tree *root)
{
    int top_min = 0, bottom_min = 0;
    if(root == NULL)
        return 0;
    int TLHeight = getMinHeight(root->t_left);
    int TRHeight = getMinHeight(root->t_right);
    int BRHeight = getMinHeight(root->b_right);
    int BLHeight = getMinHeight(root->b_left);

    if(TLHeight < TRHeight)
        top_min = TLHeight;
        else
        top_min = TRHeight;

    if(BRHeight < BLHeight)
        bottom_min = BRHeight;
        else 
        bottom_min = BLHeight;
    if(top_min < bottom_min) {
        top_min++;
        return top_min;
    }else {
        bottom_min++;
        return bottom_min;
    }
}

    /*FUNCTIILE writeLevel() si writeLevelOrder() NE VOR AJUTA SA 
        SCRIEM NODURILE DIN ARBORE IN ORDINEA NIVELELOR, DACA NODUL ESTE
            FRUNZA SE VOR SCRIE SI CULORILE RGB SI TIPUL NODULUI,
                DACA NU DOAR TIPUL*/

void writeLevel(Tree *root, int level, FILE *out)
{
    if(root == NULL)
        return;
    if(level == 1) {
        if(root->type == 0) {
            fwrite(&(root->type), 1, sizeof(unsigned char), out);
        }
        else {
            fwrite(&(root->type), 1, sizeof(unsigned char), out);
            fwrite(&(root->red), 1, sizeof(unsigned char), out);
            fwrite(&(root->green), 1, sizeof(unsigned char), out);
            fwrite(&(root->blue), 1, sizeof(unsigned char), out);
        }
        return;
    }
    writeLevel(root->t_left, level - 1, out);
    writeLevel(root->t_right, level - 1, out);
    writeLevel(root->b_right, level - 1, out);
    writeLevel(root->b_left, level - 1, out);
}

void writeLevelOrder(Tree *root, FILE *out)
{
    int i, h = getMaxHeight(root);
    for(i = 0; i <= h; i++)
        writeLevel(root, i, out);
}

    /*FUNCTIA getNrLeaves() VA RETURN NR DE FRUNZE, ACESTA FIIND NR 
        DE BLOCURI PENTRU CARE SCORUL SIMILARITATII ESTE MAI MIC
            SAU EGAL CU FACTORUL*/

int getNrLeaves(Tree **root)
{
    int nr = 0;
    if(root == NULL)
        return 0;

    if((*root)->t_left == NULL) {
        (*root)->type = 1;
        return 1;
    }else {
    nr = nr + getNrLeaves(&(*root)->b_left);
    nr = nr + getNrLeaves(&(*root)->b_right);
    nr = nr + getNrLeaves(&(*root)->t_right);
    nr = nr + getNrLeaves(&(*root)->t_left);
    return nr;
    }
}

    /* FUNCTIA decomTree() VA PRIMI VECTORUL CONSTRUIT IN MAIN CU 
        AJUTORUL FISIERULUI COMPRIMAT SI VA CREEA ARBORELE ASOCIAT
            IMAGINII*/

Tree *decomTree(Tree **vecDec, int nr_nodes)
{
    int i = 0, j = 0;
    for(i = 0; i < nr_nodes; i++) {
        if(vecDec[i]->type == 0) {
            vecDec[i]->t_left = vecDec[4 * j + 1];
            vecDec[i]->t_right = vecDec[4 * j + 2];
            vecDec[i]->b_right = vecDec[4 * j + 3];
            vecDec[i]->b_left = vecDec[4 * j + 4];
            j++;
        }
    }
    return vecDec[0];
}

    /* FUNCTIA freeTree() VA ELIBERA MEMORIA ALOCATA DINAMIC PT
        ARBORELE ASOCIAT IMAGINII*/

void freeTree(Tree *root)
{
    if(root != NULL) {
    freeTree(root->b_left);
    freeTree(root->b_right);
    freeTree(root->t_right);
    freeTree(root->t_left);
    free(root);
    }
}

#endif