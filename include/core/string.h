#ifndef STRING_H
#define STRING_H

#include "defines.h"

typedef struct CTString {
    u64 length;
    u8* data;
} CTString;

typedef struct CTWideString {
    u64 length;
    u16* data;
} CTWideString;

#endif //STRING_H
