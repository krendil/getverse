/**
 * Copyright (c) 2012 David Osborne
 * getsource is licensed under the MIT License,
 * http://opensource.org/licenses/MIT
 */

#ifndef SWORDWRAPPER_H
#define SWORDWRAPPER_H

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
 * Formats the info in the passage into a given buffer, according to the format string.
 *
 * format: A format specifier string, containing zero or more of the following variables
 *          %p: Will be expanded into the passage text
 *          %t: Will be expanded into the translation name
 *          %r: Will be expanded into the passage reference
 *          %%: A literal % character
 * passage: The passage containing information to be formatted
 * return: A dynamically allocated string containing the formatted data, or NULL, if there
 *          was an error (probably an unknown variable);
 */
char * formatPassage( const char* format, Passage * passage );

#ifdef __cplusplus
}
#endif

#endif

