#include<iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

class Disk_Scheduling {
public:
    virtual int FCFS_schedule(int requests[], int numRequests, int head) = 0;
    virtual int SCAN_schedule(int requests[], int numRequests, int head, int upperLimit) = 0;
    virtual int CSCAN_schedule(int requests[], int numRequests, int head, int upperLimit) = 0;
};

class FCFS : public Disk_Scheduling {
public:
    int FCFS_schedule(int requests[], int numRequests, int head) override {
        int SeekTime = 0;
        int Pos = head;

        for (int i = 0; i < numRequests; i++) {
            SeekTime += abs(requests[i] - Pos);
            Pos = requests[i];
        }

        return SeekTime;
    }

    int SCAN_schedule(int requests[], int numRequests, int head, int upperLimit) override {
        return -1; // Not applicable for FCFS class
    }

    int CSCAN_schedule(int requests[], int numRequests, int head, int upperLimit) override {
        return -1; // Not applicable for FCFS class
    }
};

class SCAN : public Disk_Scheduling {
public:
    int FCFS_schedule(int requests[], int numRequests, int head) override {
        return -1; // Not applicable for SCAN class
    }

    int SCAN_schedule(int requests[], int numRequests, int head, int upperLimit) override {
        int SeekTime = 0;
        int Pos = head;

        sort(requests, requests + numRequests);

        int i;
        for (i = 0; i < numRequests; i++) {
            if (requests[i] >= head) break;
        }

        for (int j = i; j < numRequests; j++) {
            SeekTime += abs(requests[j] - Pos);
            Pos = requests[j];
        }

        if (Pos != upperLimit - 1) {
            SeekTime += abs(upperLimit - 1 - Pos);
            Pos = upperLimit - 1;
        }

        for (int j = i - 1; j >= 0; j--) {
            SeekTime += abs(requests[j] - Pos);
            Pos = requests[j];
        }

        return SeekTime;
    }

    int CSCAN_schedule(int requests[], int numRequests, int head, int upperLimit) override {
        return -1; // Not applicable for SCAN class
    }
};

class CSCAN : public Disk_Scheduling {
public:
    int FCFS_schedule(int requests[], int numRequests, int head) override {
        return -1; // Not applicable for CSCAN class
    }

    int SCAN_schedule(int requests[], int numRequests, int head, int upperLimit) override {
        return -1; // Not applicable for CSCAN class
    }

    int CSCAN_schedule(int requests[], int numRequests, int head, int upperLimit) override {
        int SeekTime = 0;
        int Pos = head;

        sort(requests, requests + numRequests);

        int i;
        for (i = 0; i < numRequests; i++) {
            if (requests[i] >= head) break;
        }

        for (int j = i; j < numRequests; j++) {
            SeekTime += abs(requests[j] - Pos);
            Pos = requests[j];
        }

        if (Pos != upperLimit - 1) {
            SeekTime += abs(upperLimit - 1 - Pos);
            Pos = upperLimit - 1;
        }

        SeekTime += upperLimit - 1;  // wrap around to 0

        Pos = 0;

        for (int j = 0; j < i; j++) {
            SeekTime += abs(requests[j] - Pos);
            Pos = requests[j];
        }

        return SeekTime;
    }
};

int main() {
    int upperLimit, numRequests, head, choice, time;
    cout << "Enter upper limit of cylinder: ";
    cin >> upperLimit;
    cout << "Enter number of disk requests: ";
    cin >> numRequests;

    int requests[numRequests];
    cout << "Enter the disk requests: ";
    for (int i = 0; i < numRequests; i++) {
        cin >> requests[i];
    }

    cout << "Enter the position of head: ";
    cin >> head;
    Disk_Scheduling *scheduler = nullptr;

    cout << "\nChoose the scheduling algorithm:\n";
    cout << "1. FCFS\n";
    cout << "2. SCAN\n";
    cout << "3. Circular SCAN\n";
    cout << "4. Compare all and select the best\n";
    cout << "Enter your choice: ";
    cin >> choice;

    FCFS fcfs;
    SCAN scan;
    CSCAN cscan;

    int fcfs_time = fcfs.FCFS_schedule(requests, numRequests, head);
    int scan_time = scan.SCAN_schedule(requests, numRequests, head, upperLimit);
    int cscan_time = cscan.CSCAN_schedule(requests, numRequests, head, upperLimit);

    switch (choice) {
        case 1:
            cout << "Total Seek Time (FCFS): " << fcfs_time << endl;
            break;
        case 2:
            cout << "Total Seek Time (SCAN): " << scan_time << endl;
            break;
        case 3:
            cout << "Total Seek Time (C-SCAN): " << cscan_time << endl;
            break;
        case 4:
            cout << "FCFS Seek Time: " << fcfs_time << endl;
            cout << "SCAN Seek Time: " << scan_time << endl;
            cout << "C-SCAN Seek Time: " << cscan_time << endl;

            if (fcfs_time <= scan_time && fcfs_time <= cscan_time) {
                cout << "Best algorithm is FCFS with Seek Time: " << fcfs_time << endl;
            } else if (scan_time <= fcfs_time && scan_time <= cscan_time) {
                cout << "Best algorithm is SCAN with Seek Time: " << scan_time << endl;
            } else {
                cout << "Best algorithm is C-SCAN with Seek Time: " << cscan_time << endl;
            }
            break;
        default:
            cout << "Invalid choice!" << endl;
    }

    return 0;
}
