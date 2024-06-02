#include "core/vm.h"
#include "executor/context.h"

void GetExecutionContext(CTExecutionContext* executionContext) {
    executionContext->activeModule = null;
    executionContext->currentSubroutine = null;
    executionContext->totalCycles = 0;
    executionContext->shouldExecute = false;
}

void EnterExecution(CTVM* vm, CTModule* module) {
    vm->executionContext.activeModule = module;
    vm->executionContext.shouldExecute = true;
    GetRegister(vm->registers, PC).as_ptr = module->header.entry; // since a module enters at an address and not a declared subroutine (as far as the vm is aware), we have to set position and not currentSubroutine
}
