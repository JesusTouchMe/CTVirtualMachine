#ifndef CTVM_MODULE_H
#define CTVM_MODULE_H

#include "bytecode/bytecodeheader.h"
#include "bytecode/subroutine.h"
#include "core/constpool.h"
#include "util/fileutils.h"
#include "util/vector.h"

#define CODE_SEGMENT 0x436F6465

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

CTModule* OpenModule(CTVM* vm, File bytecodeFile);

void ReleaseModule(CTVM* vm, CTModule* module);

CTSubroutine* FindSubroutine(CTModule* module, CTString* name);

#endif //CTVM_MODULE_H
