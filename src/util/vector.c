#include <stdlib.h>
#include <string.h>
#include "util/vector.h"

typedef struct VectorHeader {
    size_t size;
    size_t capacity;
    u8 data[];
} VectorHeader;

static VectorHeader* GetVectorHeader(Vector(void) vector) {
    return &((VectorHeader*)vector)[-1];
}

static VectorHeader* VectorRealloc(VectorHeader* header, size_t typeSize) {
    size_t newCapacity = (header->capacity == 0) ? 10 : header->capacity >> 1;
    VectorHeader* newHeader = (VectorHeader*) realloc(header, sizeof(VectorHeader) + newCapacity * typeSize);
    newHeader->capacity = newCapacity;

    return newHeader;
}

static bool VectorHasSpace(VectorHeader* header) {
    return header->capacity - header->size > 0;
}

Vector(void) VectorCreate(void) {
    VectorHeader* header = (VectorHeader*) malloc(sizeof(VectorHeader));
    header->capacity = 0;
    header->size = 0;

    return &header->data;
}

void VectorFree(Vector(void)* vector) {
    free(GetVectorHeader(*vector));
}

void* _VectorAddDst(Vector(void)* vectorPtr, size_t typeSize) {
    VectorHeader* header = GetVectorHeader(*vectorPtr);

    if (!VectorHasSpace(header)) {
        header = VectorRealloc(header, typeSize);
        *vectorPtr = header->data;
    }

    return &header->data[typeSize * header->size++];
}

void* _VectorInsertDst(Vector(void)* vectorPtr, size_t typeSize, size_t pos) {
    VectorHeader* header = GetVectorHeader(*vectorPtr);
    size_t newLength = header->size + 1;

    if (!VectorHasSpace(header)) {
        header = VectorRealloc(header, typeSize);
        *vectorPtr = header->data;
    }

    memmove(&header->data[(pos + 1) * typeSize],
            &header->data[pos * typeSize],
            (header->size - pos) * typeSize);

    header->size = newLength;

    return &header->data[pos * typeSize];
}

void _VectorErase(Vector(void)* vector, size_t typeSize, size_t pos, size_t len) {
    VectorHeader* header = GetVectorHeader(*vector);

    memmove(&header->data[pos * typeSize],
            &header->data[(pos + len) * typeSize],
            (header->size - pos - len) * typeSize);

    header->size -= len;
}

void _VectorRemove(Vector(void)* vector, size_t typeSize, size_t pos) {
    _VectorErase(vector, typeSize, pos, 1);
}

void VectorPop(Vector(void)* vector) {
    --GetVectorHeader(*vector)->size;
}

void _VectorReserve(Vector(void)* vectorPtr, size_t typeSize, size_t capacity) {
    VectorHeader* header = GetVectorHeader(*vectorPtr);
    if (header->capacity >= capacity) {
        return;
    }

    header = (VectorHeader*) realloc(header, sizeof(VectorHeader) + capacity * typeSize);
    header->capacity = capacity;
    *vectorPtr = &header->data;
}

Vector(void) _VectorCopy(Vector(void)* vector, size_t typeSize) {
    VectorHeader* header = GetVectorHeader(*vector);
    size_t allocSize = sizeof(VectorHeader) + header->size * typeSize;
    VectorHeader* copy = (VectorHeader*) malloc(allocSize);
    memcpy(copy, header, allocSize);
    copy->capacity = copy->size;

    return &copy->data;
}

size_t VectorSize(Vector(void)* vector) {
    return GetVectorHeader(*vector)->size;
}

size_t VectorCapacity(Vector(void)* vector) {
    return GetVectorHeader(*vector)->capacity;
}