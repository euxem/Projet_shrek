#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse_syntaxique.h"
#include "type_ast.h"

int Rec_pgm(Ast *A)
{
    if (Rec_seq_inst(A) != 0)
    {
        return 1;
    }
    if (lexeme_courant().nature != FIN)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Deux points attendu.\n");
        return 1;
    }

    return 0;
}

int Rec_seq_inst(Ast *A)
{
    // ret vaut 1 si une erreur est détectée
    int ret = 0;
    Ast A1;
    ret = Rec_inst(&A1) || ret;
    ret = Rec_suite_seq_inst(A, &A1) || ret;
    return ret;
}

int Rec_suite_seq_inst(Ast *A, Ast *A1)
{
    if (lexeme_courant().nature != P_VIRG)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Point virgule attendu.\n");
        return 1;
    }
    avancer();
    if (lexeme_courant().nature == FIN || lexeme_courant().nature == SINON || lexeme_courant().nature == FSI || lexeme_courant().nature == FAIT || lexeme_courant().nature == ROF)
    {
        (*A) = (*A1);
        return 0;
    }
    (*A) = nouvelle_cellule_ast();
    (*A)->nature = N_SEPARATEUR;
    (*A)->gauche = (*A1);
    return Rec_seq_inst(&((*A)->droite));
}

int Rec_inst(Ast *A)
{
    Ast A1;
    switch (lexeme_courant().nature)
    {
    case MAP:
        avancer();
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1;
        }
        avancer();
        if (Rec_suite_for(A) != 0)
        {
            return 1;
        }
        break;

    case SI:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_SI;
        avancer();
        if (Rec_condition(A) != 0)
        {
            return 1;
        }
        if (lexeme_courant().nature != ALORS)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un if nécessite un then.\n");
            return 1;
        }
        avancer();
        Rec_seq_inst(&A1);
        if (lexeme_courant().nature != SINON && lexeme_courant().nature != FSI)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un if doit continuer avec un else ou se terminer par un fi.\n");
            return 1;
        }
        (*A)->droite = A1;
        if (lexeme_courant().nature == SINON)
        {
            avancer();
            Rec_seq_inst(&A1);
            if (lexeme_courant().nature != FSI)
            {
                perror("Erreur syntaxique :\n");
                afficher_lexeme(lexeme_courant());
                printf("HINT : Un if doit se terminer par un fi.\n");
                return 1;
            }
            (*A)->gauche = A1;
        }
        else
        {
            (*A)->gauche = NULL;
        }
        avancer();
        break;

    case TANT_QUE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_TANT_QUE;
        avancer();
        if (Rec_condition(A) != 0)
        {
            return 1;
        }
        if (lexeme_courant().nature != FAIRE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un while nécessite un do.\n");
            return 1;
        }
        avancer();
        Rec_seq_inst(&A1);
        if (lexeme_courant().nature != FAIT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un while doit se terminer par un done.\n");
            return 1;
        }
        (*A)->droite = A1;
        avancer();
        break;

    case FOR:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_FOR;
        avancer();
        if (lexeme_courant().nature != PARO)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for attends ses arguments dans une parenthèse.\n");
            return 1;
        }
        avancer();
        if (lexeme_courant().nature != IDF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for attends un identificateur en premier argument.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1;
        }
        if (Rec_seq_aff(&A1) != 0)
        {
            return 1;
        }
        if (lexeme_courant().nature != P_VIRG)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les arguments du for doivent être séparés par des points virgules.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1;
        }
        (*A)->gauche = A1;
        avancer();
        if (Rec_condition(A) != 0)
        {
            return 1;
        }
        if (lexeme_courant().nature != P_VIRG)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les arguments du for doivent être séparés par des points virgules.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1;
        }
        avancer();
        Rec_eag(&A1);
        (*A)->gauche->centre = A1;
        if (lexeme_courant().nature != PARF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Fermez la parenthèse du for.\n");
            printf("HINT : La syntaxe du for est similaire à celle du C.\n");
            return 1;
        }
        avancer();
        if (lexeme_courant().nature != FAIRE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for nécessite un do.\n");
            return 1;
        }
        avancer();
        Rec_seq_inst(&A1);
        if (lexeme_courant().nature != ROF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Un for doit se terminer par un rof.\n");
            return 1;
        }
        (*A)->droite = A1;
        avancer();
        break;

    case CLOSUB:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_CLOSUB;
        avancer();
        break;

    case LINK:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_LINK;
        avancer();
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1;
        }
        avancer();
        if (lexeme_courant().nature != IDF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Identificateur attendu.\n");
            return 1;
        }
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        A1->nature = N_STR;
        strcpy(A1->chaine, lexeme_courant().chaine);
        avancer();
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1;
        }
        avancer();
        if (lexeme_courant().nature != IDF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Identificateur attendu.\n");
            return 1;
        }
        A1->gauche = nouvelle_cellule_ast();
        A1 = A1->gauche;
        A1->nature = N_STR;
        strcpy(A1->chaine, lexeme_courant().chaine);
        avancer();
        Rec_suite_link(&A1);
        break;

    case ECRIRE:
        avancer();
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1;
        }
        avancer();
        if (Rec_suite_ecrire(A) != 0)
        {
            return 1;
        }
        break;
    default:
        if (Rec_seq_aff(A) != 0)
        {
            return 1;
        }
        break;
    }

    return 0;
}

