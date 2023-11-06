#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <rhash.h>

#ifdef FORCE_NO_READLINE
#undef HAVE_LIBREADLINE
#endif

#ifdef HAVE_LIBREADLINE
#include <readline/readline.h>
#endif

typedef struct ParsedCmd {
    int                        is_valid;
    int                        is_file;
    int                        arg[2];     // start and len of argument
    enum rhash_ids             algorithm;
    enum rhash_print_sum_flags format;
} ParsedCmd;

const char algorithms[6][5] = { "sha1", "SHA1", "md5", "MD5", "tth", "TTH" };
const enum rhash_ids alg_ids[] = { RHASH_SHA1, RHASH_MD5, RHASH_TTH };

static int
read(char **cmd) {
#ifndef HAVE_LIBREADLINE
    printf("READLINE >> "); 
    size_t unused = 0;
    int n = getline(cmd, &unused, stdin);
    if (n == -1) {
        if (errno) {
            perror(NULL);
            free(*cmd);
            return 1;
        } else {
            printf("\n");
            exit(0);
        }
    }
    return 0;
#else
    *cmd = readline(">> ");
    if (*cmd == NULL) {
        printf("\n");
        exit(0);
    }
    return 0;
#endif
} 

static ParsedCmd
parse(char *cmd) {
    ParsedCmd res;
    res.is_valid = 0;
    
    for (int i = 0; i < 6; ++i) {
        if (strncmp(cmd, algorithms[i], strlen(algorithms[i])) == 0) {
            res.is_valid = 1;
            res.algorithm = alg_ids[i/2];
            res.format = i % 2 == 0 ? RHPR_HEX : RHPR_BASE64;
        }
    }
    if (!res.is_valid) return res;

    int i = 0;
    for(i = 0; cmd[i] != 0 && cmd[i] != ' '; ++i) ;
    for(; cmd[i] != 0 && cmd[i] == ' '; ++i) ;
    if (cmd[i] == 0) {
        res.is_valid = 0;
        return res;
    }
    res.is_file = cmd[i] == '"' ? 0 : 1;
    res.arg[0] = cmd[i] == '"' ? i + 1 : i;
    for(; cmd[i] != 0 && cmd[i] != ' ' && cmd[i] != '\n'; ++i) ;
    res.arg[1] = i - res.arg[0];
    return res;
}           
            
static int
eval(char *cmd, char **res) {
    ParsedCmd c;
    c = parse(cmd);
    if (!c.is_valid) {
        fprintf(stderr, "Invalid command syntax\n");
        free(cmd);
        return 1;
    }
    
    char digest[64];
    *res = malloc(130 * sizeof(**res));

    if (c.is_file) {
        cmd[c.arg[0] + c.arg[1]] = 0;
        int rc = rhash_file(c.algorithm, cmd + c.arg[0], digest);
        if(rc < 0) {
            fprintf(stderr, "LibRHash error: %s: %s\n", cmd + c.arg[0], strerror(errno));
            free(cmd);
            free(*res);
            return 1;
        }
        rhash_print_bytes(*res, digest, rhash_get_digest_size(c.algorithm), c.format);
    } else {
        int rc = rhash_msg(c.algorithm, cmd + c.arg[0], c.arg[1], digest);
        if(rc < 0) {
            fprintf(stderr, "message digest calculation error\n");
            free(cmd);
            free(*res);
            return 1;
        }
        rhash_print_bytes(*res, digest, rhash_get_digest_size(c.algorithm), c.format);
    }

    free(cmd);
    return 0;
}

static void
print(char *res) {
    printf("%.130s\n", res);
    free(res);
}


__attribute__(( noreturn ))
int
main(void) {
    rhash_library_init();
    char *cmd = NULL, *res;
    int rc;
    while(1) {
        rc = read(&cmd);
        if (rc) continue;
        
        rc = eval(cmd, &res);
        if (rc) continue;

        print(res);
    }
}
