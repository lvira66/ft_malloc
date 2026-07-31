#include "../inc/malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 4
#define NUM_ITERATIONS 50000 // Nombre d'actions par thread
#define MAX_ACTIVE_PTRS 256  // Nombre max d'allocations simultanées par thread
#define MAX_ALLOC_SIZE 16384 // Jusqu'à 16 KB

void* chaotic_stress_test(void* arg) {
    int thread_id = *(int*)arg;
    unsigned int seed = thread_id * 12345; 
    
    // Tableau pour stocker les pointeurs actifs du thread
    void* active_ptrs[MAX_ACTIVE_PTRS];
    for (int i = 0; i < MAX_ACTIVE_PTRS; i++) {
        active_ptrs[i] = NULL;
    }

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // On choisit un index au hasard dans notre tableau
        int idx = rand_r(&seed) % MAX_ACTIVE_PTRS;

        // S'il y a déjà un pointeur à cet emplacement, on le libère (FREE)
        if (active_ptrs[idx] != NULL) {
            free(active_ptrs[idx]);
            active_ptrs[idx] = NULL;
        } 
        // Sinon, on alloue une nouvelle zone mémoire (MALLOC)
        else {
            size_t size = (rand_r(&seed) % MAX_ALLOC_SIZE) + 1;
            active_ptrs[idx] = malloc(size);
            
            if (active_ptrs[idx] != NULL) {
                // On écrit dedans avec un motif propre au thread pour détecter 
                // si un autre thread vient écraser notre mémoire par erreur.
                memset(active_ptrs[idx], (thread_id & 0xFF), size);
            } else {
                // Si ton malloc retourne NULL, c'est soit normal (plus de RAM), 
                // soit un bug dans ta gestion des blocs.
                fprintf(stderr, "Thread %d: malloc a retourné NULL (itération %d, taille %zu)\n", thread_id, i, size);
            }
        }
    }

    // Grand nettoyage final à la fin du thread
    for (int i = 0; i < MAX_ACTIVE_PTRS; i++) {
        if (active_ptrs[i] != NULL) {
            free(active_ptrs[i]);
        }
    }

    printf("Thread %d a survécu au chaos.\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    printf("Démarrage du test chaotique : %d threads, %d itérations par thread...\n", NUM_THREADS, NUM_ITERATIONS);

    // Lancement
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, chaotic_stress_test, &thread_ids[i]) != 0) {
            perror("Erreur lors de la création des threads");
            return EXIT_FAILURE;
        }
    }

    // Attente
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Test terminé ! Si tu n'as pas eu de Segfault, ton malloc est très robuste.\n");
    return EXIT_SUCCESS;
}
