#ifndef CTVM_FILEUTILS_H
#define CTVM_FILEUTILS_H

#include <stdio.h>
#include "defines.h"
#include "core/string.h"

#define AutoClose attribute(cleanup(CleanupClose))

typedef FILE* File;

File Open(CTString* path, char* mode);

File OpenC(char* path, char* mode);

void Close(File file);

void CleanupClose(File* filep);

i64 GetFileSize(File file);

u8* GetFileBytes(File file, u64* sizePtr);

void WriteByte(File file, u8 b);

void WriteShort(File file, u16 s);

void WriteInt(File file, u32 i);

void WriteLong(File file, u64 l);

void WriteString(File file, CTString* string);

#endif //CTVM_FILEUTILS_H
