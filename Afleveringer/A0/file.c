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
  }
  //does file exist?
  if( access( argv[1], F_OK ) != -1 ) {

    //open file
    file = fopen( argv[1], "r");
    assert( file != NULL);

    //is file empty?
    c = fgetc(file);
    if ( c== EOF) {
      printf("%s: empty\n", argv[1]);
      fclose(file);
      return EXIT_SUCCESS;
    }

    while (c != EOF) {
      //is file ASCII?
      if ( !(((c >= 7) && (c <= 13)) || (c == 27) || ((c >= 32) && (c <= 126)))) {
        isASCII = 1;
      }
      c = fgetc(file);
    }
    
    if (isASCII == 0) {
      printf("%s: ASCII text\n", argv[1]);
      fclose(file);
      return EXIT_SUCCESS;
    }
    else {
      printf("%s: data\n", argv[1]);
      fclose(file);
      return EXIT_SUCCESS;
    }

  } else {
    printf("%s: cannot determine (No such file or directory)\n", argv[1]);
    return EXIT_FAILURE;
  }
}
