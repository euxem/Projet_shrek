#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lecture_caracteres.h"
#include "analyse_lexicale.h"

/* --------------------------------------------------------------------- */
// Variable globale :
Lexeme lexeme_en_cours; /* le lexeme courant */

/* --------------------------------------------------------------------- */

// Reconnait un intervalle ou une énumération de caractères et d'entiers
// Retourne un tableau contenant les caractères/entiers de l'intervalle
// Exemple : [a-e] -> [a,b,c,d,e]
// Exemple : [0-9] -> [0,1,2,3,4,5,6,7,8,9]
int intervalle(char *c)
{

    // pointeur de lecture
    char *ptr = c;

    // termes de l'intervalle
    char terme1, terme2;

    // tableau étendu de l'intervalle
    int *tab = malloc(TAILLE_INTER_MAX * sizeof(int));

    if (*ptr != '[')
    {
        perror("Erreur d'intervalle :\n");
        afficher_lexeme(lexeme_en_cours);
        printf("HINT : un intervalle doit commencer par '['");
        exit(1);
    }

    // on lit le premier terme de l'intervalle
    terme1 = *(++ptr);
    if (*(++ptr) != '-')
    {
        perror("Erreur d'intervalle :\n");
        afficher_lexeme(lexeme_en_cours);
        printf("HINT : un intervalle doit contenir '-'");
        exit(1);
    }

    // on lit le second terme de l'intervalle
    terme2 = *(++ptr);
    if (*(++ptr) != ']')
    {
        perror("Erreur d'intervalle :\n");
        afficher_lexeme(lexeme_en_cours);
        printf("HINT : un intervalle doit se terminer par ']'");
        exit(1);
    }    

    // on vérifie que l'intervalle est bien formé
    if (terme1 > terme2)
    {
        perror("Erreur d'intervalle :\n");
        afficher_lexeme(lexeme_en_cours);
        printf("HINT : le premier terme de l'intervalle doit être inférieur au second");
        exit(1);
    }

    // on remplit le tableau avec les caractères de l'intervalle
    int i = 0;
    for (char c = terme1; c <= terme2; c++)
    {
        tab[i++] = c;
        tab[i++] = ','; // on ajoute une virgule entre chaque caractère
    }
    tab[i - 1] = '\0'; // on remplace la dernière virgule par un caractère de fin de chaine

    return *tab;
}

/* --------------------------------------------------------------------- */

void demarrer(char *nom_fichier)
{
    demarrer_car(nom_fichier);
    avancer();
}

/* --------------------------------------------------------------------- */

void avancer()
{
    reconnaitre_lexeme();
}

/* --------------------------------------------------------------------- */

Lexeme lexeme_courant()
{
    return lexeme_en_cours;
}

/* --------------------------------------------------------------------- */

