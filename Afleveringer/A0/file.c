#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int usage_error() {
  return fprintf(stdout, "Usage: file path\n");
}
int empty_file(char *path) {
  return fprintf(stdout, "%s: empty\n", path);
}
int assci_file(char *path){
  return fprintf(stdout, "%s: ASCII text\n", path);
}
int data_file(char *path){
  return fprintf(stdout, "%s: data\n", path);
}
int print_error(char *path, int errnum) {
  return fprintf(stdout, "%s: cannot determine (%s)\n", path, strerror(errnum));
}


int main(int argc, char* argv[]) {
  FILE* file;
  char c;
  int isASCII = 0; //Used as bool and starts out by being false
  
  //only 2 arguments?
  if (argc != 2) {
    usage_error();
  }
  //does file exist?
  if( access( argv[1], F_OK ) != -1 ) {

    //open file
    file = fopen( argv[1], "r");
    assert( file != NULL);

    //is file empty?
    c = fgetc(file);
    if ( c== EOF) {
      return empty_file(argv[1]);
    }

    //is file not readable? If yes, it means that it exists bot cannot be read
    if (access( argv[1], R_OK) == -1) {
      return print_error(argv[1],errno);
    }

    while (c != EOF) {
      //is file ASCII?
      if ( !(((c >= 7) && (c <= 13)) || (c == 27) || ((c >= 32) && (c <= 126)))) {
        isASCII = 1;
      }
      c = fgetc(file);
    }
    
    if (isASCII == 0) {
      return assci_file(argv[1]);
    }
    else {
      return data_file(argv[1]);
    }

  } else {
    return print_error(argv[1],errno);
  }
}
