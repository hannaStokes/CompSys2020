#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  
 //Make sure number of arguments is correct
  if (argc != 3) {
    printf("Argument number is wrong.\n");
  return EXIT_FAILURE;
  }
  
  unsigned index = atoi(argv[2]);
  unsigned value = atoi(argv[1]);
  unsigned newValue = value & ~(1 << index);
  printf("%i", newValue);
  return EXIT_SUCCESS;
  
}
