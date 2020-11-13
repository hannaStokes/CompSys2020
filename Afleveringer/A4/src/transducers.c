#include "transducers.h"

int file_pipe(FILE* files[2]) {
  int fds[2];

  int r = pipe(fds);

  if (r == 0) {
    files[0] = fdopen(fds[0], "r");
    files[1] = fdopen(fds[1], "w");

    if (files[0] && files[1]) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return r;
  }
}

struct stream {
  FILE* stream_pointer;
  int is_read;
};

void transducers_free_stream(stream *s) {
  free(s);
}

int transducers_link_source(stream **out,
                            transducers_source s, const void *arg) {
  stream* str = malloc(sizeof(out));
  FILE* files[2];
  file_pipe(files);
  FILE* write_end = files[1];
  FILE* read_end = files[0];
  str->stream_pointer = read_end;

  //Stream does not have a reader yet
  str->is_read = 0; 

  //The out pointer is set to the stream pointer str that was just created
  *out = str;

  if (fork() == 0) {
    fclose(read_end);
    s(arg,write_end);
    fclose(write_end);
    exit(0);
  }
  else {
    fclose(write_end);
  }
  return 0;
}

int transducers_link_sink(transducers_sink s, void *arg,
                          stream *in) {
  if (in->is_read == 1) {
    return 1;
  }

  //in now has a reader
  in->is_read = 1;

  s(arg,in->stream_pointer);

  return 0;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {
  if (in->is_read == 1) {
    return 1;
  }
  //in now has a reader
  in->is_read = 1;

  stream* str = malloc(sizeof(out));
  FILE* files[2];
  file_pipe(files);
  FILE* write_end = files[1];
  FILE* read_end = files[0];
  str->stream_pointer = read_end;

  //Stream does not have a reader yet
  str->is_read = 0; 

  //The out pointer is set to the stream pointer str that was just created
  *out = str;  

  if(fork() == 0) {
    fclose(read_end);
    t(arg,write_end,in->stream_pointer);
    fclose(write_end);
    exit(0);
  }
  else {
    fclose(write_end);
  }
  return 0;
}

int transducers_link_2(stream **out,
                       transducers_2 t, const void *arg,
                       stream* in1, stream* in2) {
  if (in1->is_read == 1 || in2->is_read == 1) {
    return 1;
  }

  //In1 and in2 now has a reader
  in1->is_read = 1;
  in2->is_read = 1;
  
  stream* str = malloc(sizeof(out));
  FILE* files[2];
  file_pipe(files);
  FILE* write_end = files[1];
  FILE* read_end = files[0];
  str->stream_pointer = read_end;

  //Stream does not have a reader yet
  str->is_read = 0; 

  //The out pointer is set to the stream pointer str that was just created
  *out = str;  

  if(fork() == 0) {
    fclose(read_end);
    t(arg,write_end,in1->stream_pointer,in2->stream_pointer);
    fclose(write_end);
    exit(0);
  }
  else {
    fclose(write_end);
  }
  return 0;
}

int transducers_dup(stream **out1, stream **out2,
                    stream *in) {
  if (in->is_read == 1) {
    return 1;
  }

  //In1 and in2 now has a reader
  in->is_read = 1;

  FILE* files[2];

  //Creating first dublication
  stream* str1 = malloc(sizeof(in));
  file_pipe(files);
  FILE* write_end1 = files[1];
  FILE* read_end1 = files[0];
  str1->stream_pointer = read_end1;

  //Creating second dublication
  stream* str2 = malloc(sizeof(in));
  file_pipe(files);
  FILE* write_end2 = files[1];
  FILE* read_end2 = files[0];
  str2->stream_pointer = read_end2;
  
  //The two streams does not have a reader yet
  str1->is_read = 0; 
  str2->is_read = 0; 

  //The out pointer is set to the stream pointer str that was just created
  *out1 = str1;  
  *out2 = str2;  

  if (fork() == 0) {
    fclose(read_end1);
    fclose(read_end2);
    char c;
    while (fread(c, sizeof(char), 1, in) == 1) {
      fwrite(c, sizeof(char), 1, write_end1);
      fwrite(c, sizeof(char), 1, write_end2);
    }
    fclose(write_end1);
    fclose(write_end2);
    exit(0);
  }

  fclose(write_end1);
  fclose(write_end2);

  return 0;
}
