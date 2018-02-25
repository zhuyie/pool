#include "pool.h"
#include <stdlib.h>

typedef struct MemBlock
{
    size_t size;
    struct MemBlock* next;
} MemBlock;

typedef struct MemClass
{
    size_t size;
    size_t length;
    size_t max_length;
    MemBlock *free_block;
} MemClass;

static MemClass mem_classes[] =
{
    { 64,   0, 256, NULL },
    { 128,  0, 256, NULL },
    { 256,  0, 128, NULL },
    { 512,  0, 128, NULL },
    { 1024, 0, 64,  NULL },
    { 2048, 0, 64,  NULL }
};

static inline MemClass* get_class(size_t size)
{
    for (size_t i = 0; i < sizeof(mem_classes) / sizeof(mem_classes[0]); i++)
    {
        if (mem_classes[i].size >= size)
            return &mem_classes[i];
    }
    return NULL;
}

PoolStats stats;

void pool_init()
{
}

void pool_uninit()
{
    for (size_t i = 0; i < sizeof(mem_classes) / sizeof(mem_classes[0]); i++)
    {
        MemClass *c = &mem_classes[i];
        MemBlock *p = c->free_block;
        while (p)
        {
            MemBlock *next = p->next;
            free(p);
            p = next;
        }
        c->free_block = NULL;
        c->length = 0;
    }
}

void* pool_alloc(size_t size)
{
    stats.alloc_times++;
    MemBlock *block = NULL;
    size_t block_size = size;
    MemClass *class = get_class(size);
    if (class != NULL)
    {
        if (class->free_block != NULL)
        {
            block = class->free_block;
            class->free_block = block->next;
            class->length--;
            block++;
        }
        else
        {
            block_size = class->size;
        }
    }
    if (block == NULL)
    {
        stats.real_alloc_times++;
        block = (MemBlock*)malloc(sizeof(MemBlock) + block_size);
        if (block != NULL)
        {
            block->size = size;
            block->next = NULL;
            block++;
        }
    }
    return block;
}

void pool_free(void * memory)
{
    stats.free_times++;
    MemBlock *block = (MemBlock*)memory - 1;
    MemClass *class = get_class(block->size);
    if (class != NULL)
    {
        if (class->length < class->max_length)
        {
            block->next = class->free_block;
            class->free_block = block;
            class->length++;
            block = NULL;
        }
    }
    if (block != NULL)
    {
        stats.real_free_times++;
        free(block);
    }
}

void pool_stats(PoolStats *s)
{
    *s = stats;
}
