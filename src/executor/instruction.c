#include "core/vm.h"
#include "executor/code.h"

// instruction generation macros
#define inst(opcode, argcBytes, ...)  case opcode: GetRegister(vm->registers, PC).as_ptr += argcBytes;
#define wide_inst(opcode, argcBytes, wideArgcBytes, ...) case opcode: GetRegister(vm->registers, PC).as_ptr += (wide) ? wideArgcBytes : argcBytes;

// operand variable generation macros
#define _op1() (*operands++)
#define _op2() ({ \
    u8 t1 = *operands++; \
    u8 t2 = *operands++; \
    (u16) ((t1 << 8) | t2);      \
})
#define _op4() ({ \
    u8 t1 = *operands++; \
    u8 t2 = *operands++; \
    u8 t3 = *operands++; \
    u8 t4 = *operands++; \
    (u32) ((t1 << 24) | (t2 << 16) | (t3 << 8) | t4); \
})
#define _op8() ({ \
    u64 t1 = *operands++; \
    u64 t2 = *operands++; \
    u64 t3 = *operands++; \
    u64 t4 = *operands++; \
    u64 t5 = *operands++; \
    u64 t6 = *operands++; \
    u64 t7 = *operands++; \
    u64 t8 = *operands++; \
    (u64) ((t1 << 56) | (t2 << 48) | (t3 << 40) | (t4 << 32) | (t5 << 24) | (t6 << 16) | (t7 << 8) | t8); \
})

#define op(name) u8 name = _op1()
#define op2(name) u16 name = _op2()
#define op4(name) u32 name = _op4()
#define op8(name) u64 name = _op8()
#define wide_op(name) u16 name = (wide) ? _op2() : _op1()

// utility macros
#define ret_none(...) return CT_null
#define regalloc(name) register const __auto_type name

static inline void JumpUnconditional(CTVM* vm, u64 relative) {
    GetRegister(vm->registers, PC).as_long += relative;
}

static inline void JumpConditional(CTVM* vm, u64 relative, u64 conditionFlag) {
    if (GetFlag(vm->registers, conditionFlag)) {
        JumpUnconditional(vm, relative);
    }
}

static inline void JumpConditionalNot(CTVM* vm, u64 relative, u64 conditionFlag) {
    if (!GetFlag(vm->registers, conditionFlag)) {
        JumpUnconditional(vm, relative);
    }
}

