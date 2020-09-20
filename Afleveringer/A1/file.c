#include <stdio.h>  // fprintf, stdout.
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <string.h> // strerror.
#include <errno.h>  // errno.
#include <unistd.h> // access().

#define UTF8_1B(x) (( (~(x)) & (1 << 7)) == (1 << 7))
#define UTF8_2B(x) ((( (x) & (3 << 6)) == (3 << 6)) && (( (~(x)) & (1 << 5)) == (1 << 5)))
#define UTF8_3B(x) ((( (x) & (7 << 5)) == (7 << 5)) && (( (~(x)) & (1 << 4)) == (1 << 4)))
#define UTF8_4B(x) ((( (x) & (15 << 4)) == (15 << 4)) && (( (~(x)) & (1 << 3)) == (1 << 3)))
#define UTF8_CONT(x) ((( (x) & (1 << 7)) == (1 << 7)) && (( (~(x)) & (1 << 6)) == (1 << 6)))

enum file_type {
	DATA,
	EMPTY,
	ASCII,
	ISO8859,
	UTF8,
	LITTLE_ENDIAN,
	BIG_ENDIAN
};

const char * const FILE_TYPE_STRINGS[] = {
	"data",
	"empty",
	"ASCII text",
	"ISO-8859",
	"UTF-8 Unicode Text",
	"Little-endian UTF-16 Unicode Text",
	"Big-endian UTF-16 Unicode Text"
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
	if (currentChar == EOF) {
		return EMPTY;
	}
	//Unless the file ends after the first byte of data...
	if (nextChar != EOF) {

		//...check if the file is little-endian UTF-16
		if (((unsigned char)currentChar == 0xFF) && ((unsigned char)nextChar == 0xFE)) {
			return LITTLE_ENDIAN;
	        } else if (((unsigned char)currentChar == 0xFE) && ((unsigned char)nextChar == 0xFF)) {
			return BIG_ENDIAN;
		}
	}

	int ekstraByte = 0;

	//Check file byte for byte
	while(currentChar != EOF) {
		//Check for symbols not in ASCII intervals provided in assignment text
		if (!(((bell <= currentChar) && (currentChar <= carriageReturn))
				|| (currentChar == escape)
				|| ((space <= currentChar) && (currentChar <= equivalencySign)))) {
			//If a conflict is found, the file is not ASCII
			notASCII = 1;
		}

		//Check file for symbols breaking the ISO-8859 standard (endpoint omitted, as currentChar <= yWithDiaresis will always be true for a char)
		if (!((nonBreakingSpace <= (unsigned char) currentChar))){
			//If a conflict is found, the file is not ISO-8859
			notISO = 1;
		}

		//Start of UTF-8 check
                //Check if the byte we are looking at is the UTF-8 data byte in multi-byte represented UTF-8 symbols
		if (UTF8_CONT(currentChar)) {
			if (ekstraByte == 0) {
				notUTF8 = 1;
			} else {
				ekstraByte--;
			}
		//Check if the byte we are looking at is the UTF-8 marker for 1-byte UTF-8 symbols
		} else if (UTF8_1B(currentChar)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			}
		//Check if the byte we are looking at is the UTF-8 marker for 2-byte UTF-8 symbols
		} else if (UTF8_2B(currentChar)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			} else {
				ekstraByte++;
			}
		//Check if the byte we are looking at is the UTF-8 marker for 3-byte UTF-8 symbols
		} else if (UTF8_3B(currentChar)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			} else {
				ekstraByte += 2;
			}
		//Check if the byte we are looking at is the UTF-8 marker for 4-byte UTF-8 symbols
		} else if (UTF8_4B(currentChar)) {
			if (ekstraByte != 0) {
				notUTF8 = 1;
			} else {
				ekstraByte += 3;
			}
		//If all the above fails, then the file is not UTF-8
		} else {
			notUTF8 = 1;
		}
		
		//Check next byte
		currentChar = nextChar;
		nextChar = fgetc(file);
	}

	fclose(file);

	// Determine which file type
	if (!notASCII) {
		return ASCII;
	} else if (!notISO) {
		return ISO8859;
	} else if (!notUTF8) {
		return UTF8;
	} else {
		return DATA;
	}
}

int Max_Length(int str_length, char *str[]) {
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
  	} else {
  		int i;
		//The longest path type is ascii, which is in index 2 of the FILE_TYPE_STRINGS array
		int max_length = Max_Length(argc, argv);
  		for (i = 1; i < argc; i++) {
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
