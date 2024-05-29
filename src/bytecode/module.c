#include <malloc.h>
#include "bytecode/module.h"
#include "core/vm.h"
#include "util/fileutils.h"

#define CODE_SEGMENT 0x436F6465

#define GetU16(bytes) (*(bytes++) << 8 | *(bytes++))
#define GetU32(bytes) (*(bytes++) << 24 | *(bytes++) << 16 | *(bytes++) << 8 | *(bytes++))
#define GetU32NoMod(bytes) (*(bytes) << 24 | *(bytes + 1) << 16 | *(bytes + 2) << 8 | *(bytes + 3))

void OpenModule(CTVM* vm, FILE* bytecodeFile) {
    u8* bytecode = GetFileBytes(bytecodeFile, null);

    CTModule module = {0};

    module.headerBytes = bytecode;

    module.header.arch = GetU16(bytecode);
    module.header.name.data = bytecode;
    module.header.name.length = 0;

    for (; *bytecode != 0; bytecode++) {
        module.header.name.length++;
    }
    bytecode++;

    module.globalVariables.count = GetU16(bytecode);
    module.globalVariables.data = (CTValue*) malloc(sizeof(CTValue) * module.globalVariables.count);

    module.header.entry = (u8*) GetU32(bytecode);

    module.subroutines = VectorCreate();
    u32 subroutineCount = 0;

    while (*bytecode != 0) {
        CTSubroutine subroutine = {0};

        subroutine.visibility = *(bytecode++);
        subroutine.name.data = bytecode;
        subroutine.name.length = 0;

        for (; *bytecode != 0; bytecode++) {
            subroutine.name.length++;
        }
        bytecode++;

        subroutine.frameSize = GetU16(bytecode);
        subroutine.entry = (u8*) GetU32(bytecode); // we set it to the position it thinks it's going to have, then offset it by the code segment at the end

        VectorAdd(&module.subroutines, subroutine);
        subroutineCount++;
    }
    bytecode++;

    //TODO: constant pool

    if (GetU32NoMod(bytecode) == CODE_SEGMENT) {
        bytecode += 4;
        module.code = bytecode;

        module.header.entry += (u64) module.code;
        for (u32 i = 0; i < subroutineCount; i++) {
            module.subroutines[i].entry += (u64) module.code;
        }
    }

    VectorAdd(&vm->modules, module);
}

void ReleaseModule(CTVM* vm, CTModule* module) {
    size_t size = VectorSize((Vector(void)*) &vm->modules);
    for (size_t i = 0; i < size; i++) {
        if (&vm->modules[i] == module) {
            VectorRemove(&vm->modules, i);
            break;
        }
    }

    free(module->globalVariables.data);
    free(module->headerBytes);

    VectorFree((Vector(void)*) &module->subroutines);
}
