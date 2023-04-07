#include <stdio.h>
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "analyse_syntaxique.h"
#include "type_ast.h"
#include "arbre_ast.h"

int main(int argc, char** argv){
    Ast A;
    FILE* f;
    int SUB=0;
    switch (argc)
    {
    case 1:
        f=fopen("a.dot","w");
        analyse("",&A);
        fprintf(f,"graph G {\n");
        SUB=interpreter(A,f);
        if (SUB){
            fprintf(f,"\t}\n");    
        }
        fprintf(f,"}\n");
        printf(".dot file output : a.dot\n");
        free_ast(A);
        break;
    case 2:
        f=fopen("a.dot","w");
        analyse(argv[1],&A);
        fprintf(f,"graph G {\n");
        SUB=interpreter(A,f);
        if (SUB){
            fprintf(f,"\t}\n");    
        }
        fprintf(f,"}\n");
        printf(".dot file output : a.dot\n");
        free_ast(A);
        break;
    case 3:
        f=fopen(argv[2],"w");
        analyse(argv[1],&A);
        fprintf(f,"graph G {\n");
        SUB=interpreter(A,f);
        if (SUB){
            fprintf(f,"\t}\n");    
        }
        fprintf(f,"}\n");
        printf(".dot file output : %s\n", argv[2]);
        free_ast(A);
        break;
    default:
        printf ("Usage: %s <shrek_file> <output_file_name>\n",argv[0]);
        return 1;
    }
    free_chainee();
    fclose(f);
    return 0;
}
