#include "input.h"

/* number of characters per line in minix console */
#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25

/* initial size of input buffer. at least 2 */
#define BUFF_CHUNK 100

struct input_buffer {
    char * prompt;
    char * buff;
    size_t buff_size;
    int pos;
    int old_pos;
};

struct input_buffer * input_buffer__new(size_t init_size) {
    struct input_buffer * b = malloc(sizeof(struct input_buffer));
    b->prompt = "";
    b->buff = malloc(init_size * sizeof(char));
    b->buff_size = init_size;
    b->pos = 0;
    b->buff[0] = 0;
    return b;
}

// special keys enum
#define KEY_ERROR   (-1)
#define KEY_UNKNOWN (-2)
#define KEY_UP      (-3)
#define KEY_DOWN    (-4)
#define KEY_RIGHT   (-5)
#define KEY_LEFT    (-6)
#define KEY_DEL     (-7)

/**
 * read a keayboard key input and returns its ascii code if any or one of
 * the previous custom key codes
 */
int read_key() {
    char c;
    if(read(STDIN_FILENO, &c, 1) != 1) return KEY_ERROR;
    if(c < 0) { /* ignore negative ascii codes */
        return KEY_UNKNOWN;
    } else if(c == 27 /* ESC */) {
        /* Check for arrows.
           Arrows don't have a corresponding ASCII code.
           Instead, it pushes to STDIN a sequence of 3 characters:
           ESC (27), '[' (91), followed by 'A' (65), 'B' (66), 'C' (67) or 'D' (68)
           respectively for up, down, right or left
           If Ctrl, Shift or Alt is pressed at the same time than the arrow
           additional characters are added */
        if(read(STDIN_FILENO, &c, 1) != 1) return KEY_ERROR;
            if(c != '[') return KEY_UNKNOWN;
            if(read(STDIN_FILENO, &c, 1) != 1) return KEY_ERROR;
            if(c == '1') { /* Ctrl, Shift or Alt is pressed */
                if(read(STDIN_FILENO, &c, 1) != 1) return KEY_ERROR; // supposed to read ';'
                if(c != ';') return KEY_UNKNOWN;
                if(read(STDIN_FILENO, &c, 1) != 1) return KEY_ERROR; // supposed to read a number. 2 for Shift, 3 for Alt and 5 for Ctrl
                if(c != '2' && c != '3' && c != '5') return KEY_UNKNOWN;
                if(read(STDIN_FILENO, &c, 1) != 1) return KEY_ERROR; // reads the letter for the following switch
            }
            switch(c) {
                case 'A':
                    return KEY_UP;
                case 'B':
                    return KEY_DOWN;
                case 'C':
                    return KEY_RIGHT;
                case 'D':
                    return KEY_LEFT;
                default:
                    return KEY_UNKNOWN;
            }
    } else if(c == 127) {
        return KEY_DEL;
    } else {
        return c;
    }
}

/**
 * A strlen version ignoring ANSI escape codes for color: \033[xx(;yy)m
 */
int mystrlen(char *s) {
    int n = 0; /* size */
    for(int i = 0; s[i]; ++i) {
        if(s[i] == '\033') {
            int j = 0;
            while(s[i+j] != 'm' && j < 10) { ++j; } /* the 100 upper boundary is to detect bad formed ANSI sequences */
            if(j == 10) { /* bad formed ANSI escape sequence for color */
                // just consider that it was an isolated \033. The for loop will increment i by 1
            } else {
                i+=j;
            }
        } else {
            ++n;
        }
    }
    return n;
}


/**
 *
 */
