#ifndef __ARBRE_AST_H__
#define __ARBRE_AST_H__

# include "type_ast.h"
# include "table_symbole.h"

int interpreter(Ast A, FILE* f);
void interpreter_subtitle(FILE* f, Ast A);
void interpreter_link(FILE* f, Ast A);
void interpreter_forlink(FILE* f, Ast A);
void ecrire_float(float f);
void ecrire_char(char *idf);
int condition(Ast A);

#endif
