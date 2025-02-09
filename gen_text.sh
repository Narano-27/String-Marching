#!/bin/bash

# step 1 : Compilation

echo "Compilation du programme C..."
gcc gen_text.c -o gen_text
if [ $? -ne 0 ]; then
    echo "Erreur : la compilation a échoué. Vérifiez votre code."
    exit 1
fi
echo "Compilation réussie."

# step 2 : generete text files
# define alphabet sizes
alphabet_sizes=(2 4 20 70)
text_length=500000

echo "Début de la génération des fichiers..."

# Parcours des tailles d'alphabet et génération des fichiers
for size in "${alphabet_sizes[@]}"; do
    echo "Génération du fichier pour alphabet de taille $size..."
    ./gen_text "$text_length" "$size"
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution pour alphabet de taille $size."
        exit 1
    fi
done

echo "Tous les fichiers ont été générés avec succès."
