#include <iostream>
#include <algorithm> // for min function

using namespace std;


int total_waiting_time = 0;
int total_wt = 0;
void firstComeFirstServe(int processes[], int n, int burst_time[]) {
    int waiting_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += waiting_time;
        waiting_time += burst_time[i];
    }
    cout << "Average waiting time using First Come First Serve: " << (float)total_waiting_time / n << endl;
}



void roundRobinScheduling(int processes[], int bt[], int n, int quantum) {
    int wt[n] = {0};
    int remaining_time[n];

    // Initialize remaining time with burst time
    for (int i = 0; i < n; i++) {
        remaining_time[i] = bt[i];
    }

    int currentTime = 0;
    int completedProcesses = 0;

    // Array to store the sequence of processes executed in the ready queue
    int readyQueue[n * n]; // Maximum possible size

    int idx = 0; // Index to keep track of the readyQueue array

    // Round Robin scheduling
    while (completedProcesses < n) {
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                int executionTime = min(quantum, remaining_time[i]);
                remaining_time[i] -= executionTime;
                currentTime += executionTime;

                // Add the process to the ready queue
                readyQueue[idx++] = processes[i];

                if (remaining_time[i] == 0) {
                    completedProcesses++;
                    wt[i] = currentTime - bt[i]; // Calculate waiting time
                }
            }
        }
    }

    cout << "Sequence of processes executed in the ready queue: ";
    for (int i = 0; i < idx; i++) {
        cout << "P" << readyQueue[i] << " ";
    }
    cout << endl;

    // Calculate total waiting time
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
    }
}

int main() {
    int interactiveProcesses[6];
    int batchProcesses[4];
    int interactiveBurstTime[6];
    int batchBurstTime[4];
    int timeQuantum;

    // Inputting Process IDs and Burst Times for Interactive Queue
    cout << "Enter IDs and Burst Times of 6 interactive processes: " << endl;
    for (int i = 0; i < 6; ++i) {
        cout << "Process ID: ";
        cin >> interactiveProcesses[i];
        cout << "Burst Time: ";
        cin >> interactiveBurstTime[i];
    }

    // Inputting Process IDs and Burst Times for Batch Queue
    cout << "Enter IDs and Burst Times of 4 batch processes: " << endl;
    for (int i = 0; i < 4; ++i) {
        cout << "Process ID: ";
        cin >> batchProcesses[i];
        cout << "Burst Time: ";
        cin >> batchBurstTime[i];
    }

    // Inputting Time Quantum for Round Robin
    cout << "Enter time quantum for Round Robin: ";
    cin >> timeQuantum;

    // Implementing Round Robin for Interactive Queue
    roundRobinScheduling(interactiveProcesses, interactiveBurstTime, 6, timeQuantum);

    // Implementing FCFS for Batch Queue
    cout << "Processing Batch Processes using FCFS:" << endl;
    for (int i = 0; i < 4; ++i) {
        cout << "Processing Batch Process ID " << batchProcesses[i] << " for " << batchBurstTime[i] << " units" << endl;
    }

    // Calculate average waiting time for both queues
    float avgInteractiveWaitingTime = (float)total_wt / 6;
    firstComeFirstServe(batchProcesses, 4, batchBurstTime); // Calling FCFS for Batch Queue
    float avgBatchWaitingTime = (float)total_waiting_time/4; // No waiting time calculation for batch processes in round robin

    // Calculate total average waiting time for all processes
    float totalAvgWaitingTime = avgInteractiveWaitingTime  + avgBatchWaitingTime ;

    // Displaying average waiting times
    cout << fixed << "Average Waiting Time for Interactive Queue: " << avgInteractiveWaitingTime << " units" << endl;
    
    cout << "Average Waiting Time for all Processes: " << totalAvgWaitingTime << " units" << endl;

    return 0;
}
