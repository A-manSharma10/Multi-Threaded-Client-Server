#pragma once
#ifndef SHARED_H
#define SHARED_H

#include <string>

// Enum for scheduling algorithms
enum ScheduleAlgo {
    FCFS,
    ROUND_ROBIN,
    PRIORITY
};

// Protocol Method Types
enum MethodType {
    GET_TASK,
    POST_DATA,
    DELETE_RES
};

// Error Classifications
enum ErrorType {
    NONE,
    NET_FAIL,
    OVERLOAD,
    TIMEOUT_ERR
};

// Header concept for realism
struct Header {
    char key[32];
    char value[64];
};

// Represents a network request payload
struct RequestPayload {
    int clientId;
    int requestId;
    int priority;    // 1 (Highest) to 10 (Lowest)
    int burstTime;   // ms of simulated work
    unsigned long long timestamp;   // Time it was sent
    MethodType method;
    char path[128];
};

// Represents a network response payload
struct ResponsePayload {
    int requestId;
    int status;          // 200 = OK, 500 = Error
    ErrorType error;
    int processingDelay; // ms spent in queue + processing
    char message[64];
};

#endif
