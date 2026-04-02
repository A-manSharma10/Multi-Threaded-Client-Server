#include <iostream>
#include <algorithm>
using namespace std;

// Function prototypes
void WaitingTime(int processes[], int n, int bt[], int wt[]);
void displayP(int processes[], int bt[], int wt[], int n);
void shortestJobFirst(int processes[], int bt[], int at[], int n);
void shortestJobFirstPreemptive(int processes[], int bt[], int at[], int n);
void priorityScheduling(int processes[], int bt[], int at[], int priority[], int n);
void roundRobinScheduling(int processes[],int bt[],int at[],int n,int quantum);

class base {
public: 
    virtual void funFCFS(int[], int[], int[], int) = 0;
    virtual void funSJF(int[], int[], int[], int) = 0;
    virtual void funPS(int[] , int[] , int[] ,int[] , int) = 0;
    virtual void funRR(int[], int[], int[], int) = 0;
};

class FCFS : public base {
public:
    void funFCFS(int processes[], int bt[], int at[], int n) override {
        int wt[n];
        WaitingTime(processes, n, bt, wt);
        displayP(processes, bt, wt, n);
    }

    void funSJF(int processes[], int bt[], int at[], int n) override {
        // Implementation for SJF (if required)
    }

    void funPS(int processes[], int bt[], int at[],int p[], int n) override {
        // Implementation for PS (if required)
    }

    void funRR(int processes[], int bt[], int at[], int n) override {
        // Implementation for RR (if required)
    }
};

class SJF : public base {
public:
    void funSJF(int processes[], int bt[], int at[], int n) override {
        shortestJobFirst(processes, bt, at, n);
    }

    void funFCFS(int processes[], int bt[], int at[], int n) override {
        // Implementation for FCFS (if required)
    }

    void funPS(int processes[], int bt[], int at[],int p[], int n) override {
        // Implementation for PS (if required)
    }

    void funRR(int processes[], int bt[], int at[], int n) override {
        // Implementation for RR (if required)
    }
};

class PS : public base {
public:
    void funPS(int processes[], int bt[], int at[],int p[], int n) override {
        priorityScheduling(processes, bt, at, p, n);
    }

    void funFCFS(int processes[], int bt[], int at[], int n) override {
        // Implementation for FCFS (if required)
    }

    void funSJF(int processes[], int bt[], int at[], int n) override {
        // Implementation for SJF (if required)
    }

    void funRR(int processes[], int bt[], int at[], int n) override {
        // Implementation for RR (if required)
    }
};

class RR : public base {
public:
    void funRR(int processes[], int bt[], int at[], int n) override {
        int quantum;
        cout << "Enter time quantum for Round Robin: ";
        cin >> quantum;
        roundRobinScheduling(processes, bt, at, n, quantum);
    }

    void funFCFS(int processes[], int bt[], int at[], int n) override {
        // Implementation for FCFS (if required)
    }

    void funSJF(int processes[], int bt[], int at[], int n) override {
        // Implementation for SJF (if required)
    }

    void funPS(int processes[], int bt[], int at[],int p[], int n) override {
        // Implementation for PS (if required)
    }
};

void WaitingTime(int processes[], int n, int bt[], int wt[]) {
    wt[0] = 0;
    for (int i = 1; i < n; i++)
        wt[i] = bt[i - 1] + wt[i - 1];
}

void displayP(int processes[], int bt[], int wt[], int n) {
    cout << "Processes  "<< " Burst time  "
         << " Waiting time \n ";

    for (int i = 0; i < n; i++) {
        cout << "   " << processes[i] << "\t\t" << bt[i] <<"\t    "
            << wt[i] <<endl;
    }
}

