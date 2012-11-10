#include "swordwrapper.h"

#include <swfiltermgr.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <listkey.h>

#include <ctype.h>
/**
 * Copyright (c) 2012 David Osborne
 * getsource is licensed under the MIT License,
 * http://opensource.org/licenses/MIT
 */

#include <regex.h>

using namespace sword;

static char * trimWhitespace(char * in) {
    int start = -1;
    while( isspace(in[++start]) );
    int end = strlen(in);
    while ( isspace(in[--end]) );
    int size = end - start + 2;
    char * result = (char *) malloc(size);
    memcpy(result, in + start, size);
    result[size] = 0x0;
    free(in);
    return result;
}

int getPassage( Passage * passage, const char* translation, const char * reference, unsigned int flags) {


    //Set up the manager space
    SWMgr * manager = new SWMgr(new SWFilterMgr());
    //Get the right translation
    SWModule * module = manager->getModule(translation);
    if( !module ) {
        delete manager;
        return MODULE_NOT_FOUND;
    }
    //Turn the key into a set of list
    VerseKey * key = new VerseKey();
    key->setText(reference, false); //Currently SWORD normalises anyway
    if( key->getChapter() > key->getChapterMax() || key->getChapter() < 0 ) {
        delete manager;
        return CHAPTER_NOT_FOUND;
    }
    if( key->getVerse() > key->getVerseMax() || key->getVerse() < 0 ) {
        delete manager;
        return VERSE_NOT_FOUND;
    }

    ListKey list = key->ParseVerseList(reference, 0, true, true);
    module->setKey(list);

    if( list.Count() == 0 ) {
        delete manager;
        return BOOK_NOT_FOUND;
    }

    *passage = initPassage(reference, translation);
    //Get each verse
    passage->text = (char*)calloc(1, 1);
    int size = 0;

    //Get ready to remove brackets
    regex_t bracketMatch;
    int err = regcomp(&bracketMatch, " \\[\\] ", 0);

    for(list = TOP; !list.Error(); module->setKey(++list)) {
        const char * verse = module->StripText();
        size += strlen(verse) + 1;
        passage->text = (char*)realloc(passage->text, size);

        regmatch_t match[1];
        int start = 0;
        for(;;) {
            err = regexec(&bracketMatch, verse + start, 1, match, 0);
            if(err == REG_NOMATCH) {
                strcat(passage->text, verse + start);
                strcat(passage->text, " ");
                break;
            }

            // Match found.  Copy the bit before the match, and skip to the end
            strncat(passage->text, verse + start, match[0].rm_so);
            start += match[0].rm_eo;
        }
    }

    delete manager;
    regfree(&bracketMatch);

    passage->text = trimWhitespace(passage->text);

    return 0;
}

Passage initPassage(const char * reference, const char * translation) {
    Passage passage;
    passage.reference = (char*) malloc(strlen(reference));
    strcpy(passage.reference, reference);
    passage.translation = (char*) malloc(strlen(translation));
    strcpy(passage.translation, translation);
    passage.text = NULL;
    return passage;
}

void deletePassage(Passage p) {
    free(p.reference);
    free(p.translation);
    if(p.text) {
        free(p.text);
    }
}

char * formatPassage( const char* format, Passage * passage ) {
    char * result = NULL;
    const char * var = NULL;
    int fi, ri, offset, size;
    fi = ri = 0;
    size = 1; //Null terminator
    while( fi < size ) {
        offset = strcspn( format + fi, "%" );
        result = (char*)realloc(result, size += offset);
        strncpy( result + ri, format + fi, offset);
        ri += offset;
        fi += offset;

        if( !format[fi] ) {
            break;
        }
        switch(format[fi+1]) {
            case 'p':
                var = passage->text;
                break;
            case 't':
                var = passage->translation;
                break;
            case 'r':
                var = passage->reference;
                break;
            case '%':
                var = "%";
                break;
            default:
                return NULL;
        }
        offset = strlen(var);
        result = (char*)realloc(result, size += offset);
        strcpy( result + ri, var);
        ri += offset;
        fi += 2;
    }
    result[ri] = 0x0;
    return result;
}
