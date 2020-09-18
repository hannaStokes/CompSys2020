#include <stdio.h>  // fprintf, stdout.
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access().

#define UTF8_1B(x) (((~x)&(1<<7))==(1<<7))
#define UTF8_2B(x) ((x&(3<<6))==(3<<6)) && (((~x)&(1<<5))==(1<<5))
#define UTF8_3B(x) ((x&(7<<5))==(7<<5)) && (((~x)&(1<<4))==(1<<4))
#define UTF8_4B(x) ((x&(15<<4))==(15<<4)) && (((~x)&(1<<3))==(1<<3))
#define UTF8_Extra(x) (((x)&(1<<7))==(1<<7)) && (((~x)&(1<<6))==(1<<6))

enum file_type {
	DATA,
	EMPTY,
	ASCII,
	ISO8859,
	UTF8,
	LittleEdian,
	BigEdian
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

enum ASCII_constraints {
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
	char c = fgetc(file);
	//if the first char encountered is the end of file marker
	if(c == EOF) {
		return EMPTY;
	} 
	int notAscii = 0;
	int notISO = 0;
	int notUTF8 = 0;
	/*int notLittleEdian = 0;*/
	/*int notBigEdian = 0;*/

	int ekstraByte = 0;

	//iterate through the file, if you encounter something that is not in the 
	//accepted ranges of ASCII characters, deem the file data, otherwise ASCII
	while(c != EOF) {
		if (!(((bell <= c) && (c <= carriageReturn))
				|| (c == escape)
				|| ((space <= c) && (c <= equivalencySign)))) {
			notAscii = 1;
			if (!((nonBreakingSpace <= (unsigned char) c) && ((unsigned char) c <= yWithDiaeresis))){
				notISO = 1;
			}
		} 
		if (UTF8_Extra(c)) {
			if (ekstraByte == 0) {
				notUTF8 = 1;
			}
			else {
				ekstraByte--;
			}
		}
		else if (UTF8_1B(c)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			}
		}
		else if (UTF8_2B(c)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			}
			else {
				ekstraByte++;
			}
		}
		else if (UTF8_3B(c)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			}
			else {
				ekstraByte += 2;
			}
		}
		else if (UTF8_4B(c)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			}
			else {
				ekstraByte += 3;
			}
		}
		else {
			notUTF8 = 1;
		}
		
		c = fgetc(file);
	}

	fclose(file);

	if (!notAscii) {
		return ASCII;
	}
	else if (!notISO) {
		return ISO8859;
	}
	else if (!notUTF8) {
		return UTF8;
	}
	else {
		return DATA;
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
  

  //check if a file exists on path, if it does, open it and print typecheck string to stdout,
  //otherwise display error message
  
}
