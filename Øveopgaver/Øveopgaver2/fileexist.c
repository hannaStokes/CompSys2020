#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  assert(argc == 2);
  if( access( argv[1], F_OK ) != -1 ) { //file exists
    printf("The file exists.");
    return EXIT_SUCCESS;
  } else {
    printf("The file does not exist."); //file doesn't exist
    return EXIT_FAILURE;
  }
}
