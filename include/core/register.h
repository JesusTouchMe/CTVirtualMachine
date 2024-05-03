#ifndef REGISTER_H
#define REGISTER_H

#include "core/value.h"

#define R0 1
#define R1 2
#define R2 3
#define R3 4
#define R4 5
#define R5 6
#define R6 7
#define R7 8
#define R8 9
#define R9 10
#define R10 11
#define R11 12
#define R12 13
#define R13 14
#define R14 15
#define R15 16
#define F0 17
#define F1 18
#define F2 19
#define F3 20
#define F4 21
#define F5 22
#define F6 23
#define F7 24
#define PC 25
#define ACC 0
#define SP 26
#define FP 27
#define FLAG 28

#define REGISTER_ID_LOW 0
#define REGISTER_ID_HIGH 28

#define GetRegister(registers, id) (registers[id])
#define IsValidRegisterID(id) (id >= REGISTER_ID_LOW && id <= REGISTER_ID_HIGH)

typedef CTValue registers_t[29];

#endif //REGISTER_H
