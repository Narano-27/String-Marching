#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

void Bon_pref(const char *mot, int *Tab_bon_pref, int m){
  Tab_bon_pref[0] = -1; 
  int i = 0;

  for (int j = 1; j < m; j++){
    Tab_bon_pref[j] = i;
    while (i >= 0 && mot[i] != mot[j]){
        i = Tab_bon_pref[i]; // Reculer dans le tableau bon-pref
      }
    i++;
  }
  Tab_bon_pref[m] = i; // Dernière valeur
}

void Morris_Pratt(const char *texte, long texte_size, const char *mot){
    
  int n = texte_size;                                  
  int m = strlen(mot);                                 
  int *Tab_bon_pref = (int *)malloc((m + 1) * sizeof(int)); 

  Bon_pref(mot, Tab_bon_pref, m);

  int i = 0; 
  for (int j = 0; j < n; j++){ 
    while (i >= 0 && mot[i] != texte[j]){
      i = Tab_bon_pref[i]; 
    }
    i++;

    if (i == m){ 
      printf("Mot trouvé à la position %d\n", j - m + 1);
      i = Tab_bon_pref[i]; 
    }
  }

  free(Tab_bon_pref); 
}


char *read_file(const char *filename, long *size){
  FILE *file = fopen(filename, "r");
  if (file == NULL){
      fprintf(stderr, "Erreur d'ouverture du fichier %s\n", filename);
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

double mesurer_temps(void (*funct)(const char *, long, const char *), const char *texte, long texte_size, const char *mot){
  struct timeval start_time, end_time;

  gettimeofday(&start_time, NULL);

  funct(texte, texte_size, mot);

  gettimeofday(&end_time, NULL);


    double execution_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;  // secondes en millisecondes
    execution_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;  // ajout des millisecondes

  return execution_time;
}
void enregistrer_resultats(FILE *fichier_csv, const char *texte, const char *mot, const char *algorithme, double temps) {
    fprintf(fichier_csv, "\"%s\",\"%s\",\"%s\",%f\n", texte, mot, algorithme, temps);
}

int main()
{
       const char *textes[] = {
        "text_4.txt"
    };

    const char *mots[] = {
        "words_4_4.txt", "words_4_5.txt", "words_4_6.txt", "words_4_7.txt",
        "words_4_8.txt", "words_4_9.txt", "words_4_10.txt", "words_4_15.txt",
        "words_4_20.txt", "words_4_30.txt", "words_4_40.txt", "words_4_50.txt"
    };


    int taille_textes = sizeof(textes) / sizeof(textes[0]);
    int taille_mots = sizeof(mots) / sizeof(mots[0]);
    // Boucle pour parcourir les fichiers textes et mots
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

            // Lire le texte et le mot depuis les fichiers
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

            // Mesurer le temps d'exécution de l'algorithme
            double temps = mesurer_temps(Morris_Pratt, texte, texte_size, mot);


            enregistrer_resultats(fichier_csv, textes[i], mots[j], "Morris_Pratt", temps);

            // Libérer la mémoire
            free(texte);
            free(mot);
        }
        fprintf(fichier_csv, "\n"); // Nouvelle ligne après chaque texte
    }

    // Fermer le fichier CSV
    fclose(fichier_csv);

    printf("Les résultats ont été exportés dans 'resultats_recherche.csv'.\n");

    return 0;
}