#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 3
#define NUM_THREADS NUM_PRODUCERS + NUM_CONSUMERS
#define MAX_PRODUCER_PAUSE 1000000
#define MAX_CONSUMER_PAUSE 1000000
#define MAX_QUEUE 10
#define LEN_NAME 7

struct element {char payload[LEN_NAME]; struct element* next;};
typedef struct element element_t;
element_t *head = 0, *tail = 0;
int size = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /* mutex for inter-thread communication */
pthread_cond_t sent = PTHREAD_COND_INITIALIZER;

int inter_producer_delta;
int inter_consumer_delta;

/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
  int tid;
  double stuff;
} thread_data_t;
 

void send(char *message) {
  pthread_mutex_lock(&mutex); 
  if (head == 0) {
    head = malloc(sizeof(element_t));
    strcpy (head->payload, message);
    head->next = 0;
    tail = head;
  } else {
    tail->next = malloc(sizeof(element_t));
    tail = tail->next;
    strcpy (tail->payload, message);
    tail->next = 0;
  }
  size++;
  pthread_cond_signal(&sent);
  pthread_mutex_unlock(&mutex);
}

void *get(char *message) {
  element_t* element;

  pthread_mutex_lock(&mutex);
  while (size == 0) {
    pthread_cond_wait(&sent, &mutex);
  }
  strcpy (message, head->payload);
  element = head;
  head = head->next;
  free(element);
  size--; 
  pthread_mutex_unlock(&mutex); 
}

// here are some helper code snippets

/* generate a random patient name */
void *patient() {
  while(1) { 
    char *name;
    name[0] = rand() % 26 + 'A';
    for (int i = 1; i < LEN_NAME-1; i++)
      name[i] = rand() % 26 + 'a';
    name[LEN_NAME-1] = '\0';

    // TODO: some form of conditional wait to check
    // if a doctor is available to treat.
    // Output "turn away ambulance" if the doctor
    // threads are busy
    
    printf("Patient coming named %s\n", name);
    send(name);
    usleep(rand() % inter_producer_delta);
  }
  return NULL;
}

void *doctor() {
  while(1) {
    printf("Doctor is treating %s\n", &head->payload);
    get((char*) &head->payload);
    usleep(rand() % inter_consumer_delta);
  }
  return NULL;
}

/* pause for a random delay to give you time to see output! */
//  usleep(rand() % inter_producer_delta);

/*
 * Develop a simulation using multiple producer threads and
 * multiple consumer threads that are running concurrently. 
 * We will simulate a Quebec hospital in which:
 * - Ambulances bring incoming patients, and
 * - doctors treat the patients before releasing them
 *
 * For simplicity of the simulation, there is no triage:
 * patients are seen on a first-come, first-served basis
 * and the hospital has a maximum capacity in the waiting area (of MAX_QUEUE).
 * 
 * If the waiting room is full, arriving ambulances are turned away.
 * If the waiting room is empty, the doctors can take a breather.
 */
 
int main(int argc, char **argv) {
  pthread_t *thr;
  thread_data_t *thr_data;
  int i, rc;
  int opt;

  /* set defaults */
  int n_producers = NUM_PRODUCERS;
  int n_consumers = NUM_CONSUMERS;
  inter_producer_delta = MAX_PRODUCER_PAUSE;
  inter_consumer_delta = MAX_CONSUMER_PAUSE;

  while ((opt = getopt(argc, argv, "p:c:a:d:")) != -1) {
    switch (opt) {
    case 'p':
      n_producers = atoi(optarg);
      break;
    case 'c':
      n_consumers = atoi(optarg);
      break;
    case 'a':
      inter_producer_delta = atoi(optarg) * 1000;
      break;
    case 'd':
      inter_consumer_delta = atoi(optarg) * 1000;
      break;
    default: /* '?' */
      fprintf(stderr, "Usage: %s [-p producers] [-c consumers] [-a max inter_producer_delay] [-d max inter_consumer_delay]\n",
                           argv[0]);
      exit(EXIT_FAILURE);
    }
  }
  
  printf("running with %d producers (pausing for a max of %d ms) and %d consumers (pausing for a max of %d ms)\n",
	 n_producers, inter_producer_delta, n_consumers, inter_consumer_delta);
  
  if (optind > argc) {
    fprintf(stderr, "Expected argument after options\n");
    exit(EXIT_FAILURE);
  }

  /* allocate storage for threads arrays */
  thr = (pthread_t *)malloc(sizeof (pthread_t) * (n_producers + n_consumers));
  thr_data = (thread_data_t *)malloc(sizeof (thread_data_t) * (n_producers + n_consumers));

  /*
   * TODO: you can now create one of the threads (with index i) as follows:
   */
  for (int pro_i = 0; pro_i < n_producers; pro_i++) {
    pthread_create(&thr[pro_i], NULL, patient, &thr_data[pro_i]);
  }
  
  for (int con_i = 0; con_i < n_consumers; con_i++) {
   pthread_create(&thr[con_i], NULL, doctor, &thr_data[con_i]);
  }
  
  // wait for a very long time to see things in action
  usleep(10000000000);
  
//  if ((rc = pthread_create(&thr[i], NULL, create_patient, &thr_data[i]))) {
//    fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
//    return EXIT_FAILURE;
//  }
 
  

  return EXIT_SUCCESS;
}
