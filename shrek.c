#include <stdio.h>
#include <unistd.h>
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "analyse_syntaxique.h"
#include "type_ast.h"
#include "arbre_ast.h"

int main(int argc, char **argv)
{
    Ast A;
    FILE *f;
    int SUB = 0;

    // Default arguments values
    char *ipath = "";
    char *opath = "output.dot";

    int c;

    while ((c = getopt(argc, argv, "i:o:h")) != -1)
    {
        switch (c)
        {
        case 'i':
            ipath = optarg;
            break;
        case 'o':
            opath = optarg;
            break;
        case 'h':
            printf("Usage: %s [-i input file] [-o output file] [-h]\n", argv[0]);
            printf("If no input is specified, stdin is used.\n");
            printf("If no output is specified, output.dot is used.\n");
            return 0;
        case '?':
        default:
            return 1;
        }
    }

    f = fopen(opath, "w");
    analyse(ipath, &A);
    fprintf(f, "graph G {\n");
    SUB = interpreter(A, f);
    if (SUB)
    {
        fprintf(f, "\t}\n");
    }
    fprintf(f, "}\n");
    printf(".dot file output : %s\n", opath);
    free_ast(A);

    free_chainee();
    fclose(f);
    return 0;
}
