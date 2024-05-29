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
    u8 t1 = *operands++; \
    u8 t2 = *operands++; \
    u8 t3 = *operands++; \
    u8 t4 = *operands++; \
    u8 t5 = *operands++; \
    u8 t6 = *operands++; \
    u8 t7 = *operands++; \
    u8 t8 = *operands++; \
    (u32) ((t1 << 56) | (t2 << 48) | (t3 << 40) | (t4 << 32) | (t5 << 24) | (t6 << 16) | (t7 << 8) | t8); \
})

#define op(name) u8 name = _op1()
#define op2(name) u16 name = _op2()
#define op4(name) u32 name = _op4()
#define op8(name) u64 name = _op8()
#define wide_op(name) u16 name = (wide) ? _op2() : _op1()

// utility macros
#define ret_none(...) return CT_null
#define regalloc(name) register const __auto_type name

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
            CTValue reg1Value = GetRegister(vm->registers, reg1);

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
    }
}
