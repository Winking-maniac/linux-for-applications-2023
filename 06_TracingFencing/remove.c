#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <string.h>

typedef int (*remove_type)(const char *);

int remove(const char *pathname) {
    char protected[] = "PROTECT";
    if (strstr(pathname, protected) != NULL) {
        errno = EACCES;
        return -1;
    }
    int res;
    res = ((remove_type)(dlsym(RTLD_NEXT, "remove")))(pathname);
    return res;
}
