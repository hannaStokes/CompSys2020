#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Argument number is WRONG!\n");
        return EXIT_FAILURE;
    } else if (argv[1][0] == 'A') {
        printf("No beginning A's are allowed!\n");
        return EXIT_FAILURE;
    } else {
        printf("%s", argv[1]);
    }
  }
