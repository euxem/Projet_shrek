#include <stdio.h>
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "analyse_syntaxique.h"
#include "type_ast.h"
#include "arbre_ast.h"

int main(){
    Ast A;
    FILE* f;
    f=fopen("new.dot","w");
    analyse("shrek_lex_test.shrek",&A);
    interpreter(A,f);
    printf("BG\n");
    fclose(f);
}
