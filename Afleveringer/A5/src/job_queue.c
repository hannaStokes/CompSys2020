#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"


int job_queue_init(struct job_queue *job_queue, int capacity) {
  job_queue->jobs = malloc(sizeof(void*) * capacity); //Allocates memory space for the jobs
  job_queue->size = capacity;
  job_queue->in = 0;
  job_queue->out = 0;
  job_queue->destroyed = 0;
  assert(pthread_cond_init(&job_queue->pushed, NULL) == 0); //Initializes the pthread condition
  assert(pthread_cond_init(&job_queue->popped, NULL) == 0); //Initializes the pthread condition
  assert(pthread_mutex_init(&job_queue->mutex, NULL) == 0); //Initializes the pthread mutex
  return 0;
}

int job_queue_destroy(struct job_queue *job_queue) {
  assert(pthread_mutex_lock(&job_queue->mutex) == 0);
  job_queue->destroyed = 1;
  assert(pthread_cond_broadcast(&job_queue->pushed) == 0); //Signals to all threads waiting for the 'job_queue->pushed' condition
  while(1) {
    assert(pthread_cond_wait(&job_queue->popped, &job_queue->mutex) == 0); //Waits for the 'job_queue_pop threads' to finish the function 
    if(job_queue->in == job_queue->out) {
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      free(job_queue->jobs); //The allocated jobs are freed 
      return 0;
    }
  }
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  assert(pthread_mutex_lock(&job_queue->mutex) == 0);
  while(1) { 
    if(!(job_queue->in-job_queue->out == job_queue->size)) {//if queue not full
      job_queue->jobs[job_queue->in%(job_queue->size)] = data; //data is inserted in the job_queue
      job_queue->in++;
      assert(pthread_cond_signal(&job_queue->pushed) == 0); //Signals that a job has been pushed
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      return 0;
    }
    assert(pthread_cond_wait(&job_queue->popped,&job_queue->mutex) == 0); //waiting for a job to be popped because the queue is full
  }
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  assert(pthread_mutex_lock(&job_queue->mutex) == 0);
  while(1) {
    if(job_queue->destroyed && job_queue->in == job_queue->out){ //if job_queue is empty and destroyed
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      assert(pthread_cond_signal(&job_queue->popped) == 0); //Signals that the pop function is done
      return -1;
    }
    if(!(job_queue->in == job_queue->out)){ //If job_queue is not empty
      *data = job_queue->jobs[job_queue->out%(job_queue->size)]; //The data pointer's address is set to the value of the first job in the job_queue
      job_queue->out++;
      assert(pthread_cond_signal(&job_queue->popped) == 0); //Signals that a job has been popped from the job queue
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      return 0;
    }
    assert(pthread_cond_wait(&job_queue->pushed,&job_queue->mutex) == 0); //Waits for a job to be pushed or the job_queue to be destroyed
  }
}
