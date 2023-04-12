#ifndef __ANALYSE_SYNTAXIQUE_H__
#define __ANALYSE_SYNTAXIQUE_H__

#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "type_ast.h"

int Rec_pgm(Ast *A);
int Rec_seq_inst(Ast *A);
int Rec_inst(Ast *A);
int Rec_suite_seq_inst(Ast *A, Ast *A1);
int Rec_seq_aff(Ast *A);
int Rec_suite_ecrire(Ast *A);
int Rec_suite_node(Ast *A);
int Rec_suite_for(Ast *A);
void Rec_suite_link(Ast *A);
int Rec_condition(Ast *A);
int Rec_suite_concat(Ast *A);
void Rec_eag(Ast *A);
void Rec_seq_terme(Ast *A);
void Rec_terme(Ast *A);
void Rec_suite_seq_terme(Ast *A);
void Rec_seq_facteur(Ast *A);
void Rec_facteur(Ast *A);
void Rec_suite_seq_facteur(Ast *A);

// Fonction principale de l'analyseur syntaxique
// Appelle la fonction de démarrage de lecture du fichier
// Et appelle la fonction de reconnaissance de la grammaire
int analyse(char *nom_ficher, Ast *arbre);

#endif
