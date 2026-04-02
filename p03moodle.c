#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char program[100]; // Buffer to store user program name
    printf("Enter the program name to execute: ");
    scanf("%s", program); // Read the program name from the user

    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process10
        // Execute the user program using execvp()
        execlp(program, program, NULL);

        // If execlp returns, it means it failed
        perror("Execvp failed");
        return 1;
    } else {
        // Parent process
        // Wait for the child process to finish
        wait(NULL);
        printf("Child process has finished execution\n");
    }

    return 0;
}
