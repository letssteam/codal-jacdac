// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include <cstdlib>

#include "jd_protocol.h"

void jd_alloc_stack_check(void) {}

void jd_alloc_init(void) {}

void* jd_alloc(uint32_t size)
{
    return calloc(size, 1);
}

void* jd_alloc_emergency_area(uint32_t size)
{
    return calloc(size, 1);
}

// uint32_t jd_available_memory()
// {
//     return HEAP_END - (uint32_t)aptr;
// }

void jd_free(void* ptr)
{
    free(ptr);
}
