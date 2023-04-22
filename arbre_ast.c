#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "arbre_ast.h"

void print_tab_in_subgraph(FILE* f, int *nb_clause, int *nb_sub){
    for (int i = (*nb_sub)-(*nb_clause); i>0; i--){
        fprintf(f, "\t");
    }
}

// Fonction de traduction d'arbre en fichier .dot
// Renvoie 1 si un subgraph est ouvert, 0 sinon
int interpreter(Ast A, FILE *f, int* nb_clause, int* nb_sub)
{
    char *temp_char;
    int erreur=0;
    float flot=0.0;
    int res_cond_temp;
    if (A == NULL)
    {
        return 0;
    }
    switch (A->nature)
    {
    case N_SEPARATEUR:
        erreur = interpreter(A->gauche, f, nb_clause, nb_sub) || interpreter(A->droite, f, nb_clause, nb_sub);
        return erreur;
    case N_SI:
        switch (condition(A->centre))
        {
        case 0:
            erreur = interpreter(A->droite, f, nb_clause, nb_sub);
            return erreur;
        case -1:
            erreur = interpreter(A->gauche, f, nb_clause, nb_sub);
            return erreur;
        default:
            return 1;
        }
    case N_TANT_QUE:
        res_cond_temp = condition(A->centre); 
        while (res_cond_temp == 0 && !erreur)
        {
            erreur = erreur || interpreter(A->droite, f, nb_clause, nb_sub);
            res_cond_temp = condition(A->centre);
        }
        if (res_cond_temp == 1){
            return 1;
        }
        return erreur;
    case N_FOR:
        erreur = interpreter(A->gauche, f, nb_clause, nb_sub);
        res_cond_temp = condition(A->centre); 
        while (res_cond_temp == 0 && !erreur)
        {
            erreur = erreur || interpreter(A->droite, f, nb_clause, nb_sub);
            temp_char = malloc(sizeof(char) * (256));
            erreur = erreur || evaluer(A->gauche->centre,&flot);
            erreur = erreur || creer_idf(FLOAT, A->gauche->gauche->chaine, temp_char, flot);
            free(temp_char);
            res_cond_temp = condition(A->centre);
        }
        if (res_cond_temp == 1){
            return 1;
        }
        return erreur;

    case N_SUB:
        erreur = interpreter_subtitle(f, A->gauche, nb_clause, nb_sub);
        return erreur;
    case N_NODE:
        temp_char = malloc(sizeof(char) * (256));
        if (evaluer_char(A->droite, &temp_char) != 0)
        {
            perror("Erreur AST : identificateur non défini");
            return 1;
        }
        erreur = creer_idf(STRING, A->gauche->chaine, temp_char, 0);
        free(temp_char);
        return erreur;
    case N_LIRENODE:
        temp_char = malloc(sizeof(char) * (256));
        printf("Veuillez rentrer un nom de noeuds pour %s : ", A->gauche->chaine);
        fgets(temp_char, 256, stdin);
        erreur = creer_idf(STRING, A->gauche->chaine, temp_char, 0);
        free(temp_char);
        return erreur;
    case N_LIREFLOAT:
        temp_char = malloc(sizeof(char) * (256));
        printf("Veuillez rentrer un nombre pour %s : ", A->gauche->chaine);
        fgets(temp_char, 256, stdin);
        temp_char[strlen(temp_char) - 1] = '\0';
        flot = atof(temp_char);
        erreur = creer_idf(FLOAT, A->gauche->chaine, temp_char, flot);
        free(temp_char);
        return erreur;
    case N_ECRIREFLOAT:
        erreur = evaluer(A->gauche, &flot);
        ecrire_float(flot);
        return erreur;
    case N_ECRIRENODE:
        temp_char = malloc(sizeof(char) * (256));
        if (evaluer_char(A->gauche, &temp_char) != 0)
        {
            perror("Erreur AST : identificateur non défini");
            return 1;
        }
        ecrire_char(temp_char);
        free(temp_char);
        return 0;
    case N_AFFINT:
        temp_char = malloc(sizeof(char) * (256));
        erreur = evaluer(A->droite, &flot);
        erreur = erreur || creer_idf(FLOAT, A->gauche->chaine, temp_char, flot);
        free(temp_char);
        return erreur;
    case N_LINK:
        erreur = interpreter_link(f, A->gauche, nb_clause, nb_sub);
        return erreur;
    case N_MAPLINK:
        erreur = interpreter_maplink(f, A->gauche, nb_clause, nb_sub);
        return erreur;
    case N_MAPNODE:
        erreur = creer_idf(L_STR, A->gauche->chaine, A->droite->chaine, 0);
        return erreur;
    case N_CLOSUB:
        (*nb_clause)++;
        if (nb_clause > nb_sub)
        {
            perror("Erreur AST : Subgraph non créé");
            return 1;
        }
        print_tab_in_subgraph(f, nb_clause, nb_sub);
        fprintf(f, "\t}\n");
        return 0;
    default:
        perror("Erreur AST : noeud inconnu\n");
        printf("Nature : %d\n", A->nature);
        printf("Chaine : %s\n", A->chaine);
        return 1;
    }
}

