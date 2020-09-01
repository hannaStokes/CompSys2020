#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Argument number is WRONG!\n");
    return EXIT_FAILURE;
  }
  else {
    printf("%s\n%s\n", argv[1], argv[1]);
    return EXIT_SUCCESS;
  }

}
