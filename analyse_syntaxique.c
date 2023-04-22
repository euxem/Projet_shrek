#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse_syntaxique.h"
#include "type_ast.h"
//////////////////////////////////////////////////////////////////
/*
Information globale sur le programme :
    - La variable erreur de type int permet de retourner le code erreur
    des fonctions récursives :
        - erreur = 0 -> pas d'erreur
        - erreur = 1 -> erreur d'analyse, le main affiche un warning
    - La variable Ast *A représente l'endoit dans notre arbre principale ou la fonction
    en cours d'exécution doit insérer son sous-arbre.
    - Les autres variables de type floattante, Ast ou char sont elles utilisé pour les calculs
    intermédiaires des fonctions.
*/
//////////////////////////////////////////////////////////////////
// Fontions Récursives :
int Rec_pgm(Ast *A)
{
    
    if (Rec_seq_inst(A) != 0)
    {
        return 1; // Si erreur
    }
    if (lexeme_courant().nature != FIN)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Deux points attendu.\n");
        return 1; // Si erreur
    }

    return 0; // Aucune erreur
}
////////////
int Rec_seq_inst(Ast *A)
{
    Ast A1;
    if (Rec_inst(&A1) != 0)
    {
        return 1; // Si erreur
    }
    if (Rec_suite_seq_inst(A, &A1) != 0)
    {
        return 1; // Si erreur
    }
    return 0; // Aucune erreur
}
////////////
int Rec_suite_seq_inst(Ast *A, Ast *A1)
{
    if (lexeme_courant().nature != P_VIRG)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Point virgule attendu.\n");
        return 1; // Si erreur
    }
    if (avancer() != 0)
        {
            return 1; // Si erreur
        }
    if (lexeme_courant().nature == FIN || lexeme_courant().nature == SINON || lexeme_courant().nature == FSI || lexeme_courant().nature == FAIT || lexeme_courant().nature == ROF)
    {
        (*A) = (*A1);
        return 0; // Aucune erreur
    }
    (*A) = nouvelle_cellule_ast();
    (*A)->nature = N_SEPARATEUR;
    (*A)->gauche = (*A1);
    return Rec_seq_inst(&((*A)->droite));
}
////////////
int Rec_inst(Ast *A)
{
    Ast A1;
    switch (lexeme_courant().nature)
    {
    case MAP:
        /*
        L'erreur de type 2 ne peux être envoyé que si le lexeme est une liste donc [], ~~, {}, '' 
        Donc l'erreur de avancer nous intéresse que si le mot clé est map, sinon c'est une erreur syntaxique.
        */
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_suite_map(A) != 0)
        {
            return 1; // Si erreur
        }
        break;

    case SI:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_SI;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_condition(A) != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != ALORS)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un if nécessite un then.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        Rec_seq_inst(&A1);
        if (lexeme_courant().nature != SINON && lexeme_courant().nature != FSI)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un if doit continuer avec un else ou se terminer par un fi.\n");
            return 1; // Si erreur
        }
        (*A)->droite = A1;
        if (lexeme_courant().nature == SINON)
        {
            if (avancer() != 0)
            {
                return 1; // Si erreur
            }
            Rec_seq_inst(&A1);
            if (lexeme_courant().nature != FSI)
            {
                perror("Erreur syntaxique :\n");
                afficher_lexeme(lexeme_courant());
                printf("HINT : Un if doit se terminer par un fi.\n");
                return 1; // Si erreur
            }
            (*A)->gauche = A1;
        }
        else
        {
            (*A)->gauche = NULL;
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        break;

    case TANT_QUE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_TANT_QUE;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_condition(A) != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != FAIRE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un while nécessite un do.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        Rec_seq_inst(&A1);
        if (lexeme_courant().nature != FAIT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un while doit se terminer par un done.\n");
            return 1; // Si erreur
        }
        (*A)->droite = A1;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        break;

    case FOR:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_FOR;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != PARO)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for attends ses arguments dans une parenthèse.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != IDF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for attends un identificateur en premier argument.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1; // Si erreur
        }
        if (Rec_seq_aff(&A1) != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != P_VIRG)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les arguments du for doivent être séparés par des points virgules.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1; // Si erreur
        }
        (*A)->gauche = A1;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_condition(A) != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != P_VIRG)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les arguments du for doivent être séparés par des points virgules.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_eag(&A1) != 0){
            return 1; // Si erreur
        }
        (*A)->gauche->centre = A1;
        if (lexeme_courant().nature != PARF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Fermez la parenthèse du for.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != FAIRE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for nécessite un do.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        Rec_seq_inst(&A1);
        if (lexeme_courant().nature != ROF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for doit se terminer par un rof.\n");
            return 1; // Si erreur
        }
        (*A)->droite = A1;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        break;

    case CLOSUB:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_CLOSUB;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        break;

    case LINK:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_LINK;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != IDF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Identificateur attendu.\n");
            return 1; // Si erreur
        }
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        A1->nature = N_IDF;
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != IDF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Identificateur attendu.\n");
            return 1; // Si erreur
        }
        A1->gauche = nouvelle_cellule_ast();
        A1 = A1->gauche;
        A1->nature = N_IDF;
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        Rec_suite_link(&A1);
        break;

    case ECRIRE:
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_suite_ecrire(A) != 0)
        {
            return 1; // Si erreur
        }
        break;
    default:
        if (Rec_seq_aff(A) != 0)
        {
            return 1; // Si erreur
        }
        break;
    }

    return 0; // Aucune erreur
}
////////////
int Rec_suite_ecrire(Ast *A)
{
    Ast A1;
    switch (lexeme_courant().nature)
    {
    case NODE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_ECRIRENODE;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != IDF && lexeme_courant().nature != STRING)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Identificateur attendu.\n");
            return 1; // Si erreur
        }
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        if (lexeme_courant().nature == STRING){
            A1->nature = N_STR;
        }else{
            A1->nature = N_IDF;
        }
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        Rec_suite_concat(&A1);
        break;
    case FLOAT:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_ECRIREFLOAT;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_eag(&A1) != 0){
            return 1;// Si erreur
        }
        (*A)->gauche = A1;
        break;

    default:
        break;
    }
    return 0; // Aucune erreur
}
////////////
int Rec_seq_aff(Ast *A)
{
    Ast A1;
    switch (lexeme_courant().nature)
    {
    case NODE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_NODE;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_suite_node(A) != 0)
        {
            return 1; // Si erreur
        }
        break;

    case SUBGRAPH:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_SUB;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != AFF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Affectation attendue.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != STRING)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Nom du subgraph manquant.\n");
            return 1; // Si erreur
        }
        A1 = nouvelle_cellule_ast();
        A1->nature = N_STR;
        strcpy(A1->chaine, lexeme_courant().chaine);
        (*A)->gauche = A1;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        break;

    case LIRE:
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        switch (lexeme_courant().nature)
        {
        case NODE:
            (*A) = nouvelle_cellule_ast();
            (*A)->nature = N_LIRENODE;
            if (avancer() != 0)
            {
                return 1; // Si erreur
            }
            if (lexeme_courant().nature != IDF)
            {
                perror("Erreur syntaxique :\n");
                afficher_lexeme(lexeme_courant());
                printf("HINT : Identificateur attendu.\n");
                return 1; // Si erreur
            }
            A1 = nouvelle_cellule_ast();
            (*A)->gauche = A1;
            A1->nature = N_IDF;
            strcpy(A1->chaine, lexeme_courant().chaine);
            if (avancer() != 0)
        {
            return 1; // Si erreur
        }
            break;
        case IDF:
            (*A) = nouvelle_cellule_ast();
            (*A)->nature = N_LIREFLOAT;
            A1 = nouvelle_cellule_ast();
            (*A)->gauche = A1;
            A1->nature = N_IDF;
            strcpy(A1->chaine, lexeme_courant().chaine);
            if (avancer() != 0)
            {
                return 1; // Si erreur
            }
            break;
        default:
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Node ou identificateur attendu après instruction read.\n");
            return 1; // Si erreur
        }

        break;

    case IDF:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_AFFINT;
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        A1->nature = N_IDF;
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != AFF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Affectation attendue.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_eag(&A1) != 0){
            return 1; // Si erreur
        }
        (*A)->droite = A1;
        break;
    default:
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        return 1; // Si erreur
    }
    return 0; // Aucune erreur
}
////////////
int Rec_eag(Ast *A)
{
    return Rec_seq_terme(A);
}
////////////
int Rec_seq_terme(Ast *A)
{
    Ast A1;
    if (Rec_terme(&A1) != 0){
        return 1; // Si erreur
    }
    if (Rec_suite_seq_terme(A1, A) != 0){
        return 1; // Si erreur
    }
    return 0;
}
////////////
int Rec_terme(Ast *A)
{
    return Rec_seq_facteur(A);
}
////////////
int Rec_facteur(Ast *A)
{
    float f = 1;
    while (lexeme_courant().nature == MOINS)
    {
        f = f * (-1);
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
    }
    if (lexeme_courant().nature == FLOAT)
    {
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_FLOAT;
        (*A)->valeur = lexeme_courant().valeur * f;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
    }
    else if (lexeme_courant().nature == IDF)
    {
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_IDF;
        strcpy((*A)->chaine, lexeme_courant().chaine);
        (*A)->valeur = f;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
    }
    else if (lexeme_courant().nature == PARO)
    {
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_eag(A) != 0){
            return 1; // Si erreur
        }
        (*A)->valeur = f;
        if (lexeme_courant().nature == PARF)
        {
            if (avancer() != 0)
            {
                return 1; // Si erreur
            }
        }
        else
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Parenthèse Fermé attendu.\n");
            return 1; // Si erreur
        }
    }
    else
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        return 1; // Si erreur
    }
    return 0;
}
////////////
int Rec_suite_seq_facteur(Ast A1, Ast *A)
{
    Ast A2,A3;
    if (op2(&A3))
    {
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_facteur(&A2) != 0){
            return 1; // Si erreur
        }
        A3->droite = A2;
        A3->gauche = A1;
        if (Rec_suite_seq_facteur(A3, A) != 0){
            return 1; // Si erreur
        }
    }
    else
    {
        (*A) = A1;
    }
    return 0; // Aucune erreur
}
////////////
int Rec_seq_facteur(Ast *A)
{
    Ast A1;
    if (Rec_facteur(&A1) != 0){
        return 1; // Si erreur
    }
    if (Rec_suite_seq_facteur(A1, A) != 0){
        return 1; // Si erreur
    }
    return 0; // Aucune erreur
}
////////////
int Rec_suite_seq_terme(Ast A1, Ast *A)
{
    Ast A2,A3;
    if (op1(&A3))
    {
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (Rec_terme(&A2)){
            return 1; // Si erreur
        }
        A3->gauche = A1;
        A3->droite = A2;
        if (Rec_suite_seq_terme(A3, A)){
            return 1; // Si erreur
        }
    }
    else
    {
        (*A) = A1;
    }
    return 0; // Aucune erreur
}
////////////
int Rec_suite_node(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature != DEUX_POINT)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Deux point attendu.\n");
        return 1; // Si erreur
    }
    if (avancer() != 0)
    {
        return 1; // Si erreur
    }
    if (lexeme_courant().nature != IDF)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Identificateur attendu.\n");
        return 1; // Si erreur
    }
    A1 = nouvelle_cellule_ast();
    A1->nature = N_IDF;
    strcpy(A1->chaine, lexeme_courant().chaine);
    (*A)->gauche = A1;
    if (avancer() != 0)
    {
        return 1; // Si erreur
    }
    if (lexeme_courant().nature != AFF)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Affectation attendue.\n");
        return 1; // Si erreur
    }
    if (avancer() != 0)
    {
        return 1; // Si erreur
    }
    if (lexeme_courant().nature != STRING && lexeme_courant().nature != IDF && lexeme_courant().nature != FLOAT)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : L'instruction node attend une chaine de caractere, un identificateur ou un nombre.\n");
        return 1; // Si erreur
    }
    A1 = nouvelle_cellule_ast();
    if (lexeme_courant().nature == STRING || lexeme_courant().nature == FLOAT)
    {
        A1->nature = N_STR;
    }else{
        A1->nature = N_IDF;
    }
    strcpy(A1->chaine, lexeme_courant().chaine);
    (*A)->droite = A1;
    if (avancer() != 0)
    {
        return 1; // Si erreur
    }
    Rec_suite_concat(&A1);

    return 0; // Aucune erreur
}
////////////
int Rec_suite_concat(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature != CONCAT)
    {
        return 0; // Aucune erreur
    }
    A1 = nouvelle_cellule_ast();
    (*A)->droite = A1;
    A1->nature = N_CONCAT;
    if (avancer() != 0)
    {
        return 1; // Si erreur
    }
    if (lexeme_courant().nature != STRING && lexeme_courant().nature != IDF && lexeme_courant().nature != FLOAT)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : L'instruction concat attend une chaine de caractere, un identificateur ou un nombre.\n");
        return 1; // Si erreur
    }
    A1->droite = nouvelle_cellule_ast();
    A1 = A1->droite;
    if (lexeme_courant().nature == STRING || lexeme_courant().nature == FLOAT)
    {
        A1->nature = N_STR;
    }else{
        A1->nature = N_IDF;
    }
    strcpy(A1->chaine, lexeme_courant().chaine);
    if (avancer() != 0)
    {
        return 1; // Si erreur
    }
    Rec_suite_concat(&A1);

    return 0; // Aucune erreur
}
////////////
int Rec_suite_map(Ast *A)
{
    Ast A1;
    int a, b, c, d;
    switch (lexeme_courant().nature)
    {
    case NODE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_MAPNODE;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != L_NODE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de noeuds attendue.\n");
            return 1; // Si erreur
        }
        A1 = nouvelle_cellule_ast();
        A1->nature = N_IDF;
        a = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        (*A)->gauche = A1;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != L_STR)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de chaine de caractères attendue.\n");
            return 1; // Si erreur
        }
        A1 = nouvelle_cellule_ast();
        A1->nature = N_STR;
        b = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        (*A)->droite = A1;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (a != b)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les listes de noeuds et de chaines de caractères doivent être de même taille.\n");
            return 1; // Si erreur
        }
        break;

    case LINK:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_MAPLINK;
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != L_NODE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de noeuds attendue.\n");
            return 1; // Si erreur
        }
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        A1->nature = N_IDF;
        a = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != L_NODE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de noeuds attendue.\n");
            return 1; // Si erreur
        }
        A1->gauche = nouvelle_cellule_ast();
        A1 = A1->gauche;
        A1->nature = N_IDF;
        b = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (a != b)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les listes de noeuds doivent être de même taille.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != L_FLOAT)
        {
            return 0; // Aucune erreur
        }
        A1->gauche = nouvelle_cellule_ast();
        A1 = A1->gauche;
        A1->nature = N_STR;
        c = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (a != c)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les listes de noeuds et de flottants doivent être de même taille.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        if (lexeme_courant().nature != L_COLOR)
        {
            return 0; // Aucune erreur
        }
        A1->gauche = nouvelle_cellule_ast();
        A1 = A1->gauche;
        A1->nature = N_STR;
        d = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        A1 = A1->gauche;
        if (a != d)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les listes de noeuds et de couleurs doivent être de même taille.\n");
            return 1; // Si erreur
        }
        if (avancer() != 0)
        {
            return 1; // Si erreur
        }
        break;
    default:
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        return 1; // Si erreur
        break;
    }

    return 0; // Aucune erreur
}
////////////
int Rec_suite_link(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature == P_VIRG)
    {
        return 0; // Aucune erreur
    }
    if (Rec_eag(&A1) != 0){
        return 1; // Si erreur
    }
    (*A)->gauche = A1;
    if (lexeme_courant().nature != COLOR)
    {
        return 0; // Aucune erreur
    }
    A1 = nouvelle_cellule_ast();
    (*A)->centre = A1;
    A1->nature = N_STR;
    strcpy(A1->chaine, lexeme_courant().chaine);
    if (avancer() != 0)
    {
        return 1; // Si erreur
    }
    return 0; // Aucune erreur
}
////////////
int Rec_condition(Ast *A)
{
    Ast A1, AC;
    if (Rec_eag(&A1) != 0){
        return 1; // Si erreur
    }
    if (lexeme_courant().nature != OPE_BOOL)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Opérateur booléen attendu après un if.\n");
        return 1; // Si erreur
    }
    AC = nouvelle_cellule_ast();
    AC->gauche = A1;
    AC->nature = N_STR;
    strcpy(AC->chaine, lexeme_courant().chaine);
    if (avancer() != 0)
        {
            return 1; // Si erreur
        }
    if (Rec_eag(&A1) != 0){
        return 1; // Si erreur
    }
    AC->droite = A1;
    (*A)->centre = AC;

    return 0; // Aucune erreur
}
//////////////////////////////////////////////////////////////////
// Predicat op :
int op1(Ast *A){ 
	switch(lexeme_courant().nature){
		case PLUS:
            (*A)=nouvelle_cellule_ast();
			(*A)->nature=N_PLUS;
            return 1;
		case MOINS:
            (*A)=nouvelle_cellule_ast();
			(*A)->nature=N_MOINS;
            return 1;
        case MOD:
            (*A)=nouvelle_cellule_ast();
			(*A)->nature=N_MOD;
            return 1;
		default: 
			return 0;
	}
}
////////////
int op2(Ast *A){ 
	switch(lexeme_courant().nature){
		case MUL:
            (*A)=nouvelle_cellule_ast();
			(*A)->nature=N_MUL;
            return 1;
		case DIV:
            (*A)=nouvelle_cellule_ast();
			(*A)->nature=N_DIV;
            return 1;
		default: 
			return 0;
	}
}
//////////////////////////////////////////////////////////////////
// Fonction Analyse :
int analyse(char *nom_ficher, Ast *arbre)
{
    demarrer(nom_ficher);
    return Rec_pgm(arbre);
}
//////////////////////////////////////////////////////////////////