void shortestJobFirst(int processes[], int bt[], int at[], int n) {
    int wt[n] = {0}, tat[n] = {0}, total_wt = 0, currentTime = 0;

    // Sort processes based on arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (at[j] > at[j + 1]) {
                swap(at[j], at[j + 1]);
                swap(bt[j], bt[j + 1]);
                swap(processes[j], processes[j + 1]);
            }
        }
    }

    // Execution
    for (int i = 0; i < n; i++) {
        currentTime += bt[i]; // Process executes
        tat[i] = currentTime - at[i]; // Turnaround time
        wt[i] = tat[i] - bt[i]; // Waiting time
        total_wt += wt[i]; // Total waiting time
    }

    // Display process details
    displayP(processes, bt, wt, n);
}

void shortestJobFirstPreemptive(int processes[], int bt[], int at[], int n) {
    // Implementation for preemptive SJF (if required)
}


void priorityScheduling(int processes[], int bt[], int at[], int priority[], int n) {
    int wt[n], tat[n], total_wt = 0;
    bool preemptive;
    WaitingTime(processes, n, bt, wt);

    cout<<"Choose [0]Preemptive; [1]NON preemptive:";
    cin>>preemptive;


    if (preemptive) {
        // Preemptive priority scheduling
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (priority[j] > priority[j + 1]) {
                    swap(at[j], at[j + 1]);
                    swap(bt[j], bt[j + 1]);
                    swap(processes[j], processes[j + 1]);
                    swap(priority[j], priority[j + 1]);
                }
            }
        }
        // Implement sorting logic here

        cout << "Preemptive priority scheduling" << endl;
    } else {
        // Non-preemptive priority scheduling
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (priority[j] > priority[j + 1]) {
                    swap(at[j], at[j + 1]);
                    swap(bt[j], bt[j + 1]);
                    swap(processes[j], processes[j + 1]);
                    swap(priority[j], priority[j + 1]);
                }
            }
        }
        // Implement sorting logic here

        cout << "Non-preemptive priority scheduling" << endl;
    }


    // Calculate waiting time for each process
    for (int i = 0; i < n; i++) {
        total_wt = total_wt + wt[i];
    }

    displayP(processes, bt, wt,  n);

    cout << "Average waiting time = "
        << (float)total_wt / (float)n;
}


void roundRobinScheduling(int processes[], int bt[], int at[], int n, int quantum) {
    int wt[n] = {0}, tat[n] = {0}, total_wt = 0;
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
                    tat[i] = currentTime - at[i];
                    wt[i] = tat[i] - bt[i];
                }
            }
        }
    }

    cout << "Sequence of processes executed in the ready queue: ";
    for (int i = 0; i < idx; i++) {
        cout << "P" << readyQueue[i] << " ";
    }
    cout << endl;

    // Display process details
    displayP(processes, bt, wt, n);

    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
    }

    cout << "Average waiting time = " << (float)total_wt / n << endl;
}



int main() {
    int n, a=0;
    do {
        cout << "Select Algorithm: [1] FCFS; [2] SJF; [3] PS; [4] RR; [0] Exit: ";
        cin >> a;
        cout << "Enter no. of processes: ";
        cin >> n;
        int processes[n], bt[n], at[n], pr[n];
        for (int i = 0; i < n; i++) {
            cout << "Enter " << i + 1 << " process Burst time: ";
            cin >> bt[i];
            cout << "Enter " << i + 1 << " process Arrival time: ":
            cin >> at[i];
            if (a == 3) {
                cout << "Enter the priority of " << i + 1 << " process: ";
                cin >> pr[i];
            }
            processes[i] = i + 1;
        }

        base *b = nullptr;

        switch (a) {
            case 1: {
                FCFS f;
                b = &f;
                b->funFCFS(processes, bt, at, n);
                break;
            }
            case 2: {
                SJF s;
                b = &s;
                b->funSJF(processes, bt, at, n);
                break;
            }
            case 3: {
                PS p;
                b = &p;
                b->funPS(processes, bt, at, pr, n);
                break;
            }
            case 4: {
                RR r;
                b = &r;
                b->funRR(processes, bt, at, n);
                break;
            }
            case 0:{
                exit;
            }
            default:
                cout << "Invalid choice" << endl;
                delete b;
                continue;
        }

        delete b;

    } while (a!=0);

    return 0;
}