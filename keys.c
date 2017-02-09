#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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

    char c;
    while(read(STDIN_FILENO, &c, 1) == 1) {
        printf("%c (%d)\n", c, c);
    }

    /* ok, restore initial behavior */
    tcsetattr(STDIN_FILENO,TCSANOW,&old);
    return 0;
}
