# ifndef __ANALYSE_SYNTAXIQUE_H__
# define __ANALYSE_SYNTAXIQUE_H__

# include "analyse_lexicale.h"
# include "lecture_caracteres.h"
# include "type_ast.h"

void Rec_pgm(Ast *A);
void Rec_seq_inst(Ast *A);
void Rec_inst(Ast *A);
void Rec_suite_seq_inst(Ast *A, Ast *A1);
void Rec_seq_aff(Ast *A);
void Rec_suite_ecrire(Ast *A);
void Rec_suite_node(Ast *A);
void Rec_suite_for(Ast *A);
void Rec_suite_link(Ast *A);
void Rec_condition(Ast *A);
void Rec_suite_concat(Ast *A);
void Rec_eag(Ast *A);
void Rec_seq_terme(Ast *A);
void Rec_terme(Ast *A);
void Rec_suite_seq_terme(Ast *A);
void Rec_seq_facteur(Ast *A);
void Rec_facteur(Ast *A);
void Rec_suite_seq_facteur(Ast *A);
void analyse(char* nom_ficher,Ast *arbre);
# endif
