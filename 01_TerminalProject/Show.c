#include<ncurses.h>
#include<stdlib.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

enum
{
    TEXT_LEFT_MARGIN = 2,
    TEXT_RIGHT_MARGIN = 2,
    TEXT_UP_MARGIN = 2,
    TEXT_DOWN_MARGIN = 1,
    BORDER_WIDTH = 1
};

#define TEXT_LINES  (LINES - (int)TEXT_UP_MARGIN - (int)TEXT_DOWN_MARGIN - 2 * BORDER_WIDTH)
#define TEXT_COLS  (COLS - (int)TEXT_LEFT_MARGIN - (int)TEXT_RIGHT_MARGIN - 2 * BORDER_WIDTH)



int
main(int argc, char**argv)
{
    // Check argument was provided
    if (argc != 2) {
        fprintf(stderr, "Arguments got: %d, expected: 1\n", argc - 1);
        exit(1);
    }
   
    // Open file and check its readability
    FILE *fd; 
    fd = fopen(argv[1], "r");
    if (fd == NULL) {
        perror(argv[1]);
        exit(2);
    }
    
    // Get file size
    fseek(fd, 0L, SEEK_END);
    long sz = ftell(fd);
    rewind(fd);

    // Trying allocate memory for file 
    char *file = malloc(sz * sizeof(*file) + 1);
    if (file  == NULL) {
        perror("malloc");
        exit(3);
    }

    // Reading file
    size_t read_bytes = fread(file, 1, sz, fd);
    if (read_bytes != sz) {
        fprintf(stderr, "fread: %ld bytes read instead of %ld\n", read_bytes, sz);
        exit(4);
    }
    file[sz] = 0;
    
    // Calculate LINES and ROWS for pad
    int file_lines = 0;
    int max_line = 0;
    int prev_line = 0;
    int is_binary = 0;
    
    for (size_t i = 0; i < sz; ++i) {
        // Check chars not to be control chars(except \t) and \n)
        if (file[i] >= 128 || (file[i] < 32 && file[i] != '\n' && file[i] != '\t')) {
            is_binary = 1;
        }
        if (file[i] == '\n') {
            file_lines++;
            max_line = max(max_line, i - prev_line);
            prev_line = i+1;
        }
    }
    file_lines++;
        
    int number_len = 0;
    for (int i = file_lines+1; i != 0; i /= 10) number_len++;
        
    // Exit if binary
    if (is_binary == 1) {
        fprintf(stderr, "File seems to be binary, exiting");
        exit(5);
    }
    
    // Initialize all screen attributes 
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // Check color support and initialize colors
    if (has_colors() == FALSE || can_change_color() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
    	exit(6);
    } 
    init_color(COLOR_BLACK, 0, 0, 0); //BLACK
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    
    // Initialize main window, print box and file name
    WINDOW *text_win = newwin(TEXT_LINES + 2 * BORDER_WIDTH, TEXT_COLS + 2 * BORDER_WIDTH, TEXT_UP_MARGIN, TEXT_LEFT_MARGIN);
    box(text_win, 0, 0);
    wattr_set(stdscr, WA_BOLD, COLOR_PAIR(2), NULL);
   
    wprintw(stdscr, "%s, %d lines, %ld bytes", argv[1], file_lines, sz);
    wrefresh(stdscr);
    wrefresh(text_win);
    
    // Create text pad and fill it with text
    WINDOW *pad = newpad(file_lines, max_line);
    
    wattr_on(pad, COLOR_PAIR(2), NULL);
    prev_line = 0;    
    int cur_line = 0;

    for (size_t i = 0; i < sz; ++i) {
        if (file[i] == '\n') {
            // Print line start
            // (highlighter can be possibly called there)
            mvwprintw(pad, cur_line, 0, "%.*s", i - prev_line, file + prev_line);
            wmove(pad, cur_line, number_len + 1 + i - prev_line);
            wclrtoeol(pad);
            // Print line end
            cur_line++; 
            prev_line = i+1;
        }
    }
    
    // Create line numbers pad and fill it with text
    WINDOW *line_num_pad = newpad(file_lines, number_len);
    
    wattr_on(line_num_pad, WA_BOLD | COLOR_PAIR(1), NULL);
    
    for (size_t i = 0; i < file_lines; ++i) 
        mvwprintw(line_num_pad, i, 0, "%*d ", number_len, i+1);
    
    // Open first view on file
    prefresh(pad, 0, 0, TEXT_UP_MARGIN + BORDER_WIDTH, TEXT_LEFT_MARGIN + BORDER_WIDTH + number_len + 2, TEXT_UP_MARGIN + TEXT_LINES, TEXT_LEFT_MARGIN + TEXT_COLS);
    prefresh(line_num_pad, 0, 0, TEXT_UP_MARGIN + BORDER_WIDTH, TEXT_LEFT_MARGIN + BORDER_WIDTH + 1, TEXT_UP_MARGIN + TEXT_LINES, TEXT_LEFT_MARGIN + number_len + BORDER_WIDTH);
    
    // Initialize current position and maximums(minimums are 0)
    int y = 0;
    int x = 0;
    int max_x = max_line - TEXT_COLS;
    int max_y = file_lines - TEXT_LINES; 

    // Start endless loop of scrolling
    while(1) {
        int c = wgetch(stdscr);
        switch(c) {
            case KEY_RIGHT:
                x = min(x + 1, max_x);
                goto UPDATE_TEXT;
            case KEY_LEFT: 
                x = max(x - 1, 0);
                goto UPDATE_TEXT;
            case KEY_UP: 
                y = max(y - 1, 0);
                goto UPDATE_BOTH;
            case KEY_DOWN:
            case ' ':
                y = min(y + 1, max_y);
                goto UPDATE_BOTH;
            case KEY_NPAGE:
                y = min(y + TEXT_LINES - 2, max_y);
                goto UPDATE_BOTH;
            case KEY_PPAGE:
                y = max(y - TEXT_LINES + 2, 0);
                goto UPDATE_BOTH;
            case 27:
                endwin();
                return 0;
            default:
                break;
            UPDATE_BOTH:
                prefresh(line_num_pad, y, 0, TEXT_UP_MARGIN + BORDER_WIDTH, TEXT_LEFT_MARGIN + BORDER_WIDTH + 1, TEXT_UP_MARGIN + TEXT_LINES, TEXT_LEFT_MARGIN + number_len + BORDER_WIDTH);
            UPDATE_TEXT:
                prefresh(pad, y, x, TEXT_UP_MARGIN + BORDER_WIDTH, TEXT_LEFT_MARGIN + BORDER_WIDTH + number_len + 2, TEXT_UP_MARGIN + TEXT_LINES, TEXT_LEFT_MARGIN + TEXT_COLS);
        }
    }
    endwin();
}
