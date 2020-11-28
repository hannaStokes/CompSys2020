#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "job_queue.h"


int job_queue_init(struct job_queue *job_queue, int capacity) {
  job_queue->jobs[0] = malloc(sizeof(void*)*capacity);
  job_queue->size = capacity;
  job_queue->in = 0;
  job_queue->out = 0;
  job_queue->destroyed = 0;
  assert(pthread_cond_init(&job_queue->empty, NULL) == 0);
  assert(pthread_cond_init(&job_queue->full, NULL) == 0);
  assert(pthread_mutex_init(&job_queue->mutex, NULL) == 0);
  return 0;
}

int job_queue_destroy(struct job_queue *job_queue) {
  assert(pthread_mutex_lock(&job_queue->mutex) == 0);
  job_queue->destroyed = 1;
  assert(pthread_cond_broadcast(&job_queue->empty) == 0);
  while(1) {
    if(job_queue->in == job_queue->out) {
      free(job_queue);
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      return 0;
    }
  }
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  assert(pthread_mutex_lock(&job_queue->mutex) == 0);
  while(1) {  
    if(job_queue->destroyed) {
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      return -1;
    }
    if(!(job_queue->in-job_queue->out == job_queue->size)) {//if queue not full
      job_queue->jobs[job_queue->in%(job_queue->size)] = data;
      job_queue->in++;
      assert(pthread_cond_signal(&job_queue->empty) == 0);
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      return 0;
    }
    pthread_cond_wait(&job_queue->full,&job_queue->mutex);
  }
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  assert(pthread_mutex_lock(&job_queue->mutex) == 0);
  while(1) {
    if(job_queue->destroyed && job_queue->in == job_queue->out){
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      return -1;
    }
    if(!(job_queue->in == job_queue->out)){
      *data = job_queue->jobs[job_queue->out%(job_queue->size)];
      job_queue->out++;
      assert(pthread_cond_signal(&job_queue->full) == 0);
      assert(pthread_mutex_unlock(&job_queue->mutex) == 0);
      return 0;
    }
    assert(pthread_cond_wait(&job_queue->empty,&job_queue->mutex) == 0);
  }
}
