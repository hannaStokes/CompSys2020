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
  return 0;
}

int job_queue_destroy(struct job_queue *job_queue) {
  while(1) {
    if(job_queue->in == job_queue->out) {
      
      free(job_queue);
      return 0;
    }
  }
}

int job_queue_push(struct job_queue *job_queue, void *data) {
  while(1) {  
    if(job_queue->destroyed) {
      return -1;
    }
    if(!(job_queue->in-job_queue->out == job_queue->size)) {//if queue not full
      job_queue->jobs[job_queue->in%(job_queue->size-1)] = data;
      job_queue->in += 1;
      return 0;
    }
  }
}

int job_queue_pop(struct job_queue *job_queue, void **data) {
  while(1) {
  if(&job_queue == 0){
    return -1;
  }
  if(!(job_queue->in == job_queue->out))
    *data = job_queue->jobs[job_queue->out%(job_queue->size-1)];
    job_queue->out +=1;
    return 0;
  }
}
