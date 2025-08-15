#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10

// Buffers e seus índices
static int buffer_0[BUFFER_SIZE];
static int buffer_1[BUFFER_SIZE];
int in_0 = 0;
int in_1 = 0;
int out_0 = 0;
int out_1 = 0;

// Mutex e variáveis de condição
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

// Função para adicionar um item em um dos buffers
void add(int item) {
    pthread_mutex_lock(&mutex);
    // Espera enquanto ambos os buffers estão cheios
    while ((buffer_0[BUFFER_SIZE - 1] != 0) && (buffer_1[BUFFER_SIZE - 1] != 0)) {
        pthread_cond_wait(&not_full, &mutex);
    }
    if (buffer_0[BUFFER_SIZE - 1] == 0 && buffer_1[0] == 0) {
        // Adiciona ao buffer_0
        buffer_0[in_0] = item;
        in_0 = (in_0 + 1) % BUFFER_SIZE;
        printf("1p\n");
    } else if (buffer_1[BUFFER_SIZE - 1] == 0 && buffer_0[0] == 0){
        // Adiciona ao buffer_1
        buffer_1[in_1] = item;
        in_1 = (in_1 + 1) % BUFFER_SIZE;
        printf("2p\n");
    }
    pthread_cond_signal(&not_empty);  // Sinaliza que buffer não está vazio
    pthread_mutex_unlock(&mutex);
}

// Função para remover um item de um dos buffers
void remover() {
    pthread_mutex_lock(&mutex);
    // Espera enquanto ambos os buffers estão vazios
    while ((buffer_0[BUFFER_SIZE - 1] == 0) && (buffer_1[BUFFER_SIZE - 1] == 0)) {
        pthread_cond_wait(&not_empty, &mutex);
    }
    if (buffer_0[BUFFER_SIZE - 1] != 0) {
        // Remove do buffer_0
        buffer_0[out_0] = 0;
        out_0 = (out_0 + 1) % BUFFER_SIZE;
        printf("1\n");
    } else if (buffer_1[BUFFER_SIZE - 1] != 0) {
        // Remove do buffer_1
        buffer_1[out_1] = 0;
        out_1 = (out_1 + 1) % BUFFER_SIZE;
        printf("2\n");
    }
    pthread_cond_signal(&not_full); //Sinaliza que buffer não está cheio
    pthread_mutex_unlock(&mutex);
}

