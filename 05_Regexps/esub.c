#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define MAXGR 10

int main(int argc, char *argv[]) {
    char *buf;
    size_t len = 0;
    int chars;
    regex_t regex;
    regmatch_t bags[MAXGR];


    int err = regcomp(&regex, argv[1], REG_EXTENDED);
    if (err) {
        int err_buf_len = regerror(err, &regex, 0, 0);
        char *buf = malloc(sizeof(*buf) * err_buf_len);
        regerror(err, &regex, buf, err_buf_len);
        fprintf(stderr, "%s\n", buf);
        free(buf);
        exit(1);
    }

    // Check for subs
    int max_sub = 0;
    for(int i = 0; argv[2][i] && argv[2][i+1]; ++i)
        if (argv[2][i] == '\\' && argv[2][i+1] > '0' && argv[2][i+1] <= '9')
            max_sub = (max_sub > argv[2][i+1] -'0') ? max_sub : (argv[2][i+1] - '0');
    if (max_sub > regex.re_nsub) {
        fprintf(stderr, "Invalid substitution: unsufficient capture groups in regex: %d\n", max_sub);
        exit(1);
    }

    err = regexec(&regex, argv[3], MAXGR, bags, REG_EXTENDED);
    if (err) {
        int err_buf_len = regerror(err, &regex, 0, 0);
        char *buf = malloc(sizeof(*buf) * err_buf_len);
        regerror(err, &regex, buf, err_buf_len);
        fprintf(stderr, "%s\n", buf);
        free(buf);
        exit(1);
    }
    
    // printf("%d %d\n\n", bags[0].rm_so, bags[0].rm_eo); 
    printf("%.*s", bags[0].rm_so, argv[3]);
    
    int i;
    for(i = 0; argv[2][i] && argv[2][i+1]; ++i) {
        if (argv[2][i] == '\\' && argv[2][i+1] > '0' && argv[2][i+1] <= '9') {
            printf("%.*s", bags[argv[2][i+1] - '0'].rm_eo - bags[argv[2][i+1] - '0'].rm_so, argv[3] + bags[argv[2][i+1] - '0'].rm_so);
            i++;
        } else if (argv[2][i] == '\\' && argv[2][i+1] == '\\') {
            printf("\\"); i++;
        } else {
            printf("%c", argv[2][i]);
        }
    }
    if (argv[2][i]) printf("%c", argv[2][i]);

    
    printf("%s\n", argv[3] + bags[0].rm_eo);
    /* for (buf = NULL; (chars = getline(&buf, &len, stdin)) != -1; buf = NULL) {
        buf[chars - 1] = 0;
        if (regexec(&regex, buf, MAXGR, bags, 0) == 0) {
            puts(buf);
            for(int i=0; i<MAXGR && bags[i].rm_so>=0; i++) {
                int b = bags[i].rm_so, e = bags[i].rm_eo;
                printf("%d %d/%d\t%.*s\n", i, b, e, e - b, buf + b);
            }
        }
        free(buf);
    } */ 
    regfree(&regex);
    return 0;
}
