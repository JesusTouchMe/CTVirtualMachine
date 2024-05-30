#include <malloc.h>
#include "core/stack.h"
#include "core/vm.h"

void CreateStack(CTStack* stack) {
    stack->size = 0;
    stack->data = (CTValue*) malloc(sizeof(CTValue) * 120000);
}

void ReleaseStack(CTStack* stack) {
    free(stack->data);
    stack->data = null;
}

void PushValue(CTVM* vm, CTValue value) {
    *(++GetRegister(vm->registers, SP).as_value_ptr) = value;
}

CTValue PopValue(CTVM* vm) {
    return *(GetRegister(vm->registers, SP).as_value_ptr--);
}

CTValue GetVariable(CTVM* vm, u32 index) {
    return *(GetRegister(vm->registers, FP).as_value_ptr + (index + 1));
}

void SetVariable(CTVM* vm, u32 index, CTValue value) {
    *(GetRegister(vm->registers, FP).as_value_ptr + (index + 1)) = value;
}
