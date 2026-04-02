#include <iostream>
#include <algorithm> // For std::sort
#include <cstdlib>   // For abs()

using namespace std;

class Disk_Scheduling {
public:
    virtual int schedule(int requests[], int numRequests, int head) = 0;
};

class FCFS : public Disk_Scheduling {
public:
    int schedule(int requests[], int numRequests, int head) override {
        int totalSeekTime = 0;
        int currentPosition = head;

        for (int i = 0; i < numRequests; i++) {
            totalSeekTime += abs(requests[i] - currentPosition);
            currentPosition = requests[i];
        }

        return totalSeekTime;
    }
};

class SCAN : public Disk_Scheduling {
public:
    int schedule(int requests[], int numRequests, int head) override {
        int totalSeekTime = 0;
        int currentPosition = head;

        // Sort the requests
        sort(requests, requests + numRequests);

        // Find the position to start scanning (first request greater than or equal to head)
        int index = 0;
        while (index < numRequests && requests[index] < head) {
            index++;
        }

        // Scan towards the end of the disk
        for (int i = index; i < numRequests; i++) {
            totalSeekTime += abs(requests[i] - currentPosition);
            currentPosition = requests[i];
        }

        // If there are requests before the head position, scan towards the start of the disk
        if (index > 0) {
            totalSeekTime += abs(currentPosition - requests[index - 1]);
            currentPosition = requests[index - 1];

            for (int i = index - 2; i >= 0; i--) {
                totalSeekTime += abs(requests[i] - currentPosition);
                currentPosition = requests[i];
            }
        }

        return totalSeekTime;
    }
};

class C_SCAN : public Disk_Scheduling {
public:
    int schedule(int requests[], int numRequests, int head) override {
        int totalSeekTime = 0;
        int currentPosition = head;

        // Sort the requests
        sort(requests, requests + numRequests);

        // Find the position to start scanning (first request greater than or equal to head)
        int index = 0;
        while (index < numRequests && requests[index] < head) {
            index++;
        }

        // Scan towards the end of the disk
        for (int i = index; i < numRequests; i++) {
            totalSeekTime += abs(requests[i] - currentPosition);
            currentPosition = requests[i];
        }

        // If there are requests before the head position, wrap around to the start of the disk
        if (index > 0) {
            totalSeekTime += abs(currentPosition - requests[0]);
            currentPosition = requests[0];

            for (int i = 1; i < index; i++) {
                totalSeekTime += abs(requests[i] - currentPosition);
                currentPosition = requests[i];
            }
        }

        return totalSeekTime;
    }
};

int main() {
    int upperLimit, numRequests, head;

    cout << "Enter upper limit of cylinder: ";
    cin >> upperLimit;
    cout << "Enter number of disk requests: ";
    cin >> numRequests;

    int* requests = new int[numRequests];
    cout << "Enter the disk requests: ";
    for (int i = 0; i < numRequests; i++) {
        cin >> requests[i];
    }

    cout << "Enter the position of head: ";
    cin >> head;

    Disk_Scheduling* scheduler;

    cout << "\nChoose the scheduling algorithm:\n";
    cout << "1. FCFS\n";
    cout << "2. SCAN\n";
    cout << "3. C-SCAN\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            scheduler = new FCFS();
            break;
        case 2:
            scheduler = new SCAN();
            break;
        case 3:
            scheduler = new C_SCAN();
            break;
        default:
            cout << "Invalid choice!" << endl;
            delete[] requests;
            return 1;
    }

    int totalSeekTime = scheduler->schedule(requests, numRequests, head);
    cout << "Total seek time for the chosen scheduling algorithm is: " << totalSeekTime << endl;

    delete scheduler;
    delete[] requests;

    return 0;
}
