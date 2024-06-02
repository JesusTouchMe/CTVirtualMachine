#include <malloc.h>
#include "util/fileutils.h"

File Open(CTString* path, char* mode) {

}

File OpenC(char* path, char* mode) {
    File file;
    errno_t err = fopen_s(&file, path, mode);

    if (err != 0) {
        return null;
    }

    return file;
}

void Close(File file) {
    fclose(file);
}

void CleanupClose(File* filep) {
    Close(*filep);
}

i64 GetFileSize(File file) {
    fseek(file, 0, SEEK_END);
    i64 size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

u8* GetFileBytes(File file, u64* sizePtr) {
    i64 size = GetFileSize(file);
    u8* buf = (u8*) malloc(size);
    fread(buf, 1, size, file);

    if (sizePtr) *sizePtr = size;
    return buf;
}

void WriteByte(File file, u8 b) {
    fputc(b, file);
}

void WriteShort(File file, u16 s) {
    fputc((u8) (s >> 8), file);
    fputc((u8) s, file);
}

void WriteInt(File file, u32 i) {
    fputc((u8) (i >> 24), file);
    fputc((u8) (i >> 16), file);
    fputc((u8) (i >> 8), file);
    fputc((u8) i, file);
}

void WriteLong(File file, u64 l) {
    fputc((u8) (l >> 56), file);
    fputc((u8) (l >> 48), file);
    fputc((u8) (l >> 40), file);
    fputc((u8) (l >> 32), file);
    fputc((u8) (l >> 24), file);
    fputc((u8) (l >> 16), file);
    fputc((u8) (l >> 8), file);
    fputc((u8) l, file);
}

void WriteString(File file, CTString* string) {
    fwrite(string->data, 1, string->length, file);
}
