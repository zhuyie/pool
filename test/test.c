#include <pool.h>

int main(int argc, char** argv)
{
    void *p;

    pool_init();

    p = pool_alloc(100);
    pool_free(p);

    p = pool_alloc(65536);
    pool_free(p);

    pool_uninit();
    return 0;
}
