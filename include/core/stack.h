#ifndef CTVM_STACK_H
#define CTVM_STACK_H

#include "defines.h"
#include "core/value.h"

typedef struct CTVM CTVM;

typedef struct CTStack {
    u32 size;
    CTValue* data;
} CTStack;

void CreateStack(CTStack* stack);

void PushValue(CTVM* vm, CTValue value);

CTValue PopValue(CTVM* vm);

CTValue GetVariable(CTVM* vm, u32 index);

void SetVariable(CTVM* vm, u32 index, CTValue value);

#endif //CTVM_STACK_H
