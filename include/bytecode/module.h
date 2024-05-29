#ifndef CTVM_MODULE_H
#define CTVM_MODULE_H

#include <stdio.h>
#include "bytecode/bytecodeheader.h"
#include "bytecode/subroutine.h"
#include "core/constpool.h"
#include "util/vector.h"

typedef struct CTVM CTVM;

typedef struct CTGlobalVariables {
    u16 count;
    CTValue* data;
} CTGlobalVariables;

typedef struct CTModule {
    CTBytecodeHeader header;
    Vector(CTSubroutine) subroutines;

    CTConstPool constPool;
    CTGlobalVariables globalVariables;

    u8* headerBytes;
    u8* code;
} CTModule;

void OpenModule(CTVM* vm, FILE* bytecodeFile);

void ReleaseModule(CTVM* vm, CTModule* module);

#endif //CTVM_MODULE_H
