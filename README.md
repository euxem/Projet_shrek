# Projet_shrek
Interpréteur d'un nouveau langage de programmation : SHREK.
SHREK est l'acroyme de Schematic Hierarchy for Representing Entities and Knowledge.

## Pourquoi utilisers SHREK ?
SHREK est un nouveau langage de programmation qui a pour but d'être traduit en fichier dot (langage graphique) afin de pouvoir représenter des informations sous forme de graphes.
Le langage dot est utilisé par de nombreux logiciels comme Graphviz, Gephi, etc. mais a pour défaut de n'être que déclaratif.
SHREK essaie de combler ce manque en proposant un langage de programmation impératif qui permet de définir des graphes complexes plus rapidement.

## Comment utiliser SHREK ?
SHREK est un langage de programmation qui se traduit en fichier dot avec un interpréteur.
Nous avons développé un interpréteur en C qu'il est possible d'utiliser en ligne de commande.
Pour l'utiliser, il suffit de compiler le projet avec la commande `make`. Un exécutable `shrek` est créé.

Il est possible d'utiliser l'interpréteur en ligne de commande avec les arguments suivants :
`./shrek2dot [-h] [-i <fichier.shrek>] [-o <fichier.dot>] [-f <dossier>]`
* `-h` : Affiche l'aide
* `-i` : Indique un fichier .shrek d'entrée (par défaut : stdin)
* `-o` : Indique un fichier .dot de sortie (par défaut : output.dot)
* `-f` : Indique un dossier de fichiers .shrek d'entrée (les dot compilés seront placés dans un sous dossier `dot_generes`)

Exemples d'usage de l'interpreteur :
```bash
./shrek -i data/shrek_examples/for_loop.shrek
```
```bash
./shrek -i data/shrek_examples/while_loop.shrek
```
```bash
./shrek -f data/shrek_examples
```
> L'utilisation de cette option avec le dossier data/shrek_errors renvoie 4 errerurs sur 5 fichiers car le 5ème est évalué en tant que fichier vide (commentaire non fermé).

## Comment écrire un programme SHREK ?
Un programme SHREK peut être composé de plusieurs instructions.
Chaque instruction est séparée par un point-virgule.
Il est possible d'écrire des commentaires en les encadrant entre `#`.

Les instructions disponibles sont les suivantes :
* `node:var = cat;` → Crée un noeud de label `nom` et l'assigne à la variable `var`.
* `link:a:b p c;` p Crée un lien entre les noeuds `a` et `b` de poids `p` et de couleur `c`. `p` et `c` sont optionnels. Attention, pour que l’instruction marche, il faut bien sûr que les nœuds `a` et `b` existent.
* `sub = nom`; → Crée un sous-graph  dans le fichier dot de label nom.
* `closesub` → Ferme le sous-graph ouvert.
* `for (var = 0; var < 10; var = var + 1) ... rof;`  → Boucle for classique. Doit être fermée par `rof;`. Les instructions de la boucle doivent être sur de nouvelles lignes.
* `while x <= 10 do ... done;` → Boucle while classique. Doit être fermée par `done;`. Les instructions de la boucle doivent être sur de nouvelles lignes.
* `var = eag;` → Affectation d'une valeur à une variable. L'affectation est la valeur obtenue par l’interprétation de l’eag.
* `a | "str" | b;` → Concaténation entre deux noeuds, deux chaines ou un de chaque ; peuvent être chainées. Renvoie une chaine de caractère composée du nom du node a, de "str" et du nom du node b concaténés. Peut être utilisé pour nommer des noeuds : `node:b = a | "bis";`.
* `read:var;` → Lit une valeur au clavier et l'affecte à la variable `var`.
* `write:type var;` → Affiche la valeur ou le nom de la variable `var` selon le type `type` (node, float). var est une eag si type est float, ou une chaine/concatenation/noeud avec node.
* `map:node [b,d,e] 'i,j,k';` → Crée trois noeuds nommés `b`, `d` et `e` et leur assigne les labels `i`, `j` et `k` respectivement. var peut être une chaine de caractère éventuellement concaténée.
* `map:link [a,b,c] [b,c,a] {1,2,3} ~blue,red,green~;` → Crée trois liens `a -- b`, `b -- c` et `c -- a` de poids `1`, `2` et `3` et de couleur `blue`, `red` et `green` respectivement. Les valeurs de poids ne peuvent pas être des eag, se sont des valeurs directe.
