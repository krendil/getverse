#include "swordwrapper.h"

#include <swfiltermgr.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <listkey.h>

#include <regex.h>

using namespace sword;

Passage getPassage( const char* translation, const char * reference, unsigned int flags) {
    //Set up the manager space
    SWMgr * manager = new SWMgr(new SWFilterMgr());
    //Get the right translation
    SWModule * module = manager->getModule(translation);
    //Turn the key into a set of list
    VerseKey * key = new VerseKey(reference);

    ListKey list = key->ParseVerseList(reference, 0, true, true);
    module->setKey(list);

    Passage passage = initPassage(reference, translation);

    //Get each verse
    passage.text = (char*)calloc(1, 1);
    int size = 0;

    //Get ready to remove brackets
    regex_t bracketMatch;
    int err = regcomp(&bracketMatch, " \\[\\] ", 0);

    for(list = TOP; !list.Error(); module->setKey(++list)) {
        const char * verse = module->StripText();
        size += strlen(verse) + 1;
        passage.text = (char*)realloc(passage.text, size);

        regmatch_t match[1];
        int start = 0;
        for(;;) {
            err = regexec(&bracketMatch, verse + start, 1, match, 0);
            if(err == REG_NOMATCH) {
                strcat(passage.text, verse + start);
                strcat(passage.text, " ");
                break;
            }

            // Match found.  Copy the bit before the match, and skip to the end
            strncat(passage.text, verse + start, match[0].rm_so);
            start += match[0].rm_eo;
        }
    }

    delete key;
    delete module;
    delete manager;
    regfree(&bracketMatch);

    return passage;
}

Passage initPassage(const char * reference, const char * translation) {
    Passage passage;
    passage.reference = (char*) malloc(strlen(reference));
    strcpy(passage.reference, reference);
    passage.translation = (char*) malloc(strlen(translation));
    strcpy(passage.translation, translation);
    passage.text = 0x0;
    return passage;
}

void deletePassage(Passage p) {
    free(p.reference);
    free(p.translation);
    free(p.text);
}