CTValue ExecuteInstruction(CTVM* vm, u8 opcode, bool wide) {
    // should not be touched outside a macro
    u8* operands = GetRegister(vm->registers, PC).as_ptr;

    switch (opcode) {
        inst(NOP, 0) {
            ret_none();
        }

        wide_inst(REPEAT, 1, 2, (iterations)) {
            wide_op(iterations);

            for (u16 i = 0; i < iterations; i++) {
                u8 nextOpcode = *(GetRegister(vm->registers, PC).as_ptr++);
                ExecuteInstruction(vm, nextOpcode, false);
            }

            ret_none();
        }

        inst(PUSH, 1, (reg)) {
            op(reg);

            PushValue(vm, GetRegister(vm->registers, reg));

            ret_none();
        }

        wide_inst(PUSHVAR, 1, 2, (index)) {
            wide_op(index);

            PushValue(vm, GetVariable(vm, index));

            ret_none();
        }

        wide_inst(PUSHGLOBAL, 1, 2, (index)) {
            wide_op(index);

            PushValue(vm, vm->executionContext.activeModule->globalVariables.data[index]);

            ret_none();
        }

        inst(POP, 1, (reg)) {
            op(reg);

            GetRegister(vm->registers, reg) = PopValue(vm);

            ret_none();
        }

        wide_inst(POPVAR, 1, 2, (index)) {
            wide_op(index);

            SetVariable(vm, index, PopValue(vm));

            ret_none();
        }

        wide_inst(POPGLOBAL, 1, 2, (index)) {
            wide_op(index);

            vm->executionContext.activeModule->globalVariables.data[index] = PopValue(vm);

            ret_none();
        }

        inst(DUP, 0) {
            PushValue(vm, vm->stack.data[GetRegister(vm->registers, SP).as_int]);
            ret_none();
        }

        inst(MOVE, 2, (dest, source)) {
            op(dest);
            op(source);

            GetRegister(vm->registers, dest) = GetRegister(vm->registers, source);

            ret_none();
        }

        inst(SWAP, 2, (reg1, reg2)) {
            op(reg1);
            op(reg2);
            regalloc(reg1Value) = GetRegister(vm->registers, reg1);

            GetRegister(vm->registers, reg1) = GetRegister(vm->registers, reg2);
            GetRegister(vm->registers, reg2) = reg1Value;

            ret_none();
        }

        wide_inst(GLOBALLOAD, 2, 3, (dest, index)) {
            op(dest);
            wide_op(index);

            GetRegister(vm->registers, dest) = vm->executionContext.activeModule->globalVariables.data[index];

            ret_none();
        }

        wide_inst(GLOBALSTORE, 2, 3, (index, source)) {
            wide_op(index);
            op(source);

            vm->executionContext.activeModule->globalVariables.data[index] = GetRegister(vm->registers, source);

            ret_none();
        }

        wide_inst(LOAD, 2, 3, (dest, index)) {
            op(dest);
            wide_op(index);

            GetRegister(vm->registers, dest) = GetVariable(vm, index);

            ret_none();
        }

        wide_inst(STORE, 2, 3, (index, source)) {
            wide_op(index);
            op(source);

            SetVariable(vm, index, GetRegister(vm->registers, source));

            ret_none();
        }

        inst(CONSTLOAD, 3, (dest, index)) {
            op(dest);
            op2(index);

            GetRegister(vm->registers, dest) = vm->executionContext.activeModule->constPool.data[index];

            ret_none();
        }

        inst(ADD, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long += GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(FADD, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_double += GetRegister(vm->registers, right).as_double;

            ret_none();
        }

        inst(SUB, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long -= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(FSUB, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_double -= GetRegister(vm->registers, right).as_double;

            ret_none();
        }

        inst(MUL, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long *= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(FMUL, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_double *= GetRegister(vm->registers, right).as_double;

            ret_none();
        }

        inst(DIV, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long /= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(FDIV, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_double /= GetRegister(vm->registers, right).as_double;

            ret_none();
        }

        inst(AND, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long &= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(OR, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long |= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(XOR, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long ^= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(SHL, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long <<= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(SHR, 2, (left, right)) {
            op(left);
            op(right);

            GetRegister(vm->registers, left).as_long >>= GetRegister(vm->registers, right).as_long;

            ret_none();
        }

        inst(INC, 1, (reg)) {
            op(reg);

            GetRegister(vm->registers, reg).as_long++;

            ret_none();
        }

        inst(DEC, 1, (reg)) {
            op(reg);

            GetRegister(vm->registers, reg).as_long--;

            ret_none();
        }

        inst(NOT, 1, (reg)) {
            op(reg);

            GetRegister(vm->registers, reg).as_long = ~GetRegister(vm->registers, reg).as_long;

            ret_none();
        }

        inst(NEG, 1, (reg)) {
            op(reg);

            GetRegister(vm->registers, reg).as_long = -GetRegister(vm->registers, reg).as_long;

            ret_none();
        }

        inst(TEST, 2, (left, right)) {
            op(left);
            op(right);
            regalloc(leftValue) = GetRegister(vm->registers, left).as_long;
            regalloc(rightValue) = GetRegister(vm->registers, right).as_long;

            GetRegister(vm->registers, FLAG) = CT_null;

            if (leftValue == rightValue) SetFlag(vm->registers, FLAG_EQ);
            if (leftValue < rightValue) SetFlag(vm->registers, FLAG_LF);
            if (leftValue > rightValue) SetFlag(vm->registers, FLAG_GF);

            ret_none();
        }

        inst(JMP, 1, (reg)) {
            op(reg);

            JumpUnconditional(vm, GetRegister(vm->registers, reg).as_long);

            ret_none();
        }

        inst(JMP_REL8, 1, (rel)) {
            op(rel);

            JumpUnconditional(vm, rel);

            ret_none();
        }

        inst(JMP_REL32, 4, (rel)) {
            op4(rel);

            JumpUnconditional(vm, rel);

            ret_none();
        }

        inst(JEQ, 1, (reg)) {
            op(reg);

            JumpConditional(vm, GetRegister(vm->registers, reg).as_long, FLAG_EQ);

            ret_none();
        }

        inst(JEQ_REL8, 1, (rel)) {
            op(rel);

            JumpConditional(vm, rel, FLAG_EQ);

            ret_none();
        }

        inst(JEQ_REL32, 4, (rel)) {
            op4(rel);

            JumpConditional(vm, rel, FLAG_EQ);

            ret_none();
        }

        inst(JNE, 1, (reg)) {
            op(reg);

            JumpConditionalNot(vm, GetRegister(vm->registers, reg).as_long, FLAG_EQ | FLAG_LF | FLAG_GF);

            ret_none();
        }

        inst(JNE_REL8, 1, (rel)) {
            op(rel);

            JumpConditionalNot(vm, rel, FLAG_EQ | FLAG_LF | FLAG_GF);

            ret_none();
        }

        inst(JNE_REL32, 4, (rel)) {
            op4(rel);

            JumpConditionalNot(vm, rel, FLAG_EQ | FLAG_LF | FLAG_GF);

            ret_none();
        }

        inst(JLT, 1, (reg)) {
            op(reg);

            JumpConditional(vm, GetRegister(vm->registers, reg).as_long, FLAG_LF);

            ret_none();
        }

        inst(JLT_REL8, 1, (rel)) {
            op(rel);

            JumpConditional(vm, rel, FLAG_LF);

            ret_none();
        }

        inst(JLT_REL32, 4, (rel)) {
            op4(rel);

            JumpConditional(vm, rel, FLAG_LF);

            ret_none();
        }

        inst(JGT, 1, (reg)) {
            op(reg);

            JumpConditional(vm, GetRegister(vm->registers, reg).as_long, FLAG_GF);

            ret_none();
        }

        inst(JGT_REL8, 1, (rel)) {
            op(rel);

            JumpConditional(vm, rel, FLAG_GF);

            ret_none();
        }

        inst(JGT_REL32, 4, (rel)) {
            op4(rel);

            JumpConditional(vm, rel, FLAG_GF);

            ret_none();
        }

        inst(JLE, 1, (reg)) {
            op(reg);

            JumpConditional(vm, GetRegister(vm->registers, reg).as_long, FLAG_EQ | FLAG_LF);

            ret_none();
        }

        inst(JLE_REL8, 1, (rel)) {
            op(rel);

            JumpConditional(vm, rel, FLAG_EQ | FLAG_LF);

            ret_none();
        }

        inst(JLE_REL32, 4, (rel)) {
            op4(rel);

            JumpConditional(vm, rel, FLAG_EQ | FLAG_LF);

            ret_none();
        }

        inst(JGE, 1, (reg)) {
            op(reg);

            JumpConditional(vm, GetRegister(vm->registers, reg).as_long, FLAG_EQ | FLAG_GF);

            ret_none();
        }

        inst(JGE_REL8, 1, (rel)) {
            op(rel);

            JumpConditional(vm, rel, FLAG_EQ | FLAG_GF);

            ret_none();
        }

        inst(JGE_REL32, 4, (rel)) {
            op4(rel);

            JumpConditional(vm, rel, FLAG_EQ | FLAG_GF);

            ret_none();
        }

        inst(GOTO, 2, (loc)) {
            op2(loc);

            GetRegister(vm->registers, PC).as_ptr = vm->executionContext.activeModule->code + loc;

            ret_none();
        }

        inst(GOTO_W, 4, (loc)) {
            op4(loc);

            GetRegister(vm->registers, PC).as_ptr = vm->executionContext.activeModule->code + loc;

            ret_none();
        }

        inst(ENTER, 1, (frameSize)) {
            op(frameSize);

            PushValue(vm, GetRegister(vm->registers, FP));
            GetRegister(vm->registers, FP) = GetRegister(vm->registers, SP);
            GetRegister(vm->registers, SP).as_value_ptr += frameSize;

            ret_none();
        }

        inst(ENTER_0, 0) {
            PushValue(vm, GetRegister(vm->registers, FP));
            GetRegister(vm->registers, FP) = GetRegister(vm->registers, SP);

            ret_none();
        }

        inst(LEAVE, 0) {
            GetRegister(vm->registers, SP) = GetRegister(vm->registers, FP);
            GetRegister(vm->registers, FP) = PopValue(vm);

            ret_none();
        }

        inst(JSR, 2, (branch)) {
            op2(branch);

            PushValue(vm, GetRegister(vm->registers, PC));
            GetRegister(vm->registers, PC).as_ptr = vm->executionContext.activeModule->code + branch;

            ret_none();
        }

        inst(JSR_W, 4, (branch)) {
            op4(branch);

            PushValue(vm, GetRegister(vm->registers, PC));
            GetRegister(vm->registers, PC).as_ptr = vm->executionContext.activeModule->code + branch;

            ret_none();
        }

        inst(CALL, 2, (index)) {
            op2(index);
            regalloc(subroutine) = (CTSubroutine*) vm->executionContext.activeModule->constPool.data[index].as_ptr;

            if (subroutine->visibility == NATIVE) {
                native_subroutine_t native = (native_subroutine_t) subroutine->entry;

                GetRegister(vm->registers, ACC) = native(vm);
            } else {
                PushValue(vm, GetRegister(vm->registers, PC));
                GetRegister(vm->registers, PC).as_ptr = subroutine->entry;

                PushValue(vm, GetRegister(vm->registers, FP));
                GetRegister(vm->registers, FP) = GetRegister(vm->registers, SP);
                GetRegister(vm->registers, SP).as_value_ptr += subroutine->frameSize;

                PushValue(vm, (CTValue) {.as_ptr = (u8*) vm->executionContext.currentSubroutine});
                vm->executionContext.currentSubroutine = subroutine;
            }

            ret_none();
        }

        inst(RSR, 0) {
            GetRegister(vm->registers, PC) = PopValue(vm);

            ret_none();
        }

        inst(RET, 0) {
            if (vm->executionContext.currentSubroutine == null) {
                Exit(GetRegister(vm->registers, ACC).as_int);
            }

            regalloc(subroutine) = (CTSubroutine*) PopValue(vm).as_ptr;

            GetRegister(vm->registers, SP) = GetRegister(vm->registers, FP);
            GetRegister(vm->registers, FP) = PopValue(vm);

            GetRegister(vm->registers, PC) = PopValue(vm);

            vm->executionContext.currentSubroutine = subroutine;

            ret_none();
        }

        inst(LDI_8, 2, (dest, immediate)) {
            op(dest);
            op(immediate);

            GetRegister(vm->registers, dest).as_byte = immediate;

            ret_none();
        }

        inst(LDI_16, 3, (dest, immediate)) {
            op(dest);
            op2(immediate);

            GetRegister(vm->registers, dest).as_short = immediate;

            ret_none();
        }

        inst(LDI_32, 5, (dest, immediate)) {
            op(dest);
            op4(immediate);

            GetRegister(vm->registers, dest).as_int = immediate;

            ret_none();
        }

        inst(LDI_64, 9, (dest, immediate)) {
            op(dest);
            op8(immediate);

            GetRegister(vm->registers, dest).as_long = immediate;

            ret_none();
        }

        inst(WIDE, 0) {
            u8 nextOpcode = *(GetRegister(vm->registers, PC).as_ptr++);
            ExecuteInstruction(vm, nextOpcode, true);

            ret_none();
        }

        default: {
            // unknown opcode or unimplemented
            printf("%u\n", opcode);
            Exit(1);
        }
    }
}
