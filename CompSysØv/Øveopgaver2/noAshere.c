#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  assert(argc == 2);
  FILE* file;
  char c;
  int bool = 0;

  if( access( argv[1], F_OK ) != -1 ) { //the file exists
    printf("The file exists.");

    // open file in read mode
    file = fopen(argv[1], "r");
    assert(file != NULL);

    //check for A's
    c = fgetc(file);
    while (c != EOF ) {
      if (c == 'A') {
      bool = 1;
      }
      c = fgetc(file);
    }

    //print according to presence of A's
    if (bool == 0){
      printf("Ok");
      fclose(file);
      return EXIT_SUCCESS;
    } else {
      printf("No A's please.");
      fclose(file);
      return EXIT_FAILURE;
    }
  } else {
    printf("The file does not exist."); //file doesn't exist
    return EXIT_FAILURE;
  }
}
