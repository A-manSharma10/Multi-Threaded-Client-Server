/**
 * @file logger.cpp
 * @brief Thread-safe telemetry system for NetSched Pro Simulation
 * 
 * © 2026 NetSched Pro Systems Engineering
 */

#include "logger.h"
#include <time.h>
#include <stdio.h>

Logger* Logger::instance = NULL;

Logger::Logger() {
    // Open persistent log storage in append mode
    fileStream.open("server_log.txt", std::ios::out | std::ios::app);
    // Initialize OS-level sync
    InitializeCriticalSection(&logMutex);
}

Logger::~Logger() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
    // Release OS resources
    DeleteCriticalSection(&logMutex);
}

Logger* Logger::getInstance() {
    if (instance == NULL) {
        instance = new Logger();
    }
    return instance;
}

void Logger::log(const std::string& level, const std::string& component, const std::string& message) {
    // Shared resource locking. Prevents interlaced log entries from multiple workers.
    EnterCriticalSection(&logMutex);
    
    SYSTEMTIME st;
    GetLocalTime(&st); // High-precision local time
    
    char timeBuffer[64];
    sprintf(timeBuffer, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    // Standardized log entry serialization
    std::string formattedLog = "[" + std::string(timeBuffer) + "] [" + level + "] [" + component + "] " + message;
    
    std::cout << formattedLog << std::endl;
    if (fileStream.is_open()) {
        fileStream << formattedLog << std::endl;
        fileStream.flush(); // Ensure data reaches disk instantly
    }
    
    LeaveCriticalSection(&logMutex);
}

void Logger::info(const std::string& component, const std::string& message) { log("INFO", component, message); }
void Logger::warn(const std::string& component, const std::string& message) { log("WARN", component, message); }
void Logger::error(const std::string& component, const std::string& message) { log("ERROR", component, message); }
void Logger::stat(const std::string& component, const std::string& message) { log("STAT", component, message); }
