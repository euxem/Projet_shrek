#ifndef __ARBRE_AST_H__
#define __ARBRE_AST_H__

# include "type_ast.h"
# include "table_symbole.h"

void print_tab_in_subgraph(FILE *f, int* nb_clause, int* nb_sub);
int interpreter(Ast A, FILE* f, int* nb_clause, int* nb_sub);
int interpreter_subtitle(FILE* f, Ast A, int* nb_clause, int* nb_sub);
int interpreter_link(FILE* f, Ast A, int* nb_clause, int* nb_sub);
int interpreter_maplink(FILE* f, Ast A, int* nb_clause, int* nb_sub);
void ecrire_float(float f);
void ecrire_char(char *idf);
int condition(Ast A);

#endif