int interpreter_subtitle(FILE *f, Ast A, int* nb_clause, int* nb_sub)
{
    if (A != NULL && A->nature == N_STR)
    {
        (*nb_sub)++;
        print_tab_in_subgraph(f, nb_clause, nb_sub);
        fprintf(f, "subgraph cluster_%d {\n", *nb_sub);
        print_tab_in_subgraph(f, nb_clause, nb_sub);
        fprintf(f, "\tlabel = \"%s\";\n", A->chaine);
        return 0;
    }
    else
    {
        perror("Erreur AST : titre subgraph de mauvais type\n");
        return 1;
    }
}

int interpreter_link(FILE *f, Ast A, int* nb_clause, int* nb_sub)
{
    char aff[256];
    float flot;
    int erreur;
    if (A == NULL || A->nature != N_IDF)
    {
        perror("Erreur AST : noeud de mauvais type\n");
        printf("Link a besoin d'une chaine\n");
    }
    print_tab_in_subgraph(f, nb_clause, nb_sub);
    if (trouver_idf_char(A->chaine, aff)){
        return 1;
    }
    fprintf(f, "\t%s --", aff);
    A = A->gauche;
    if (A == NULL || A->nature != N_IDF)
    {
        perror("Erreur AST : noeud de mauvais type\n");
        printf("Link a besoin d'une chaine\n");
        return 1;
    }
    if (trouver_idf_char(A->chaine, aff)){
        return 1;
    }
    fprintf(f, " %s", aff);
    if (A->gauche == NULL)
    {
        fprintf(f, ";\n");
        return 0;
    }
    erreur = evaluer(A->gauche,&flot);
    fprintf(f, " [label=\"%g\", weight=%g", flot, flot);
    A = A->centre;
    if (A == NULL || A->nature != N_STR)
    {
        fprintf(f, "];\n");
        return erreur;
    }
    fprintf(f, ", color=%s];\n", A->chaine);
    return erreur;
}

