/* TANASIE Daria-Maria - 312CD */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "gridFunc.h"
#include "treeFunc.h"


int main(int argc, char const *argv[]){
    // declaram variabilele necesare la inceput
    int task_nr = 0, factor;
    int i,j;
    FILE *in;
    FILE *out;

    /* in functie de cerinta care se doreste a fie executata vom compara argumentul
        indicat pentru a afla nr taskului care tb executat */
    if(strcmp(argv[1], "-c1") == 0)
        task_nr = 1;
    if(strcmp(argv[1], "-c2") == 0)
        task_nr = 2;
    if(strcmp(argv[1], "-d") == 0)
        task_nr = 3;

    /* daca nr taskului este 1 sau 2, argumentul de pe pozitia 2 reprezinta factorul
        de similaritate, iar cel de pe poz 3 este fisierul de unde citim*/
    if(task_nr == 1 || task_nr == 2) {
        factor = atoi(argv[2]);
        in = fopen(argv[3], "rb");

        if(in == NULL) {
		    return 0;
	    }
    }

    /* daca nr taskului este 1 va tb sa scriem text in fisierul dat ca argument */
    if(task_nr == 1)
        out = fopen(argv[4], "wt");

    /* daca nr taskului este 2 va tb sa scriem in format sbinar in
            fisierul dat ca argument */  
    if(task_nr == 2)
        out = fopen(argv[4], "wb");

    /*declaram variabilele necesare pt citirea headerului din fisierul ppm */
    unsigned char file_type[3];
    int width, height;
    char max_value[3];

    //initializam radacina arborelui ce urmeaza sa fie construit
    Tree *root = NULL;

    if(task_nr != 3) {

        /*citim din fisier header ul*/
        fscanf(in, "%s\n", file_type);
        file_type[strlen(file_type)] = '\0';
        fscanf(in, "%d %d\n", &width, &height);
        fscanf(in, "%s", max_value);
        fseek(in, 1, SEEK_CUR);
        /*citim si pixelii intr un vector alocat dinamic*/
        unsigned char *pixels = (unsigned char *) 
                malloc(width * height * 3 * sizeof(unsigned char));
        fread(pixels, sizeof(unsigned char), width*height*3, in);

        /*voi folosi o matrice de pixeli a carei structura am definit o in 
            fisierul structs.h*/
        grid **grid_mat = (grid **) malloc(width * height * sizeof(grid*));
    
        int i_ind = 0;

        /*construim matricea de pixeli pe baza vectorului pixels, fiecare element din
            matricea grid_mat avand o valoare pt red, green si blue*/
        for(i = 0; i < height; i++) {
            int j_ind = 0;
            grid_mat[i_ind] =(grid*) malloc(width * sizeof(grid));
            for(j = 0; j < width; j++) {
                int nr_rgb = (i*width + j)*3;
                grid_mat[i_ind][j_ind].red = pixels[nr_rgb];
                grid_mat[i_ind][j_ind].green = pixels[nr_rgb+1];
                grid_mat[i_ind][j_ind].blue = pixels[nr_rgb+2];
                j_ind++;
            }
            i_ind++;
        }

        free(pixels); //eliberam memoria alocata [t vectorul de pixeli

        /*cream un arbore cu ajutorul functiei createTree() ce va construi
            arborele asociat imaginii avand culori doar in nodurile tip frunza*/
        createTree(&root, grid_mat, 0, 0, width, factor);

        int levels = getMaxHeight(root);
        int nr_leaves = getNrLeaves(&root); 
    
        if(task_nr == 1) {
            /*daca nr taskului este 1 vom scrie in fisier informatiile cerute*/
            int minHeight = 0;
            fprintf(out, "%d\n", levels);
            fprintf(out, "%d\n", nr_leaves);
            int p = 1;
            minHeight = getMinHeight(root);
            for(i = 0; i < minHeight-1; i++)
                p = p*2;
            minHeight = width/p;    
            fprintf(out, "%d\n", minHeight);
        }
    


        if(task_nr == 2) {

            //daca nr taskului este 2 scriem informatiile in fisier binar
            fwrite(&height, 1, sizeof(unsigned int), out);
            writeLevelOrder(root, out);
        }

        //eliberam memoria alocata pt matricea grid_mat si pt arbore
        freeGrid(grid_mat, height);
        freeTree(root);
    }

    if(task_nr == 3) {

        /*daca nr taskului este 3 scriem headerul si apoi citim din fisierul binar
            datele despre arborele asociat imaginii si il cream, apoi
                reconstituim matricea de pixeli si sciem fisierul ppm*/
        unsigned char *file_form = "P6";
        in = fopen(argv[2], "rb");
        
        if(in == NULL) {
            return 0;
        }    
        
        out = fopen(argv[3], "wt");
        fread(&width, sizeof(unsigned int), 1, in);
        unsigned int height3 = width;

        Tree **vecDec = (Tree **) malloc(sizeof(Tree *) *  4 * width * width);
        i = 0;
        while(!feof(in)) {
            vecDec[i] = createNode();
            fread(&vecDec[i]->type, sizeof(unsigned char), 1, in);
            if(vecDec[i]->type == 1) {
                fread(&vecDec[i]->red, sizeof(unsigned char), 1, in);
                fread(&vecDec[i]->green, sizeof(unsigned char), 1, in);
                fread(&vecDec[i]->blue, sizeof(unsigned char), 1, in);
            }
            i++;
        }

        int nr_nodes = i;

        root = decomTree(vecDec, nr_nodes);

        grid **grid_mat = (grid **) malloc (width * width * sizeof(grid *));
        for(i = 0; i < width; i++)
            grid_mat[i]= (grid *) malloc(width * sizeof(grid));

        getMat(root, grid_mat, 0, 0, width);

        fprintf(out, "P6\n");
        fprintf(out, "%d %d\n", width, width);
        fprintf(out, "255\n");

        for(i = 0; i < width; i++) {
            for(j = 0; j < width; j++) {
                fwrite(&grid_mat[i][j].red, sizeof(unsigned char), 1, out);
                fwrite(&grid_mat[i][j].green, sizeof(unsigned char), 1, out); 
                fwrite(&grid_mat[i][j].blue, sizeof(unsigned char), 1, out); 
            }    
        }

        //deoarece avem 2 structuri de tip matrice puteam folosi aceasi functie de free

        freeGrid(grid_mat, width);
        freeGrid(vecDec, nr_nodes);
    }
    //inchidem fisierele deschise
    fclose(in);
    fclose(out);
}

