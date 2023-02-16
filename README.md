# MPNA - Projet Padé-Rayleigh-Ritz

Ceci est la notice d'instruction du projet Padé-Rayleigh-Ritz. Ce projet a été réalisé par DOEBELE Grégoire et VIDAL Antoine dans le cadre du Master 2 Calcul Haute Performance et Simulation (CHPS) de l'UVSQ.

## Sommaire

1. Présentation du projet
2. Structure du projet
3. Installation du programme
4. Utilisation du programme
5. Crédits

## Présentation du projet

La méthode de Padé-Rayleigh-Ritz est une méthode qui permet de trouver les éléments propres d'une matrice symétrique A de taille n. C'est une méthode itérative : elle ne calcule pas les solutions exactes mais une approximation des solutions exactes. Le nombre d'éléments propres souhaités est noté `m`.

## Structure du projet

* `logs/` : contient les résultats obtenus après exécution de notre implémentation de la méthode. Le nom du dossier est de la forme `m${valeur de m}` où `m` indique le nombre d'éléments propres souhaités. Ces dossiers contiennent des couples de fichiers : un fichier `.cfg` et un fichier `.dat`. Le fichier `.cfg` contient les paramètres à utiliser lors de l'exécution tandis que le fichier `.dat` contient les résultats obtenus. Il comporte dans cet ordre&nbsp;: le nombre d'itérations effectuées, le temps d'exécution en μs et le résidu maximal obtenu. Ces fichiers sont nommés ainsi : `${id}_${compilateur}_mpi-${nombre de processus MPI}_omp-${nombre de threads OpenMP}`.

* `matrices/` : contient les matrices utilisées pour tester notre implémentation de la méthode Padé-Rayleigh-Ritz. Seuls les formats `.txt` et `.mtx` sont supportés. Les fichiers doivent respecter une certaine nomenclature. L'exemple est ici une matrice 2x2. Ici `n` représente la dimension de la matrice symétrique, `nblines` le nombre de lignes du fichiers (ici le nombre de lignes représente le nombre d'éléments de la matrice) et `value` la valeur que l'on souhaite attribuer.

Pour utiliser une matrice au format mtx :
```
n n nblines
i j value
i j value
i j value
i j value
```

Pour utiliser une matrice au format txt :
```
n n 
value value
value value
```

* `plot/` : contient les scripts liés à la création des graphiques. Ces graphiques se basent sur les résultats obtenus dans le dossier `logs/`.

* `src/` : contient le code source, ici réalisé en langage C.

* `README.md` : ceci est le fichier que vous êtes en train de lire.

* `build.sh` : ceci est le script permettant de lancer la compilation pour obtenir les exécutables.

* `run_bench.sh` et `run_bench.cfg` : fichiers permettant de lancer le programme. `run_bench.sh` utilise le contenu de `run_bench.cfg` pour effectuer ses tests.

## Installation du programme

### Compilation

#### Outils nécessaires

Les outils suivants sont utilisés pour compiler notre programme :

* Le compilateur `gcc` avec les librairies `OpenBLAS` et `OpenMPI`.
* Le compilateur `icc` avec les librairies `MKL` et `Intel MPI`.

Le projet a été pensé et compilé pour le cluster [Ruche](https://mesocentre.pages.centralesupelec.fr/user_doc/) ce qui signifie que les modules et librairies sont à installer manuellement si vous ne comptez pas utiliser ce cluster. Si vous utiliser ce cluster les outils sont déjà installés.

#### Obtention des exécutables

Pour compiler le programme, il suffit de lancer `./build.sh`. Ceci produira quatre exécutables : 
* `prr_gcc` : version séquentielle utilisant le compilateur `gcc`
* `prr_icc` : version séquentielle utilisant le compilateur `icc`
* `multiprr_gcc` : version parallèle utilisant le compilateur `gcc`
* `multiprr_icc` : version parallèle utilisant le compilateur `icc`


## Utilisation du programme

Une fois les exécutables obtenus, il ne reste plus qu'à configurer les tests pour pouvoir lancer les exécutables.

### Fichier de configuration `run_bench.cfg`

Le fichier `run_bench.cfg` comme son nom l'indique est un fichier de configuration. Il permet de définir le contexte et les paramètres avec lesquelles le programme va se lancer. Il se définit comme ci-dessous :

```
#cc mpi omp filename m epsilon nb_rep freq max_it
icc 1 1 bcsstm12.mtx 10 1e-2 300 100 600
#icc 1 16 bcsstm12.mtx 10 1e-2 1000 100 1000
icc 8 16 bcsstk19.mtx 10 1e-2 1000 100 600
gcc 2 2 bcsstk19.txt 4 1e-2 4 100 0
gcc 1 3 bcsstk19.txt 4 1e-2 4 100 0
```

Le contenu de ce fichier est semblable à celui d'un fichier `.csv`. La première ligne représente l'en-tête et indique ce que chaque colonne représente. Une ligne représente une seule exécution. Ici un espace représente le séparateur. Un commentaire est représenté par une ligne qui commence par un dièse `#`. 

| Nom       | Signification |
| ----------|---------------|
| cc        | compilateur : `gcc` ou `icc`     |
| mpi       | nombre de processus MPI          |
| omp       | nombre de threads OpenMP     |
| filename  | nom du fichier contenant la matrice à traiter     |
| m         | nombre d'éléments propres à déterminer     |
| epsilon   | précision des résidus      |
| nb_rep    | nombre de fois que l'algorithme sera exécuté     |
| freq      | fréquence à laquelle les processus MPI vont communiquer     |
| max_it    | nombre d'itérations maximal lors d'une seule répétition     |

Il suffit donc de l'éditer avec n'importe quel éditeur de texte pour y ajouter les tests souhaités.

### Lancement des tests avec le fichier `run_bench.sh`

Une fois le fichier `run_config.cfg` modifié, il suffit de lancer `./run_bench.sh` pour lancer les tests. Les résultats des tests se retrouvent ensuite dans le dossier `logs/`.

## Crédits

### Auteurs 

* DOEBELE Grégoire et VIDAL Antoine : réalisation du projet
* EMAD Nahid : apport des connaissances et ressources sur la méthode Padé-Rayleigh-Ritz

### Outils

* Compilateurs `gcc` avec les librairies `OpenBLAS` et `OpenMPI` et `icc` avec les librairies `MKL` et `Intel MPI`
* Gnuplot pour la création des graphiques
* MatrixMarket et [Catonmat](https://catonmat.net/tools/generate-symmetric-matrices) pour l'obtention de matrices symétriques
* Visual Studio Code et VIM pour l'édition de texte
* Git et Github pour le versioning et l'hébergement des fichiers
* Overleaf (et donc LaTeX) pour la rédaction du rapport
