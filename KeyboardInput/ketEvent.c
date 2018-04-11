/* 
 * http://shaeod.tistory.com/836
 *
 * Keyboard input speed control command : kbdrate -d [delay : 250 ~ 1000 ms]
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>          /* File control header file */
#include <errno.h>          /*  error check */
#include <linux/input.h>     /* Linux Kernel header file.*/
#include <string.h>

static const char *const evval[3] = {
    "RELEASED",
    "PRESSED",
    "REPEATED"
};

int main(void) {
    const char *dev = "/dev/input/???" /* your keyboard information */
    struct input_event ev;
    ssize_t n;
    int fd;

    fd = open(dev, O_RDONLY);
    if(fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        return EXIT_FAILURE;
    }

    while(1) {
        n = read(fd, &ev, sizeof(ev));
        if (n == (ssize_t) - 1) {
            if(errno == EINTR) {
                continue;
            }
            else {
                break;
            }
        }
        else if(n != sizeof ev) {
            errno = EIO;
            break;
        }

        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2)
            printf("%s 0x04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
    }

    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
    return EXIT_FAILURE;

    return 0;
}