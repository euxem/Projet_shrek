#ifndef __ANALYSE_LEX_H__
#define __ANALYSE_LEX_H__

#define TAILLE_INTER_MAX 100

#define DEBUG 0

typedef enum
{
   C_CHIFFRE,
   C_INTER,
   C_PARENT,
   C_CROCH,
   C_STR,
   C_LETTRE,
   C_FIN,
   C_AFF,
   C_GUI,
   C_DESC,
   C_OP,
   C_VAGUE,
   C_ERREUR,
   C_OPBOOL,
   C_CONCAT,
   C_EOF
} Nature_Caractere;

typedef enum
{
   NODE,
   LINK,
   SUBGRAPH,
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
// int intervalle(char c);
void demarrer(char *nom_fichier);
void avancer();
Lexeme lexeme_courant();
void reconnaitre_lexeme();
Nature_Caractere nature_caractere(char c);
Nature_Lexeme reconnaitre_mot_cle();
void ajouter_caractere(char *s, char c);
void afficher_lexeme(Lexeme lex);
int est_caractère_lexeme_suivant(char c);
void perror(const char *s);

#endif
