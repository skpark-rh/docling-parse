#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

static void* (*real_malloc)(size_t) = NULL;
static size_t threhold = 10 * 1024 * 1024; // 10MB

void* malloc(size_t size) {
    if (!real_malloc)
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    
    void* ptr = real_malloc(size);

    if (size > threhold) {
        fprintf(stderr, ">>> BIG ALLOC: %zu bytes (%zu MB) at %p\n",
                size, size / (1024 * 1024), ptr);
        void* bt[16];
        int n = backtrace(bt, 16);
        backtrace_symbols_fd(bt, n, 2);
    }
    return ptr;
}