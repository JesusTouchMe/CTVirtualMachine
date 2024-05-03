#ifndef VM_H
#define VM_H

#include "bytecode/bytecodeheader.h"
#include "core/register.h"

typedef struct CTVM {
    CTBytecodeHeader header;
} CTVM;

#endif //VM_H
