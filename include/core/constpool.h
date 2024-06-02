#ifndef CTVM_CONSTPOOL_H
#define CTVM_CONSTPOOL_H

#include "defines.h"
#include "value.h"

enum ConstPoolFieldTypes : u8 {
    ConstImm8 = 1,
    ConstImm16 = 2,
    ConstImm32 = 3,
    ConstImm64 = 4,
    ConstString = 5,
    ConstSubroutine = 6,
};

typedef struct CTConstPool {
     u16 size;
     CTValue* data;
} CTConstPool;

#endif //CTVM_CONSTPOOL_H
