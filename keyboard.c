#define _GNU_SOURCE
#define _XOPEN_SOURCE 600 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <errno.h>
#define __USE_BSD
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>

#define STD_INPUT	0
#define STD_OUTPUT	1

static struct termios old, new;

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED ",
    "REPEATED"
};

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_cflag &= ISIG;	/* handle ^C and ^Z signals */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? (ECHO | ECHOCTL)  : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}


int main(int argc, char *argv[])
{
	
	initTermios(0);
	//system("stty -echo");

	const char *dev = "/dev/input/by-path/platform-i8042-serio-0-event-kbd";
    struct input_event ev;
    ssize_t n;
    int fd;

	fd = open(dev, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }

    while (1) {
		n = read(fd, &ev, sizeof(ev));
		if (n == (ssize_t)-1) {
            if (errno == EINTR)		/* Interrupted system call */
                continue;
            else
                break;
        } 
        else if (n != sizeof ev) {
            errno = EIO;			/* I/O error */
            break;
        }

		if (ev.type == EV_KEY && ev.value == 1){
			if(ev.code == 108){
				printf("down 0x%04x, (%d)\n", (char)ev.code, (int)ev.code);
      }
			else if(ev.code == 106){
				printf("right\n");
			}
			else if(ev.code == 16){
				printf("a\n");
			}
			/*else{
				printf("(%d)\n", (int)ev.code);
			}*/
    }
	}

    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return EXIT_FAILURE;
}