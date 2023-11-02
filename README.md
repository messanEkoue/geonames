# GeoNames

## Description

Le projet `Geonames` propose des données géographiques. Et à l'aide de ces
données nous allons afficher les villes les plus peuplées au monde, selon
le base de données fournie et en supposant que cette dernière est à jour.
Pour se faire nous allons créer un exécutable `geonames`. Et avec la commande:

```sh
./geonames N
```

afficher les N villes les plus peuplées du monde.

## Auteur

Messan, Ekoué Andy Scott 

## Fonctionnement

Tout d'abord, afin de s'assurer qu'il n'y ait pas de fichiers indésirables:

```sh
make clean
```

Ensuite, il faut télécharger la base de donnée à utiliser:

```sh
make database
```

Puis, il faut lancer la commande :

```sh
make
```

pour compiler le programme.
Pour finir, on peut executer le programme en faisant :

```sh
./geonames N
```

en remplaçant le N par le nombre de villes que l'on veut afficher.

## Tests

Pour lancer la suite de tests, il faut faire la commande:

```sh
make test
```

Cette commande va d'abord compiler le programme puis exécuter les test ensuite.

## Dépendances

* GCC  
* [Bats](https://github.com/bats-core/bats-core)
* pandoc

## Références

* Pour le fichier [.gitignore](https://www.toptal.com/developers/gitignore/) généré
* Pour l'implementation de la fonction de [tri](http://www.java2s.com/Code/C/Data-Structure-Algorithm/AQuicksortforstructuresoftypeaddress.htm). 
