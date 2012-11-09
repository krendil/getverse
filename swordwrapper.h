#ifndef SWORDWRAPPER_H
#define SWORDWRAPPER_H

#define RED_LETTER (1 >> 0)

//Note that it is very tricky to tell if a chapter:verse exists in sword,
//since it goes on to the next book if the chapter is too high
// and likewise with chapter and verse.
#define MODULE_NOT_FOUND    -1
#define BOOK_NOT_FOUND      -2
#define CHAPTER_NOT_FOUND   -3
#define VERSE_NOT_FOUND     -4


typedef struct {
    char * reference;
    char * text;
    char * translation;
} Passage;

#ifdef __cplusplus
extern "C" {
#endif

Passage initPassage( const char* reference, const char * translation );
void deletePassage(Passage passage);

int getPassage( Passage * passage, const char * translation, const char * reference, unsigned int flags);

char * formatPassage( const char* format, Passage * passage );

#ifdef __cplusplus
}
#endif

#endif

