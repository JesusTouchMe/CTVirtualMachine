#include <stdio.h>

#include "defines.h"
#include "core/vm.h"
#include "util/fileutils.h"
#include "executor/code.h"

void MakeTestModule() {
    AutoClose File file = OpenC("test.ct", "wb");
    CTString moduleName = CreateConstString("test_module");
    CTString mainName = CreateConstString("main");

    // header info
    WriteShort(file, 1); // arch = 1
    WriteString(file, &moduleName);
    WriteByte(file, 0);

    WriteShort(file, 0); // global variables
    WriteInt(file, 0); // entry at 0

    // subroutines
    WriteByte(file, PUBLIC); // visibility
    WriteString(file, &mainName); // name
    WriteByte(file, 0);
    WriteShort(file, 1); // local variables
    WriteInt(file, 2); // entry point. starts at 2 since 0 and 1 will be used for ENTER 0 (not to be confused with ENTER_0) as it does not look for symbols and therefor doesn't allocate local variables at module entry
    WriteByte(file, 0); // end of subroutine decl

    // const pool size
    WriteShort(file, 1);

    WriteByte(file, ConstImm32);
    WriteInt(file, 69);

    WriteByte(file, 0);

    WriteInt(file, CODE_SEGMENT);

    WriteByte(file, ENTER);
    WriteByte(file, 1);

    WriteByte(file, CONSTLOAD);
    WriteByte(file, ACC);
    WriteShort(file, 0);

    WriteByte(file, RET);
}

i32 main(void) {
    MakeTestModule();
    StartVM();

    CTVM* vm = GetVM();

    File testModuleFile = OpenC("test.ct", "rb");
    CTModule* testModule = OpenModule(vm, testModuleFile);
    
    Close(testModuleFile);

    EnterExecution(vm, testModule);

    DoCodeExecution(vm);

    printf("Improper VM exit. Accumulator value: %i, process exit value: -1\n", GetRegister(vm->registers, ACC).as_int);
    return -1;
}