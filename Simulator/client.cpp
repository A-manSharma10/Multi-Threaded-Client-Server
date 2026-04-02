/**
 * @file client.cpp
 * @brief Synthetic Workload Generator for NetSched Pro Simulation
 * 
 * Simulates multiple concurrent network clients transmitting data 
 * with varying priorities and computational demands over TCP/IP.
 * 
 * © 2026 NetSched Pro Systems Engineering
 */

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include "shared.h"
#include "logger.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define NUM_REQUESTS 30 // Burst to test 15-queue saturation

/**
 * @brief Represents an individual client worker thread
 */
DWORD WINAPI clientTask(LPVOID lpParam) {
    int reqId = (int)(intptr_t)lpParam;
    int clientId = (reqId % 5) + 1; // Round-robin assignment of ID 1-5
    Logger* logger = Logger::getInstance();
    
    char caller[32], pathBuf[128], logMsg[256];
    sprintf(caller, "Agent-%d", clientId);
    
    // Simulate staggered arrival
    Sleep(rand() % 500); 

    // Socket Initialization
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connection Attempt
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        logger->error(caller, "[NETWORK_FAIL] Server refused connection or is offline.");
        closesocket(sock);
        return 1;
    }

    // Protocol Payload Generation
    RequestPayload req;
    req.clientId = clientId;
    req.requestId = reqId;
    req.priority = 1 + (rand() % 10);
    req.burstTime = 100 + (rand() % 500); 
    
    int methodChoice = rand() % 3;
    if (methodChoice == 0) { req.method = GET_TASK; sprintf(pathBuf, "/api/task/%d", reqId); }
    else if (methodChoice == 1) { req.method = POST_DATA; sprintf(pathBuf, "/api/data/submit"); }
    else { req.method = DELETE_RES; sprintf(pathBuf, "/api/resource/%d", reqId); }
    
    strcpy(req.path, pathBuf);
    req.timestamp = GetTickCount();

    // Data Transmission
    send(sock, (const char*)&req, sizeof(req), 0);
    sprintf(logMsg, "-> [SENT] %s %s | Priority: %d | Workload: %d ms", 
            (req.method == GET_TASK ? "GET" : req.method == POST_DATA ? "POST" : "DELETE"), 
            req.path, req.priority, req.burstTime);
    logger->info(caller, logMsg);

    // Response Await
    ResponsePayload resp;
    int bytesRead = recv(sock, (char*)&resp, sizeof(ResponsePayload), 0);
    
    if (bytesRead > 0) {
        if (resp.status == 200) {
            sprintf(logMsg, "<- [OK_200] ID:%d | Processing Delay: %d ms", resp.requestId, resp.processingDelay);
            logger->info(caller, logMsg);
        } else if (resp.status == 503) {
            sprintf(logMsg, "<- [FAIL_503] ID:%d | ERROR: Server Congestion Point Reached.", resp.requestId);
            logger->warn(caller, logMsg);
        } else {
            sprintf(logMsg, "<- [FAIL_500] ID:%d | ERROR: Simulated Failure Condition Met.", resp.requestId);
            logger->error(caller, logMsg);
        }
    } else {
         sprintf(logMsg, "[DISCONNECT] Connection closed by server abruptly for Request %d.", reqId);
         logger->error(caller, logMsg);
    }

    closesocket(sock);
    return 0;
}

/**
 * @brief Simulation Entry Point
 */
int main() {
    srand((unsigned int)time(NULL));

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    HANDLE threads[NUM_REQUESTS];
    Logger* logger = Logger::getInstance();
    
    logger->stat("SimulatorLauncher", "Initiating High-Workload Load Scenario. Target: Cluster-Alpha.");

    for (int i = 0; i < NUM_REQUESTS; ++i) {
        threads[i] = CreateThread(NULL, 0, clientTask, (LPVOID)(intptr_t)(i + 1), 0, NULL);
        if (i % 5 == 0) Sleep(50); // Burst staggered arrival
    }

    // Synchronize Termination
    WaitForMultipleObjects(NUM_REQUESTS, threads, TRUE, INFINITE);
    for (int i = 0; i < NUM_REQUESTS; ++i) CloseHandle(threads[i]);

    WSACleanup();
    logger->stat("SimulatorLauncher", "High-Workload scenario completed successfully.");
    
    printf("\n[SUCCESS] All client agents have reported back. Check server_log.txt for data points.\n");
    return 0;
}
