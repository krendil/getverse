#include "swordwrapper.h"

#include <swfiltermgr.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <listkey.h>

using namespace sword;

Passage getPassage( const char* translation, const char * reference, unsigned int flags) {
    //Set up the manager space
    SWMgr * manager = new SWMgr(new SWFilterMgr());
    //Get the right translation
    SWModule * module = manager->getModule(translation);
    //Turn the key into a set of list
    VerseKey * key = new VerseKey(reference);

    ListKey list = key->ParseVerseList(reference);

    Passage passage = initPassage(reference, translation);

    //Get each verse
    passage.text = (char*)calloc(1, 1);
    int size = 0;
    /*int count = list.Count();
    for(int i = 0; i < list.Count(); i++) {
        SWKey * key = list.getElement(i);
        //Build the passage*/
    for(list = TOP; !list.Error(); list++) {
        module->setKey(list);
        const char * verse = module->StripText();
        size += strlen(verse);
        passage.text = (char*)realloc(passage.text, size);
        strcat(passage.text, verse);
    }

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


