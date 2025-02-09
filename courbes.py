import pandas as pd
import matplotlib.pyplot as plt
import re

# Charger le fichier CSV
df = pd.read_csv('search_70.csv')

# Extraction de la longueur des mots et du nombre d'alphabet à partir des noms de fichiers
df['Longueur_Mot'] = df['Mot'].apply(lambda x: int(re.search(r'(\d+)\.txt$', x).group(1)))  # Extraction de N
df['Alphabet'] = df['Mot'].apply(lambda x: int(re.search(r'words_(\d+)_\d+.txt$', x).group(1)))  # Extraction de 70

# Création d'un graphique pour comparer les temps d'exécution en fonction de la longueur des mots
plt.figure(figsize=(12, 8))

# Pour chaque algorithme, tracer les résultats
for algorithme in df['Algorithme'].unique():
    subset = df[df['Algorithme'] == algorithme]
    plt.plot(subset['Longueur_Mot'], subset['Temps (milliseconde)'], label=algorithme, marker='o', markersize=8)

# Ajuster les axes pour une meilleure visibilité
plt.yscale('log')  # Utilisation de l'échelle logarithmique pour l'axe des Y

# Ajouter des labels, une légende et un titre
plt.xlabel('Longueur des mots')
plt.ylabel('Temps d\'exécution (millisecondes)')
plt.title('Comparaison des algorithmes de recherche')

# Afficher la légende
plt.legend()

# Ajouter une grille pour améliorer la lisibilité
plt.grid(True, which='both', linestyle='--', linewidth=0.5)

# Afficher le graphique
plt.tight_layout()  # Ajuste la disposition pour une meilleure lisibilité
plt.show()
