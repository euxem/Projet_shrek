#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------------------------------*/
// Include :
#include "lecture_caracteres.h"
#include "analyse_lexicale.h"

/* --------------------------------------------------------------------- */
// Variable globale :
Lexeme lexeme_en_cours; /* le lexeme courant */

/* --------------------------------------------------------------------- */

int demarrer(char *nom_fichier)
{
    int erreur = demarrer_car(nom_fichier); // Demarre l'analyse du fichier
    if (erreur == -1) // Si erreur = -1 alors le fichier est vide
    {
        return -1;
    }
    return erreur || avancer(); // Demarre la reconnaissance de lexeme
}

/* --------------------------------------------------------------------- */

int avancer()
{
    return reconnaitre_lexeme(); // reconnait le lexeme en cours
}

/* --------------------------------------------------------------------- */

Lexeme lexeme_courant()
{
    return lexeme_en_cours;
}

/* --------------------------------------------------------------------- */

int reconnaitre_lexeme()
{
    ///////////////////////////////////////////
    // Variable utile pour la fonction :
    Etat_Automate etat = E_INIT;
    char verif[10];
    ///////////////////////////////////////////
    // Vérification si la fin de fichier n'est pas atteinte.
    if (caractere_courant() == EOF)
    {
        lexeme_en_cours.nature = FIN;
        return 0;
    }

    lexeme_en_cours.chaine[0] = '\0';
    lexeme_en_cours.valeur = 0;
    lexeme_en_cours.taille = 0;

    // On utilise ensuite un automate pour reconnaitre et construire le prochain lexeme :

    while (etat != E_FIN) // Boucle jusqu'a la fin du lexeme
    {
        // on commence par lire et ignorer les separateurs
        while (est_separateur(caractere_courant()))
        {
            avancer_car();
        }
        switch (etat)
        {
        case E_INIT:
            // On initialise les numéros de lignes et colonnes
            lexeme_en_cours.ligne = numero_ligne();
            lexeme_en_cours.colonne = numero_colonne();
            // On regarde la nature du premier caractère du lexeme
            switch (nature_caractere(caractere_courant()))
            {
            case C_LETTRE: // c'est une lettre, on reconnait le mot clé
                lexeme_en_cours.nature = reconnaitre_mot_cle();
                etat = E_FIN;
                break;

            case C_CHIFFRE: // c'est un chiffre on passe dans le cas réel de l'automate
                lexeme_en_cours.nature = FLOAT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                lexeme_en_cours.valeur = caractere_courant() - '0';
                etat = E_REEL;
                avancer_car();
                break;

            case C_INTER: // c'est un intervalle, On reconnait les identificateurs dans cette intervalle (voir la grammaire)
                lexeme_en_cours.nature = L_NODE;
                if (nature_caractere(caractere_courant()) == C_CHIFFRE)
                {
                    perror("Erreur Lexicale :\n");
                    printf("Ligne %d, Colonne %d : caractère inattendu '%c'\n",
                           numero_ligne(), numero_colonne(), caractere_courant());
                    printf("HINT : Un nom de variable ne peut pas commencer par un chiffre\n");
                    return 1;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                while (caractere_courant() != ']' && caractere_courant() != EOF)
                {
                    do
                    {
                        avancer_car();
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    } while (caractere_courant() != ',' && caractere_courant() != ']' && caractere_courant() != EOF);
                    lexeme_en_cours.taille++;
                }
                if (caractere_courant() == EOF){
                    perror("Erreur Lexicale :\n");
                    printf("Ligne %d, Colonne %d : Liste IDENTIFICATEUR non fermé\n",
                    numero_ligne(), numero_colonne());
                    return 1;
                }
                etat = E_FIN;
                avancer_car();
                break;

            case C_VAGUE: // c'est un intervalle de couleur, On reconnait les couleurs dans cette intervalle (voir la grammaire)
                lexeme_en_cours.nature = L_COLOR;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                avancer_car();
                while (caractere_courant() != '~' && caractere_courant() != EOF)
                {
                    do
                    {
                        if (nature_caractere(caractere_courant()) != C_LETTRE && caractere_courant() != ',')
                        {
                            perror("Erreur Lexicale :\n");
                            printf("Ligne %d, Colonne %d : caractère inattendu '%c'\n",
                            numero_ligne(), numero_colonne(), caractere_courant());
                            printf("HINT : Un nom de couleur ne contient que des lettres\n");
                            return 1;
                        }
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                        if (caractere_courant() != ',')
                        {
                            ajouter_caractere(verif, caractere_courant());
                        }
                        else
                        {
                            if (strcmp(verif, "red") != 0 && strcmp(verif, "green") != 0 && strcmp(verif, "blue") != 0 && strcmp(verif, "yellow") != 0 && strcmp(verif, "black") != 0 && strcmp(verif, "grey") != 0)
                            {
                                perror("Erreur Lexicale :\n");
                                printf("Ligne %d, Colonne %d : caractère inattendu '%c'\n",
                                numero_ligne(), numero_colonne(), caractere_courant());
                                printf("HINT : Cette couleur n'existe pas.\n");
                                printf("Les couleurs disponibles sont : red, green, blue, yellow, black, grey\n");
                                return 1;
                            }
                            verif[0] = '\0';
                        }
                        avancer_car();
                    } while (caractere_courant() != ',' && caractere_courant() != '~' && caractere_courant() != EOF);
                    lexeme_en_cours.taille++;
                }
                if (caractere_courant() == EOF){
                    perror("Erreur Lexicale :\n");
                    printf("Ligne %d, Colonne %d : Liste COULEUR non fermé\n",
                    numero_ligne(), numero_colonne());
                    return 1;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_PARENT: // C'est une parenthèse, on reconnait le lexeme
                if (caractere_courant()=='('){
                    lexeme_en_cours.nature=PARO;
                }else{
                    lexeme_en_cours.nature=PARF;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_CROCH: // c'est un intervalle de réels, On reconnait les réels dans cette intervalle (voir la grammaire)
                lexeme_en_cours.nature = L_FLOAT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                avancer_car();
                while (caractere_courant() != '}' && caractere_courant() != EOF)
                {
                    do
                    {
                        if (nature_caractere(caractere_courant()) != C_CHIFFRE && caractere_courant() != ',' && caractere_courant() != '}')
                        {
                            perror("Erreur Lexicale :\n");
                            printf("Ligne %d, Colonne %d : caractère inattendu '%c'\n",
                            numero_ligne(), numero_colonne(), caractere_courant());
                            printf("HINT : Un crochet ne peut contenir que des chiffres\n");
                            return 1;
                        }
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                        avancer_car();
                    } while (caractere_courant() != ',' && caractere_courant() != '}' && caractere_courant() != EOF);
                    lexeme_en_cours.taille++;
                }
                if (caractere_courant() == EOF){
                    perror("Erreur Lexicale :\n");
                    printf("Ligne %d, Colonne %d : Liste FLOAT non fermé\n",
                    numero_ligne(), numero_colonne());
                    return 1;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_STR: // c'est un intervalle de string, On reconnait les chaîne de caractères dans cette intervalle (voir la grammaire)
                lexeme_en_cours.nature = L_STR;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                avancer_car();
                while (caractere_courant() != '\'' && caractere_courant() != EOF)
                {
                    do
                    {
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                        avancer_car();
                    } while (caractere_courant() != ',' && caractere_courant() != '\'' && caractere_courant() != EOF);
                    if (caractere_courant() != ',')
                    {
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    }
                    lexeme_en_cours.taille++;
                }
                if (caractere_courant() == EOF){
                    perror("Erreur Lexicale\n");
                    printf("Ligne %d, Colonne %d : Liste STR non fermé\n",
                    numero_ligne(), numero_colonne());
                    return 1;
                }
                etat = E_FIN;
                avancer_car();
                break;

            case C_GUI: // C'est le début d'une chaine de caractères, on passe dans l'état guillemet de l'automate
                lexeme_en_cours.nature = STRING;
                etat = E_GUI;
                avancer_car();
                break;

            case C_AFF: // C'est une afffectation, on reconnait le lexeme
                lexeme_en_cours.nature = AFF;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                if (caractere_courant()=='='){ // Cas "==", ça devient un opérateur booléen
                    ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    lexeme_en_cours.nature = OPE_BOOL;
                    avancer_car();
                }
                break;

            case C_OPBOOL: // C'est un opérateur booléen, on reconnait le lexeme
                lexeme_en_cours.nature = OPE_BOOL;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                if (caractere_courant()=='=' || caractere_courant()=='>'){
                    ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    avancer_car();
                }
                break;

            case C_FIN: // C'est un ;, on reconnait le lexeme
                lexeme_en_cours.nature = P_VIRG;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_DESC: // C'est un :, on reconnait le lexeme
                lexeme_en_cours.nature = DEUX_POINT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_OP: // c'est un opérateur, on reconnait le lexeme
                switch (caractere_courant())
                {
                case '+':
                    lexeme_en_cours.nature = PLUS;
                    break;
                case '-':
                    lexeme_en_cours.nature = MOINS;
                    break;
                case '*':
                    lexeme_en_cours.nature = MUL;
                    break;
                case '/':
                    lexeme_en_cours.nature = DIV;
                    break;
                case '%':
                    lexeme_en_cours.nature = MOD;
                    break;
                default:
                    break;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_CONCAT: // c'est un opérateur de concaténation, on reconnait le lexeme
                lexeme_en_cours.nature = CONCAT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_EOF: // c'est la fin du fichier, on reconnait le lexeme
                lexeme_en_cours.nature = FIN;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                break;

            case C_ERREUR: // le premier caractère produit une erreur, on va dans l'état erreur de l'automatez
                etat = E_ERREUR;
                break;
            }
            break;

        case E_GUI: // reconnaissance d'une chaîne de caractères
            if (caractere_courant() == '"')
            {
                avancer_car();
                etat = E_FIN;
            }
            else
            {
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                avancer_car();
                etat = E_GUI;
            }
            break;

        case E_REEL: // reconnaissance d'un reel
            if (nature_caractere(caractere_courant()) == C_CHIFFRE || caractere_courant() == '.')
            {
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                lexeme_en_cours.valeur = atof(lexeme_en_cours.chaine);
                etat = E_REEL;
                avancer_car();
            }
            else
            {
                etat = E_FIN;
            }
            break;

        case E_FIN: // etat final
            break;

        default:
            perror("Erreur Lexicale :\n");
            printf("Ligne %d, Colonne %d : caractère inattendu '%c'\n",
                   numero_ligne(), numero_colonne(), caractere_courant());
            return 1;
            break;
        }
    }
    if (DEBUG){ // Permet d'afficher le lexeme dans la console si le mode DEBUG est activé
        afficher_lexeme(lexeme_en_cours);
    }
    return 0;
}

Nature_Caractere nature_caractere(char c)
{
    if (c == ';')
    {
        return C_FIN;
    }
    else if (c == '=')
    {
        return C_AFF;
    }
    else if (c == ':')
    {
        return C_DESC;
    }
    else if (c == '[' || c == ']')
    {
        return C_INTER;
    }
    else if (c == '~'){
        return C_VAGUE;
    }
    else if (c == '(' || c == ')')
    {
        return C_PARENT;
    }
    else if (c == '{' || c == '}')
    {
        return C_CROCH;
    }
    else if (c == '\'')
    {
        return C_STR;
    }
    else if (c >= '0' && c <= '9')
    {
        return C_CHIFFRE;
    }
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return C_LETTRE;
    }
    else if (c == '"')
    {
        return C_GUI;
    }
    else if (c == '+' || c == '-' || c == '%' || c=='*' || c=='/' || c=='%')
    {
        return C_OP;
    }
    else if (c == '|')
    {
        return C_CONCAT;
    }
    else if (c == '<' || c == '>' )
    {
        return C_OPBOOL;
    }
    else if (c == EOF)
    {
        return C_EOF;
    }
    else
    {
        return C_ERREUR;
        perror("Caractère inconnu :\n");
        printf("L%dC%d : %c", lexeme_courant().ligne, lexeme_courant().colonne, c);
    }
}

Nature_Lexeme reconnaitre_mot_cle()
{
    /*Tant que le mot clé en cours de reconnaissance n'est pas un séparateur ou un caractère d'un autre lexeme, on ajoute à la chaine du lexeme en cours*/
    while (!est_separateur(caractere_courant()) && !est_caractère_lexeme_suivant(caractere_courant()))
    {
        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
        avancer_car();
    }
    /*On compare ensuite cette chaine à nos mots clé*/
    if (strcmp(lexeme_en_cours.chaine, "node") == 0)
    {
        return NODE;
    }
    else if (strcmp(lexeme_en_cours.chaine, "link") == 0)
    {
        return LINK;
    }
    else if (strcmp(lexeme_en_cours.chaine, "sub") == 0)
    {
        return SUBGRAPH;
    }
    else if (strcmp(lexeme_en_cours.chaine, "map") == 0)
    {
        return MAP;
    }
    else if (strcmp(lexeme_en_cours.chaine, "for") == 0)
    {
        return FOR;
    }
    else if (strcmp(lexeme_en_cours.chaine, "float") == 0)
    {
        return FLOAT;
    }
    else if (strcmp(lexeme_en_cours.chaine, "write") == 0)
    {
        return ECRIRE;
    }
    else if (strcmp(lexeme_en_cours.chaine, "if") == 0)
    {
        return SI;
    }
    else if (strcmp(lexeme_en_cours.chaine, "rof") == 0)
    {
        return ROF;
    }
    else if (strcmp(lexeme_en_cours.chaine, "then") == 0)
    {
        return ALORS;
    }
    else if (strcmp(lexeme_en_cours.chaine, "else") == 0)
    {
        return SINON;
    }
    else if (strcmp(lexeme_en_cours.chaine, "fi") == 0)
    {
        return FSI;
    }
    else if (strcmp(lexeme_en_cours.chaine, "closesub") == 0)
    {
        return CLOSUB;
    }
    else if (strcmp(lexeme_en_cours.chaine, "read") == 0)
    {
        return LIRE;
    }
    else if (strcmp(lexeme_en_cours.chaine, "while") == 0)
    {
        return TANT_QUE;
    }
    else if (strcmp(lexeme_en_cours.chaine, "do") == 0)
    {
        return FAIRE;
    }
    else if (strcmp(lexeme_en_cours.chaine, "done") == 0)
    {
        return FAIT;
    }
    else if (strcmp(lexeme_en_cours.chaine, "quit") == 0)
    {
        return QUIT;
    }
    else if ((strcmp(lexeme_en_cours.chaine, "red") == 0) || (strcmp(lexeme_en_cours.chaine, "green") == 0) || (strcmp(lexeme_en_cours.chaine, "blue") == 0) || (strcmp(lexeme_en_cours.chaine, "yellow") == 0) || (strcmp(lexeme_en_cours.chaine, "black") == 0) || (strcmp(lexeme_en_cours.chaine, "grey") == 0))
    {
        return COLOR;
    }
    else
    {
        return IDF;
    }
}

void ajouter_caractere(char *s, char c)
{
    int l;
    l = strlen(s);
    s[l] = c;
    s[l + 1] = '\0';
}

void afficher_lexeme(Lexeme lex)
{
    char natstr[20];
    switch (lex.nature)
    {
    case NODE:
        strcpy(natstr, "NODE,");
        break;
    case SI:
        strcpy(natstr, "SI,");
        break;
    case ALORS:
        strcpy(natstr, "ALORS,");
        break;
    case SINON:
        strcpy(natstr, "SINON,");
        break;
    case TANT_QUE:
        strcpy(natstr, "TANT_QUE,");
        break;
    case FAIRE:
        strcpy(natstr, "FAIRE,");
        break;
    case FAIT:
        strcpy(natstr, "FAIT,");
        break;
    case FSI:
        strcpy(natstr, "FSI,");
        break;
    case LINK:
        strcpy(natstr, "LINK,");
        break;
    case SUBGRAPH:
        strcpy(natstr, "SUBGRAPH,");
        break;
    case MAP:
        strcpy(natstr, "MAP,");
        break;
    case FOR:
        strcpy(natstr, "FOR,");
        break;
    case IDF:
        strcpy(natstr, "IDF,");
        break;
    case STRING:
        strcpy(natstr, "STRING,");
        break;
    case FLOAT:
        strcpy(natstr, "FLOAT,");
        break;
    case COLOR:
        strcpy(natstr, "COLOR,");
        break;
    case L_FLOAT:
        strcpy(natstr, "L_FLOAT,");
        break;
    case L_NODE:
        strcpy(natstr, "L_NODE,");
        break;
    case L_STR:
        strcpy(natstr, "L_STR,");
        break;
    case L_COLOR:
        strcpy(natstr, "L_COLOR,");
        break;
    case P_VIRG:
        strcpy(natstr, "P_VIRG,");
        break;
    case LIRE:
        strcpy(natstr, "LIRE,");
        break;
    case PARO:
        strcpy(natstr, "PARO,");
        break;
    case ECRIRE:
        strcpy(natstr, "ECRIRE,");
        break;
    case DEUX_POINT:
        strcpy(natstr, "DEUX_POINT,");
        break;
    case PLUS:
        strcpy(natstr, "PLUS,");
        break;
    case MUL:
        strcpy(natstr, "MUL,");
        break;
    case MOINS:
        strcpy(natstr, "MOINS,");
        break;
    case DIV:
        strcpy(natstr, "DIV,");
        break;
    case ROF:
        strcpy(natstr, "ROF,");
        break;
    case OPE_BOOL:
        strcpy(natstr, "OPE_BOOL,");
        break;
    case AFF:
        strcpy(natstr, "AFF,");
        break;
    case CONCAT:
        strcpy(natstr, "CONCAT,");
        break;
    case FIN:
        strcpy(natstr, "FIN,");
        break;
    case QUIT:
        strcpy(natstr, "QUIT,");
        break;
    case ERREUR:
        strcpy(natstr, "ERREUR,");
        break;
    default:
        break;
    };
    // On afiche, selon la nature du lexeme, ses informations.
    printf("Ligne %d, Colonne %d : ", numero_ligne(), numero_colonne());
    printf("Nature : %-8s Valeur : %g, \tChaine : %s\n", natstr, lex.valeur, lex.chaine);
}

int est_caractère_lexeme_suivant(char c){
    // Liste tout les caractères succeptible d'apartenir au lexeme suivant
    return (c==':' || 
            c==';' || 
            c=='(' || 
            c==')' || 
            c=='=' || 
            c=='<' || 
            c=='>' || 
            c=='+' || 
            c=='-' || 
            c=='*' || 
            c=='/' || 
            c=='%' || 
            c=='"' || 
            c=='|' || 
            c=='~' || 
            c==EOF);
}

void perror(const char *s)
{
    // Format du printf permettant d'afficher les erreur en gras et rouge
    printf("\x1b[1;31m%s\x1b[0m", s);
}

void pwarn(const char *s)
{
    // Format du printf permettant d'afficher les warning en gras et jaune
    printf("\x1b[1;33m%s\x1b[0m", s);
}
