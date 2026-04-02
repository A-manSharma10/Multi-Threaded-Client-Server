#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Here I am just before first forking statement\n");
    
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        return 1;
    } else if (pid1 == 0) {
        printf("Child 1: Hello World from process %d!\n", getpid());
    } else {
        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            return 1;
        } else if (pid2 == 0) {
            printf("Child 2: Hello World from process %d!\n", getpid());
        } else {
            pid_t pid3 = fork();
            if (pid3 == -1) {
                perror("fork");
                return 1;
            } else if (pid3 == 0) {
                printf("Child 3: Hello World from process %d!\n", getpid());
            } else {
                printf("Parent: Hello World from process %d!\n", getpid());
            }
        }
    }
    
    return 0;
}