/**
 * @file server.cpp
 * @brief NetSched Pro High-Concurrency OS & Network Simulation Engine
 * 
 * This server implements a multi-threaded, networked task simulator.
 * Incoming requests follow a simplified HTTP-like protocol and are scheduled 
 * in the system queue based on selected OS algorithms (FCFS, Priority, RR).
 * 
 * © 2026 NetSched Pro Systems Engineering
 */

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shared.h"
#include "logger.h"
#include "scheduler.h"

#pragma comment(lib, "ws2_32.lib")

// --- SYSTEM CONSTANTS ---
#define PORT 8080
#define NUM_WORKERS 4      // Max parallel processing threads
#define QUANTUM_MS 50     // Time slice for Round Robin
#define MAX_QUEUE_SIZE 15  // Overload Protection Threshold

// --- GLOBAL STATE ---
bool serverRunning = true;
LONG processedCount = 0;
LONG failedCount = 0;
LONG totalWaitTime = 0;
LONG totalResponseTime = 0;

// --- ERROR CLASSIFICATION ---
LONG netFailCount = 0;
LONG overloadCount = 0;
LONG timeoutCount = 0;

ScheduleAlgo currentAlgo = FCFS;
Scheduler* scheduler = NULL;

/**
 * @brief Conversion utility for Request Method to String
 */
const char* methodToStr(MethodType m) {
    switch (m) {
        case GET_TASK: return "GET";
        case POST_DATA: return "POST";
        default: return "DELETE";
    }
}

/**
 * @brief Simulates probabilistic failure scenarios
 */
bool randomlyFail(ErrorType& type) {
    int r = rand() % 100;
    if (r < 3) { type = NET_FAIL; return true; }    // 3% Network Drop
    if (r < 6) { type = TIMEOUT_ERR; return true; } // 3% Gateway Timeout
    return false;
}

/**
 * @brief Renders the Real-Time CLI Dashboard
 */
void printDashboard() {
    system("cls");
    printf("\n");
    printf("   ====================================================\n");
    printf("        🚀 NETSCHED PRO: OS & NETWORK SIMULATOR        \n");
    printf("   ====================================================\n");
    printf("    Status: %s | Algorithm: %s \n", 
           serverRunning ? "[ ONLINE ]" : "[ OFFLINE ]",
           (currentAlgo == FCFS) ? "FCFS" : ((currentAlgo == ROUND_ROBIN) ? "RR" : "PRIORITY"));
    printf("   ----------------------------------------------------\n");
    printf("    [ REQUESTS ]   Processed: %ld | Failed: %ld\n", processedCount, failedCount);
    printf("    [ WORKLOAD ]   Queue Util: %zu / %d\n", scheduler ? scheduler->getQueueSize() : 0, MAX_QUEUE_SIZE);
    
    if (processedCount > 0) {
        printf("    [ LATENCY  ]   Avg Wait: %ld ms | Avg Resp: %ld ms\n", 
               totalWaitTime / processedCount, totalResponseTime / processedCount);
    } else {
        printf("    [ LATENCY  ]   Avg Wait: - ms | Avg Resp: - ms\n");
    }
    printf("   ----------------------------------------------------\n");
    printf("    [ FAILURES ]   Network: %ld | Overload: %ld | Timeout: %ld\n", 
           netFailCount, overloadCount, timeoutCount);
    printf("   ====================================================\n");
    printf("\n");
}

/**
 * @brief Main worker thread logic for request processing
 */