void reconnaitre_lexeme()
{
    Etat_Automate etat = E_INIT;

    char verif[10];

    if (caractere_courant() == EOF)
    {
        lexeme_en_cours.nature = FIN;
        return;
    }

    lexeme_en_cours.chaine[0] = '\0';
    lexeme_en_cours.valeur = 0;
    lexeme_en_cours.taille = 0;

    // On utilise ensuite un automate pour reconnaitre et construire le prochain lexeme :

    while (etat != E_FIN)
    {
        // on commence par lire et ignorer les separateurs
        while (est_separateur(caractere_courant()))
        {
            avancer_car();
        }
        switch (etat)
        {
        case E_INIT:
            lexeme_en_cours.ligne = numero_ligne();
            lexeme_en_cours.colonne = numero_colonne();
            switch (nature_caractere(caractere_courant()))
            {
            case C_LETTRE:
                lexeme_en_cours.nature = reconnaitre_mot_cle();
                etat = E_FIN;
                if (!est_caractère_lexeme_suivant(caractere_courant()))
                {
                    avancer_car();
                }
                break;

            case C_CHIFFRE:
                lexeme_en_cours.nature = FLOAT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                lexeme_en_cours.valeur = caractere_courant() - '0';
                etat = E_REEL;
                avancer_car();
                break;

            case C_INTER:
                lexeme_en_cours.nature = L_NODE;
                if (nature_caractere(caractere_courant()) == C_CHIFFRE)
                {
                    printf("Erreur_Lexicale à la ligne %d, colonne %d : caractère inattendu '%c',%d\n",
                           numero_ligne(), numero_colonne(), caractere_courant(), caractere_courant());
                    printf("HINT : Un nom de variable ne peut pas commencer par un chiffre\n");
                    exit(1);
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                while (caractere_courant() != ']')
                {
                    do
                    {
                        avancer_car();
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    } while (caractere_courant() != ',' && caractere_courant() != ']');
                    lexeme_en_cours.taille++;
                }
                etat = E_FIN;
                avancer_car();
                break;

            case C_VAGUE:
                lexeme_en_cours.nature = L_COLOR;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                avancer_car();
                while (caractere_courant() != '~')
                {
                    do
                    {
                        if (nature_caractere(caractere_courant()) != C_LETTRE && caractere_courant() != ',')
                        {
                            printf("Erreur_Lexicale à la ligne %d, colonne %d : caractère inattendu '%c',%d\n",
                                   numero_ligne(), numero_colonne(), caractere_courant(), caractere_courant());
                            printf("HINT : Un nom de couleur ne contient que des lettres\n");
                            exit(1);
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
                                printf("Erreur_Lexicale à la ligne %d, colonne %d : caractère inattendu '%c',%d\n",
                                       numero_ligne(), numero_colonne(), caractere_courant(), caractere_courant());
                                printf("HINT : Cette couleur n'existe pas.\n");
                                printf("Les couleurs disponibles sont : red, green, blue, yellow, black, grey\n");
                                exit(1);
                            }
                            verif[0] = '\0';
                        }
                        avancer_car();
                    } while (caractere_courant() != ',' && caractere_courant() != '~');
                    lexeme_en_cours.taille++;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_PARENT:
                if (caractere_courant()=='('){
                    lexeme_en_cours.nature=PARO;
                }else{
                    lexeme_en_cours.nature=PARF;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_CROCH:
                lexeme_en_cours.nature = L_FLOAT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                avancer_car();
                while (caractere_courant() != '}')
                {
                    do
                    {
                        if (nature_caractere(caractere_courant()) != C_CHIFFRE && caractere_courant() != ',' && caractere_courant() != '}')
                        {
                            printf("Erreur_Lexicale à la ligne %d, colonne %d : caractère inattendu '%c',%d\n",
                                   numero_ligne(), numero_colonne(), caractere_courant(), caractere_courant());
                            printf("HINT : Un crochet ne peut contenir que des chiffres\n");
                            exit(1);
                        }
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                        avancer_car();
                    } while (caractere_courant() != ',' && caractere_courant() != '}');
                    lexeme_en_cours.taille++;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_STR:
                lexeme_en_cours.nature = L_STR;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                avancer_car();
                while (caractere_courant() != '\'')
                {
                    do
                    {
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                        avancer_car();
                    } while (caractere_courant() != ',' && caractere_courant() != '\'');
                    if (caractere_courant() != ',')
                    {
                        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    }
                    lexeme_en_cours.taille++;
                }
                etat = E_FIN;
                avancer_car();
                break;

            case C_GUI:
                lexeme_en_cours.nature = STRING;
                etat = E_GUI;
                avancer_car();
                break;

            case C_AFF:
                lexeme_en_cours.nature = AFF;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                if (caractere_courant()=='='){
                    ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    lexeme_en_cours.nature = OPE_BOOL;
                    avancer_car();
                }
                break;

            case C_OPBOOL:
                lexeme_en_cours.nature = OPE_BOOL;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                if (caractere_courant()=='=' || caractere_courant()=='>'){
                    ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                    avancer_car();
                }
                break;

            case C_FIN:
                lexeme_en_cours.nature = P_VIRG;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_DESC:
                lexeme_en_cours.nature = DEUX_POINT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_OP:
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
                default:
                    break;
                }
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_CONCAT:
                lexeme_en_cours.nature = CONCAT;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                avancer_car();
                break;

            case C_EOF:
                lexeme_en_cours.nature = FIN;
                ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
                etat = E_FIN;
                break;

            default:
                etat = E_ERREUR;
                break;
            }
            break;

        case E_GUI:
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
            printf("Erreur_Lexicale à la ligne %d, colonne %d : caractère inattendu '%c',%d\n",
                   numero_ligne(), numero_colonne(), caractere_courant(), caractere_courant());
            exit(1);
            break;
        }
    }
    if (DEBUG){
        afficher_lexeme(lexeme_en_cours);
    }
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
    else if (c == '+' || c == '-' || c == '%' || c=='*' || c=='/')
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
    while (!est_separateur(caractere_courant()) && !est_caractère_lexeme_suivant(caractere_courant()))
    {
        ajouter_caractere(lexeme_en_cours.chaine, caractere_courant());
        avancer_car();
    }
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
    case ERREUR:
        strcpy(natstr, "ERREUR,");
        break;
    default:
        break;
    };

    printf("Ligne %d, Colonne %d : ", numero_ligne(), numero_colonne());
    printf("Nature : %-8s Valeur : %g, \tChaine : %s\n", natstr, lex.valeur, lex.chaine);
}

int est_caractère_lexeme_suivant(char c){
    return (c==':' || c==';' || c=='(' || c==')' || c=='=' || c=='<' || c=='>' || c=='+' || c=='-' || c=='*' || c=='/' || c=='%' || c=='"' || c=='|');
}

void perror(const char *s)
{
    printf("\x1b[1;31%s\x1b[0m", s);
}
