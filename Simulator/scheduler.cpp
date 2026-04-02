/**
 * @file scheduler.cpp
 * @brief Kernel Scheduler logic for NetSched Pro Simulation
 * 
 * © 2026 NetSched Pro Systems Engineering
 */

#include "scheduler.h"
#include <algorithm>

Scheduler::Scheduler(ScheduleAlgo a) : algo(a), isRunning(true) {
    // OS Mutex Initialization
    InitializeCriticalSection(&qMutex);
    
    // Low-Latency Kernel Event. Used for worker thread sleep/wake cycles.
    qEvent = CreateEvent(NULL, FALSE, FALSE, NULL); 
}

Scheduler::~Scheduler() {
    stop();
    DeleteCriticalSection(&qMutex);
    CloseHandle(qEvent);
}

void Scheduler::setAlgo(ScheduleAlgo a) {
    EnterCriticalSection(&qMutex);
    algo = a;
    LeaveCriticalSection(&qMutex);
}

void Scheduler::stop() {
    EnterCriticalSection(&qMutex);
    isRunning = false;
    SetEvent(qEvent); // Flush any waiting threads
    LeaveCriticalSection(&qMutex);
}

void Scheduler::enqueue(const ServerTask& task) {
    EnterCriticalSection(&qMutex);
    if (!isRunning) {
        LeaveCriticalSection(&qMutex);
        return;
    }
    
    // Core Queue Insertion (TCB Ready List Entry)
    queue.push_back(task);
    
    // Signal a waiting worker thread across the system bus
    SetEvent(qEvent); 
    LeaveCriticalSection(&qMutex);
}

bool Scheduler::dequeue(ServerTask& outTask) {
    while (true) {
        EnterCriticalSection(&qMutex);
        
        // System Shutdown check
        if (!isRunning && queue.empty()) {
            LeaveCriticalSection(&qMutex);
            return false;
        }
        
        // Dispatching Logic Selection
        if (!queue.empty()) {
            if (algo == FCFS || algo == ROUND_ROBIN) {
                // First-In Standard Dispatch
                outTask = queue.front();
                queue.erase(queue.begin());
            } 
            else if (algo == PRIORITY) {
                // Priority Preemption Scan
                std::vector<ServerTask>::iterator highestIt = queue.begin();
                for (std::vector<ServerTask>::iterator it = queue.begin() + 1; it != queue.end(); ++it) {
                    // Lower priority value (1, 2, 3) indicates higher system importance.
                    if (it->req.priority < highestIt->req.priority) {
                        highestIt = it;
                    }
                }
                outTask = *highestIt;
                queue.erase(highestIt);
            }
            
            // Re-signal if further items remain for peer workers
            if (!queue.empty()) SetEvent(qEvent); 
            
            LeaveCriticalSection(&qMutex);
            return true;
        }
        
        LeaveCriticalSection(&qMutex);
        
        // Low-power wait state. Thread rests until signaled by Enqueue.
        WaitForSingleObject(qEvent, 2000); // 2s timeout for safety
    }
}

size_t Scheduler::getQueueSize() {
    EnterCriticalSection(&qMutex);
    size_t size = queue.size();
    LeaveCriticalSection(&qMutex);
    return size;
}
