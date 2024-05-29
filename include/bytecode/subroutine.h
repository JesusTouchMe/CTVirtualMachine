#ifndef SUBROUTINE_H
#define SUBROUTINE_H

#include "defines.h"
#include "core/string.h"

typedef enum CTSubroutineVisibility {
    PUBLIC = 1,
    PRIVATE = 2,
    NATIVE = 3,
} CTSubroutineVisibility;

typedef struct CTSubroutine {
    CTSubroutineVisibility visibility;
    CTString name;
    u16 frameSize; // amount of variables to allocate in its frame
    u8* entry;
} CTSubroutine;

#endif //SUBROUTINE_H
