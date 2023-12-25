#include <stdio.h>
#include <string.h>
#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

enum { GUESS_LEN = 20 };

int
main(void) {
    setlocale(LC_ALL, "");
    bindtextdomain("guess", ".");
    textdomain("guess");
    printf(_("Guess a number from 1 to 100\n"));
    int l = 1, r = 100;
    while (l != r) {
        int cur = (l + r) / 2;
        char guess[GUESS_LEN];
        printf(_("Is it greater than %d? (YES/NO) "), cur);
        int unused = scanf("%19s", guess);
        if (strcmp(guess, _("YES")) == 0) {
            l = cur + 1;
        } else {
            r = cur;
        }
    }
    printf(_("You guessed %d!\n"), l);
    return 0;
}
