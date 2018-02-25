#ifndef POOL_H_INCLUDED
#define POOL_H_INCLUDED

#include <stddef.h>

void pool_init();

void pool_uninit();

void* pool_alloc(size_t size);

void pool_free(void* memory);

typedef struct PoolStats
{
    size_t alloc_times;
    size_t free_times;
    size_t real_alloc_times;
    size_t real_free_times;
} PoolStats;

void pool_stats(PoolStats *stats);

#endif
