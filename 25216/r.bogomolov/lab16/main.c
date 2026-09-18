#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main(void) {
    struct termios old_term;
    struct termios new_term;
    char answer;

    if (tcgetattr(0, &old_term) == -1) {
        perror("tcgetattr");
        return 1;
    }

    new_term = old_term;

    new_term.c_lflag &= ~ICANON;

    new_term.c_cc[VMIN] = 1;
    new_term.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &new_term) == -1) {
        perror("tcsetattr");
        return 1;
    }

    printf("Are you OK? (y/n): ");
    fflush(stdout);

    if (read(0, &answer, 1) == -1) {
        perror("read");

        tcsetattr(0, TCSANOW, &old_term);
        return 1;
    }

    if (tcsetattr(0, TCSANOW, &old_term) == -1) {
        perror("tcsetattr");
        return 1;
    }

    printf("\nYou entered: %c\n", answer);

    return 0;
}
