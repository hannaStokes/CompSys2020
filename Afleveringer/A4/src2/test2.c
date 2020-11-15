#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void string_stream(const void *arg, FILE *out) {
  fputs((const char*) arg, out);
}

void concatenate_streams(const void *arg, FILE *out, FILE *in1, FILE *in2) {
  (void)arg; //Unused

  char c;
  while (fread(&c, sizeof(char), 1, in1) == 1) {
      assert(fwrite(&c, sizeof(char), 1, out) == 1);
  }
  while (fread(&c, sizeof(char), 1, in2) == 1) {
      assert(fwrite(&c, sizeof(char), 1, out) == 1);
  }
}

void save_stream(void *arg, FILE *in) {
  /* We will be writing bytes to this location. */
  unsigned char *d = arg;

  while (fread(d, sizeof(unsigned char), 1, in) == 1) {
    d++; /* Move location ahead by one byte. */
  }
}

int main() {
  stream* s[3];

  char *input1 = "Hello,";
  char *input2 = " World!";
  char *output = malloc(strlen(input1)+strlen(input2)+1);
  output[strlen(input1)+strlen(input2)] = '\0'; /* Ensure terminating NULL. */

  assert(transducers_link_source(&s[0], string_stream, input1) == 0);
  assert(transducers_link_source(&s[1], string_stream, input2) == 0);
  assert(transducers_link_2(&s[2], concatenate_streams, NULL, s[0], s[1]) == 0);
  assert(transducers_link_sink(save_stream, output, s[2]) == 0);

  assert(strcmp("Hello, World!",output) == 0);

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }

  free(output);

  return 0;
}
