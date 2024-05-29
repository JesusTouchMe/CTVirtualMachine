#include "core/vm.h"
#include "executor/code.h"

void DoCodeExecution(CTVM* vm) {
    while (vm->executionContext.shouldExecute) {
        CycleCodeExecutor(vm);
    }
}

void CycleCodeExecutor(CTVM* vm) {
    u8 opcode = *(GetRegister(vm->registers, PC).as_ptr++);
    ExecuteInstruction(vm, opcode, false);
    vm->executionContext.totalCycles++;

}