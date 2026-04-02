#include <iostream>
#include <Windows.h>

// Function to display error messages
void displayErrorMessage(const char* message) {
    std::cerr << "Error: " << message << " (error code: " << GetLastError() << ")" << std::endl;
}

// Function to display process exit messages
void displayProcessExitMessage(DWORD processId) {
    std::cout << "Child process with ID " << processId << " has exited and released its resources." << std::endl;
}

int main() {
    // Initialize STARTUPINFO and PROCESS_INFORMATION structures
    STARTUPINFOA si1, si2;
    PROCESS_INFORMATION pi1, pi2;

    // Zero out memory for STARTUPINFO and PROCESS_INFORMATION structures
    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi1, sizeof(pi1));
    ZeroMemory(&pi2, sizeof(pi2));

    // Create the first child process
    if (!CreateProcessA(NULL, const_cast<LPSTR>("ChildProcess1.exe"), NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) {
        displayErrorMessage("CreateProcess failed for ChildProcess1.exe");
        return 1;
    }

    // Create the second child process
    if (!CreateProcessA(NULL, const_cast<LPSTR>("ChildProcess2.exe"), NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) {
        displayErrorMessage("CreateProcess failed for ChildProcess2.exe");
        // Close handles for the first child process if the second one fails to create
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        return 1;
    }

    // Wait for the first child process to finish
    WaitForSingleObject(pi1.hProcess, INFINITE);
    displayProcessExitMessage(pi1.dwProcessId);
    // Close handles for the first child process
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);

    // Wait for the second child process to finish
    WaitForSingleObject(pi2.hProcess, INFINITE);
    displayProcessExitMessage(pi2.dwProcessId);
    // Close handles for the second child process
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    return 0;
}