#ifndef __ANALYSE_LEX_H__
#define __ANALYSE_LEX_H__

/*---------------------------------------------------------------*/
#define DEBUG 0 // DEBUG Variable -> permet d'avoir un affichage des lexemes lu.

/*---------------------------------------------------------------*/
// Enumerate Type -> pour analyser le premier caractère.
typedef enum
{
   C_CHIFFRE, // le caractère est 0,1,...,9
   C_INTER,   // le caractère est [
   C_PARENT,  // le caractère est (
   C_CROCH,   // le caractère est {
   C_STR,     // le caractère est '
   C_FIN,     // le caractère est ;
   C_AFF,     // le caractère est =
   C_GUI,     // le caractère est "
   C_DESC,    // le caractère est :
   C_OP,      // le caractère est + ou - ou / ou *
   C_VAGUE,   // le caractère est ~
   C_ERREUR,  // Si le caractère est inconnue
   C_OPBOOL,  // le caractère est < ou >
   C_CONCAT,  // le caractère est |
   C_EOF,     // le caractère est EOF
   C_LETTRE   // le caractère est une lettre (a-z ou A-Z)
} Nature_Caractere;

/*---------------------------------------------------------------*/
// Enumerate Type -> Définie la nature du lexeme
typedef enum
{
   NODE,       // La nature du lexeme est le mot clé node
   LINK,       // La nature du lexeme est le mot clé link
   SUBGRAPH,   // La nature du lexeme est le mot clé sub
   MAP,
   FOR,
   ROF,
   IDF,
   AFF,
   STRING,
   FLOAT,
   COLOR,
   L_FLOAT,
   L_NODE,
   L_STR,
   L_COLOR,
   P_VIRG,
   LIRE,
   ECRIRE,
   DEUX_POINT,
   PLUS,
   MOINS,
   MUL,
   DIV,
   PARO,
   PARF,
   FIN,
   SI,
   ALORS,
   SINON,
   FSI,
   OPE_BOOL,
   CONCAT,
   TANT_QUE,
   FAIRE,
   FAIT,
   CLOSUB,
   ERREUR
} Nature_Lexeme;

typedef struct
{
   Nature_Lexeme nature; // nature du lexeme
   char chaine[256];     // chaine de caracteres
   float valeur;         // valeur d'un réel
   int taille;           // nombre d'élément dans un intervalle.
   int ligne;
   int colonne;
} Lexeme;

typedef enum
{
   E_INIT,
   E_REEL,
   E_GUI,
   E_ERREUR,
   E_FIN
} Etat_Automate;

/* --------------------------------------------------------------------- */
// Fonctions :
// int intervalle(char c); // Implémentée mais non utilisée
void demarrer(char *nom_fichier);
void avancer();
Lexeme lexeme_courant();
void reconnaitre_lexeme();
Nature_Caractere nature_caractere(char c);
Nature_Lexeme reconnaitre_mot_cle();
void ajouter_caractere(char *s, char c);

// Fonctions pour l'affichage d'un lexeme
// Affiche la ligne et la colonne du lexeme dans le fichier
// Et le contenu du lexeme
void afficher_lexeme(Lexeme lex);
int est_caractère_lexeme_suivant(char c);

// Afficher une string d'erreur
// (En rouge et en gras)
void perror(const char *s);

#endif
