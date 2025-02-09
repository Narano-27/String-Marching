#!/bin/bash

# Step 1 : Compilation
echo "Compilation du programme C..."
gcc gen_words.c -o word_generator
if [ $? -ne 0 ]; then
    echo "Erreur : la compilation a échoué. Vérifiez votre code."
    exit 1
fi
echo "Compilation réussie."

# Step 2 : Définition des paramètres
alphabet_sizes=(2 4 20 70)
word_lengths=(4 5 6 7 8 9 10 15 20 30 40 50)
num_words=100
output_dir="word_sets"


# Step 3 : Génération des fichiers
echo "Début de la génération des fichiers..."

for alphabet_size in "${alphabet_sizes[@]}"; do
    for word_length in "${word_lengths[@]}"; do
        output_file="$words_${alphabet_size}_${word_length}.txt"
        echo "Génération de $num_words mots de longueur $word_length sur un alphabet de taille $alphabet_size..."
        
        ./word_generator "$num_words" "$word_length" "$alphabet_size"
        if [ $? -ne 0 ]; then
            echo "Erreur lors de l'exécution pour alphabet de taille $alphabet_size et longueur $word_length."
            exit 1
        fi
        
        # Déplace le fichier généré vers le dossier de sortie
        mv "words_${alphabet_size}_${word_length}.txt" "$output_dir/"
    done
done

echo "Tous les fichiers ont été générés avec succès dans le dossier "
