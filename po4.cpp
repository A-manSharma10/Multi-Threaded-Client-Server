#include <iostream>
#include <windows.h>

int main() {
    STARTUPINFO si1, si2;
    PROCESS_INFORMATION pi1, pi2;

    // Initialize startup info for first child process
    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);

    // Create first child process
    if (!CreateProcess(
            NULL,                   // Application name (NULL since we're using command line)
            LPSTR("child1.exe"),    // Command line
            NULL,                   // Process handle not inheritable
            NULL,                   // Thread handle not inheritable
            FALSE,                  // Set handle inheritance to FALSE
            0,                      // No creation flags
            NULL,                   // Use parent's environment block
            NULL,                   // Use parent's starting directory
            &si1,                   // Pointer to STARTUPINFO structure
            &pi1                    // Pointer to PROCESS_INFORMATION structure
        )) {
        std::cerr << "Error: Failed to create child process 1." << std::endl;
        return 1;
    }

    std::cout << "Child process 1 created successfully." << std::endl;

    // Initialize startup info for second child process
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);

    // Create second child process
    if (!CreateProcess(
            NULL,                   // Application name (NULL since we're using command line)
            LPSTR("child2.exe"),    // Command line
            NULL,                   // Process handle not inheritable
            NULL,                   // Thread handle not inheritable
            FALSE,                  // Set handle inheritance to FALSE
            0,                      // No creation flags
            NULL,                   // Use parent's environment block
            NULL,                   // Use parent's starting directory
            &si2,                   // Pointer to STARTUPINFO structure
            &pi2                    // Pointer to PROCESS_INFORMATION structure
        )) {
        std::cerr << "Error: Failed to create child process 2." << std::endl;
        // If failed to create child process 2, close the handle of child process 1 and exit
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        return 1;
    }

    std::cout << "Child process 2 created successfully." << std::endl;

    // Wait for the first child process to finish
    WaitForSingleObject(pi1.hProcess, INFINITE);
    std::cout << "Child process 1 has finished its tasks and released its resources." << std::endl;

    // Close handles for first child process
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);

    // Wait for the second child process to finish
    WaitForSingleObject(pi2.hProcess, INFINITE);
    std::cout << "Child process 2 has finished its tasks and released its resources." << std::endl;

    // Close handles for second child process
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    return 0;
}
