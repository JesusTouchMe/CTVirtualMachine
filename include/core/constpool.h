#ifndef CTVM_CONSTPOOL_H
#define CTVM_CONSTPOOL_H

#include "defines.h"
#include "value.h"

typedef struct CTConstPool {
     u16 size;
     CTValue* data;
} CTConstPool;

#endif //CTVM_CONSTPOOL_H
