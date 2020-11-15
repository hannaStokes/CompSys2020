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
  assert(file_pipe(files) == 0);
  FILE* write_end = files[1];
  FILE* read_end = files[0];
  str->stream_pointer = read_end;

  //Stream does not have a reader yet
  str->is_read = 0; 

  //The out pointer is set to the stream pointer str that was just created
  *out = str;

  if (fork() == 0) {
    assert(fclose(read_end) == 0);
    s(arg,write_end);
    assert(fclose(write_end) == 0);
    exit(0);
  }
  else {
    assert(fclose(write_end) == 0);
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
  assert(fclose(in->stream_pointer) == 0);

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
  assert(file_pipe(files) == 0);
  FILE* write_end = files[1];
  FILE* read_end = files[0];
  str->stream_pointer = read_end;

  //Stream does not have a reader yet
  str->is_read = 0; 

  //The out pointer is set to the stream pointer str that was just created
  *out = str;  

  if(fork() == 0) {
    assert(fclose(read_end) == 0);
    t(arg,write_end,in->stream_pointer);
    assert(fclose(write_end) == 0);
    assert(fclose(in->stream_pointer) == 0);
    exit(0);
  }
  else {
    assert(fclose(write_end) == 0);
    assert(fclose(in->stream_pointer) == 0);
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
  assert(file_pipe(files) == 0);
  FILE* write_end = files[1];
  FILE* read_end = files[0];
  str->stream_pointer = read_end;

  //Stream does not have a reader yet
  str->is_read = 0; 

  //The out pointer is set to the stream pointer str that was just created
  *out = str;  

  if(fork() == 0) {
    assert(fclose(read_end) == 0);
    t(arg,write_end,in1->stream_pointer,in2->stream_pointer);
    assert(fclose(write_end) == 0);
    assert(fclose(in1->stream_pointer) == 0);
    assert(fclose(in2->stream_pointer) == 0);
    exit(0);
  }
  else {
    assert(fclose(write_end) == 0);
    assert(fclose(in1->stream_pointer) == 0);
    assert(fclose(in2->stream_pointer) == 0);
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
  assert(file_pipe(files) == 0);
  FILE* write_end1 = files[1];
  FILE* read_end1 = files[0];
  str1->stream_pointer = read_end1;

  //Creating second dublication
  stream* str2 = malloc(sizeof(in));
  assert(file_pipe(files) == 0);
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
    assert(fclose(read_end1) == 0);
    assert(fclose(read_end2) == 0);
    char c;

    //fread inserts the value of 1 byte from the in->stream_pointer into c.
    //For each iteration of the while-loop in->stream_pointer points at the next byte/element
    while (fread(&c, sizeof(char), 1, in->stream_pointer) == 1) {
      //Writes the value on c into the write_end array. 
      //It is asserted to be equal to 1, because it returns 1 
      //if exactly one member is inserted correctly (the one member "c")
      assert(fwrite(&c, sizeof(char), 1, write_end1) == 1);
      assert(fwrite(&c, sizeof(char), 1, write_end2) == 1);
    }
    assert(fclose(write_end1) == 0);
    assert(fclose(write_end2) == 0);
    assert(fclose(in->stream_pointer) == 0);
    exit(0);
  }

  assert(fclose(write_end1) == 0);
  assert(fclose(write_end2) == 0);
  assert(fclose(in->stream_pointer) == 0);

  return 0;
}
