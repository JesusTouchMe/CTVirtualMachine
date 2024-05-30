#include "core/vm.h"

#include <stdlib.h>

static CTVM vm = {0};

void StartVM() {
    if (vm.modules != null) {
        return; //TODO: error?
    }

    vm.modules = VectorCreate();

    CreateStack(&vm.stack);
    GetExecutionContext(&vm.executionContext);

    GetRegister(vm.registers, SP).as_value_ptr = vm.stack.data - 1; // TODO: something better than this cuz it quite literally points to a out of reach address
    GetRegister(vm.registers, FP).as_value_ptr = vm.stack.data;
}

void ReleaseVM() {
    if (vm.modules == null) {
        return;
    }

    size_t size = VectorSize((Vector(void)*) &vm.modules);
    for (size_t i = 0; i < size; i++) {
        free(vm.modules[i].globalVariables.data);
        free(vm.modules[i].constPool.data);
        free(vm.modules[i].headerBytes);

        VectorFree((Vector(void)*) &vm.modules[i].subroutines);
    }

    VectorFree((Vector(void)*) &vm.modules);
    ReleaseStack(&vm.stack);

    vm.modules = null;
}

CTVM* GetVM() {
    if (vm.modules == null) {
        return null;
    }
    return &vm;
}

void Exit(i32 code) {
    if (vm.modules != null) {
        ReleaseVM();
    }

    exit(code);
}
