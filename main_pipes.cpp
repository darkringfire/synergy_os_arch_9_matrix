#include <iostream>
//#include <random>
#include <unistd.h>
#include <wait.h>
#include <cstring>

using namespace std;

int main() {
    int fd_p2c[2];
    int fd_c2p[2];

    char fixed_str[] = "<this is yor string";
    char input_str[100];
    pid_t p;

    if (pipe(fd_p2c) == -1) {
        fprintf(stderr, "Pipe 1 failed!");
        return 1;
    }
    if (pipe(fd_c2p) == -1) {
        fprintf(stderr, "Pipe 2 failed!");
        return 1;
    }

    scanf("%s", input_str);
    p = fork();

    if (p < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    } else if (p > 0) {
        // Parent
        char concat_str[100];
        // P->C: close-read, write, close-write;
        close(fd_p2c[0]);
        write(fd_p2c[1], input_str, strlen(input_str) + 1);
        close(fd_p2c[1]);
        // Wait child ending
        wait(nullptr);
        // P<-C: close-write, read, close-read;
        close(fd_c2p[1]);
        read(fd_c2p[0], concat_str, 100);
        close(fd_c2p[0]);

        printf("Concatenated string %s\n", concat_str);
    } else {
        // Child
        char concat_str[100];
        // C<-P: close-write, read, close-read
        close(fd_p2c[1]);
        read(fd_p2c[0], concat_str, 100);
        close(fd_p2c[0]);

        int k = (int)strlen(concat_str);
        int i;
        for (i = 0; i < strlen(fixed_str); ++i) {
            concat_str[k++] = fixed_str[i];
        }
        concat_str[k] = '\0';
        // C->P: close-read, write, close-write
        close(fd_c2p[0]);
        write(fd_c2p[1], concat_str, strlen(concat_str) + 1);
        close(fd_c2p[1]);
    }

    return 0;
}
