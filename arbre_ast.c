#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "arbre_ast.h"

int SUB = 0;
int nb_sub = 0;

int interpreter(Ast A, FILE *f)
{
    char* temp_char;
    if (A == NULL)
    {
        return SUB;
    }
    switch (A->nature)
    {
    case N_SEPARATEUR:
        interpreter(A->gauche, f);
        interpreter(A->droite, f);
        break;
    case N_SI:
        if (condition(A->centre)){
            interpreter(A->droite, f);
        }else{
            interpreter(A->gauche, f);
        }
        break;
    case N_TANT_QUE:
        while (condition(A->centre)){
            interpreter(A->droite, f);
        }
        break;
    case N_FOR:
        interpreter(A->gauche, f);
        while (condition(A->centre))
        {
            interpreter(A->droite, f);
            temp_char=malloc(sizeof(char) * (256));
            creer_idf(FLOAT, A->gauche->gauche->chaine, temp_char, evaluer(A->gauche->centre));
            free(temp_char);
        }
        break;
        
    case N_SUB:
        interpreter_subtitle(f, A->gauche);
        break;
    case N_NODE:
        temp_char=malloc(sizeof(char) * (256));
        evaluer_char(A->droite, &temp_char);
        creer_idf(STRING, A->gauche->chaine, temp_char, 0);
        free(temp_char);
        break;
    case N_LIRENODE:
        temp_char=malloc(sizeof(char) * (256));
        printf("Veuillez rentrer un nom de noeuds pour %s : ",A->gauche->chaine);
        fgets(temp_char,256,stdin);
        creer_idf(STRING, A->gauche->chaine, temp_char, 0);
        free(temp_char);
        break;
    case N_LIREFLOAT:
        temp_char=malloc(sizeof(char) * (256));
        float flot=0.0;
        printf("Veuillez rentrer un nombre pour %s : ",A->gauche->chaine);
        fgets(temp_char, 256, stdin);
        temp_char[strlen(temp_char)-1]='\0';
        flot = atof(temp_char);
        creer_idf(FLOAT, A->gauche->chaine, temp_char, flot);
        free(temp_char);
        break;
    case N_ECRIREFLOAT:
        ecrire_float(evaluer(A->gauche));
        break;
    case N_ECRIRENODE:
        temp_char=malloc(sizeof(char) * (256));
        evaluer_char(A->gauche,&temp_char);
        ecrire_char(temp_char);
        free(temp_char);
        break;
    case N_AFFINT:
        temp_char=malloc(sizeof(char) * (256));
        creer_idf(FLOAT, A->gauche->chaine, temp_char, evaluer(A->droite));
        free(temp_char);
        break;
    case N_LINK:
        interpreter_link(f, A->gauche);
        break;
    case N_MAPLINK:
        interpreter_forlink(f, A->gauche);
        break;
    case N_MAPNODE:
        creer_idf(L_STR, A->gauche->chaine, A->droite->chaine, 0);
        break;
    case N_CLOSUB:
        if (!SUB){
            printf("ERROR: Subgraph non créer");
            exit(0);
        }
        SUB=0;
        fprintf(f,"\t}\n");
        break;
    default:
        printf("erreur:ast\n");
        break;
    }

    return SUB;
}

void interpreter_subtitle(FILE *f, Ast A)
{
    if (SUB){
        fprintf(f, "\t}\n");
        SUB = 0;
    }
    if (A != NULL && A->nature == N_STR)
    {
        fprintf(f,"\tsubgraph cluster_%d {\n", nb_sub);
        nb_sub++;
        fprintf(f,"\t\tlabel = \"%s\";\n",A->chaine);
        SUB = 1;
    }
    else
    {
        printf("Erreur title\n");
    }
}

