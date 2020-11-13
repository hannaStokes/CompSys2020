#include "transducers.h"

struct stream {
  int hasReader;
  FILE *source;
  FILE *read_write[2];
};

static int pipe_wrapper(FILE *files[2]) {
  int file_descriptors[2];

  int result = pipe(file_descriptors);

  if (result == 0) {
    files[0] = fdopen(fds[0], "r");
    files[1] = fdopen(fds[1], "w");

    if (files[0] && files[1]) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return result;
  }
}

void transducers_free_stream(stream *s) {
  s=s; /* unused */
}

int transducers_link_source(stream **out,         //put the address of the generated stream at this address
                            transducers_source s, //this function generates the values in the stream
			    const void *arg) {    //this argument is passed to the generator function

  *out = malloc(sizeof(struct stream));
  *out-> has_reader = 0;
  pipe_wrapper(*out->read_write);
  FILE *read_end = *out->read_write[0];
  FILE *write_end = *out->read_write[1];

  if (fork() == 0) { //child process
    fclose(read_end);
    (*s)(arg, write_end);
    fclose(write_end);
    exit(0);
  }
  fclose(read_end);
  fclose(write_end);
  return 0;
}

int transducers_link_sink(transducers_sink s, //this function processes the values in the instream
		          void *arg,          //this argument is passed to the processor function
                          stream *in) {       //this is the address of the instream
  s=s; /* unused */
  arg=arg; /* unused */
  in=in; /* unused */
  return 1;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {
  out=out; /* unused */
  t=t; /* unused */
  arg=arg; /* unused */
  in=in; /* unused */
  return 1;
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
