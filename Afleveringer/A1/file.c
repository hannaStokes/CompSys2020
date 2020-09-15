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

enum ASCII_constraints {
	bell = 7,
	carriageReturn = 13,
	escape = 27,
	space = 32,
	equivalencySign = 126,
};

char * path;
FILE * file;

//prints an error message describing what went wrong when accessing @path
int print_error(char *path, int errnum) {
	fprintf(stdout, "%s cannot determine (%s)\n", 
		path, strerror(errnum));
	return EXIT_SUCCESS;
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
		if (!(((bell <= c) && (c <= carriageReturn))
				|| (c == escape)
				|| ((space <= c) && (c <= equivalencySign)))) {
			return DATA;
		} 
		c = fgetc(file);
	}
	fclose(file);
	return ASCII;
}

int main(int argc, char *argv[]) {
  //check number of arguments
  //if no arguments, print to stderr
	if(argc == 1) {
		fprintf(stderr, "Usage: file path");
		return EXIT_FAILURE;
  	}
  	else 
  	{
  		int i;
		//The longest path type is ascii, which is in index 2 of the FILE_TYPE_STRINGS array
		int max_length = strlen(FILE_TYPE_STRINGS[2]);
  		for (i = 1; i < argc; i++)
		{
	  		path = argv[i];
	  		if(access(path, F_OK) == 0) {
				file = fopen(path, "r");
				fprintf(stdout, "%s:%*s %s\n", path, max_length - strlen(path),FILE_TYPE_STRINGS[check_type(file)]);
  	  		} else {
	    		return print_error(path, errno);
	  		}
		}
		return EXIT_SUCCESS;
  	}
  

  //check if a file exists on path, if it does, open it and print typecheck string to stdout,
  //otherwise display error message
  
}


