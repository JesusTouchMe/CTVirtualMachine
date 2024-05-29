#ifndef CTVM_VECTOR_H
#define CTVM_VECTOR_H

#include "defines.h"

#define Vector(type) type*

#if _MSC_VER == 0 || __STDC_VERSION__ >= 202311L || defined __cpp_decltype

#define VectorAddDst(vec_addr)\
	((typeof(*vec_addr))(\
	    _VectorAddDst((Vector(void)*)vec_addr, sizeof(**(vec_addr)))\
	))
#define VectorInsertDst(vec_addr, pos)\
	((typeof(*vec_addr))(\
	    _VectorInsertDst((Vector(void)*)(vec_addr), sizeof(**(vec_addr)), pos)))

#define VectorAdd(vec_addr, value)\
	(*VectorAddDst(vec_addr) = value)
#define VectorInsert(vec_addr, pos, value)\
	(*VectorInsertDst(vec_addr, pos) = value)

#else

#define VectorAddDst(vec_addr, type)\
	((type*)_VectorAddDst((Vector(void)*)vec_addr, sizeof(type)))
#define VectorInsertDst(vec_addr, type, pos)\
	((type*)_VectorInsertDst((Vector(void)*)vec_addr, sizeof(type), pos))

#define VectorAdd(vec_addr, type, value)\
	(*VectorAddDst(vec_addr, type) = value)
#define VectorInsert(vec_addr, type, pos, value)\
	(*VectorInsertDst(vec_addr, type, pos) = value)

#endif

#define VectorErase(vec_addr, pos, len)\
	(_VectorErase((Vector(void)*)(vec_addr), sizeof(**(vec_addr)), pos, len))
#define VectorRemove(vec_addr, pos)\
	(_VectorRemove((Vector(void))(vec_addr), sizeof(**(vec_addr)), pos))

#define VectorReserve(vec_addr, capacity)\
	(_vector_reserve((Vector(void)*)(vec_addr), sizeof(**(vec_addr)), capacity))

#define VectorCopy(vec_addr)\
	(_VectorCopy((Vector(void))(vec_addr), sizeof(**(vec_addr))))

Vector(void) VectorCreate(void);

void VectorFree(Vector(void)* vector);

void* _VectorAddDst(Vector(void)* vectorPtr, size_t typeSize);

void* _VectorInsertDst(Vector(void)* vectorPtr, size_t typeSize, size_t pos);

void _VectorErase(Vector(void)* vector, size_t typeSize, size_t pos, size_t len);

void _VectorRemove(Vector(void)* vector, size_t typeSize, size_t pos);

void VectorPop(Vector(void)* vector);

void _VectorReserve(Vector(void)* vectorPtr, size_t typeSize, size_t capacity);

Vector(void) _VectorCopy(Vector(void)* vector, size_t typeSize);

size_t VectorSize(Vector(void)* vector);

size_t VectorCapacity(Vector(void)* vector);

#endif //CTVM_VECTOR_H
