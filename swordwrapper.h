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

/**
 * Sets up an empty Passage with the given reference and translation strings. Text is set to NULL
 *
 */
Passage initPassage( const char* reference, const char * translation );

/**
 * Frees the pointers in the given passage.
 *
 */
void deletePassage(Passage passage);

/**
 * An empty (uninitialised) passage with the given reference, translation and passage text.
 *
 * passage: The Passage to fill
 * translation: the SWORD module from which to get the text
 * reference: the BOOK CH:VS style reference to retrieve
 * flags: various flags to affect the process, currently unimplemented
 * return: 0 on success, or an error code, as defined above
 */
int getPassage( Passage * passage, const char * translation, const char * reference, unsigned int flags);

/**
 * Currently unimplemented, but will eventually allow formatting of passage info in a string
 */
char * formatPassage( const char* format, Passage * passage );

#ifdef __cplusplus
}
#endif

#endif

