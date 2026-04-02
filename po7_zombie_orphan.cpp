#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

void createOrphan() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << ") is orphaned!" << std::endl;
        sleep(2); // Let the parent process terminate first
        std::cout << "Child process (PID: " << getpid() << ") exits." << std::endl;
    } else if (child_pid > 0) {
        // Parent process
        std::cout << "Parent process (PID: " << getpid() << ") exits." << std::endl;
    } else {
        // Fork failed
        std::cerr << "Fork failed!" << std::endl;
    }
}

void createZombie() {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        std::cout << "Child process (PID: " << getpid() << ") is a zombie!" << std::endl;
        exit(0); // Child process exits immediately
    } else if (child_pid > 0) {
        // Parent process
        std::cout << "Parent process (PID: " << getpid() << ") waits for child to become a zombie." << std::endl;
        sleep(2); // Let the child become a zombie
        std::cout << "Parent process (PID: " << getpid() << ") exits." << std::endl;
    } else {
        std::cerr << "Fork failed!" << std::endl;
    }
}

int main() {
    std::cout << "Demonstrating Orphan Process:" << std::endl;
    createOrphan();

    std::cout << "\nDemonstrating Zombie Process:" << std::endl;
    createZombie();

    return 0;
}
