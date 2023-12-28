#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

enum { BUFLEN = 10000 };

int
main(int argc, char **argv) {
    if (argc != 3) {
        return ENOENT;
    }

    char *buf = calloc(BUFLEN, 1);
    if (buf == NULL) {
        return ENOMEM;
    }

    int in = open(argv[1], O_RDONLY);
    if (in == -1) {
        perror(NULL);
        return errno;
    }
    
    int out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (out == -1) {
        perror(NULL);
        int err = errno;
        close(in);
        return err;
    }

    ssize_t c;
    while ((c = read(in, buf, BUFLEN)) == BUFLEN) {
        ssize_t wrote = write(out, buf, c);
        if (wrote == -1) {
            perror(NULL);
            int err = errno;
            close(in);
            close(out);
            remove(argv[2]);
            return err;
        }
    } 
    if (c == -1) {
        perror(NULL);
        int err = errno;
        close(in);
        close(out);
        remove(argv[2]);
        return err;
    } else if (c > 0) {
        ssize_t wrote = write(out, buf, c);
        if (wrote == -1) {
            perror(NULL);
            int err = errno;
            close(in);
            close(out);
            remove(argv[2]);
            return err;
        }
    }
    if(close(out) == -1) {
        perror(NULL);
        int err = errno;
        close(in);
        remove(argv[2]);
        return err;
    }
    if(close(in) == -1) {
        perror(NULL);
        int err = errno;
        remove(argv[1]);
        return err;
    }
    if(remove(argv[1]) == -1) {
        perror(NULL);
        return errno;
    }
    return 0;
}
