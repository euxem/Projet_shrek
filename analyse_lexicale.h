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
   MAP,        // La nature du lexeme est le mot clé map
   FOR,        // La nature du lexeme est le mot clé for
   ROF,        // La nature du lexeme est le mot clé rof (fin du for)
   IDF,        // La nature du lexeme est le mot clé un identificateur
   AFF,        // La nature du lexeme est une affectation (=)
   STRING,     // La nature du lexeme est une string ("example")
   FLOAT,      // La nature du lexeme est un réel (10.2)
   COLOR,      // La nature du lexeme est une couleur (red, blue...)
   L_FLOAT,    // La nature du lexeme est une liste de floattant ({1,2,3})
   L_NODE,     // La nature du lexeme est une liste d'identificateur ([a,b,c])
   L_STR,      // La nature du lexeme est une liste de string 'i,j,k'
   L_COLOR,    // La nature du lexeme est une liste de couleur ~red,blue,green~
   P_VIRG,     // La nature du lexeme est une point virgule (;)
   LIRE,       // La nature du lexeme est le mot clé read
   ECRIRE,     // La nature du lexeme est le mot clé write
   DEUX_POINT, // La nature du lexeme est un deux point (:)
   PLUS,       // La nature du lexeme est un plus (+)
   MOINS,      // La nature du lexeme est un moins (-)
   MUL,        // La nature du lexeme est une étoile (*)
   DIV,        // La nature du lexeme est un slash (\)
   PARO,       // La nature du lexeme est une parenthèse ouverte (()
   PARF,       // La nature du lexeme est une parenthèse fermé ())
   FIN,        // La nature du lexeme est une fin de fichier (EOF)
   SI,         // La nature du lexeme est le mot clé if
   ALORS,      // La nature du lexeme est le mot clé then
   SINON,      // La nature du lexeme est le mot clé else
   FSI,        // La nature du lexeme est le mot clé fi (fin de if)
   OPE_BOOL,   // La nature du lexeme est un opérateur booléen (<,>,<=,==,<>,>=)
   CONCAT,     // La nature du lexeme est l'opérateur de concaténation (|)
   TANT_QUE,   // La nature du lexeme est le mot clé while
   FAIRE,      // La nature du lexeme est le mot clé do
   FAIT,       // La nature du lexeme est done (fin de while)
   CLOSUB,     // La nature du lexeme est le mot clé closesub (fin de subgraph)
   ERREUR      // Erreur de nature lexeme
} Nature_Lexeme;

/*---------------------------------------------------------------*/
// Struct type -> Type Lexeme
typedef struct
{
   Nature_Lexeme nature; // nature du lexeme
   char chaine[256];     // chaine de caracteres
   float valeur;         // valeur d'un réel
   int taille;           // nombre d'élément dans un intervalle.
   int ligne;
   int colonne;
} Lexeme;

/*---------------------------------------------------------------*/
// Enumerate Type ->  Automate reconnaître lexeme
typedef enum
{
   E_INIT,       // Etat initial (reconnaît le premier caractère du lexeme)
   E_REEL,       // Etat Reel (reconnait les réelles)
   E_GUI,        // Etat Gui  (reconnait les chaînes de caractères)
   E_ERREUR,     // Etat Erreur (erreur lors de l'analyse du lexeme)
   E_FIN         // Sortie de l'automate
} Etat_Automate;

/*---------------------------------------------------------------*/
// Fonctions :
/*Demarre l'analyse du fichier et analyse le premier lexeme,
renvoie une erreur (1) si le fichier n'a pas pu être lu ou le lexeme est erroné*/
int demarrer(char *nom_fichier);
//////////
/*Reconnait le lexeme suivant,
Si le lexeme suivant est incorrecte, renvoie 1 sinon 0.*/
int avancer();
//////////
/*Retourne le lexeme en cours*/
Lexeme lexeme_courant();
//////////
/*Reconnait le lexeme suivant grâce à l'automate et la Nature du caractère lu,
Si le lexeme suivant est incorrecte, renvoie 1 sinon 0.*/
int reconnaitre_lexeme();
//////////
/*Renvoie la nature du caractère c*/
Nature_Caractere nature_caractere(char c);
//////////
/*Renvoie la nature correspondant au mot clé lu*/
Nature_Lexeme reconnaitre_mot_cle();
//////////
/*Ajoute le caractère c à la chaîne de caractères s*/
void ajouter_caractere(char *s, char c);
//////////
// Fonctions pour l'affichage d'un lexeme
// Affiche la ligne et la colonne du lexeme dans le fichier
// Et le contenu du lexeme
void afficher_lexeme(Lexeme lex);
//////////
// Prédicat permettant de savoir si le caractère apartient au lexeme en cours ou au suivant
int est_caractère_lexeme_suivant(char c);
//////////
// Afficher une string d'erreur
// (En rouge et en gras)
void perror(const char *s);

#endif
