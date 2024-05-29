#ifndef VM_H
#define VM_H

#include "bytecode/bytecodeheader.h"
#include "bytecode/module.h"
#include "core/register.h"
#include "core/stack.h"
#include "util/vector.h"
#include "executor/context.h"

typedef struct CTVM {
    Vector(CTModule) modules;

    registers_t registers;
    CTStack stack;
    CTExecutionContext executionContext;
} CTVM;

void StartVM();

CTVM* GetVM();

#endif //VM_H
