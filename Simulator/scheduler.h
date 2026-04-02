/**
 * @file scheduler.h
 * @brief Kernel Scheduler for NetSched Pro Simulation
 * 
 * Orchestrates incoming request dispatching based on user-defined
 * OS algorithms. Supports High-Priority preemption and Round Robin slicing.
 * 
 * © 2026 NetSched Pro Systems Engineering
 */

#pragma once
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "shared.h"
#include <vector>
#include <windows.h>

/**
 * @struct ServerTask
 * @brief Represents a single unit of work in the kernel queue.
 */
struct ServerTask {
    RequestPayload req;    // Network payload
    SOCKET clientSocket;    // Communication endpoint
    DWORD arrivalTime;      // System uptime on entry
};

/**
 * @class Scheduler
 * @brief Manages the Ready Queue and task dispatching strategies.
 */
class Scheduler {
private:
    std::vector<ServerTask> queue; // Kernel Task Control Block (TCB)
    CRITICAL_SECTION qMutex;       // Queue exclusion lock
    HANDLE qEvent;                 // Consumer thread wait primitive
    ScheduleAlgo algo;             // Strategy configuration
    bool isRunning;                // Lifecycle state

public:
    Scheduler(ScheduleAlgo a);
    ~Scheduler();

    void setAlgo(ScheduleAlgo a);
    void enqueue(const ServerTask& task);
    
    /**
     * @brief Blocking task extractor based on the current strategy.
     * @param outTask Task output parameter.
     * @return bool False if system has decommissioned.
     */
    bool dequeue(ServerTask& outTask); 
    
    void stop(); 
    size_t getQueueSize();
};

#endif
