#include <iostream>

int main() {
    std::cout << "Child Process 2 is executing." << std::endl;
    // Simulate some task
    for (int i = 0; i < 3; ++i) {
        // Simulate work
        for (int j = 0; j < 10000000; ++j) {}
    }
    std::cout << "Child Process 2 completed its task." << std::endl;
    return 0;
}s