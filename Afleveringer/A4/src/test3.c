#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void string_stream(const void *arg, FILE *out) {
  fputs((const char*) arg, out);
}

void interleave_stream(const void *arg, FILE *out, FILE *in1, FILE *in2) {
  arg = arg;
  unsigned char c, d;
  while ((fread(&c, sizeof(unsigned char), 1, in1) == 1) &&
         (fread(&d, sizeof(unsigned char), 1, in2) == 1)) {
      assert(fwrite(&c, sizeof(unsigned char), 1, out) == 1);
      assert(fwrite(&d, sizeof(unsigned char), 1, out) == 1);
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
  stream* s[4];

  char *input = "Hello, World!";
  char *output = malloc((strlen(input))*2+1);
  output[strlen(input)*2] = '\0'; /* Ensure terminating NULL. */
  
  assert(transducers_link_source(&s[0], string_stream, input) == 0);
  assert(transducers_dup(&s[1], &s[2], s[0]) == 0);
  assert(transducers_link_2(&s[3], interleave_stream, NULL, s[1], s[2]) == 0);
  assert(transducers_link_sink(save_stream, output, s[3]) == 0);

  /* We cannot use the '==' operator for comparing strings, as strings
     in C are just pointers.  Using '==' would compare the _addresses_
     of the two strings, which is not what we want. */
  assert(strcmp("HHeelllloo,,  WWoorrlldd!!",output) == 0);

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }

  free(output);

  return 0;
}
