#include "../inc/malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ALLOCS 500

int main(void)
{
    char    *ptrs[NUM_ALLOCS];
    size_t  sizes[NUM_ALLOCS];

    printf("--- 1. PHASE D'ALLOCATION INITIALE (MALLOC) ---\n");
    for (int i = 0; i < NUM_ALLOCS; i++)
    {
        sizes[i] = (rand() % 4096) + 1; // Tailles variées (Tiny et Small)
        ptrs[i] = (char *)malloc(sizes[i]);

        if (!ptrs[i]) {
            fprintf(stderr, "Erreur : Malloc a renvoyé NULL à l'index %d\n", i);
            return (1);
        }
        // Remplit avec un motif reconnaissable (ex: 'A' + (i % 26))
        memset(ptrs[i], 'A' + (i % 26), sizes[i]);
    }
    printf("-> %d allocations réussies.\n\n", NUM_ALLOCS);

    printf("--- 2. PHASE DE RÉALLOCATION (REALLOC) ---\n");
    for (int i = 0; i < NUM_ALLOCS; i += 2) // On redimensionne un bloc sur deux
    {
        size_t new_size = sizes[i] * 2;
        char *new_ptr = (char *)realloc(ptrs[i], new_size);

        if (!new_ptr) {
            fprintf(stderr, "Erreur : Realloc a échoué à l'index %d\n", i);
            return (1);
        }

        ptrs[i] = new_ptr;
        // Remplit la nouvelle taille. Si realloc a donné un bloc trop petit,
        // ce memset va écraser le header du bloc suivant !
        memset(ptrs[i], 'B' + (i % 26), new_size);
    }
    printf("-> Réallocations terminées.\n\n");

    printf("--- 3. PHASE DE VÉRIFICATION DES DONNÉES ---\n");
    for (int i = 0; i < NUM_ALLOCS; i += 2)
    {
        if ((unsigned char)ptrs[i][0] != ('B' + (i % 26))) {
            fprintf(stderr, "CORRUPTION DÉTECTÉE sur ptrs[%d] !\n", i);
            return (1);
        }
    }
    printf("-> Données intactes.\n\n");

    printf("--- 4. PHASE DE LIBÉRATION (FREE) ---\n");
    for (int i = 0; i < NUM_ALLOCS; i++)
    {
        if (ptrs[i]) {
            // Si le header a été altéré pendant le realloc/memset,
            // free() affichera "invalid pointer" ou segfault ici.
            free(ptrs[i]);
        }
    }
    printf("-> Tous les blocs ont été libérés avec succès !\n\n");

    printf("--- TEST REUSSI SANS ERREUR ---\n");
    return (0);
}
