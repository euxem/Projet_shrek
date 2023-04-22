#ifndef __TYPE_AST_H__
#define __TYPE_AST_H__

////////////////////////////////////////////////////
// type enumerate -> Nature des noeuds possible pour l'abre.
typedef enum
{
	N_SEPARATEUR,	// Noeud séparateur d'instruction, à gauche de ce noeud -> instruction a faire, à droite -> suite du programme
	N_SUB,			// Noeud représentant la racine d'un sous-arbre étant interprêté comme une création de sous-graph dans un progamme .dot
	N_NODE,			// Noeud représentant la racine d'un sous-arbre étant interprêté comme l'affectation d'une chaîne de caractères à un identificateur (objet noeud dans un graph)
	N_LINK,			// Noeud représentant la racine d'un sous-arbre étant interprêté comme un lien entre 2 objets noeuds
	N_STR,			// Noeud représentant une chaîne de caractère
	N_IDF,			// Noeud représentant un identificateur
	N_CONCAT,		// Noeud représentant un opérateur de concaténation
	N_FLOAT,		// Noeud représentant un nombre réel
	N_FOR,			// Noeud représentant la racine d'un sous-arbre étant interprêté comme une boucle for
	N_MAPNODE,		// Noeud représentant la racine d'un sous-arbre étant interprêté comme une affectationde multiple de chaînes de caractères sur des identificateurs (objets noeud dans graph)
	N_LIRENODE,		// Noeud représentant la racine d'un sous-arbre étant interprêté comme une affectation sur un noeud depuis une lecture s'entrée standard
	N_LIREFLOAT,	// Noeud représentant la racine d'un sous-arbre étant interprêté comme une affectation sur un float depuis une lecture s'entrée standard
	N_ECRIRENODE,	// Noeud représentant la racine d'un sous-arbre étant interprêté comme une écriture dans le terminale d'un nom de noeud (identificateur), d'une chaîne de caractère, ou d'une concaténation
	N_ECRIREFLOAT,	// Noeud représentant la racine d'un sous-arbre étant interprêté comme une écriture dans le terminale du résultat d'une eag (identificateur comprit)
	N_AFFINT,		// Noeud représentant la racine d'un sous-arbre étant interprêté comme une affectation du résultat d'une eag à un identificateur
	N_MAPLINK,		// Noeud représentant la racine d'un sous-arbre étant interprêté comme une création multiple de lien entre les noeuds fournit
	N_SI,			// Noeud représentant la racine d'un sous-arbre étant interprêté comme un if-then-else
	N_PLUS,			// Noeud représentant l'opération addition du sous-arbre droit et gauche de ce noeud
	N_MOINS,		// Noeud représentant l'opération soustraction du sous-arbre droit et gauche de ce noeud
	N_MUL,			// Noeud représentant l'opération multiplication du sous-arbre droit et gauche de ce noeud
	N_DIV,			// Noeud représentant l'opération division du sous-arbre droit et gauche de ce noeud
	N_MOD,			// Noeud représentant l'opération modulo du sous-arbre droit et gauche de ce noeud
	N_CLOSUB,		// Noeud représentant la fin d'un subgraphe ouvert
	N_TANT_QUE		// Noeud représentant la racine d'un sous-arbre étant interprêté comme un while
} TypeAst;
////////////////////////////////////////////////////
// type structure -> Noeud d'un arbre AST
typedef struct noeud
{
	TypeAst nature;
	struct noeud *gauche, *droite, *centre;
	char chaine[256];
	float valeur;
} NoeudAst;
////////////////////////////////////////////////////
// type Arbre -> définition du type AST
typedef NoeudAst *Ast;
////////////////////////////////////////////////////
// Renvoie un pointeur vers un espacio mémoire alloué pour un noeud de l'arbre syntaxique
Ast nouvelle_cellule_ast();
////////////////////////////////////////////////////
// Libère la mémoire allouée pour l'arbre syntaxique
void free_ast(Ast A);
////////////////////////////////////////////////////
#endif
