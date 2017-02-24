#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// special keys enum
#define KEY_ERROR   (-1)
#define KEY_UNKNOWN (-2)
#define KEY_UP      (-3)
#define KEY_DOWN    (-4)
#define KEY_RIGHT   (-5)
#define KEY_LEFT    (-6)

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
    } else {
        return c;
    }
}


int main() {
    struct termios old, new;

    /* read terminal attributes */
    tcgetattr(STDIN_FILENO,&old);

    /* get old settings */
    new=old;

    /* modify the current settings (common: switch CANON and ECHO) */
    new.c_lflag &=(~ICANON & ~ECHO);

    /* push the settings on the terminal */
    tcsetattr(STDIN_FILENO,TCSANOW,&new);

    /* read unbuffered input */
    int key;
    while(1) {
        key = read_key();
        switch (key) {
            case KEY_UP:
                printf("[UP]");
                break;
            case KEY_DOWN:
                printf("[DOWN]");
                break;
            case KEY_RIGHT:
                printf("[RIGHT]");
                break;
            case KEY_LEFT:
                printf("[LEFT]");
                break;
            case KEY_UNKNOWN:
            case KEY_ERROR:
                break;
            default:
                printf("%c (%d)\n", key, key);
        }
        fflush(stdout);
    }

    /* ok, restore initial behavior */
    tcsetattr(STDIN_FILENO,TCSANOW,&old);

    return 0;
}
