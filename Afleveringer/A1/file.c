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
int print_error(char *path, int errnum, int max_length) {
	fprintf(stdout, "%s:%*s cannot determine (%s)\n", 
		path, (int)(max_length - strlen(path)), " ", strerror(errnum));
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
		if (!(((7 <= c) && (c <= 13))
				|| (c == 27)
				|| ((32 <= c) && (c <= 126)))) {
			return DATA;
		} 
		c = fgetc(file);
	}
	fclose(file);
	return ASCII;
}
int Max_Length(int str_length, char *str[])
{
    int longest = 0;
	
	//Starts from i=1 because the first argument of argv should not be included
    for(int i = 1; i < str_length; i++)
    {
        int len = strlen(str[i]);
        if(longest < len)
        {
           longest = len;
        }
   }
   return longest;
}

int main(int argc, char *argv[]) {
  	//check number of arguments
  	//if no arguments, print to stderr
	if(argc == 1) {
		fprintf(stderr, "Usage: file path/n");
		return EXIT_FAILURE;
  	}
  	else 
  	{
  		int i;
		//The longest path type is ascii, which is in index 2 of the FILE_TYPE_STRINGS array
		int max_length = Max_Length(argc, argv);
  		for (i = 1; i < argc; i++)
		{
	  		path = argv[i];
	  		if(access(path, R_OK) == 0) {
				file = fopen(path, "r");
				fprintf(stdout, "%s:%*s%s\n", 
						path, (int)(max_length - strlen(path)), " ", FILE_TYPE_STRINGS[check_type(file)]);
  	  		} else {
	    		print_error(path, errno, max_length);
	  		}
		}
		return EXIT_SUCCESS;
  	}
  

  //check if a file exists on path, if it does, open it and print typecheck string to stdout,
  //otherwise display error message
  
}
