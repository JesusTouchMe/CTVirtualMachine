#ifndef OBJECT_H
#define OBJECT_H
#include "defines.h"

typedef struct CTObject {
    u8* allocationPattern;
    u64 actualSize;

    u8 data[];
} CTObject;

#endif //OBJECT_H
