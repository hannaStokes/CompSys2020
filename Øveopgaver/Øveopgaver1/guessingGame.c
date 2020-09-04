#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  char input[8];
  int isGuessed = 0;
  int random = rand() % 101;

  if (argc != 2) {
    printf("Argument number is WRONG!\n");
    return EXIT_FAILURE;
  } else if (random == atoi(argv[1])) {
    printf("You guessed right!");
    return EXIT_SUCCESS;
  } else {
    printf("Guess again...\n");
    while (isGuessed != 1) {
      scanf("%s", input);
      if (atoi(input) > random) {
        printf("Too high! Guess again...\n");
      } else if (atoi(input) < random) {
        printf("Too low! Guess again...\n");
      } else {
        printf("You guessed right!");
        isGuessed = 1;
        return EXIT_SUCCESS;
      }
    }
  }
}

