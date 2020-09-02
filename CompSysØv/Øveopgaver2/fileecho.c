#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  assert(argc == 2);
  FILE* file;
  char c;

  if( access( argv[1], F_OK ) != -1 ) { //file exists
    printf("The file exists.");

    // open file in read mode
    file = fopen(argv[1], "r");
    assert(file != NULL);

    //print file
    c = fgetc(file);
    while (c != EOF) {
        printf("%c", c);
        c = fgetc(file);
    }

    //close file
    fclose(file);
    return EXIT_SUCCESS;

  } else {
    printf("The file does not exist."); //file doesn't exist
    return EXIT_FAILURE;
  }
}
