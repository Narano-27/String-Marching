#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#define UCHAR_MAX 256


int max(int a, int b) {
  return (a > b) ? a : b;
}


void tab_suff(const char *x, int m, int *suff){

    int f, g, i;
    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i){
            if (i > g && suff[i + m - 1 - f] < i - g){
                suff[i] = suff[i + m - 1 - f];
            }
            else{
                if (i < g){
                    g = i;
                }
                f = i;
                while (g >= 0 && x[g] == x[g + m - 1 - f]){
                    --g;
                }
                suff[i] = f - g;
            }
    }
}


void bon_suff(const char *x, int m, int bon_suff[]){

    int i, j, suff[m];
    tab_suff(x, m, suff);
    i = 0;
    for (j = m - 2; j >= -1; --j){

        if (suff[j] == j + 1 || j == -1){
            while (i < m - 1 - j)
            {
                bon_suff[i] = m - 1 - j;
                i++;
            }
        }
    }
    for (j = 0; j <= m - 2; ++j){
        bon_suff[m - 1 - suff[j]] = m - 1 - j;
    }
}

void last_occ(const char *x, int m, int last_occ[])
{
    int i;
    for (i = 0; i < UCHAR_MAX; ++i){
        last_occ[i] = m;
    }
    for (i = 0; i < m - 1; ++i){
        last_occ[(int)x[i]] = m - i - 1;
    }
}

void Boyer_Moore(const char *x, int m, const char *y, int n)
{
    int i, j, tab_bon_suff[m], tab_last_occ[UCHAR_MAX];
    bon_suff(x, m, tab_bon_suff);
    last_occ(x, m, tab_last_occ);
    int cpt = 0;
    j = m - 1;
    while (j < n){
        i = m - 1;
        while (i >= 0 && x[i] == y[j - m + 1 + i]){
            i--;
        }
        if (i < 0){
            cpt++;
            j += tab_bon_suff[0];
        }
        else{
            j += max(tab_bon_suff[i], tab_last_occ[(int)y[j - m + 1 + i]] - m + 1 + i);
        }
    }

    // return cpt to get the number of occ
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

double mesurer_temps(void (*funct)(const char *, int, const char *, int), const char *x, int m, const char *y, int n)
{
    struct timeval start_time, end_time;

    gettimeofday(&start_time, NULL);

    funct(x, m, y, n);

    gettimeofday(&end_time, NULL);

    double execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;  // secondes en millisecondes
    execution_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;  // ajout des millisecondes

  return execution_time;
}

// Fonction pour enregistrer les résultats dans un fichier CSV
void enregistrer_resultats(FILE *fichier_csv, const char *texte, const char *mot, const char *algorithme, double temps) {
    fprintf(fichier_csv, "\"%s\",\"%s\",\"%s\",%f\n", texte, mot, algorithme, temps);
}
int main() {
    const char *textes[] = {
        "text_70.txt"
    };

    const char *mots[] = {
        "words_70_4.txt", "words_70_5.txt", "words_70_6.txt", "words_70_7.txt",
        "words_70_8.txt", "words_70_9.txt", "words_70_10.txt", "words_70_15.txt",
        "words_70_20.txt", "words_70_30.txt", "words_70_40.txt", "words_70_50.txt"
    };

    int num_textes = sizeof(textes) / sizeof(textes[0]);
    int num_mots = sizeof(mots) / sizeof(mots[0]);

    FILE *fichier_csv = fopen("resultats_recherche.csv", "w");
    if (fichier_csv == NULL) {
        perror("Erreur d'ouverture du fichier CSV");
        exit(EXIT_FAILURE);
    }

    fprintf(fichier_csv, "Texte,Mot,Algorithme,Temps (milliseconde)\n");

    for (int i = 0; i < num_textes; i++) {
        long taille_texte;
        char *texte = read_file(textes[i], &taille_texte);

        if (texte == NULL) {
            fprintf(stderr, "Erreur de lecture du texte %s\n", textes[i]);
            continue;
        }

        for (int j = 0; j < num_mots; j++) {
            long taille_mot;
            char *mot = read_file(mots[j], &taille_mot);

            if (mot == NULL) {
                fprintf(stderr, "Erreur de lecture du mot %s\n", mots[j]);
                continue;
            }

            double temps_execution = mesurer_temps(Boyer_Moore, mot, taille_mot, texte, taille_texte);


            enregistrer_resultats(fichier_csv, textes[i], mots[j], "Boyer-Moore", temps_execution);

            free(mot);
        }

        free(texte);
    }

    fclose(fichier_csv);

    printf("Les résultats ont été enregistrés dans 'resultats_recherche.csv'.\n");

    return 0;
}
