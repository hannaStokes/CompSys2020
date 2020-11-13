#include "transducers.h"
#include <unistd.h>
#include <stdlib.h>

struct stream {
  int has_reader;
  FILE *read_write[2];
};

//this function takes a read_write array from a stream and pipes it, 
//such that the ends can be used for inter-process communication.
//returns 0 on success.
static int pipe_wrapper(FILE *files[2]) {
  int file_descriptors[2];

  int result = pipe(file_descriptors);

  //on success open files at the position of the descriptors
  if (result == 0) {
    files[0] = fdopen(file_descriptors[0], "r");
    files[1] = fdopen(file_descriptors[1], "w");

    //unless either end of the pipe does not exist
    if (files[0] && files[1]) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return result;
  }
}

//this function frees the memory allocated for a given stream object
void transducers_free_stream(stream *s) {
  free(s);
}

//this function initializes a stream out using the generator function s with the argument arg
//and places a reference to it at the address out.
//returns 0 on success
int transducers_link_source(stream **out,         //put the address of the generated stream at this address
                            transducers_source s, //this function generates the values in the stream
			    const void *arg) {    //this argument is passed to the generator function

  *out = malloc(sizeof(struct stream));
  
  //if memory allocation failed, return with error
  if(*out == 0) {
    return 1;
  }

  stream *work = *out;
  work->has_reader = 0;

  pipe_wrapper(work->read_write);
  FILE *write_end = work->read_write[1];

  if (fork() == 0) { //child process
    (*s)(arg, write_end);
    fclose(write_end);
    exit(0);
  }
  
  fclose(write_end);
  return 0;
}

//this function consumes a stream in and processes it with a processor function s, given the argument arg
//returns 0 on success
int transducers_link_sink(transducers_sink s, //this function processes the values in the instream
		          void *arg,          //this argument is passed to the processor function
                          stream *in) {       //this is the address of the instream
  
  if (in->has_reader != 0) {
    return 1;
  }

  in->has_reader = 1;
  FILE *input = in->read_write[0];
  (*s)(arg, input);
  fclose(input);
  return 0;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {

  if(in->has_reader != 0) {
    return 1;
  }
  
  in->has_reader = 1;

  *out = malloc(sizeof(struct stream)); 
  //if memory allocation failed, return with error
  if(*out == 0) {
    return 1;
  }
  
  stream *work = *out;
  work->has_reader = 0;
  pipe_wrapper(work->read_write);

  FILE *in_read_end = in->read_write[0]; 
  FILE *out_write_end = work->read_write[1];

  if(fork() == 0) { //child process
    (*t)(arg, out_write_end, in_read_end);
    fclose(in_read_end);
    fclose(out_write_end);
    exit(0);
  }

  fclose(out_write_end);
  return 0;

}

int transducers_link_2(stream **out,
                       transducers_2 t, const void *arg,
                       stream* in1, stream* in2) {
  out=out; /* unused */
  t=t; /* unused */
  arg=arg; /* unused */
  in1=in1; /* unused */
  in2=in2; /* unused */
  return 1;
}

int transducers_dup(stream **out1, stream **out2,
                    stream *in) {
  out1=out1; /* unused */
  out2=out2; /* unused */
  in=in; /* unused */
  return 1;
}
