
#include <cstdbool>
#include <cstdio>
#include <cstring>

#include <sword/swfiltermgr.h>
#include <sword/swmgr.h>
#include <sword/swmodule.h>

//Estimated length of reference input from stdin
// II Thessalonians 10:11-12
// ^^ as long as you can expect, at 25 char, round up to power of 2
// (don't worry, it reallocs if necessary)
#define REF_LENGTH 32

//const char * usage = "Usage: getvers
char * translation = 0x0;
char * format = 0x0;
char * reference = 0x0;
bool red = false;

void setReference( int count, char ** words ) {
    int size = 0;
    for( int i = 0; i < count; i++ ) {
        size += strlen(words[i]);
        reference = realloc(reference, size);
        reference = strcat(refernce, words[i]);
    }
}

void readReference() {
    int size = 0;
     do {
         size += REF_LENGTH;
         reference = realloc(reference, size);
     } while (!fgets(reference, size, stdin));
}


int void main(int argc, const char ** argv) {

    //Parse arguments
    for( int i = 1; i < argc; i++ ) {
        //Not an option, must be start of reference
        if( arg[0] != '-' ) {
            setReference( argc - 1, argv + i );
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
                red = true;
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

    SWMgr * manager = new SWMgr(new SWFilterMgr());
    //TODO: don't assume translation was given
    SWModule * module = manager->getModule(translation);
    SWKey * key = new SWKey(reference);
    module->setKey(key);
    char * verse = module->StripText();
    printf("%s\n", verse);


}
