#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "table_symbole.h"

Liste identifier;

Cellule *nouvelle_cellule_idf()
{
    return (Cellule *)malloc(sizeof(Cellule));
}

int creer_idf(Nature_Lexeme nature, char idf[256], char chaine[256], float val)
{
    Cellule *cel;
    int i, j, taille;
    bool b;
    b = true;
    switch (nature)
    {
    case STRING:
        cel = identifier.tete;
        while (cel != NULL)
        {
            if (strcmp(cel->idf, idf) == 0)
            {
                if (cel->nature != nature)
                {
                    perror("ERREUR: Type non reconnu, type attendu : STRING\n");
                    return 1;
                }
                strcpy(cel->aff_char, chaine);
                return 0;
            }
            cel = cel->suivant;
        }
        cel = (Cellule *)malloc(sizeof(Cellule));
        cel->nature = STRING;
        strcpy(cel->idf, idf);
        strcpy(cel->aff_char, chaine);
        cel->suivant = identifier.tete;
        identifier.tete = cel;
        break;
    case FLOAT:
        cel = identifier.tete;
        while (cel != NULL)
        {
            if (strcmp(cel->idf, idf) == 0)
            {
                if (cel->nature != nature)
                {
                    perror("ERREUR: Type non reconnu, type attendu : FLOAT\n");
                    return 1;
                }
                cel->aff_float = val;
                sprintf(chaine, "%g", val);
                strcpy(cel->aff_char, chaine);
                return 0;
            }
            cel = cel->suivant;
        }
        cel = (Cellule *)malloc(sizeof(Cellule));
        cel->nature = FLOAT;
        strcpy(cel->idf, idf);
        sprintf(chaine, "%g", val);
        strcpy(cel->aff_char, chaine);
        cel->aff_float = val;
        cel->suivant = identifier.tete;
        identifier.tete = cel;
        break;
    case L_STR:
        taille = strlen(idf);
        chaine[strlen(chaine) - 1] = '\0';
        for (i = taille; i > 0; i--)
        {
            if (idf[i - 1] == ']')
            {
                idf[i - 1] = '\0';
            }
            else if (idf[i - 1] == ',' || idf[i - 1] == '[')
            {
                cel = identifier.tete;
                while (cel != NULL && b)
                {
                    if (strcmp(cel->idf, &(idf[i - 1])) == 0)
                    {
                        if (cel->nature != nature)
                        {
                            perror("ERREUR: Type non reconnu, type attendu : LIST DE STRING\n");
                            return 1;
                        }
                        for (j = strlen(chaine); b == true; j--)
                        {
                            if (chaine[j - 1] == ',' || chaine[j - 1] == '\'')
                            {
                                strcpy(cel->aff_char, &(chaine[j]));
                                chaine[j - 1] = '\0';
                                b = false;
                            }
                        }
                    }
                    cel = cel->suivant;
                }
                if (b)
                {
                    cel = (Cellule *)malloc(sizeof(Cellule));
                    cel->nature = STRING;
                    strcpy(cel->idf, &(idf[i]));
                    for (j = strlen(chaine); b == true; j--)
                    {
                        if (chaine[j - 1] == ',' || chaine[j - 1] == '\'')
                        {
                            strcpy(cel->aff_char, &(chaine[j]));
                            chaine[j - 1] = '\0';
                            b = false;
                        }
                    }
                    cel->suivant = identifier.tete;
                    identifier.tete = cel;
                }
                idf[i - 1] = '\0';
                b = true;
            }
        }
        break;
    default:
        perror("ERREUR: Nature incorrecte dans la table symbolique.\n");
        return 1;
    }
    return 0;
}

int trouver_idf_char(char *idf, char *aff)
{
    Cellule *cel;
    cel = identifier.tete;
    while (cel != NULL && strcmp(idf, cel->idf) != 0)
    {
        cel = cel->suivant;
    }
    // Si idf n'est pas trouvé
    if (cel == NULL)
    {
        return 1;
    }
    // Si idf est trouvé
    strcpy(aff, cel->aff_char);
    return 0;
}

int trouver_idf_float(char *idf, float *aff)
{
    Cellule *cel;
    cel = identifier.tete;
    while (cel != NULL && strcmp(idf, cel->idf) != 0)
    {
        cel = cel->suivant;
    }
    if (cel == NULL)
    {
        return 1;
    }
    *aff = cel->aff_float;
    return 0;
}

int appliquer_operation(float *a, TypeAst op, float *b)
{
    int a1,b1;
    switch (op)
    {
    case N_DIV:
        if (*b == 0){
            perror("ERREUR: Division par 0.\n");
            afficher_lexeme(lexeme_courant());
            return 1;
        }
        *a = *a / *b;
        break;
    case N_PLUS:
        *a = *a + *b;
        break;
    case N_MOINS:
        *a = *a - *b;
        break;
    case N_MUL:
        *a = *a * *b;
        break;
    case N_MOD:
        a1 = (int)*a;
        b1 = (int)*b;
        *a = a1 % b1;
        break;
    default:
        perror("ERREUR: Opération non reconnue.\n");
        afficher_lexeme(lexeme_courant());
        return 1;
    }
    return 0;
}

int appliquer_concat(char *a, TypeAst op, char *b, char **c)
{
    if (op == N_CONCAT)
    {
        strcpy(*c, strcat(a, b));
        return 0;
    }
    else
    {
        perror("ERREUR: Opération non reconnue.\n");
        afficher_lexeme(lexeme_courant());
        return 1;
    }
}

int evaluer(Ast A, float* f)
{
    int erreur;
    float f_temp;
    switch (A->nature)
    {
    case N_IDF:
        erreur = trouver_idf_float(A->chaine, f);
        *f = (*f) * (A->valeur);
        return erreur;
    case N_FLOAT:
        *f = A->valeur;
        return 0;
    case N_PLUS:
    case N_DIV:
    case N_MOINS:
    case N_MOD:
    case N_MUL:
        erreur = evaluer(A->gauche,f) || evaluer(A->droite,&f_temp);
        if (erreur){
            return erreur;
        }
        if (A->valeur != 0)
        {
            erreur = erreur || appliquer_operation(f, A->nature,&f_temp);
            *f = *f * (A->valeur);
            return erreur;
        }
        return erreur || appliquer_operation(f, A->nature,&f_temp);
    default:
        perror("Erreur d'évaluation d'opération.\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : vous avez peut-être employé un opérateur non reconnu.\n");
        return 1;
        break;
    }
}

int evaluer_char(Ast A, char **c)
{
    char c1[256];
    Ast B = A->droite;
    if (A->nature == N_IDF)
    {
        if (trouver_idf_char(A->chaine, c1))
        {
            return 1;
        }
    }
    else
    {
        strcpy(c1, A->chaine);
    }
    if (B == NULL)
    {
        strcpy(*c, c1);
    }
    else
    {
        if (evaluer_char(B->droite, c) != 0)
        {
            return 1;
        }
        return appliquer_concat(c1, B->nature, *c, c);
    }
    return 0;
}

void free_chainee()
{
    Cellule *cel, *cel_temp;
    cel = identifier.tete;
    while (cel != NULL)
    {
        cel_temp = cel;
        cel = cel->suivant;
        cel_temp->suivant = NULL;
        free(cel_temp);
    }
    identifier.tete = NULL;
}