void interpreter_link(FILE *f, Ast A)
{
    char aff[256];
    float flot;
    if (A == NULL || A->nature != N_STR)
    {
        printf("erreur\n");
    }
    if (SUB){
        fprintf(f,"\t");
    }
    trouver_idf_char(A->chaine, aff);
    fprintf(f, "\t%s --", aff);
    A = A->gauche;
    if (A == NULL || A->nature != N_STR)
    {
        printf("erreur\n");
    }
    trouver_idf_char(A->chaine, aff);
    fprintf(f, " %s", aff);
    if (A->gauche == NULL)
    {
        fprintf(f, ";\n");
        return;
    }
    flot = evaluer(A->gauche);
    fprintf(f, " [label=\"%g\", weight=%g", flot, flot);
    A = A->centre;
    if (A == NULL || A->nature != N_STR)
    {
        fprintf(f, "];\n");
        return;
    }
    fprintf(f, ", color=%s];\n", A->chaine);
}

void interpreter_forlink(FILE *f, Ast A)
{
    char aff[256];
    char liste1[256];
    char liste2[256];
    char liste_weight[256];
    char liste_col[256];
    bool b = true;
    int taille;
    typedef enum {NORMAL,WEIGHT,COLOR}CAS;
    CAS cas = NORMAL;
    int temp_1 = 1, temp_2 = 1, temp_weight = 1, temp_col = 1;
    if (A == NULL || A->nature != N_STR)
    {
        printf("erreur\n");
    }
    strcpy(liste1, A->chaine);
    A = A->gauche;
    if (A == NULL || A->nature != N_STR)
    {
        printf("erreur\n");
    }
    strcpy(liste2, A->chaine);
    A = A->gauche;
    if (A != NULL && A->nature == N_STR)
    {
        strcpy(liste_weight, A->chaine);
        A = A->gauche;
        cas=WEIGHT;
        if (A != NULL && A->nature == N_STR)
        {
            strcpy(liste_col, A->chaine);
            cas=COLOR;
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
                if (SUB){
                    fprintf(f,"\t");
                }
                liste1[i] = '\0';
                trouver_idf_char(&(liste1[temp_1]), aff);
                fprintf(f, "\t%s --", aff);
                temp_1 = i + 1;
                for (int j = temp_2; b == true; j++)
                {
                    if (liste2[j] == ',' || liste2[j] == ']')
                    {
                        liste2[j] = '\0';
                        trouver_idf_char(&(liste2[temp_2]), aff);
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
                if (SUB){
                    fprintf(f,"\t");
                }
                liste1[i] = '\0';
                trouver_idf_char(&(liste1[temp_1]), aff);
                fprintf(f, "\t%s --", aff);
                temp_1 = i + 1;
                for (int j = temp_2; b == true; j++)
                {
                    if (liste2[j] == ',' || liste2[j] == ']')
                    {
                        liste2[j] = '\0';
                        trouver_idf_char(&(liste2[temp_2]), aff);
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
                if (SUB){
                    fprintf(f,"\t");
                }
                liste1[i] = '\0';
                trouver_idf_char(&(liste1[temp_1]), aff);
                fprintf(f, "\t%s --", aff);
                temp_1 = i + 1;
                for (int j = temp_2; b == true; j++)
                {
                    if (liste2[j] == ',' || liste2[j] == ']')
                    {
                        liste2[j] = '\0';
                        trouver_idf_char(&(liste2[temp_2]), aff);
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
                    if (liste_col[j] == ',' || liste_col[j] == ')')
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
        printf("ERREUR: Interprétation forlink");
        break;
    }
}

void ecrire_char(char *s)
{
    printf("%s\n",s);
}

void ecrire_float(float f)
{
    printf("%g\n", f);
}

int condition(Ast A){
    float a,b;
    a=evaluer(A->gauche);
    b=evaluer(A->droite);
    if (strcmp(A->chaine,"==")==0){
        return a==b;
    }
    if (strcmp(A->chaine,"<=")==0){
        return a<=b;
    }
    if (strcmp(A->chaine,">=")==0){
        return a>=b;
    }
    if (strcmp(A->chaine,"<>")==0){
        return a!=b;
    }
    if (strcmp(A->chaine,"<")==0){
        return a<b;
    }
    if (strcmp(A->chaine,">")==0){
        return a>b;
    }
    printf("Erreur Bool");
    exit(1);
}
