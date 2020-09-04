#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  assert(argc == 3);
  
  char* input_filename = argv[1];
  char* output_filename = argv[2];

  char c;

  if( access( argv[1], F_OK ) != -1 ) { //file exists
    printf("The file exists.");

    // open file in read mode
    FILE* file = fopen(input_filename, "r");
    assert(file != NULL);

    //open new file in write mode
    FILE* new = fopen(output_filename, "w");

    if (new == NULL) {
      printf("The file could not be created.");
    } else {

      //print file
      c = fgetc(file);
      while (c != EOF) {
        fprintf(new, "%c", c);
        c = fgetc(file);
      }

      //close file
      fclose(file);
      return EXIT_SUCCESS;
    }

  } else {
    printf("The file does not exist."); //file doesn't exist
    return EXIT_FAILURE;
  }
}
