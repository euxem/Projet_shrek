#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "analyse_lexicale.h"
#include "lecture_caracteres.h"
#include "analyse_syntaxique.h"
#include "type_ast.h"
#include "arbre_ast.h"

////////////////////////////////////////////////////
// Permet de vider le buffer d'entree standard
void viderBuffer()
{
    int c = caractere_courant();
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

////////////////////////////////////////////////////
// Affiche le menu d'aide dans le terminale
void show_help(char **argv)
{
    printf("Usage: %s [-i input file] [-o output file] [-h]\n", argv[0]);
    printf("Usage: %s [-f input folder] [-h]\n", argv[0]);
    printf("Si aucun fichier d'entrée n'est spécifié, l'entrée standard est utilisée.\n");
    printf("Si aucun fichier de sortie n'est spécifié, le fichier 'output.dot' est utilisé.\n");
    printf("Si un dossier est spécifié, tous les fichiers .shrek du dossier seront traduits.\n");
    printf("Le fichiers .dot seront créés dans un sous dossier /dot_generes.\n");
}

////////////////////////////////////////////////////
/* Lance l'analyse lexicale et syntaxique du fichier, 
pour ensuite lancer l'interprétation et la traduction du fichier en .dot */
int traduire_fichier(char *ipath, char *opath)
{
    /////////////////////////////
    // Types :
    Ast A=NULL;             // Racine de l'arbre Ast à interpréter
    FILE *f;                // Fichier de sortie (output en .dot)
    int nb_closed_sub = 0;  // Nombre de sous_graph fermé
    int nb_sub = 0;         // Nombre de sous_graph ouvert

    f = fopen(opath, "w");  // Ouverture du fichier d'écriture

    int err_analyse = analyse(ipath, &A);   // Analyse du fichier
    if (err_analyse > 0)                    // Si 1, erreur d'analyse -> WARNING
    {
        pwarn("WARNING: analyse\n");
        printf("Erreur d'analyse du fichier %s\n", ipath ? ipath : "stdin");
        return 1;
    }
    else if (err_analyse < 0)               // Si -1, fichier vide -> WARNING
    {
        pwarn("WARNING: fichier vide\n");
        printf("Le fichier %s est vide\n", ipath ? ipath : "stdin");
        return 0;
    }

    viderBuffer();                          // Vide le buffer en cours (utile pour le mode entrée standard)

    fprintf(f, "graph G {\n");              // Ouvre le graph principale dans le .dot
    if (interpreter(A, f, &nb_closed_sub, &nb_sub) != 0) // Interprétation du programme (si !=0 -> WARNING)
    {
        pwarn("WARNING: interprétation\n");
        printf("Erreur d'interprétation du fichier %s\n", ipath ? ipath : "stdin");
        return 1;
    }
    if (nb_closed_sub < nb_sub)             // Si il y a plus de sous-graph ouvert que de sous-graph clos -> ERREUR D'INTERPRÉTATION
    {
        perror("Erreur: interprétation\n");
        printf("Subgraph non clos\n");
        pwarn("WARNING: \n");
        printf("Erreur d'interprétation du fichier %s\n", ipath ? ipath : "stdin");
        return 1;
    }
    if (nb_closed_sub > nb_sub)             // Si il y a moins de sous-graph ouvert que de sous-graph clos -> ERREUR D'INTERPRÉTATION
    {
        perror("Erreur: interprétation\n");
        printf("Subgraph clos mais jamais ouvert\n");
        pwarn("WARNING: \n");
        printf("Erreur d'interprétation du fichier %s\n", ipath ? ipath : "stdin");
        return 1;
    }
    fprintf(f, "}\n");      // Fermeture du graph principale 

    printf("\x1b[1;32mSuccès de la traduction : \x1b[0m");  // succès de la traduction
    printf("fichier %s généré.\n", opath);                  // lieu de la génération

    free_ast(A);    // libération de l'arbre AST
    free_chainee(); // libération de la liste chaînée
    fclose(f);      // fermeture du fichier

    return 0;
}

////////////////////////////////////////////////////
// Programme principale, gère l'interface utilisateur
int main(int argc, char **argv)
{
    // Compteur d'erreurs et de fichiers traités
    int err = 0;
    int nb_files = 0;

    // Valeurs par défaut des arguments
    char *ipath = NULL;
    char *opath = NULL;
    char *fpath = NULL;

    int c;

    int folder = 0;

    if (argc == 2)
    {
        show_help(argv);
        return 1;
    }

    // Parcours des options
    while ((c = getopt(argc, argv, "i:o:f:h")) != -1)
    {
        switch (c)
        {
        case 'i':
            ipath = optarg;
            break;
        case 'o':
            opath = optarg;
            break;
        case 'f':
            fpath = optarg;
            folder = 1;
            break;
        case 'h':
            show_help(argv);
            return 0;
        case '?':
        default:
            return 1;
        }
    }

    // Si un dossier est spécifié
    if (folder)
    {
        DIR *d;
        struct dirent *dir;
        d = opendir(fpath); // Ouverture du dossier

        if (d == NULL)
        {
            perror("Erreur d'ouverture du dossier\n");
            printf("%s\n", fpath);
            printf("HINT : vérifiez que le chemin est correct et que vous avez les droits.\n");
            return 1;
        }

        // Vérifier que fpath ne termine pas par un /
        if (fpath[strlen(fpath) - 1] == '/')
        {
            fpath[strlen(fpath) - 1] = '\0';
        }

        // Crée le dossier .dot en sous dossier
        char *dotpath = malloc(strlen(fpath) + 13);
        strcpy(dotpath, fpath);
        strcat(dotpath, "/dot_generes");
        struct stat st = {0};
        if (stat(dotpath, &st) == -1)
        {
            mkdir(dotpath, 0700);
        }

        // Pour chaque fichier dans le dossier
        while ((dir = readdir(d)) != NULL)
        {
            // Si le fichier est un fichier régulier
            if (dir->d_type == DT_REG)
            {
                // Si le fichier a l'extension .shrek
                char *ext = strrchr(dir->d_name, '.');
                if (ext && strcmp(ext, ".shrek") == 0)
                {
                    // On compte le fichier traité
                    nb_files++;

                    // Assemble le chemin du fichier d'entrée
                    ipath = (char *)malloc((strlen(dir->d_name) + strlen(fpath) + 2) * sizeof(char));
                    strcpy(ipath, fpath);
                    strcat(ipath, "/");
                    strcat(ipath, dir->d_name);

                    // Assemble le chemin du fichier .dot (sans l'extension shrek)
                    opath = (char *)malloc((strlen(dir->d_name) + strlen(dotpath) + 6) * sizeof(char));
                    strcpy(opath, dotpath);
                    strcat(opath, "/");
                    strncat(opath, dir->d_name, strlen(dir->d_name) - 6);
                    strcat(opath, ".dot");

                    if (traduire_fichier(ipath, opath) != 0)
                    {
                        // Si une erreur est rencontrée, on incrémente le compteur d'erreurs
                        err++;
                        // Et on supprime le fichier .dot
                        remove(opath);
                    }
                    printf("\n------\n");

                    // Free les strings des chemins
                    free(ipath);
                    free(opath);
                }
            }
        }
        printf("\n\x1b[2F---------------\n");
        printf("Tous les fichiers ont été traités avec succès.\n");
        free(dotpath);
        closedir(d);
    }
    else // Si on travaille sur un fichier unique (stdin ou file)
    {
        // Si aucun fichier de sortie n'est spécifié, on utilise output.dot
        if (opath == NULL)
        {
            opath = malloc(11);
            strcpy(opath, "output.dot");
        }
        traduire_fichier(ipath, opath);
    }

    // Affichage de l'erreur rencontrée
    if (err)
    {
        char *err_msg = malloc(30);
        printf("\n\x1b[1F---------------\n");
        sprintf(err_msg, "%d/%d fichier(s) ont généré des erreurs.\n", err, nb_files);
        perror(err_msg);
        printf("Un fichier .dot n'a pas été généré pour ces programmes.\n");
        return err;
    }

    return 0;
}
