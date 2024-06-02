#ifndef STRING_H
#define STRING_H

#include "defines.h"

#define CreateString(str) (CTString) {strlen(str), str}
#define CreateConstString(str) (CTString) {sizeof(str) - 1, (u8*) str}

typedef struct CTString {
    u64 length;
    u8* data;
} CTString;

typedef struct CTWideString {
    u64 length;
    u16* data;
} CTWideString;

bool StringEquals(CTString* string1, CTString* string2);

#endif //STRING_H
