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

int traduire_fichier(char *ipath, char *opath)
{
    Ast A;
    FILE *f;
    int SUB = 0;

    f = fopen(opath, "w");

    if (analyse(ipath, &A) != 0)
    {
        printf("\x1b[1;33mWARNING : \x1b[0m");
        printf("Erreur d'analyse du fichier %s\n\n", ipath);
        return 1;
    }

    fprintf(f, "graph G {\n");
    SUB = interpreter(A, f);
    if (SUB)
    {
        fprintf(f, "\t}\n");
    }
    fprintf(f, "}\n");

    printf("\x1b[1;32mSuccès de la traduction : \x1b[0m");
    printf("fichier %s généré.\n", opath);

    free_ast(A);
    free_chainee();
    fclose(f);

    return 0;
}

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
            printf("Usage: %s [-i input file] [-o output file] [-h]\n", argv[0]);
            printf("Usage: %s [-f input folder] [-h]\n", argv[0]);
            printf("Si aucun fichier d'entrée n'est spécifié, l'entrée standard est utilisée.\n");
            printf("Si aucun fichier de sortie n'est spécifié, le fichier 'output.dot' est utilisé.\n");
            printf("Si un dossier est spécifié, tous les fichiers .shrek du dossier seront traduits.\n");
            printf("Le fichiers .dot seront créés dans un sous dossier /dot_generes.\n");
            return 0;
        case '?':
        default:
            return 1;
        }
    }

    if (folder)
    {
        DIR *d;
        struct dirent *dir;
        d = opendir(fpath);

        if (d == NULL)
        {
            perror("Erreur d'ouverture du dossier\n");
            printf("%s/%s\n", argv[0], fpath);
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
                    // Free
                    free(ipath);
                    free(opath);
                }
            }
        }
        free(dotpath);
        closedir(d);
    }
    else
    {
        // Si aucun fichier d'entrée n'est spécifié, on utilise l'entrée standard
        if (ipath == NULL)
        {
            ipath = malloc(1);
            strcpy(ipath, "");
        }
        // Si aucun fichier de sortie n'est spécifié, on utilise output.dot
        if (opath == NULL)
        {
            opath = malloc(11);
            strcpy(opath, "output.dot");
        }
        traduire_fichier(ipath, opath);
        free(ipath);
        free(opath);
    }

    if (err)
    {
        char *err_msg = malloc(30);
        printf("---------------\n");
        sprintf(err_msg, "%d/%d fichier(s) ont généré des erreurs.\n", err, nb_files);
        perror(err_msg);
        printf("Un fichier .dot n'a pas été généré pour ces fichiers.\n");
        return err;
    }

    return 0;
}
