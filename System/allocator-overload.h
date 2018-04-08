#pragma once

#ifdef _DEBUG 
#define ACTIVATE_MEMORY_MANAGER
#endif

#define ACTIVATE_MEMORY_MANAGER

#include "mem-undef.h"

#ifdef ACTIVATE_MEMORY_MANAGER


// Posible allocation/deallocation types.  
typedef char ALLOC_TYPE;
const ALLOC_TYPE MM_UNKNOWN        = 0;  // Declared as characters to minimize memory footprint, 
const ALLOC_TYPE MM_NEW            = 1;  //   char = 1 byte
const ALLOC_TYPE MM_NEW_ARRAY      = 2;  //   enum types = int = 32 bits = 8 bytes on standard machines
const ALLOC_TYPE MM_MALLOC         = 3;
const ALLOC_TYPE MM_CALLOC         = 4;
const ALLOC_TYPE MM_REALLOC        = 5;
const ALLOC_TYPE MM_DELETE         = 6;
const ALLOC_TYPE MM_DELETE_ARRAY   = 7;
const ALLOC_TYPE MM_FREE           = 8;

void *AllocateMemory(const char *file, int line, size_t size, ALLOC_TYPE type, void *address) 
{
	if (type == MM_REALLOC)
	{
	}
	else if (type == MM_CALLOC)
	{
	}

	return R_MEMORYPOOL().Alloc(size);
}

void deAllocateMemory(void *address, ALLOC_TYPE type) 
{
	R_MEMORYPOOL().Free(address);
}

inline void* operator new(size_t size)    { return R_MEMORYPOOL().Alloc(size); }
inline void* operator new[](size_t size)  { return R_MEMORYPOOL().Alloc(size); }
inline void* operator new(size_t size, const char *file, int line) 
{
	return R_MEMORYPOOL().Alloc(size);
}

inline void* operator new[](size_t size, const char *file, int line)
{
	return R_MEMORYPOOL().Alloc(size);
}

/*
inline void operator delete(void *address)
{
	if (!address) 
		return;  // ANSI states that delete will allow NULL pointers.

	R_MEMORYPOOL().Free(address);
}

inline void operator delete[](void *address)
{
	if (!address) 
		return;  // ANSI states that delete will allow NULL pointers.

	R_MEMORYPOOL().Free(address);
}

inline void operator delete(void *address, const char *file, int line)   { R_MEMORYPOOL().Free(address); }
inline void operator delete[](void *address, const char *file, int line) { R_MEMORYPOOL().Free(address); }
*/
#include "mem-redef.h"

#endif