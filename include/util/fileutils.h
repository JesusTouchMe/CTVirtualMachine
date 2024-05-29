#ifndef CTVM_FILEUTILS_H
#define CTVM_FILEUTILS_H

#include <stdio.h>
#include "defines.h"
#include "core/string.h"

FILE* Open(CTString* path, char* mode);

FILE* OpenC(char* path, char* mode);

i64 GetFileSize(FILE* file);

u8* GetFileBytes(FILE* file, u64* sizePtr);

#endif //CTVM_FILEUTILS_H
