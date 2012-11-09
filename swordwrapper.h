#ifndef SWORDWRAPPER_H
#define SWORDWRAPPER_H

#define RED_LETTER (1 >> 0)

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

Passage getPassage( const char* translation, const char * reference, unsigned int flags);

char * formatPassage( const char* format, Passage * passage );

#ifdef __cplusplus
}
#endif

#endif

