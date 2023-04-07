#include <stdio.h>
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "analyse_syntaxique.h"
#include "type_ast.h"

int main(){
    Ast A;
    analyse("shrek_lex_test.shrek",&A);
    printf("BG\n");
}
