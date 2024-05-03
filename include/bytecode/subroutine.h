#ifndef SUBROUTINE_H
#define SUBROUTINE_H

#include "defines.h"
#include "core/string.h"

typedef struct CTSubroutine {
    CTString name;


    u8* entry;
} CTSubroutine;

#endif //SUBROUTINE_H