int Rec_suite_ecrire(Ast *A)
{
    Ast A1;
    switch (lexeme_courant().nature)
    {
    case NODE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_ECRIRENODE;
        avancer();
        if (lexeme_courant().nature != IDF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Identificateur attendu.\n");
            return 1;
        }
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        A1->nature = N_STR;
        strcpy(A1->chaine, lexeme_courant().chaine);
        avancer();
        Rec_suite_concat(&A1);
        break;
    case FLOAT:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_ECRIREFLOAT;
        avancer();
        Rec_eag(&A1);
        (*A)->gauche = A1;
        break;

    default:
        break;
    }
    return 0;
}

int Rec_seq_aff(Ast *A)
{
    Ast A1;
    switch (lexeme_courant().nature)
    {
    case NODE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_NODE;
        avancer();
        if (Rec_suite_node(A) != 0)
        {
            return 1;
        }
        break;

    case SUBGRAPH:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_SUB;
        avancer();
        if (lexeme_courant().nature != AFF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Affectation attendue.\n");
            return 1;
        }
        avancer();
        if (lexeme_courant().nature != STRING)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Nom du subgraph manquant.\n");
            return 1;
        }
        A1 = nouvelle_cellule_ast();
        A1->nature = N_STR;
        strcpy(A1->chaine, lexeme_courant().chaine);
        (*A)->gauche = A1;
        avancer();
        break;

    case LIRE:
        avancer();
        if (lexeme_courant().nature != DEUX_POINT)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Deux points attendu.\n");
            return 1;
        }
        avancer();
        switch (lexeme_courant().nature)
        {
        case NODE:
            (*A) = nouvelle_cellule_ast();
            (*A)->nature = N_LIRENODE;
            avancer();
            if (lexeme_courant().nature != IDF)
            {
                perror("Erreur syntaxique :\n");
                afficher_lexeme(lexeme_courant());
                printf("HINT : Identificateur attendu.\n");
                return 1;
            }
            A1 = nouvelle_cellule_ast();
            (*A)->gauche = A1;
            A1->nature = N_STR;
            strcpy(A1->chaine, lexeme_courant().chaine);
            avancer();
            break;
        case IDF:
            (*A) = nouvelle_cellule_ast();
            (*A)->nature = N_LIREFLOAT;
            A1 = nouvelle_cellule_ast();
            (*A)->gauche = A1;
            A1->nature = N_STR;
            strcpy(A1->chaine, lexeme_courant().chaine);
            avancer();
            break;
        default:
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Node ou identificateur attendu après instruction read.\n");
            return 1;
        }

        break;

    case IDF:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_AFFINT;
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        A1->nature = N_STR;
        strcpy(A1->chaine, lexeme_courant().chaine);
        avancer();
        if (lexeme_courant().nature != AFF)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Affectation attendue.\n");
            return 1;
        }
        avancer();
        Rec_eag(&A1);
        (*A)->droite = A1;
        break;
    default:
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        return 1;
    }
    return 0;
}

