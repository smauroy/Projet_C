
#include "save.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Fonction de sauvegarde */
int save_translation(const char *original, const char *translated, const char *source_language, const char *target_language) {
    
    /* Vérification des données */
    if (original == NULL || translated == NULL ||
        source_language == NULL || target_language == NULL)
        return 1;

    /* Vérification des limites de taille */
    if (strlen(original) >= 10000 ||
        strlen(translated) >= 10000 ||
        strlen(source_language) >= 100 ||
        strlen(target_language) >= 100)
        return 1;

    /* Ouverture du fichier en ajout binaire */
    FILE *f = fopen("src/storage/history.bin", "ab");

    if (f == NULL)
        return 1;

    /* Calcul de la taille de chaque chaîne de caractères */
    int original_size = strlen(original);
    int translated_size = strlen(translated);
    int source_size = strlen(source_language);
    int target_size = strlen(target_language);

    /* Enregistrement des tailles dans le fichier binaire */
    /* Indispensable pour une retranscription claire de l'historique */
    int success =
        fwrite(&original_size, sizeof(int), 1, f) == 1 &&
        fwrite(&translated_size, sizeof(int), 1, f) == 1 &&
        fwrite(&source_size, sizeof(int), 1, f) == 1 &&
        fwrite(&target_size, sizeof(int), 1, f) == 1;

    /* Enregistrement du contenu des chaînes de caractères */
    if (success) {
        success =
            fwrite(original, sizeof(char), original_size, f) == (size_t)original_size &&
            fwrite(translated, sizeof(char), translated_size, f) == (size_t)translated_size &&
            fwrite(source_language, sizeof(char), source_size, f) == (size_t)source_size &&
            fwrite(target_language, sizeof(char), target_size, f) == (size_t)target_size;
    }

    /* Fermeture du fichier */
    if (fclose(f) != 0)
        success = 0;

    if (!success)
        return 1;

    return export_history_to_txt();
}


/* Fonction de retranscription du fichier binaire vers le fichier texte */
int export_history_to_txt(void)
{
    /* Ouverture du fichier binaire en lecture */
    FILE *bin = fopen("src/storage/history.bin", "rb");

    /* Ouverture du fichier texte en écriture */
    FILE *txt = fopen("src/storage/history.txt", "w");

    /* Vérifie que les deux fichiers ont bien été ouverts */
    if (bin == NULL || txt == NULL) {
        if (bin != NULL) fclose(bin);
        if (txt != NULL) fclose(txt);
        return 1;
    }

    /* Tableau contenant les tailles des quatre chaînes */
    int sizes[4];

    /* Parcourt les enregistrements présents dans le fichier binaire */
    while (fread(&sizes[0], sizeof(int), 1, bin) == 1) {

        /* Récupère les trois autres tailles enregistrées */
        if (fread(&sizes[1], sizeof(int), 3, bin) != 3) {
            fclose(bin);
            fclose(txt);
            return 1;
        }

        /* Vérifie que les tailles correspondent aux limites définies */
        /* Évite notamment les tailles négatives ou trop importantes */
        if (sizes[0] < 0 || sizes[0] >= 10000 ||
            sizes[1] < 0 || sizes[1] >= 10000 ||
            sizes[2] < 0 || sizes[2] >= 100 ||
            sizes[3] < 0 || sizes[3] >= 100) {
            fclose(bin);
            fclose(txt);
            return 1;
        }

        /* Tableau de pointeurs destiné à contenir les quatre chaînes */
        char *texts[4];

        /* Alloue dynamiquement la mémoire nécessaire à chaque chaîne */
        /* On ajoute 1 octet pour pouvoir placer le caractère '\0' */
        for (int i = 0; i < 4; i++) {
            texts[i] = malloc((size_t)sizes[i] + 1);

            /* Vérifie que l'allocation mémoire a réussi */
            if (texts[i] == NULL) {

                /* Libère les mémoires déjà allouées */
                for (int j = 0; j < i; j++)
                    free(texts[j]);

                fclose(bin);
                fclose(txt);
                return 1;
            }
        }

        /* Indique si la lecture des quatre chaînes s'effectue correctement */
        int success = 1;

        /* Lit successivement les quatre chaînes dans le fichier binaire */
        for (int i = 0; i < 4; i++) {

            /* Vérifie que le nombre d'octets lus correspond à la taille attendue */
            if (fread(texts[i], 1, (size_t)sizes[i], bin) != (size_t)sizes[i]) {
                success = 0;
                break;
            }

            /* Ajoute le caractère de fin de chaîne pour permettre son utilisation en C */
            texts[i][sizes[i]] = '\0';
        }

        /* Vérifie que toutes les chaînes ont bien été récupérées */
        if (!success) {

            /* Libère la mémoire des quatre chaînes */
            for (int i = 0; i < 4; i++)
                free(texts[i]);

            fclose(bin);
            fclose(txt);
            return 1;
        }

        /* Retranscrit les informations dans le fichier texte */
        /* Les indices correspondent aux données enregistrées :
           0 : texte original
           1 : traduction
           2 : langue source
           3 : langue cible */
        int write_success = fprintf(txt,
            "Original : %s\n"
            "Traduction : %s\n"
            "Langue source : %s\n"
            "Langue cible : %s\n\n",
            texts[0], texts[1], texts[2], texts[3]) >= 0;

        /* Libère la mémoire utilisée par les quatre chaînes */
        for (int i = 0; i < 4; i++)
            free(texts[i]);

        if (!write_success) {
            fclose(bin);
            fclose(txt);
            return 1;
        }
    }

    /* Ferme les deux fichiers */
    fclose(bin);

    if (fclose(txt) != 0)
        return 1;

    /* Retourne 0 si la retranscription s'est déroulée correctement */
    return 0;
}