#include <malloc.h>
#include "bytecode/module.h"
#include "core/vm.h"
#include "util/fileutils.h"

#define GetU16(bytes) ({    \
    u8 t1 = *(bytes)++;     \
    u8 t2 = *(bytes)++;     \
    (u16) ((t1 << 8) | t2);         \
})
#define GetU32(bytes) ({    \
    u8 t1 = *(bytes)++;     \
    u8 t2 = *(bytes)++;     \
    u8 t3 = *(bytes)++;     \
    u8 t4 = *(bytes)++;     \
    (u64) ((t1 << 24) | (t2 << 16) | (t3 << 8) | t4); \
})
#define GetU32NoMod(bytes) (*(bytes) << 24 | *(bytes + 1) << 16 | *(bytes + 2) << 8 | *(bytes + 3))
#define GetU64(bytes) ({    \
    u64 t1 = *(bytes)++;     \
    u64 t2 = *(bytes)++;     \
    u64 t3 = *(bytes)++;     \
    u64 t4 = *(bytes)++;     \
    u8 t5 = *(bytes)++;     \
    u8 t6 = *(bytes)++;     \
    u8 t7 = *(bytes)++;     \
    u8 t8 = *(bytes)++;     \
    (u64) ((t1 << 56) | (t2 << 48) | (t3 << 40) | (t4 << 32) | (t5 << 24) | (t6 << 16) | (t7 << 8) | t8); \
})

static CTValue ParseConstant(CTVM* vm, CTModule* module, u8** bytecodePtr) {
    register CTValue value;
    register u8* bytecode = *bytecodePtr;

    switch (*bytecode++) {
        case ConstImm8: {
            value.as_byte = (i8) *bytecode++;
            break;
        }

        case ConstImm16: {
            value.as_short = GetU16(bytecode);
            break;
        }

        case ConstImm32: {
            value.as_int = GetU32(bytecode);
            break;
        }

        case ConstImm64: {
            value.as_long = GetU64(bytecode);
            break;
        }

        case ConstString: {
            value.as_string = (CTString*) malloc(sizeof(CTString)); // TODO: string object arena allocator
            value.as_string->data = bytecode;
            value.as_string->length = 0;

            for (; *bytecode != 0; bytecode++) {
                value.as_string->length++;
            }
            bytecode++;

            break;
        }

        case ConstSubroutine: {
            CTString string;
            string.data = bytecode;
            string.length = 0;

            for (; *bytecode != 0; bytecode++) {
                string.length++;
            }

            CTSubroutine* subroutine = FindSubroutine(module, &string); // TODO: across module searching
            if (subroutine == null) {
                puts("Constant subroutine not found.");
                Exit(1);
            }

            value.as_ptr = (u8*) subroutine;

            break;
        }

        default: {
            puts("Unknown constant entry.");
            Exit(1);
        }
    }

    *bytecodePtr = bytecode;
    return value;
}

CTModule* OpenModule(CTVM* vm, File bytecodeFile) {
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
    if (module.globalVariables.count > 0) module.globalVariables.data = (CTValue*) malloc(sizeof(CTValue) * module.globalVariables.count);

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

    module.constPool.size = GetU16(bytecode);
    if (module.constPool.size > 0) {
        module.constPool.data = (CTValue*) malloc(sizeof(CTValue) * module.constPool.size);

        register u16 i = 0;
        while (*bytecode != 0) {
            module.constPool.data[i++] = ParseConstant(vm, &module, &bytecode);
        }
    }
    bytecode++;

    if (GetU32NoMod(bytecode) == CODE_SEGMENT) {
        bytecode += 4;
        module.code = bytecode;

        module.header.entry += (u64) module.code;
        for (u32 i = 0; i < subroutineCount; i++) {
            module.subroutines[i].entry += (u64) module.code;
        }
    }

    VectorAdd(&vm->modules, module);
    return &vm->modules[VectorSize((Vector(void)*) &vm->modules) - 1];
}

void ReleaseModule(CTVM* vm, CTModule* module) {
    size_t size = VectorSize((Vector(void)*) &vm->modules);
    for (size_t i = 0; i < size; i++) {
        if (&vm->modules[i] == module) {
            VectorRemove(&vm->modules, i);
            break;
        }
    }

    if (module->globalVariables.count > 0) free(module->globalVariables.data);
    if (module->constPool.size > 0) free(module->constPool.data);
    free(module->headerBytes);

    VectorFree((Vector(void)*) &module->subroutines);
}

CTSubroutine* FindSubroutine(CTModule* module, CTString* name) {
    size_t size = VectorSize((Vector(void)*) &module->subroutines);
    for (size_t i = 0; i < size; i++) {
        if (StringEquals(&module->subroutines[i].name, name))
            return &module->subroutines[i];
    }

    return null;
}