void Rec_eag(Ast *A)
{
    Rec_seq_terme(A);
}

void Rec_seq_terme(Ast *A)
{
    Ast A1;
    Rec_terme(&A1);
    Rec_suite_seq_terme(A);
    if ((*A) == NULL)
    {
        (*A) = A1;
    }
    else
    {
        (*A)->gauche = A1;
    }
}

void Rec_terme(Ast *A)
{
    Rec_seq_facteur(A);
}

void Rec_facteur(Ast *A)
{
    float f = 1;
    while (lexeme_courant().nature == MOINS)
    {
        f = f * (-1);
        avancer();
    }
    if (lexeme_courant().nature == FLOAT)
    {
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_FLOAT;
        (*A)->valeur = lexeme_courant().valeur * f;
        avancer();
    }
    else if (lexeme_courant().nature == IDF)
    {
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_STR;
        strcpy((*A)->chaine, lexeme_courant().chaine);
        (*A)->valeur = f;
        avancer();
    }
    else if (lexeme_courant().nature == PARO)
    {
        avancer();
        Rec_eag(A);
        (*A)->valeur = f;
        if (lexeme_courant().nature == PARF)
        {
            avancer();
        }
        else
        {
            // printf("Erreur syntaxique.\n");
            return;
        }
    }
    else
    {
        // printf("Erreur syntaxique.\n");
        return;
    }
}

void Rec_suite_seq_facteur(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature == MUL || lexeme_courant().nature == DIV)
    {
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_OP;
        strcpy((*A)->chaine, lexeme_courant().chaine);
        avancer();
        Rec_seq_facteur(&A1);
        (*A)->droite = A1;
    }
    else
    {
        (*A) = NULL;
    }
}

void Rec_seq_facteur(Ast *A)
{
    Ast A1;
    Rec_facteur(&A1);
    Rec_suite_seq_facteur(A);
    if ((*A) == NULL)
    {
        (*A) = A1;
    }
    else
    {
        (*A)->gauche = A1;
    }
}

void Rec_suite_seq_terme(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature == PLUS || lexeme_courant().nature == MOINS)
    {
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_OP;
        strcpy((*A)->chaine, lexeme_courant().chaine);
        avancer();
        Rec_seq_terme(&A1);
        (*A)->droite = A1;
    }
    else
    {
        (*A) = NULL;
    }
}

int Rec_suite_node(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature != DEUX_POINT)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Deux point attendu.\n");
        return 1;
    }
    avancer();
    if (lexeme_courant().nature != IDF)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Identificateur attendu.\n");
        return 1;
    }
    A1 = nouvelle_cellule_ast();
    A1->nature = N_STR;
    strcpy(A1->chaine, lexeme_courant().chaine);
    (*A)->gauche = A1;
    avancer();
    if (lexeme_courant().nature != AFF)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Affectation attendue.\n");
        return 1;
    }
    avancer();
    if (lexeme_courant().nature != STRING && lexeme_courant().nature != IDF && lexeme_courant().nature != FLOAT)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : L'instruction node attend une chaine de caractere, un identificateur ou un nombre.\n");
        return 1;
    }
    A1 = nouvelle_cellule_ast();
    A1->nature = N_STR;
    if (lexeme_courant().nature == STRING || lexeme_courant().nature == FLOAT)
    {
        A1->valeur = 1;
    }
    strcpy(A1->chaine, lexeme_courant().chaine);
    (*A)->droite = A1;
    avancer();
    Rec_suite_concat(&A1);

    return 0;
}

