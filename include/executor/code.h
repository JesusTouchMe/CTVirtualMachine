#ifndef CTVM_CODE_H
#define CTVM_CODE_H

#include "executor/context.h"

enum : u8 {
    NOP = 0x00,
    REPEAT = 0x01,

    PUSH = 0x02,
    PUSHVAR = 0x03,
    PUSHGLOBAL = 0x04,

    POP = 0x05,
    POPVAR = 0x06,
    POPGLOBAL = 0x07,
    DUP = 0x08,

    MOVE = 0x09,
    SWAP = 0x0A,

    NEW = 0x0B,
    NEWI = 0x0C,
    NEWARRAY = 0x0D,
    NEWARRAYI = 0x0E,
    NEWARRAYII = 0x0F,
    DELETE = 0x10,
    DELETEARRAY = 0x11,
    ISPATTERN = 0x4B,

    READ = 0x12,
    AREAD = 0x13,
    AREADI = 0x14,
    WRITE = 0x15,
    AWRITE = 0x16,
    AWRITEI = 0x17,

    GLOBALLOAD = 0x18,
    GLOBALSTORE = 0x19,
    LOAD = 0x1A,
    STORE = 0x1B,
    CONSTLOAD = 0x42,

    ADD = 0x1C,
    FADD = 0x1D,
    SUB = 0x1E,
    FSUB = 0x1F,
    MUL = 0x20,
    FMUL = 0x21,
    DIV = 0x22,
    FDIV = 0x23,

    AND = 0x24,
    OR = 0x25,
    XOR = 0x26,
    SHL = 0x27,
    SHR = 0x28,

    INC = 0x29,
    DEC = 0x2A,

    NOT = 0x2B,
    NEG = 0x2C,

    TEST = 0x2D,

    JMP = 0x2E,
    JMP_REL8 = 0x2F,
    JMP_REL32 = 0x30,
    JEQ = 0x31,
    JEQ_REL8 = 0x32,
    JEQ_REL32 = 0x33,
    JNE = 0x34,
    JNE_REL8 = 0x35,
    JNE_REL32 = 0x36,
    JLT = 0x37,
    JLT_REL8 = 0x38,
    JLT_REL32 = 0x39,
    JGT = 0x3A,
    JGT_REL8 = 0x3B,
    JGT_REL32 = 0x3C,
    JLE = 0x3D,
    JLE_REL8 = 0x3E,
    JLE_REL32 = 0x3F,
    JGE = 0x40,
    JGE_REL8 = 0x41,
    JGE_REL32 = 0x43,

    GOTO = 0x44,
    GOTO_W = 0x45,

    ENTER = 0x46,
    ENTER_0 = 0x47,
    LEAVE = 0x48,
    JSR = 0x49,
    JSR_W = 0x4A,
    CALL = 0x4C,
    RSR = 0x4D,
    RET = 0x4E,

    LDI_8 = 0x4F,
    LDI_16 = 0x50,
    LDI_32 = 0x51,
    LDI_64 = 0x52,

    WIDE = 0x53,
};

/**
 * Assuming the whole provided VM is initialized and has entered, this will begin executing code until it's cut off
 */
void DoCodeExecution(CTVM* vm);

/**
 * Runs a single cycle (instruction) of the executor
 */
void CycleCodeExecutor(CTVM* vm);

CTValue ExecuteInstruction(CTVM* vm, u8 opcode, bool wide);

#endif //CTVM_CODE_H
