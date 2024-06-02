#ifndef CTVM_POINTER_H
#define CTVM_POINTER_H

#include "defines.h"
#include <malloc.h>

#define UniquePointer(type) attribute(cleanup(free)) type*
#define MakeUnique(type) malloc(sizeof(type))

#endif //CTVM_POINTER_H
