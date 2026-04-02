/**
 * @file logger.h
 * @brief Thread-safe telemetry system for NetSched Pro Simulation
 * 
 * Provides centralized logging with shared access protection via Windows
 * Critical Sections. All entries are timestamped with millisecond precision.
 * 
 * © 2026 NetSched Pro Systems Engineering
 */

#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

/**
 * @class Logger
 * @brief Manages thread-safe logging to standard output and secondary log files.
 */
class Logger {
private:
    CRITICAL_SECTION logMutex; // Kernel-level sync
    std::ofstream fileStream; // Persistent storage

    Logger(); // Singleton enforcement
    static Logger* instance;

public:
    /**
     * @brief Singleton Accessor
     * @return Logger* 
     */
    static Logger* getInstance();

    // Disable assignment and copy
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    ~Logger();

    /**
     * @brief Primary logging entry point
     * @param level Severity: [INFO|WARN|ERROR|STAT]
     * @param component Subsystem identification (e.g., Kernel, Worker, Network)
     * @param message Detailed log payload
     */
    void log(const std::string& level, const std::string& component, const std::string& message);
    
    // Convenience Wrappers
    void info(const std::string& component, const std::string& message);
    void warn(const std::string& component, const std::string& message);
    void error(const std::string& component, const std::string& message);
    void stat(const std::string& component, const std::string& message);
};

#endif
