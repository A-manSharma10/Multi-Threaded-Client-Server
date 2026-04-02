#include <iostream>
using namespace std;

// Process structure
struct Process {
    int pid; // Process ID
    int burst_time; // Burst time
    int priority; // Priority
};

// Function to swap two processes
void swap(Process &a, Process &b) {
    Process temp = a;
    a = b;
    b = temp;
}

// Function to sort processes based on priority
void sortProcesses(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].priority > processes[j + 1].priority) {
                swap(processes[j], processes[j + 1]);
            }
        }
    }
}

// Function to implement Round Robin scheduling algorithm
void roundRobin(Process processes[], int n, int time_quantum) {
    int remaining_burst[n];
    for (int i = 0; i < n; i++) {
        remaining_burst[i] = processes[i].burst_time;
    }

    int t = 0; // Current time
    while (true) {
        bool done = true;
        for (int i = 0; i < n; i++) {
            if (remaining_burst[i] > 0) {
                done = false;
                if (remaining_burst[i] > time_quantum) {
                    t += time_quantum;
                    remaining_burst[i] -= time_quantum;
                } else {
                    t += remaining_burst[i];
                    remaining_burst[i] = 0;
                    cout << "Process " << processes[i].pid << " finished at time " << t << endl;
                }
            }
        }
        if (done) break;
    }
}

// Function to implement FCFS scheduling algorithm
void fcfs(Process processes[], int n) {
    int t = 0; // Current time
    for (int i = 0; i < n; i++) {
        cout << "Process " << processes[i].pid << " finished at time " << t + processes[i].burst_time << endl;
        t += processes[i].burst_time;
    }
}

int main() {
    int num_queues, num_processes, high_priority_ratio, low_priority_ratio;

    cout << "Enter the number of queues: ";
    cin >> num_queues;

    cout << "Enter the number of processes: ";
    cin >> num_processes;

    cout << "Enter the ratio to divide the number of queues in batch of high priority queues and low priority queues (e.g., 60 40): ";
    cin >> high_priority_ratio >> low_priority_ratio;

    Process processes[num_processes];
    // Input processes
    for (int i = 0; i < num_processes; i++) {
        cout << "Enter burst time and priority of process " << i + 1 << ": ";
        cin >> processes[i].burst_time >> processes[i].priority;
        processes[i].pid = i + 1;
    }

    int number_of_high_priority_queues = (num_queues * high_priority_ratio) / 100;              //3
    int number_of_low_priority_queues = num_queues - number_of_high_priority_queues;            //2


    // Sort processes based on priority
    sortProcesses(processes, num_processes);                                                   // sort on priority
                                                                                               // processes array ko

    // Distribute processes into high and low priority queues
    Process high_priority_processes[num_processes];
    Process low_priority_processes[num_processes];
    int high_priority_count = 0, low_priority_count = 0;

    for (int i = 0; i < num_processes; i++) {
        if (i < (num_processes * high_priority_ratio) / 100) {
            high_priority_processes[high_priority_count++] = processes[i];                  //high_priority_count = 6
        } else {                                                                            //low_priority_count = 4
            low_priority_processes[low_priority_count++] = processes[i];                    //high_priority_processes mei starting 6 sorted process aa gye
        }                                                                                   //low_priority_processes mei ending sorted process aa gye
    }

    // Sort high priority processes based on burst time
    for (int i = 0; i < high_priority_count - 1; i++) {
        for (int j = 0; j < high_priority_count - i - 1; j++) {
            if (high_priority_processes[j].burst_time > high_priority_processes[j + 1].burst_time) {
                swap(high_priority_processes[j], high_priority_processes[j + 1]);
            }
        }
    }

    // calculating no. of processes per queue
    int processes_per_queue_high = high_priority_count / number_of_high_priority_queues;
    int remaining_high_priority = high_priority_count;



    Process high_priority_queues[number_of_high_priority_queues][processes_per_queue_high];
    Process low_priority_queues[number_of_low_priority_queues];



    // Distribute high priority processes into high priority queues
    for (int i = 0; i < number_of_high_priority_queues; i++) {
        cout << "High Priority Queue " << i + 1 << ": ";
        for (int j = 0; j < processes_per_queue_high; j++) {
            cout << high_priority_processes[i * processes_per_queue_high + j].pid << " ";
        }
        cout << endl;
        remaining_high_priority -= processes_per_queue_high;
    }

    // Distribute remaining high priority processes
    int index = 0;
    for (int i = 0; i < remaining_high_priority; i++) {
        cout << "High Priority Queue " << i + 1 << ": ";
        cout << high_priority_processes[high_priority_count - remaining_high_priority + i].pid << " ";
        cout << endl;
        index++;
    }

    // Sort low priority processes based on burst time
    for (int i = 0; i < low_priority_count - 1; i++) {
        for (int j = 0; j < low_priority_count - i - 1; j++) {
            if (low_priority_processes[j].burst_time > low_priority_processes[j + 1].burst_time) {
                swap(low_priority_processes[j], low_priority_processes[j + 1]);
            }
        }
    }

    // Distribute low priority processes into low priority queues
    int processes_per_queue_low = low_priority_count / number_of_low_priority_queues;
    int remaining_low_priority = low_priority_count;
    for (int i = 0; i < number_of_low_priority_queues; i++) {
        cout << "Low Priority Queue " << i + 1 << ": ";
        for (int j = 0; j < processes_per_queue_low; j++) {
            cout << low_priority_processes[i * processes_per_queue_low + j].pid << " ";
        }
        cout << endl;
        remaining_low_priority -= processes_per_queue_low;
    }

    // Distribute remaining low priority processes
    index = 0;
    for (int i = 0; i < remaining_low_priority; i++) {
        cout << "Low Priority Queue " << i + 1 << ": ";
        cout << low_priority_processes[low_priority_count - remaining_low_priority + i].pid << " ";
        cout << endl;
        index++;
    }

    // Implement scheduling algorithms
    int time_quantum;
    cout << "Enter time quantum for Round Robin: ";
    cin >> time_quantum;

    cout << "Scheduling in High Priority Queues using Round Robin:\n";
    for (int i = 0; i < number_of_high_priority_queues; i++) {
        cout << "High Priority Queue " << i + 1 << ":\n";
        roundRobin(high_priority_processes + i * processes_per_queue_high, processes_per_queue_high, time_quantum);
    }

    cout << "Scheduling in Low Priority Queues using FCFS:\n";
    for (int i = 0; i < number_of_low_priority_queues; i++) {
        cout << "Low Priority Queue " << i + 1 << ":\n";
        fcfs(low_priority_processes + i * processes_per_queue_low, processes_per_queue_low);
    }

    return 0;
}