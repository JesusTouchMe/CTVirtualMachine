#include "core/vm.h"

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

CTVM* GetVM() {
    if (vm.modules == null) {
        return null;
    }
    return &vm;
}