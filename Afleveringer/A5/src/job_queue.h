#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <pthread.h>

struct job_queue {
  int size; //Capacity of jobs
  int in; //At which index in 'jobs' should the next job be pushed?
  int out; //At which index in 'jobs' should the outgoing job be popped?
  int destroyed; //Has the job_queue been destroyed
  void **jobs; //Job array
  pthread_cond_t pushed; //This condition is signaled when a job is pushed
  pthread_cond_t popped; //This condition is signaled when a job is popped
  pthread_mutex_t mutex; //The mutex used in the job_queue
};

// Initialise a job queue with the given capacity.  The queue starts out
// empty.  Returns non-zero on error.
int job_queue_init(struct job_queue *job_queue, int capacity);

// Destroy the job queue.  Blocks until the queue is empty before it
// is destroyed.
int job_queue_destroy(struct job_queue *job_queue);

// Push an element onto the end of the job queue.  Blocks if the
// job_queue is full (its size is equal to its capacity).  Returns
// non-zero on error.  It is an error to push a job onto a queue that
// has been destroyed.
int job_queue_push(struct job_queue *job_queue, void *data);

// Pop an element from the front of the job queue.  Blocks if the
// job_queue contains zero elements.  Returns non-zero on error.  If
// job_queue_destroy() has been called (possibly after the call to
// job_queue_pop() blocked), this function will return -1.
int job_queue_pop(struct job_queue *job_queue, void **data);

#endif