void refresh_screen(struct input_buffer * inbuff) {
    int prompt_length = mystrlen(inbuff->prompt);
    /* move to begining of prompt */
    int old_line = (prompt_length + inbuff->old_pos) / SCREEN_WIDTH;
    int old_col  = (prompt_length + inbuff->old_pos) % SCREEN_WIDTH;
    if(old_col > 0)
        printf("\033[%dD", old_col); /* move old_col times to the left */
    if(old_line > 0)
        printf("\033[%dA", old_line); /* move cursor by old_lines up */

    /* rewrite buffer */
    printf("%s%s", inbuff->prompt, inbuff->buff);

    /* clear to the end of screen */
    printf("\033[J");

    /* move back to begining */
    int end_pos = strlen(inbuff->buff);
    if(inbuff->pos == end_pos) {
        /* Scroll if cursor at the end of the screen.
           Actually adds a space each time the cursor is at the end of the
           buffer since we don't know when it is at the end of the screen.
           Who can do more can do less! */
        printf(" ");
        end_pos++;
    }
    end_pos += prompt_length;
    int end_line = end_pos / SCREEN_WIDTH;
    int end_col  = end_pos % SCREEN_WIDTH;
    if(end_col == 0) {
        end_line--;
        end_col = SCREEN_WIDTH;
    }
    //if(end_col > 0)
    printf("\033[%dD", end_col); /* move end_col times to the left */
    if(end_line > 0)
        printf("\033[%dA", end_line); /* move cursor by end_lines up */

    /* move to cursor pos */
    int cur_line = (prompt_length + inbuff->pos) / SCREEN_WIDTH;
    int cur_col  = (prompt_length + inbuff->pos) % SCREEN_WIDTH;
    // if(cur_line == SCREEN_HEIGHT) {
    //     printf(" "); // scroll
    //     printf("\033[1D"); // move back to line start
    // }
    if(cur_line > 0)
        printf("\033[%dB", cur_line); /* move cursor by cur_lines down */
    if(cur_col > 0)
        printf("\033[%dC", cur_col); /* move cur_col times to the right */

    fflush(stdout);
}


/**
 *
 */
char * input(char * prompt) {
    struct input_buffer inbuff;
    inbuff.prompt    = prompt;
    inbuff.buff      = malloc(BUFF_CHUNK * sizeof(char));
    inbuff.buff_size = BUFF_CHUNK;
    inbuff.pos       = 0;
    inbuff.old_pos   = 0;
    inbuff.buff[0]   = 0;

    char * typing_buffer = inbuff.buff;

    int c = 0;
    refresh_screen(&inbuff); /* display prompt for the first time */
    while(c != '\n') {
        // save cursor pos
        inbuff.old_pos = inbuff.pos;
        c = read_key();
        if(c >= ' ' && c < 127) { /* printable char */
            // walk to the end of the buffer
            int i;
            for(i = inbuff.pos; inbuff.buff[i] != 0; ++i);
            // check if reallocation is necessary
            if(i+1 >= inbuff.buff_size) {
                inbuff.buff_size += BUFF_CHUNK;
                inbuff.buff = realloc(inbuff.buff, inbuff.buff_size);
            }
            for(; i >= inbuff.pos; --i) {
                inbuff.buff[i+1] = inbuff.buff[i];
            }
            // add it to buffer and increment position
            inbuff.buff[inbuff.pos] = c;
            inbuff.pos++;
        } else if((c == KEY_DEL || c == '\b') && inbuff.pos > 0) {
            for(int i = inbuff.pos - 1; inbuff.buff[i] != 0; ++i) {
                inbuff.buff[i] = inbuff.buff[i+1];
            }
            inbuff.pos--;
        } else if(c == KEY_LEFT && inbuff.pos > 0) {
            inbuff.pos--;
        } else if(c == KEY_RIGHT && inbuff.buff[inbuff.pos] != 0) {
            inbuff.pos++;
        } else if(0 /* DISABLED */&& c == '\t') {
            char * autocomplete_start = strdup(inbuff.buff);
            autocomplete_start[inbuff.pos] = 0; // only take the substring preceding the cursor
            char * tmp = history_match(autocomplete_start);
            if(tmp != NULL) {
                free(inbuff.buff);
                inbuff.buff = tmp;
            }
        } else if(c == KEY_UP || c == '\t') {
            char * autocomplete_start = strdup(inbuff.buff);
            autocomplete_start[inbuff.pos] = 0; // only take the substring preceding the cursor
            char * tmp = history_next(autocomplete_start);
            if(tmp != NULL) {
                if(inbuff.buff != typing_buffer)
                    free(inbuff.buff);
                inbuff.buff = tmp;
            }
        } else if(c == KEY_DOWN) {
            char * autocomplete_start = strdup(inbuff.buff);
            autocomplete_start[inbuff.pos] = 0; // only take the substring preceding the cursor
            char * tmp = history_prev(autocomplete_start);
            if(tmp != NULL) {
                if(inbuff.buff != typing_buffer)
                    free(inbuff.buff);
                inbuff.buff = tmp;
            } else if(inbuff.buff != typing_buffer) {
                free(inbuff.buff);
                inbuff.buff = typing_buffer;
                inbuff.pos = strlen(typing_buffer);
            }
        }
        refresh_screen(&inbuff);
    }
    //int lines_to_skip = (strlen(inbuff.buff) + strlen(inbuff.prompt)) / SCREEN_WIDTH + 1;
    //for(int i = 0; i < lines_to_skip; ++i)
    printf("\n");
    return inbuff.buff;
}
