#include <stdio.h>
#include <stdlib.h>
#include <time.h>




int main(int argc, char *argv[]){


  
    // check if we pass the arguments

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num_words> <len_words> <taille_Alphabet> \n", argv[0]);
        return 1;
    }


    int num_words = atoi(argv[1]);
    int len_words = atoi(argv[2]);
    int len_alph=atoi(argv[3]);
    // check the len  

    if (len_words <= 0 ||num_words <= 0 || len_alph <= 0) {
        fprintf(stderr, "La longueur et le nombre  des mots  ainsi que la taille de l'alphabet sont censé être supérieur à 0 \n");
        return 1;
    }

    // create a file with the number of alphabet + len_words in the name 

    char name[40];

    sprintf(name, "words_%d_%d.txt", len_alph,len_words);
    FILE *file = fopen(name, "w");

    if (file == NULL) {
        printf("Erreur de création du fichier ");
        return 1;
    }

    srand((unsigned int) time(NULL));


    if (len_alph <= 26){

        // in this case the  char will be from A-Z depending on rand()
        // rand will be between  0 - len_alph -1   
        for (int j = 0; j < num_words; ++j)
        {
            for (int i = 0; i < len_words; ++i) 
                {
                char caractere = (char) ((int) 'A' + (rand() % len_alph));
                fprintf(file, "%c", caractere);
                }
            //adding space after each word
            fprintf(file, "\n");  
        }

    }else
    {
        // in this case we start from 33 to avoid NUL - SOH ..
        // same logic with len < 26 but the caracter start from "!" and not "A" 
        for (int j = 0; j < num_words; ++j){

            for (int i = 0; i < len_words; ++i) 
                {
                char caractere = (char) (33 + (rand() % len_alph));
                fprintf(file, "%c", caractere);
                }
            //adding space after each word
            fprintf(file, "\n");  

        }



    }


    
    fclose(file);

    printf("Fichier '%s' généré avec succès.\n", name);
    return 0;
    




}   