int interpreter_maplink(FILE *f, Ast A, int* nb_clause, int* nb_sub)
{
    char aff[256];
    char liste1[256];
    char liste2[256];
    char liste_weight[256];
    char liste_col[256];
    bool b = true;
    int taille;
    typedef enum
    {
        NORMAL,
        WEIGHT,
        COLOR
    } CAS;
    CAS cas = NORMAL;
    int temp_1 = 1, temp_2 = 1, temp_weight = 1, temp_col = 1;
    if (A == NULL || A->nature != N_IDF)
    {
        perror("Erreur AST : noeud de mauvais type\n");
        printf("Forlink a besoin d'une chaine\n");
        return 1;
    }
    strcpy(liste1, A->chaine);
    A = A->gauche;
    if (A == NULL || A->nature != N_IDF)
    {
        perror("Erreur AST : noeud de mauvais type\n");
        printf("Forlink a besoin d'une chaine\n");
        return 1;
    }
    strcpy(liste2, A->chaine);
    A = A->gauche;
    if (A != NULL && A->nature == N_STR)
    {
        strcpy(liste_weight, A->chaine);
        A = A->gauche;
        cas = WEIGHT;
        if (A != NULL && A->nature == N_STR)
        {
            strcpy(liste_col, A->chaine);
            cas = COLOR;
        }
    }
    switch (cas)
    {
    case NORMAL:
        taille = strlen(liste1);
        for (int i = 1; i < taille; i++)
        {
            if (liste1[i] == ',' || liste1[i] == ']')
            {
                print_tab_in_subgraph(f, nb_clause, nb_sub);
                liste1[i] = '\0';
                if (trouver_idf_char(&(liste1[temp_1]), aff)){
                    return 1;
                }
                fprintf(f, "\t%s --", aff);
                temp_1 = i + 1;
                for (int j = temp_2; b == true; j++)
                {
                    if (liste2[j] == ',' || liste2[j] == ']')
                    {
                        liste2[j] = '\0';
                        if (trouver_idf_char(&(liste2[temp_2]), aff)){
                            return 1;
                        }
                        fprintf(f, " %s;\n", aff);
                        temp_2 = j + 1;
                        b = false;
                    }
                }
                b = true;
            }
        }
        break;

    case WEIGHT:
        taille = strlen(liste1);
        for (int i = 1; i < taille; i++)
        {
            if (liste1[i] == ',' || liste1[i] == ']')
            {
                print_tab_in_subgraph(f, nb_clause, nb_sub);
                liste1[i] = '\0';
                if (trouver_idf_char(&(liste1[temp_1]), aff)){
                    return 1;
                }
                fprintf(f, "\t%s --", aff);
                temp_1 = i + 1;
                for (int j = temp_2; b == true; j++)
                {
                    if (liste2[j] == ',' || liste2[j] == ']')
                    {
                        liste2[j] = '\0';
                        if (trouver_idf_char(&(liste2[temp_2]), aff)){
                            return 1;
                        }
                        fprintf(f, " %s", aff);
                        temp_2 = j + 1;
                        b = false;
                    }
                }
                b = true;
                for (int j = temp_weight; b == true; j++)
                {
                    if (liste_weight[j] == ',' || liste_weight[j] == '}')
                    {
                        liste_weight[j] = '\0';
                        fprintf(f, " [label=\"%s\", weight=%s];\n", &(liste_weight[temp_weight]), &(liste_weight[temp_weight]));
                        temp_weight = j + 1;
                        b = false;
                    }
                }
                b = true;
            }
        }
        break;

    case COLOR:
        taille = strlen(liste1);
        for (int i = 1; i < taille; i++)
        {
            if (liste1[i] == ',' || liste1[i] == ']')
            {
                print_tab_in_subgraph(f, nb_clause, nb_sub);
                liste1[i] = '\0';
                if (trouver_idf_char(&(liste1[temp_1]), aff)){
                    return 1;
                }
                fprintf(f, "\t%s --", aff);
                temp_1 = i + 1;
                for (int j = temp_2; b == true; j++)
                {
                    if (liste2[j] == ',' || liste2[j] == ']')
                    {
                        liste2[j] = '\0';
                        if (trouver_idf_char(&(liste2[temp_2]), aff)){
                            return 1;
                        }
                        fprintf(f, " %s", aff);
                        temp_2 = j + 1;
                        b = false;
                    }
                }
                b = true;
                for (int j = temp_weight; b == true; j++)
                {
                    if (liste_weight[j] == ',' || liste_weight[j] == '}')
                    {
                        liste_weight[j] = '\0';
                        fprintf(f, " [label=\"%s\", weight=%s,", &(liste_weight[temp_weight]), &(liste_weight[temp_weight]));
                        temp_weight = j + 1;
                        b = false;
                    }
                }
                b = true;
                for (int j = temp_col; b == true; j++)
                {
                    if (liste_col[j] == ',' || liste_col[j] == '~')
                    {
                        liste_col[j] = '\0';
                        fprintf(f, " color=%s];\n", &(liste_col[temp_col]));
                        temp_col = j + 1;
                        b = false;
                    }
                }
                b = true;
            }
        }
        break;

    default:
        perror("Erreur AST : Interprétation forlink\n");
        return 1;
    }
    return 0;
}

void ecrire_char(char *s)
{
    printf("%s\n", s);
}

void ecrire_float(float f)
{
    printf("%g\n", f);
}

int condition(Ast A)
{
    float a, b;
    int erreur;
    erreur = evaluer(A->gauche,&a);
    erreur = erreur || evaluer(A->droite,&b);
    if (erreur){
        return 1;
    }
    if (strcmp(A->chaine, "==") == 0)
    {
        return ((a == b) - 1);
    }
    if (strcmp(A->chaine, "<=") == 0)
    {
        return ((a <= b) - 1);
    }
    if (strcmp(A->chaine, ">=") == 0)
    {
        return ((a >= b) - 1);
    }
    if (strcmp(A->chaine, "<>") == 0)
    {
        return ((a != b) - 1);
    }
    if (strcmp(A->chaine, "<") == 0)
    {
        return ((a < b) - 1);
    }
    if (strcmp(A->chaine, ">") == 0)
    {
        return ((a > b) - 1);
    }
    perror("Erreur AST : mauvaise interprétation de booléen\n");
    return 1;
}
