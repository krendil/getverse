
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
char * translation = 0x0;
char * format = 0x0;
char * reference = 0x0;
int red = 0;

void setReference( int count, char ** words ) {
    int size = 0;
    reference = (char*)calloc(1, strlen(words[0]));
    reference = strcat(reference, words[0]);

    for( int i = 1; i < count; i++ ) {
        size += strlen(words[i]+1);
        reference = (char*)realloc(reference, size);
        reference = strcat(strcat(reference, " "), words[i]);
    }
}

void readReference() {
    int size = 0;
     do {
         size += REF_LENGTH;
         reference = (char*)realloc(reference, size);
     } while (!fgets(reference, size, stdin));
}


int main(int argc, char ** argv) {

    //Parse arguments
    for( int i = 1; i < argc; i++ ) {
        //Not an option, must be start of reference
        if( argv[i][0] != '-' ) {
            setReference( argc - i, argv + i );
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
                //printUsage();
                exit(EXIT_SUCCESS);
                break;
            default:
                fprintf(stderr, "Unknown option %s\n", argv[i]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    //See if we need to get the reference from stdin
    if( reference == NULL ) {
        readReference();
    }

    Passage result;
    int err = getPassage(&result, translation, reference, 0);
    switch(err) {
        case MODULE_NOT_FOUND:
            fprintf(stderr, "Could not find translation '%s'. Are you sure it is installed?\n",
                    translation);
            exit(EXIT_FAILURE); break;
        case BOOK_NOT_FOUND:
            fprintf(stderr, "That reference does not exist.\n");
            exit(EXIT_FAILURE); break;
        case CHAPTER_NOT_FOUND:
            fprintf(stderr, "That book doesn't have that many chapters.\n");
            exit(EXIT_FAILURE); break;
        case VERSE_NOT_FOUND:
            fprintf(stderr, "That chapter doesn't have that many verses.\n");
            exit(EXIT_FAILURE); break;
        case 0:
            printf("%s\n", result.text);
            deletePassage(result);
            exit(EXIT_SUCCESS); break;
        default:
            fprintf(stderr, "Something when wrong. Not quit sure what.");
            exit(EXIT_FAILURE);
    }
    //Wheee! dead code zone!

}
