#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int print_error(char *path, int errnum) {
  return fprintf(stdout, "%s: cannot determine (%s)\n", path, strerror(errnum));
}

int main(int argc, char* argv[]) {
  FILE* file;
  char c;
  
  //only 2 arguments?
  if (argc != 2) {
    return fprintf(stdout, "Usage: file path\n");
  }
  //does file exist?
  if( access( argv[1], F_OK ) != -1 ) {

    //open file
    file = fopen( argv[1], "r");
    assert( file != NULL);

    //is file empty?
    c = fgetc(file);
    if ( c== EOF) {
      return fprintf(stdout, "%s: empty\n", argv[1]);
    }

    //is file not readable? If yes, it means that it exists bot cannot be read
    if (access( argv[1], R_OK) == -1) {
      return print_error(argv[1],errno);
    }

    while (c != EOF) {
      //is file ASCII?
      if ( !(((c >= 7) && (c <= 13)) || (c == 27) || ((c >= 32) && (c <= 126)))) {
        return fprintf(stdout, "%s: data\n", argv[1]);
      }
      c = fgetc(file);
    }
    
    return fprintf(stdout, "%s: ASCII text\n", argv[1]);
    

  } else {
    return print_error(argv[1],errno);
  }
}
