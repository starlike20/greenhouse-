## Greenhouse

## Description

Ce projet implémente une simulation de gestion de serre en C++ avec des ressources graphiques associées. Il inclut des fonctionnalités pour gérer les interactions avec des éléments graphiques tels que des personnages, des ennemis et des objets dans un environnement de serre simulé.

---

## Fonctionnalités

- Simulation des activités dans une serre avec des personnages animés et des ennemis.
- Éléments graphiques pour l'interaction utilisateur, y compris des objets, des insectes et l'environnement.
- Base de code organisée avec des composants modulaires pour une compréhension et une maintenance simplifiées.

---

## Structure du Projet

Le projet est organisé comme suit :

### 1. **Code Source**

- `gh.cpp` : Fichier principal contenant la logique centrale de la simulation de serre.
- `makefile` : Automatise le processus de construction du projet.

### 2. **Ressources Graphiques**

- Situées dans le répertoire `images/`, incluant :
  - **Personnages** : Animations des personnages et de leurs actions.
  - **Ennemis** : Animations pour divers insectes tels que des araignées, des chenilles et des guêpes.
  - **Environnement** : Visuels de la serre et graphiques supplémentaires.

### 3. **Module de Présentation**

- `presentation/` : Contient le code (`presentation.c` et `presentation.h`) pour gérer les éléments d'affichage et les interactions.

---

## Prérequis

- **Compilateur C++** : GCC ou tout autre compilateur C++ compatible.
- **Utilitaire Make** : Requis pour exécuter le `makefile` et construire le projet.
- **Visionneuse d'Images** : Pour afficher les ressources graphiques au format `.bmp` si nécessaire.

---

## Installation et Exécution

### 1. **Cloner le Dépôt**

```bash
git clone <url-du-dépôt>
```

### 2. **Construire le Projet**

Accédez au répertoire du projet et exécutez :

```bash
make
```

### 3. **Exécuter l'Application**

Exécutez le binaire généré :

```bash
./gh
```

---

## Contribution

Les contributions sont les bienvenues. Pour contribuer :

1. Forkez le dépôt.
2. Créez une branche pour votre fonctionnalité : `git checkout -b feature/NouvelleFonctionnalite`.
3. Commitez vos modifications : `git commit -m 'Ajout d\'une nouvelle fonctionnalité'`.
4. Poussez la branche : `git push origin feature/NouvelleFonctionnalite`.
5. Soumettez une Pull Request.

---

## Auteurs

Ce projet a été développé par [Votre Nom/Équipe].

---

## Licence

Ce projet est sous licence MIT. Consultez le fichier `LICENSE` pour plus de détails.

