#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>  // Pour gettimeofday()

#define UCHAR_MAX 256


int* last_occu(char *x) {
    int i;
    int m = strlen(x);
    int *last_occu = (int *)malloc(UCHAR_MAX * sizeof(int));

    for (i = 0; i < UCHAR_MAX; ++i)
        last_occu[i] = m + 1;
    for (i = 0; i < m; ++i)
        last_occu[x[i]] = m - i;

    return(last_occu);

}


int Quick_Search(char *x, char *y) {
    int j;
    int compteur=0;
    int m = strlen(x);
    int n = strlen(y);
    // remplissage de la table dern occ
    int *tab_last_occu=last_occu(x);
    
    
    j = 0;
    while (j <= n - m) {
        if (memcmp(x, y + j, m) == 0)
            compteur++;
        j += tab_last_occu[y[j + m]];               
    }

    return compteur;
}





// Fonction pour lire un fichier et retourner son contenu sous forme de chaîne de caractères
char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Se déplacer à la fin du fichier pour obtenir sa taille
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allouer de la mémoire pour contenir le texte
    char *content = (char *)malloc(size + 1);
    if (content == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Lire le contenu du fichier
    fread(content, 1, size, file);
    content[size] = '\0'; // Ajouter le caractère de fin de chaîne

    fclose(file);
    return content;
}

// Fonction pour mesurer le temps d'exécution d'une fonction de recherche en microsecondes
double mesurer_temps(int (*fonction)(char*, char*), const char *texte, const char *mot) {
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);  // Capture du temps avant l'exécution de la recherche

    int result = fonction((char*)texte, (char*)mot); // Appel à la fonction et capture du résultat

    gettimeofday(&end_time, NULL);  // Capture du temps après l'exécution de la recherche

    // Calcul du temps d'exécution en millisecondes
    double execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;  // secondes en millisecondes
    execution_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;  // ajout des millisecondes

    return execution_time;
}

// Fonction pour enregistrer les résultats dans un fichier CSV
void enregistrer_resultats(FILE *fichier_csv, const char *texte, const char *mot, const char *algorithme, double temps) {
    fprintf(fichier_csv, "\"%s\",\"%s\",\"%s\",%f\n", texte, mot, algorithme, temps);
}

int main() {
    // Tableaux des fichiers texte et mots
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

    // Ouvrir le fichier CSV pour enregistrer les résultats
    FILE *fichier_csv = fopen("resultats_recherche.csv", "w");
    if (fichier_csv == NULL) {
        perror("Erreur d'ouverture du fichier CSV");
        exit(EXIT_FAILURE);
    }

    // Ajouter l'en-tête du CSV
    fprintf(fichier_csv, "Texte,Mot,Algorithme,Temps (milliseconde)\n");

    // Pour chaque fichier texte
    for (int i = 0; i < num_textes; i++) {
        char *text = read_file(textes[i]);

        // Pour chaque fichier de mots
        for (int j = 0; j < num_mots; j++) {
            char *mot = read_file(mots[j]);

            // Mesure du temps d'exécution de la recherche naïve pour chaque paire texte-mot
            double temps_execution = mesurer_temps(Quick_Search, text, mot);
            
            // Calculer le nombre d'occurrences trouvé
            int occurrences = Quick_Search(text, mot);
            
            // Afficher le nombre d'occurrences dans le terminal
            printf("Pour le texte '%s' et le mot '%s', nombre d'occurrences : %d\n", textes[i], mots[j], occurrences);
            
            // Afficher le temps d'exécution dans le terminal (en microsecondes)
            printf("Temps d'exécution pour texte '%s' et mot '%s': %f milliseconde\n", textes[i], mots[j], temps_execution);

            // Enregistrer les résultats dans le fichier CSV
            enregistrer_resultats(fichier_csv, textes[i], mots[j], "Quick_Search", temps_execution);

            // Libération de la mémoire allouée pour le mot
            free(mot);
        }

        // Libération de la mémoire allouée pour le texte
        free(text);
    }

    // Fermer le fichier CSV
    fclose(fichier_csv);

    printf("Les résultats ont été enregistrés dans 'resultats_recherche.csv'.\n");

    return 0;
}
