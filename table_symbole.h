#ifndef __TABLE_SYMBOLE_H__
#define __TABLE_SYMBOLE_H__

# include "analyse_lexicale.h"
# include "type_ast.h"


struct cel{
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

void creer_idf(Nature_Lexeme nature, char idf[256], char chaine[256] ,float val);
void trouver_idf_char(char* idf, char* aff);
void trouver_idf_float(char *idf, float *aff);
float appliquer_operation(float a, char *o, float b);
float evaluer(Ast A);
void evaluer_char(Ast A, char** c);
void free_chainee();

#endif
