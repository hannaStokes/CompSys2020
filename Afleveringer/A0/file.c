#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  FILE* file;
  char c;
  int isASCII = 0; //Used as bool and starts out by being false
  
  //only 2 arguments?
  if (argc != 2) {
    printf("Argument number is wrong.\n");
    return EXIT_FAILURE;

  //does file exist?
  if( access( argv[1], F_OK ) != -1 ) {
    printf("The file exists.");

    //open file
    file = fopen( argv[1], "r");
    assert( file != NULL);

    //is file empty?
    c = fgetc(file);
    if ( c== EOF) {
      printf("Empty file");
      fclose(file);
      return EXIT_SUCCESS;
    }

    while (c != EOF) {
      //is file ASCII?
      if ( !(((c >= 7) && (c <= 13)) || (c == 27) || ((c >= 32) && (c <= 126)))) {
        isASCII = 1;
      }
      //is file other? if statement below maybe
      c = fgetc(file);
    }
    
    if (isASCII == 0) {
      printf("ASCII file");
      fclose(file);
      return EXIT_SUCCESS;
    }

  } else {
    printf("The file does not exist.");
    return EXIT_FAILURE;
  }
}
