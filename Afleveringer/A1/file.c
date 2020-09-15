#include <stdio.h>  // fprintf, stdout.
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access().

enum file_type {
	DATA,
	EMPTY,
	ASCII,
};

const char * const FILE_TYPE_STRINGS[] = {
	"data",
	"empty",
	"ASCII text",
};

char * path;
FILE * file;

//prints an error message describing what went wrong when accessing @path
int print_error(char *path, int errnum) {
	return fprintf(stdout, "%s cannot determine (%s)\n", 
			path, strerror(errnum));
}

//evaluates what type of file is located on @path
int check_type(FILE *file) {
	char c = fgetc(file);
	//if the first char encountered is the end of file marker
	if(c == EOF) {
		return EMPTY;
	} 
	//iterate through the file, if you encounter something that is not in the 
	//accepted ranges of ASCII characters, deem the file data, otherwise ASCII
	while(c != EOF) {
		if (!(((7 <= c) && (c <= 13))
				|| (c == 27)
				|| ((32 <= c) && (c <= 126)))) {
			return DATA;
		} 
		c = fgetc(file);
	}
	return ASCII;
}

int main(int argc, char *argv[]) {
  //check number of arguments
  //if no arguments, print to stderr
  if(argc == 1) {
	  fprintf(stderr, "Usage: file path");
	  return EXIT_FAILURE;
  //if number of arguments is not exactly 1, fail
  } else if(argc!=2) {
	  return EXIT_FAILURE;
  //if 1 argument, proceed with the given path
  } else {
	  path = argv[1];
  }

  //check if a file exists on path, if it does, open it and print typecheck string to stdout,
  //otherwise display error message
  if(access(path, F_OK) == 0) {
	  file = fopen(path, "r");
	  fprintf(stdout, "%s: %s\n", path, FILE_TYPE_STRINGS[check_type(file)]);
    return EXIT_SUCCESS;
  } else {
	  return print_error(path, errno);
  }
}


