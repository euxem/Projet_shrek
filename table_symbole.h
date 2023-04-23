#ifndef __TABLE_SYMBOLE_H__
#define __TABLE_SYMBOLE_H__
////////////////////////////////////////////////////
// Include :
#include "analyse_lexicale.h"
#include "type_ast.h"
////////////////////////////////////////////////////
// structurate type -> Cellule de la table symbolique
struct cel
{
    Nature_Lexeme nature;
    char idf[256];
    char aff_char[256];
    float aff_float;
    struct cel *suivant;
};
typedef struct cel Cellule;
////////////////////////////////////////////////////
// structurate type -> Tete de la liste symbolique
struct lis
{
    Cellule *tete;
};
typedef struct lis Liste;
////////////////////////////////////////////////////
// Fonctions :
//////////////
// Créer une nouvelle relation entre un identifiant et une valeur
// renvoie 1 en cas d'erreur d'identificateurs, 0 sinon.
int creer_idf(Nature_Lexeme nature, char idf[256], char chaine[256], float val);
//////////////
// Trouver l'identifiant dans la table des symboles
// renvoie 1 en cas d'erreur d'identificateurs, 0 sinon.
int trouver_idf_char(char *idf, char *aff);
//////////////
// Trouver l'identifiant dans la table des symboles
// renvoie 1 en cas d'erreur d'identificateurs, 0 sinon.
int trouver_idf_float(char *idf, float *aff);
//////////////
// Applique une opération sur deux nombres
// renvoie 1 en cas d'erreur d'opération, 0 sinon.
int appliquer_operation(float *a, TypeAst op, float *b);
//////////////
// Applique l'opération de concaténation sur 2 chaînes de caractères.
// renvoie 1 en cas d'erreur d'opération, 0 sinon.
int appliquer_concat(char *a, TypeAst op, char *b, char **c);
//////////////
// Evalue l'arbre syntaxique et applique les opérations
// renvoie 1 en cas d'erreur d'évaluation, 0 sinon.
int evaluer(Ast A, float *f);
//////////////
// Evalue l'arbre syntaxique et applique les opérations
// renvoie 1 en cas d'erreur d'évaluation, 0 sinon.
int evaluer_char(Ast A, char **c);
//////////////
// Libère la mémoire allouée pour la table des symboles
void free_chainee();
//////////////
#endif
