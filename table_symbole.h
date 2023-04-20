#ifndef __TABLE_SYMBOLE_H__
#define __TABLE_SYMBOLE_H__

#include "analyse_lexicale.h"
#include "type_ast.h"

struct cel
{
    Nature_Lexeme nature;
    char idf[256];
    char aff_char[256];
    float aff_float;
    struct cel *suivant;
};

typedef struct cel Cellule;

struct lis
{
    Cellule *tete;
};

typedef struct lis Liste;

// Créer une nouvelle relation entre un identifiant et une valeur
int creer_idf(Nature_Lexeme nature, char idf[256], char chaine[256], float val);

// Trouver l'identifiant dans la table des symboles
int trouver_idf_char(char *idf, char *aff);

// Trouver l'identifiant dans la table des symboles
int trouver_idf_float(char *idf, float *aff);

// Applique une opération sur deux nombres
int appliquer_operation(float *a, char *o, float *b);

// Evalue l'arbre syntaxique et applique les opérations
int evaluer(Ast A, float *f);

// Evalue l'arbre syntaxique et applique les opérations
int evaluer_char(Ast A, char **c);

// Libère la mémoire allouée pour la table des symboles
void free_chainee();

#endif
