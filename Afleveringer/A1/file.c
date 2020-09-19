#include <stdio.h>  // fprintf, stdout.
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access().

enum file_type {
	DATA,
	EMPTY,
	ASCII,
	ISO8859;
	UTF8;
	LITTLE_ENDIAN;
	BIG_ENDIAN;
};

const char * const FILE_TYPE_STRINGS[] = {
	"data",
	"empty",
	"ASCII text",
	"ISO-8859",
	"UTF-8 Unicode Text",
	"Little-edian UTF-16 Unicode Text",
	"Big-edian UTF-16 Unicode Text"
};

enum ASCII_Constraints {
	bell = 7,
	carriageReturn = 13,
	escape = 27,
	space = 32,
	equivalencySign = 126,
	nonBreakingSpace = 160,
	yWithDiaeresis = 255
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

	char currentChar = fgetc(file);
	char nextChar = fgetc(file);

	int notASCII = 0;
	int notISO = 0;
	int notUTF8 = 0;

	//if the first char encountered is the end of file marker
	if (c == EOF) {
		return EMPTY;
	}

	//iterate through the file, if you encounter something that is not in the 
	//accepted ranges of ASCII characters, deem the file data, otherwise ASCII
	while (currentChar != EOF && !(notASCII && notISO && notUTF8)) {
		
		//Check if the file is UTF-16
		if (nextChar != EOF) {

			//Check if the file is little-endian UTF-16
			if ((currentChar == 0xFF) && (nextChar == 0xFE)) {
				notASCII = 1;
				notISO = 1;
				notUTF8 = 1;
				return LITTLE_ENDIAN;

			//Check if the file is big-endian UTF-16
			} else if ((currentChar == 0xFE) && (nextChar == 0xFF)) {
				notASCII = 1;
				notISO = 1;
				notUTF8 = 1;
				return BIG_ENDIAN;

			} else {
				file--;
			}
		}

		//Have all bytes been ASCII so far?
		if ( !(notASCII) )  {
			//Is the current byte not a ASCII?
			if ( !(((bell <= currentChar) && (currentChar <= carriageReturn))
				|| (currentChar == escape)
				|| ((space <= currentChar) && (currentChar <= equivalencySign)))) {
				notASCII = 1;
			}
		} 

		//Have any bytes not been ASCII and have all others been ISO-8859?
		if ( !(notISO) && notASCII ) {
			//Is the current byte not a ISO-8859?
			if ( !((nonBreakingSpace <= currentChar) && (currentChar <= yWithDiaeresis))) {
				notISO = 1;
			}
		}

		//Have any bytes not been ISO-8859 and have all others been UTF8?
		if (notISO && !(notUTF8)) {
			//Is the current byte not a UTF-8?
			if (0b11110000 = (currentChar & (0b11110000))) {

				char followingChar = fgetc(file);
				for (int i = 0; i < 2; i++) {
					if ( (followingChar == EOF) || ((0b10000000) != (followingChar & (0b10000000)))) {
						notUTF8 = 1;
					}
					char get = fgetc(file);
				}

			} else if (0b11100000 != (currentChar & 0b11100000)) {
				
				char followingChar = fgetc(file);
				for (int i = 0; i < 1; i++) {
					if ( (followingChar == EOF) || ((0b10000000) != (followingChar & (0b10000000)))) {
						notUTF8 = 1;
					}
					char followingChar = fgetc(file);
				}

			} else if ((0b110000 != (currentChar & 0b110000))) {

				char followingChar = fgetc(file);
				if ( (followingChar == EOF) && ((0b10000000) != (followingChar & (0b10000000)))) { 
					notUTF8 = 1;
				}

			} else {
				notUTF8 = 1;
			}
		}

		c = fgetc(file);

	}

	fclose(file);

	// Determine which file type
	if (notASCII && notISO && notUTF8) {
		return DATA;
	} else if (notASCII && notISO) {
		return UTF8;
	} else if (notASCII) {
		return ISO8859;
	} else {
		return ASCII;
	}

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
}
