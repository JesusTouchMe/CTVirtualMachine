#ifndef CTVM_CONTEXT_H
#define CTVM_CONTEXT_H

#include "defines.h"
#include "core/value.h"

typedef struct CTVM CTVM;

typedef struct CTExecutionContext {
    CTModule* activeModule;
    CTSubroutine* currentSubroutine;

    u64 totalCycles;
    bool shouldExecute;
} CTExecutionContext;

void GetExecutionContext(CTExecutionContext* executionContext);

/**
 * Prepares the execution context to begin running within a module and its entry. Only use on newly created and initialized VM and ExecutionContext.
 */
void EnterExecution(CTExecutionContext* executionContext, CTModule* module);

#endif //CTVM_CONTEXT_H
