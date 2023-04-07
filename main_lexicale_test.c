#include <stdio.h>
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"

int main(){
    demarrer("shrek_lex_test.shrek");
    while (lexeme_courant().nature != FIN)
    {
        avancer();
    }
    printf("BG\n");
}
