#include <stdio.h>
#include <stdlib.h>
#include "type_ast.h"

Ast nouvelle_cellule_ast()
{
	return (Ast)calloc(1, sizeof(NoeudAst));
}

void free_ast(Ast A)
{
	if (A == NULL)
		return;
	free_ast(A->gauche);
	free_ast(A->droite);
	free_ast(A->centre);
	free(A);
}
