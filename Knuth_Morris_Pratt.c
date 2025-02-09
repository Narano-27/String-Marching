#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

void mei_pref(const char *mot, int *tab_mei_pref, int m){
    tab_mei_pref[0] = -1; 
    int i = 0;

    for (int j = 1; j < m; j++){
        if (mot[i] == mot[j]){
            tab_mei_pref[j] = tab_mei_pref[i];
        }
        else{
            tab_mei_pref[j] = i;
            while (i >= 0 && mot[i] != mot[j]){
                i = tab_mei_pref[i]; 
            }
        }
        i++;
    }
    tab_mei_pref[m] = i; 
}

void Knuth_Morris_Pratt(const char *texte, const char *mot){
    int n = strlen(texte);                                
    int m = strlen(mot);            
    int cpt =0;                     
    int *tab_mei_pref = (int *)malloc((m + 1) * sizeof(int)); 

    mei_pref(mot, tab_mei_pref, m);

    int i = 0; 
    for (int j = 0; j < n; j++){ 
        while (i >= 0 && mot[i] != texte[j]){
            i = tab_mei_pref[i]; 
        }
        i++;

        if (i == m){ 
            cpt++;
            i = tab_mei_pref[i]; 
        }
    }

    free(tab_mei_pref);

    //return  cpt to get the number of occu
}




// Fonction pour lire un fichier et retourner son contenu sous forme de chaîne de caractères

char *read_file(const char *filename, long *size){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        fprintf(stderr, "erreur de lecture du fichier le fichier %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    char *content = (char *)malloc(*size + 1);
    if (content == NULL){
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        fclose(file);
        return NULL;
    }

    fread(content, 1, *size, file);
    content[*size] = '\0';

    fclose(file);
    return content;
}

// Fonction pour mesurer le temps d'exécution d'une fonction de recherche en microsecondes

double mesurer_temps(void (*funct)(const char *, const char *), const char *texte, const char *mot){
    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);

    funct(texte, mot);

    gettimeofday(&end_time, NULL);

    double execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;  // secondes en millisecondes
    execution_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;  // ajout des millisecondes

  return execution_time;
}
// Fonction pour enregistrer les résultats dans un fichier CSV
void enregistrer_resultats(FILE *fichier_csv, const char *texte, const char *mot, const char *algorithme, double temps) {
    fprintf(fichier_csv, "\"%s\",\"%s\",\"%s\",%f\n", texte, mot, algorithme, temps);
}
int main()
{
   const char *textes[] = {
        "text_20.txt"
    };

    const char *mots[] = {
        "words_20_4.txt", "words_20_5.txt", "words_20_6.txt", "words_20_7.txt",
        "words_20_8.txt", "words_20_9.txt", "words_20_10.txt", "words_20_15.txt",
        "words_20_20.txt", "words_20_30.txt", "words_20_40.txt", "words_20_50.txt"
    };
    int taille_textes = sizeof(textes) / sizeof(textes[0]);
    int taille_mots = sizeof(mots) / sizeof(mots[0]);

    // Ouvrir le fichier CSV en mode écriture
    FILE *fichier_csv = fopen("resultats_recherche.csv", "w");
    if (fichier_csv == NULL)
    {
        printf("Erreur d'ouverture du fichier CSV.\n");
        return 1;
    }

    // Boucle pour parcourir les fichiers textes
    for (int i = 0; i < taille_textes; i++)
    {
        for (int j = 0; j < taille_mots; j++)
        {
            long texte_size, mot_size;

            char *texte = read_file(textes[i], &texte_size);
            if (texte == NULL)
            {
                printf("Erreur de lecture du fichier %s\n", textes[i]);
                fprintf(fichier_csv, ","); // Insérer une cellule vide en cas d'erreur
                continue;
            }
            char *mot = read_file(mots[j], &mot_size);
            if (mot == NULL)
            {
                printf("Erreur de lecture du fichier %s\n", mots[j]);
                free(texte);
                fprintf(fichier_csv, ","); // Insérer une cellule vide en cas d'erreur
                continue;
            }

            double temps = mesurer_temps(Knuth_Morris_Pratt, mot, texte);
            enregistrer_resultats(fichier_csv, textes[i], mots[j], "Knuth_Morris_Pratt", temps);

            // Libérer la mémoire
            free(texte);
            free(mot);
        }
        fprintf(fichier_csv, "\n"); // Nouvelle ligne après chaque texte
    }

    fclose(fichier_csv);

    printf("Les résultats ont été exportés dans 'resultats_recherche.csv'.\n");

    return 0;
}