DWORD WINAPI workerLoop(LPVOID lpParam) {
    int workerId = (int)(intptr_t)lpParam;
    Logger* logger = Logger::getInstance();
    char workerStr[32];
    sprintf(workerStr, "Worker-%d", workerId);
    
    while (serverRunning) {
        ServerTask task;
        // Blocking dequeue from the Scheduler Based on Algorithm
        if (!scheduler->dequeue(task)) break; 
        
        DWORD startTime = GetTickCount();
        DWORD queueDuration = startTime - task.arrivalTime;
        InterlockedExchangeAdd(&totalWaitTime, queueDuration);
        
        char logMsg[256];
        sprintf(logMsg, "[DATA_RECV] %s %s (from Client %d) - Queued for %u ms", 
                methodToStr(task.req.method), task.req.path, task.req.clientId, queueDuration);
        logger->info(workerStr, logMsg);

        ErrorType eType = NONE;
        bool failed = randomlyFail(eType);
        
        ResponsePayload resp;
        resp.requestId = task.req.requestId;
        resp.status = failed ? 500 : 200;
        resp.error = eType;
        
        if (!failed) {
            // SIMULATED OS BURST (FCFS/RR/PRIORITY)
            if (currentAlgo == ROUND_ROBIN) {
                int burstRemaining = task.req.burstTime;
                while (burstRemaining > 0) {
                    int exec = (burstRemaining > QUANTUM_MS) ? QUANTUM_MS : burstRemaining;
                    Sleep(exec);
                    burstRemaining -= exec;
                }
            } else {
                Sleep(task.req.burstTime);
            }
            strcpy(resp.message, "200 OK");
            InterlockedIncrement(&processedCount);
        } else {
            // FAILURE CLASSIFICATION
            InterlockedIncrement(&failedCount);
            if (eType == NET_FAIL) { 
                InterlockedIncrement(&netFailCount); 
                strcpy(resp.message, "500 Internal Network Failure"); 
            } else { 
                InterlockedIncrement(&timeoutCount); 
                strcpy(resp.message, "504 Gateway Timeout"); 
            }
        }

        DWORD endTime = GetTickCount();
        DWORD totalDuration = endTime - task.arrivalTime;
        resp.processingDelay = (int)totalDuration;
        
        InterlockedExchangeAdd(&totalResponseTime, totalDuration);

        sprintf(logMsg, "[RESPONSE] Sending %d to ID:%d | Total Duration: %u ms", 
                resp.status, resp.requestId, totalDuration);
        if (resp.status == 200) logger->info(workerStr, logMsg);
        else logger->error(workerStr, logMsg);
        
        // Final Network Transmission
        send(task.clientSocket, (char*)&resp, sizeof(ResponsePayload), 0);
        closesocket(task.clientSocket);
        printDashboard(); // Refresh HUD
    }
    return 0;
}

/**
 * @brief Connection listener to handle individual TCP handshakes
 */
DWORD WINAPI clientListenerThread(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;
    Logger* logger = Logger::getInstance();
    
    // Simulate Networking Protocol Layer Handshake
    logger->info("NetworkLayer", "[SYN] Incoming handshake request...");
    logger->info("NetworkLayer", "[ACK] Packet sequence validated.");

    RequestPayload req;
    int bytesRead = recv(clientSocket, (char*)&req, sizeof(RequestPayload), 0);
    
    if (bytesRead > 0) {
        // OVERLOAD PROTECTION (Industrial standard)
        if (scheduler->getQueueSize() >= MAX_QUEUE_SIZE) {
            logger->warn("Kernel", "[OVERLOAD] Saturation threshold reached. Dropping Request ID:" + std::to_string(req.requestId));
            InterlockedIncrement(&failedCount);
            InterlockedIncrement(&overloadCount);
            
            ResponsePayload resp;
            resp.requestId = req.requestId;
            resp.status = 503;
            resp.error = OVERLOAD;
            strcpy(resp.message, "503 System Under Overload");
            send(clientSocket, (char*)&resp, sizeof(ResponsePayload), 0);
            closesocket(clientSocket);
        } else {
            // Move Task to Kernel Scheduler
            ServerTask task;
            task.req = req;
            task.clientSocket = clientSocket;
            task.arrivalTime = GetTickCount();
            scheduler->enqueue(task);
            printDashboard();
        }
    } else {
        closesocket(clientSocket);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));

    // Command line argument for scheduling algorithm
    if (argc > 1) {
        std::string a(argv[1]);
        if (a == "RR") currentAlgo = ROUND_ROBIN;
        else if (a == "PRIORITY") currentAlgo = PRIORITY;
    }

    scheduler = new Scheduler(currentAlgo);
    Logger* logger = Logger::getInstance();
    
    // WinSock Initialization
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    printDashboard();

    // Spawn Worker Thread Pool
    HANDLE workers[NUM_WORKERS];
    for (int i = 0; i < NUM_WORKERS; ++i) {
        workers[i] = CreateThread(NULL, 0, workerLoop, (LPVOID)(intptr_t)(i + 1), 0, NULL);
    }

    // Accept 25 requests for simulation cycle
    int requestsToHandle = 25;
    while (serverRunning && requestsToHandle-- > 0) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET) {
            CloseHandle(CreateThread(NULL, 0, clientListenerThread, (LPVOID)clientSocket, 0, NULL));
        }
    }

    // Graceful Shutdown Sequence
    while(scheduler->getQueueSize() > 0) Sleep(200);
    Sleep(2000); // Final cooldown
    serverRunning = false;
    scheduler->stop();
    closesocket(serverSocket);
    WaitForMultipleObjects(NUM_WORKERS, workers, TRUE, INFINITE);
    WSACleanup();

    logger->stat("SystemLifecycle", "Server gracefully decommissioned.");
    return 0;
}