int Rec_suite_concat(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature != CONCAT)
    {
        return 0;
    }
    A1 = nouvelle_cellule_ast();
    (*A)->droite = A1;
    A1->nature = N_STR;
    strcpy(A1->chaine, lexeme_courant().chaine);
    avancer();
    if (lexeme_courant().nature != STRING && lexeme_courant().nature != IDF && lexeme_courant().nature != FLOAT)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : L'instruction concat attend une chaine de caractere, un identificateur ou un nombre.\n");
        return 1;
    }
    A1->droite = nouvelle_cellule_ast();
    A1 = A1->droite;
    A1->nature = N_STR;
    if (lexeme_courant().nature == STRING || lexeme_courant().nature == FLOAT)
    {
        A1->valeur = 1;
    }
    strcpy(A1->chaine, lexeme_courant().chaine);
    avancer();
    Rec_suite_concat(&A1);

    return 0;
}

int Rec_suite_for(Ast *A)
{
    Ast A1;
    int a, b, c, d;
    switch (lexeme_courant().nature)
    {
    case NODE:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_MAPNODE;
        avancer();
        if (lexeme_courant().nature != L_NODE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de noeuds attendue.\n");
            return 1;
        }
        A1 = nouvelle_cellule_ast();
        A1->nature = N_STR;
        a = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        (*A)->gauche = A1;
        avancer();
        if (lexeme_courant().nature != L_STR)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de chaine de caractères attendue.\n");
            return 1;
        }
        A1 = nouvelle_cellule_ast();
        A1->nature = N_STR;
        b = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        (*A)->droite = A1;
        avancer();
        if (a != b)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les listes de noeuds et de chaines de caractères doivent être de même taille.\n");
            return 1;
        }
        break;

    case LINK:
        (*A) = nouvelle_cellule_ast();
        (*A)->nature = N_MAPLINK;
        avancer();
        if (lexeme_courant().nature != L_NODE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de noeuds attendue.\n");
            return 1;
        }
        A1 = nouvelle_cellule_ast();
        (*A)->gauche = A1;
        A1->nature = N_STR;
        a = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        avancer();
        if (lexeme_courant().nature != L_NODE)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Liste de noeuds attendue.\n");
            return 1;
        }
        A1->gauche = nouvelle_cellule_ast();
        A1 = A1->gauche;
        A1->nature = N_STR;
        b = lexeme_courant().taille;
        strcpy(A1->chaine, lexeme_courant().chaine);
        if (a != b)
        {
            perror("Erreur syntaxique :\n");
            afficher_lexeme(lexeme_courant());
            printf("HINT : Les listes de noeuds doivent être de même taille.\n");
            return 1;
        }
        avancer();
        if (lexeme_courant().nature != L_FLOAT)
        {
            return 0;
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
            return 1;
        }
        avancer();
        if (lexeme_courant().nature != L_COLOR)
        {
            return 0;
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
            return 1;
        }
        avancer();
        break;
    default:
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        return 1;
        break;
    }

    return 0;
}

void Rec_suite_link(Ast *A)
{
    Ast A1;
    if (lexeme_courant().nature == P_VIRG)
    {
        return;
    }
    Rec_eag(&A1);
    (*A)->gauche = A1;
    if (lexeme_courant().nature != COLOR)
    {
        return;
    }
    A1 = nouvelle_cellule_ast();
    (*A)->centre = A1;
    A1->nature = N_STR;
    strcpy(A1->chaine, lexeme_courant().chaine);
    avancer();
}

int Rec_condition(Ast *A)
{
    Ast A1, AC;
    Rec_eag(&A1);
    if (lexeme_courant().nature != OPE_BOOL)
    {
        perror("Erreur syntaxique :\n");
        afficher_lexeme(lexeme_courant());
        printf("HINT : Opérateur booléen attendu après un if.\n");
        return 1;
    }
    AC = nouvelle_cellule_ast();
    AC->gauche = A1;
    AC->nature = N_STR;
    strcpy(AC->chaine, lexeme_courant().chaine);
    avancer();
    Rec_eag(&A1);
    AC->droite = A1;
    (*A)->centre = AC;

    return 0;
}

int analyse(char *nom_ficher, Ast *arbre)
{
    demarrer(nom_ficher);
    return Rec_pgm(arbre);
}
