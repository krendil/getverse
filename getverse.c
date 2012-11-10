
#include <stdio.h>
#include <string.h>

#include <stdlib.h>

#include "swordwrapper.h"

//Estimated length of reference input from stdin
// II Thessalonians 10:11-12
// ^^ as long as you can expect, at 25 char, round up to power of 2
// (don't worry, it reallocs if necessary)
#define REF_LENGTH 32

//const char * usage = "Usage: getvers

void printUsage() {
    printf(\
            "Usage:	getverse -t <translation> [-f format] [<reference>]\n"\
            "	getverse [-h | -?]\n"\
            "Options:\n"\
            "	-t	A Bible translation you have installed in SWORD (e.g. KJV)\n"\
            "	-f	A format string that will determine the format of the output\n"\
            "		 Defaults to '%%p'\n"\
            "	-h\n"\
            "	-?	Display this message and exit"\
            "\n"\
            "Format variables:\n"\
            "	%%p	The passage text\n"\
            "	%%t	The translation name\n"\
            "	%%r	The supplied reference\n"\
            "	%%%%	A literal %% character\n"\
            "\n"\
            "If no reference is supplied, getverse tries to read from stdin.\n"\
          );
}

char * setReference( int count, char ** words ) {
    int size = 0;
    char * reference = (char*)calloc(1, strlen(words[0]));
    reference = strcat(reference, words[0]);

    for( int i = 1; i < count; i++ ) {
        size += strlen(words[i]+1);
        reference = (char*)realloc(reference, size);
        reference = strcat(strcat(reference, " "), words[i]);
    }
    return reference;
}

char * readReference() {
    int size = 0;
    char * reference = NULL;
    do {
         size += REF_LENGTH;
         reference = (char*)realloc(reference, size);
     } while (!fgets(reference, size, stdin));
     return reference;
}


int main(int argc, char ** argv) {

    char * translation = NULL;
    char * format = NULL;
    char * reference = NULL;
    int red = 0;

    //Parse arguments
    for( int i = 1; i < argc; i++ ) {
        //Not an option, must be start of reference
        if( argv[i][0] != '-' ) {
            reference = setReference( argc - i, argv + i );
            break;
        }

        switch( argv[i][1] ) {
            case 'f':
                format = argv[++i];
                break;
            case 't':
                translation = argv[++i];
                break;
            case 'r':
                red = 1;
                break;
            case 'h':
            case '?':
                printUsage();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "Unknown option %s\n", argv[i]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if(translation == NULL ) {
        fprintf(stderr, "Please specify a translation with the -t option\n");
        exit(EXIT_FAILURE);
    }

    //See if we need to get the reference from stdin
    if( reference == NULL ) {
        reference = readReference();
    }

    Passage result;
    int err = getPassage(&result, translation, reference, 0);
    switch(err) {
        case MODULE_NOT_FOUND:
            fprintf(stderr, "Could not find translation '%s'. Are you sure it is installed?\n",
                    translation);
            exit(EXIT_FAILURE); break;
        case BOOK_NOT_FOUND:
            fprintf(stderr, "That passage does not exist.\n");
            exit(EXIT_FAILURE); break;
        case CHAPTER_NOT_FOUND:
            fprintf(stderr, "That book doesn't have that many chapters.\n");
            exit(EXIT_FAILURE); break;
        case VERSE_NOT_FOUND:
            fprintf(stderr, "That chapter doesn't have that many verses.\n");
            exit(EXIT_FAILURE); break;
        case 0:
            if( format ) {
                char * out = formatPassage(format, &result);
                if(!out) {
                    fprintf(stderr, "Unknown variable in format string.\n");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n", out);
                free(out);
            } else {
                printf("%s\n", result.text);
            }
            deletePassage(result);
            exit(EXIT_SUCCESS); break;
        default:
            fprintf(stderr, "Something when wrong. Not quit sure what.");
            exit(EXIT_FAILURE);
    }
    //Wheee! dead code zone!

}
