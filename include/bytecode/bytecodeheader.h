#ifndef BYTECODEHEADER_H
#define BYTECODEHEADER_H

#include "core/string.h"

typedef struct CTBytecodeHeader {
    u16 arch;
    CTString name;
} CTBytecodeHeader;

#endif //BYTECODEHEADER_H
