#ifndef __ARBRE_AST_H__
#define __ARBRE_AST_H__
////////////////////////////////////////////////////
// INCLUDES :
# include "type_ast.h"
# include "table_symbole.h"
////////////////////////////////////////////////////
// Fonctions :
//////////////
// Permet de décaler dans le fichier .dot f la ligne en cours d'écriture dans le subgraph en cours d'ouverture :
/*
Exemple:
graph G {
    subgraph 1 {
        a -- b;  <- cette ligne est dans le subgraph 1 grâce à cette focntion
    }
}*/
void print_tab_in_subgraph(FILE *f, int* nb_clause, int* nb_sub);
//////////////
// Interprête un arbre Ast pour créer un fichier .dot (fichier f).
// Si l'arbre Ast à une erreur de construction, return 1, sinon return 0.
int interpreter(Ast A, FILE* f, int* nb_clause, int* nb_sub);
//////////////
// Interprête un sous-arbre Ast représentant l'action de création de subgraph :
// Si l'arbre Ast à une erreur de construction, return 1, sinon return 0.
// Ecrit le subgraph dans le fichier .dot
int interpreter_subtitle(FILE* f, Ast A, int* nb_clause, int* nb_sub);
//////////////
// Interprête un sous-arbre Ast représentant l'action de création de lien entre 2 noeuds :
// Si l'arbre Ast à une erreur de construction, return 1, sinon return 0.
// Ecrit le lien dans le fichier .dot
int interpreter_link(FILE* f, Ast A, int* nb_clause, int* nb_sub);
//////////////
// Interprête un sous-arbre Ast représentant l'action de création de liens multiples entre plusieurs noeuds :
// Si l'arbre Ast à une erreur de construction, return 1, sinon return 0.
// Ecrit les liens dans le fichier .dot
int interpreter_maplink(FILE* f, Ast A, int* nb_clause, int* nb_sub);
//////////////
void ecrire_float(float f);
//////////////
void ecrire_char(char *idf);
//////////////
// Prédicat, analyse une expression de type eag OPERATEUR_BOOLEEN eag
//          -> return 0 si l'expression est vrai
//          -> return -1 sinon
// Si l'arbre Ast à une erreur de construction, return 1.
int condition(Ast A);
////////////////////////////////////////////////////

#endif
