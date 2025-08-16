#include <pthread.h>
#include <stdlib.h>
#include "monitor.h"

void* produtor(void* arg) {
  int id = *((int*) arg);
  free(arg);
  while (1) {
      int item = rand() % 100;  // Produz um item aleatório
      printf("Produtor %d produzindo item %d\n", id, item);
      add(item);
      sleep(1);
  }
}

void* consumidor(void* arg) {
  int id = *((int*) arg);
  free(arg);
  while (1) {
      remover();
      printf("Consumidor %d consumiu um item\n", id);
      sleep(1);
  }
}

int main() {
  pthread_t produtor_thread, consumidor_thread;

  int* produtor_id = malloc(sizeof(int));
  *produtor_id = 1;
  pthread_create(&produtor_thread, NULL, produtor, produtor_id);

  int* consumidor_id = malloc(sizeof(int));
  *consumidor_id = 1;
  pthread_create(&consumidor_thread, NULL, consumidor, consumidor_id);

  pthread_join(produtor_thread, NULL);
  pthread_join(consumidor_thread, NULL);

  return 0;
}