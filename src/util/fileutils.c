#include <malloc.h>
#include "util/fileutils.h"

FILE* Open(CTString* path, char* mode) {

}

FILE* OpenC(char* path, char* mode) {
    FILE* file;
    errno_t err = fopen_s(&file, path, mode);

    if (err != 0) {
        return null;
    }

    return file;
}

i64 GetFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    i64 size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

u8* GetFileBytes(FILE* file, u64* sizePtr) {
    i64 size = GetFileSize(file);
    u8* buf = (u8*) malloc(size);
    fread(buf, 1, size, file);

    if (sizePtr) *sizePtr = size;
    return buf;
}