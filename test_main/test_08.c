#include <pthread.h>
#include "../inc/malloc.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 10
#define NUM_ALLOCS_PER_THREAD 1000

void *stress_test(void *arg) {
    int thread_id = *(int *)arg;
    char *ptrs[NUM_ALLOCS_PER_THREAD];
    size_t sizes[NUM_ALLOCS_PER_THREAD];

    printf("Thread %d : Début des allocations\n", thread_id);

    // 1. MALLOC : Allocation et écriture
    for (int i = 0; i < NUM_ALLOCS_PER_THREAD; i++) {
        sizes[i] = (rand() % 4096) + 1; // Tailles aléatoires entre 1 et 4096 octets
        ptrs[i] = (char *)malloc(sizes[i]);
        
        if (ptrs[i]) {
            // Remplir avec un pattern unique à ce thread pour tester la corruption
            memset(ptrs[i], thread_id % 255, sizes[i]);
        }
    }

    // 2. REALLOC : Réallocation (pour tester les deadlocks internes)
    for (int i = 0; i < NUM_ALLOCS_PER_THREAD; i += 2) {
        if (ptrs[i]) {
            size_t new_size = sizes[i] * 2; // On force l'agrandissement
            char *new_ptr = (char *)realloc(ptrs[i], new_size);
            
            if (new_ptr) {
                ptrs[i] = new_ptr;
                // On écrit dans la nouvelle zone pour s'assurer qu'elle est bien mappée
                memset(ptrs[i], thread_id % 255, new_size);
            }
        }
    }

    // 3. VÉRIFICATION : Est-ce qu'un autre thread a écrasé mes données ?
    for (int i = 0; i < NUM_ALLOCS_PER_THREAD; i++) {
        if (ptrs[i]) {
            // On vérifie le premier octet
            if ((unsigned char)ptrs[i][0] != (thread_id % 255)) {
                fprintf(stderr, "Erreur de corruption détectée par le thread %d !\n", thread_id);
                exit(1);
            }
        }
    }

    // 4. FREE : Libération
    for (int i = 0; i < NUM_ALLOCS_PER_THREAD; i++) {
        if (ptrs[i]) {
            free(ptrs[i]);
        }
    }

    printf("Thread %d : Terminé avec succès\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    printf("--- DÉBUT DU STRESS TEST THREAD-SAFE ---\n");

    // Lancement des threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, stress_test, &thread_ids[i]) != 0) {
            perror("Erreur création thread");
            return 1;
        }
    }

    // Attente de la fin de tous les threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("--- TOUS LES THREADS ONT TERMINÉ ---\n");

    // Test de show_alloc_mem à la fin (devrait afficher très peu de choses si tout est bien free)
    // show_alloc_mem();

    printf("Si le programme s'arrête ici sans segfault, ni freeze, ni corruption : c'est parfait !\n");
    return 0;
}
