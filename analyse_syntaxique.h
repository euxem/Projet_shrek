#ifndef __ANALYSE_SYNTAXIQUE_H__
#define __ANALYSE_SYNTAXIQUE_H__

/*---------------------------------------------------------------*/
// Include :
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "type_ast.h"

/*---------------------------------------------------------------*/
// Fonctions :
/* Les fonctions réccursive suivante permette de créer un arbre ast (voir type_ast.h) après lecture d'un fichier .shrek,
Chaque fonction renvoie 0, 1 ou 2 en fonction de l'erreur*/
//////////
/* Analyse le fichier, une fois cette fonction termier, on obtient dans l'arbre Ast complet et le fichier a atteint sa fin*/
int Rec_pgm(Ast *A);
//////////
/* Analyse toutes les instructions, une fois cette fonction termier, on obtient dans l'arbre Ast complet*/
int Rec_seq_inst(Ast *A);
//////////
/* 
Analyse si l'instruction en cours est un :
    - map,
    - if,
    - while,
    - for,
    - write,
    - closesub
    - ou link, 
    - sinon appelle Rec_seq_aff,
A la fin de cette fonction, on obtient un sous-arbre Ast correspondant à l'instruction en cours
*/
int Rec_inst(Ast *A);
//////////
/* 
Détermine si il y a une autre instruction a analyser :
    - Si oui, appelle Rec_seq_inst
    - Si non, termine l'analyse (return 0)
*/
int Rec_suite_seq_inst(Ast *A, Ast *A1);
//////////
/* 
Analyse si l'instruction en cours est une affectation de:
    - réel (IDF)
    - node
    - lecture (node ou réel)
    - subgraph (nom du subgraph)
A la fin de l'exécution, on obtient un sous-arbre Ast correspondant a une instruction d'affectation
*/
int Rec_seq_aff(Ast *A);
//////////
/*
Analyse la suite du mot clé d'instruction d'écriture :
    - node (écrire le nom du node, chaîne de caractères correspondant à l'identificateur)
    - float (écrire le réel correspondant à l'identificateur)
A la fin de l'exécution, on obitent un sous-arbre Ast correspondant à l'instruction write
*/
int Rec_suite_ecrire(Ast *A);
//////////
/*
Analyse la suite du mot clé d'instruction node (si il y a une affectaion, symbole de concaténation...)
A la fin de l'exécution, on obtient un sous-arbre Ast correspondant à l'instruction node
*/
int Rec_suite_node(Ast *A);
//////////
/*
Analyse la suite du mot clé d'instruction map :
    - node (affection multiple d'identificateur à une chaîne de caractère)
    - link (création multiple de lien entre noeuds)
A la fin de l'exécution, on obitient un sous-arbre Ast correspondant à l'instruction map
*/
int Rec_suite_map(Ast *A);
//////////
/*
Analyse la suite du mot clé d'instruction link (si il y a un poids, une couleur)
A la fin de l'exécution, on obtitent un sous-arbre Ast correspondant à l'instruction link
*/
int Rec_suite_link(Ast *A);
//////////
/*
Analyse une condtion (opérateur booléen) :
A la fin de l'exécution, on obient un sous-arbre Ast que nous pouvons interpréter en tant que condition
*/
int Rec_condition(Ast *A);
//////////
/*
Analyse une possible concaténation :
    - Si le symbole est présent et la syntaxe respecter :
        -> on complète l'arbre Ast et on rappelle la fonction
    - Si le symbole n'est pas présent :
        -> on fait rien (return 0)
*/
int Rec_suite_concat(Ast *A);
//////////
/*
Analyse les expressions ayant besoin d'être calculé (TP calculatrice)
A la fin de l'exécution, on obtient un arbre Ast pouvant être interpréter en tant que calcul.
*/
int Rec_eag(Ast *A);
//////////
/*
Analyse les expressions ayant besoin d'être calculé (TP calculatrice)
A la fin de l'exécution, on obtient un sous-arbre Ast pouvant être interpréter en tant que calcul.
*/
int Rec_seq_terme(Ast *A);
//////////
/*
Analyse le terme d'une l'expression
A la fin de l'exécution, on obtient un sous-arbre Ast pouvant être interpréter comme un floattant, un moins unaire, un produit, une division ou une eag parenthésé.
*/
int Rec_terme(Ast *A);
//////////
/*
Analyse l'opérateur en fonction de la nature du lexeme suivant :
    - Le lexeme est un opérateur +, - ou %
        -> Construction de l'arbre Ast correspondant (Appel de Rec_terme puis Rec_suite_seq_terme)
    - Le lexeme est diférent
        -> Fin de l'eag (sous-arbre dans A)
*/
int Rec_suite_seq_terme(Ast A1, Ast *A);
//////////
/*
Analyse la séquence de facteur d'une l'expression
A la fin de l'exécution, on obtient un sous-arbre Ast pouvant être interpréter comme un floattant, une eag parenthésé, un produit ou une division.
*/
int Rec_seq_facteur(Ast *A);
//////////
/*
Analyse le facteur d'une l'expression
A la fin de l'exécution, on obtient un sous-arbre Ast pouvant être interpréter comme un floattant, un moins unaire ou une eag parenthésé.
*/
int Rec_facteur(Ast *A);
//////////
/*
Analyse l'opérateur en fonction de la nature du lexeme suivant :
    - Le lexeme est un opérateur * ou /
        -> Construction de l'arbre Ast correspondant (Appel de Rec_facteur puis Rec_suite_seq_facteur)
    - Le lexeme est diférent
        -> Fin de l'eag (sous-arbre dans A)
*/
int Rec_suite_seq_facteur(Ast A1, Ast *A);
//////////
/*
Predicat :
    -> vrai si le lexème est un opérateur +, - ou %
    -> faux sinon
*/
int op1(Ast *A);
//////////
/*
Predicat :
    -> vrai si le lexème est un opérateur *, /
    -> faux sinon
*/
int op2(Ast *A);
//////////
// Fonction principale de l'analyseur syntaxique
// Appelle la fonction de démarrage de lecture du fichier
// Et appelle la fonction de reconnaissance de la grammaire
/* Renvoie :
        - 0 si il n'y a pas d'erreur
        - 1 si il y a erreur syntaxique, le main affiche un warning syntaxique
        - 2 si il y a erreur lexicale, le main affiche un warning lexicale
        - -1 si le fichier est vide
*/
int analyse(char *nom_ficher, Ast *arbre);

#endif
