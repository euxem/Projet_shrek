#ifndef __TYPE_AST_H__
#define __TYPE_AST_H__

typedef enum
{
	N_SEPARATEUR,
	N_SUB,
	N_NODE,
	N_LINK,
	N_STR,
	N_FLOAT,
	N_FOR,
	N_MAPNODE,
	N_LIRENODE,
	N_LIREFLOAT,
	N_ECRIRENODE,
	N_ECRIREFLOAT,
	N_AFFINT,
	N_MAPLINK,
	N_SI,
	N_OP,
	N_CLOSUB,
	N_TANT_QUE
} TypeAst;

typedef struct noeud
{
	TypeAst nature;
	struct noeud *gauche, *droite, *centre;
	char chaine[256];
	float valeur;
} NoeudAst;

typedef NoeudAst *Ast;

// Renvoie un pointeur vers un espacio mémoire alloué pour un noeud de l'arbre syntaxique
Ast nouvelle_cellule_ast();

// Libère la mémoire allouée pour l'arbre syntaxique
void free_ast(Ast A);

#endif
