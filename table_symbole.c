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

void creer_idf(Nature_Lexeme nature, char idf[256], char chaine[256], float val)
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
                    printf("Erreur de type\n");
                    exit(2);
                }
                strcpy(cel->aff_char, chaine);
                return;
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
                    printf("Erreur de type %d\n", cel->nature);
                    exit(2);
                }
                cel->aff_float = val;
                sprintf(chaine, "%g", val);
                strcpy(cel->aff_char, chaine);
                return;
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
                            printf("Erreur de type\n");
                            exit(2);
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
        printf("erreur dans la table symbolique\n");
        exit(2);
        break;
    }
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

void trouver_idf_float(char *idf, float *aff)
{
    Cellule *cel;
    cel = identifier.tete;
    while (cel != NULL && strcmp(idf, cel->idf) != 0)
    {
        cel = cel->suivant;
    }
    if (cel == NULL)
    {
        return;
    }
    *aff = cel->aff_float;
    return;
}

float appliquer_operation(float a, char *o, float b)
{
    if (*o == '+')
    {
        return a + b;
    }
    else if (*o == '-')
    {
        return a - b;
    }
    else if (*o == '%')
    {
        int a1 = (int)a;
        int b1 = (int)b;
        return a1 % b1;
    }
    else if (*o == '*')
    {
        return a * b;
    }
    else if (*o == '/')
    {
        return a / b;
    }
    else
    {
        perror("ERREUR: Opération non reconnue.\n");
        afficher_lexeme(lexeme_courant());
        exit(1);
    }
}

void appliquer_concat(char *a, char *o, char *b, char **c)
{
    if (*o == '|')
    {
        strcpy(*c, strcat(a, b));
    }
    else
    {
        perror("ERREUR: Opération non reconnue.\n");
        afficher_lexeme(lexeme_courant());
        exit(1);
    }
}

float evaluer(Ast A)
{
    float f;
    switch (A->nature)
    {
    case N_STR:
        trouver_idf_float(A->chaine, &f);
        return f * (A->valeur);
    case N_FLOAT:
        return A->valeur;
    case N_OP:
        if (A->valeur != 0)
        {
            return appliquer_operation(evaluer(A->gauche), A->chaine, evaluer(A->droite)) * (A->valeur);
        }
        return appliquer_operation(evaluer(A->gauche), A->chaine, evaluer(A->droite));
    default:
        perror("Erreur d'évaluation d'opération.\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : vous avez peut-être employé un opérateur non reconnu.\n");
        exit(1);
        break;
    }
}

int evaluer_char(Ast A, char **c)
{
    char c1[256];
    Ast B = A->droite;
    if (A->nature == N_STR && A->valeur == 0)
    {
        if (trouver_idf_char(A->chaine, c1) != 0)
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
        appliquer_concat(c1, B->chaine, *c, c);
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
