#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_PROCESSES = 100;
const int MAX_RESOURCES = 100;

class BankersAlgorithm {
private:
    int available[MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int maxNeed[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int numProcesses, numResources;

public:
    BankersAlgorithm(int nP, int nR) : numProcesses(nP), numResources(nR) {}

    void setAvailable(int resources[]) {
        for (int i = 0; i < numResources; ++i)
            available[i] = resources[i];
    }

    void setAllocation(int process, int resources[]) {
        for (int i = 0; i < numResources; ++i)
            allocation[process][i] = resources[i];
    }

    void setMaxNeed(int process, int resources[]) {
        for (int i = 0; i < numResources; ++i)
            maxNeed[process][i] = resources[i];
    }

    void calculateNeed() {
        for (int i = 0; i < numProcesses; ++i) {
            for (int j = 0; j < numResources; ++j) {
                need[i][j] = maxNeed[i][j] - allocation[i][j];
            }
        }
    }

    bool isSafe() {
        bool finish[numProcesses] = {false};
        int work[numResources];
        for (int i = 0; i < numResources; ++i) {
            work[i] = available[i];
        }

        int safeSequence[numProcesses];
        int count = 0;

        while (count < numProcesses) {
            bool found = false;
            for (int i = 0; i < numProcesses; ++i) {
                if (!finish[i]) {
                    bool canAllocate = true;
                    for (int j = 0; j < numResources; ++j) {
                        if (need[i][j] > work[j]) {
                            canAllocate = false;
                            break;
                        }
                    }
                    if (canAllocate) {
                        for (int j = 0; j < numResources; ++j) {
                            work[j] += allocation[i][j];
                        }
                        safeSequence[count++] = i;
                        finish[i] = true;
                        found = true;
                    }
                }
            }
            if (!found) {
                return false; // Unsafe state
            }
        }
        cout << "Safe Sequence: ";
        for (int i = 0; i < numProcesses; ++i) {
            cout << safeSequence[i] << " ";
        }
        cout << endl;
        return true; // Safe state
    }

    bool requestResources(int process, int request[]) {
        for (int i = 0; i < numResources; ++i) {
            if (request[i] > need[process][i] || request[i] > available[i]) {
                return false; // Process has exceeded its maximum claim
            }
        }
        for (int i = 0; i < numResources; ++i) {
            available[i] -= request[i];
            allocation[process][i] += request[i];
            need[process][i] -= request[i];
        }
        if (!isSafe()) {
            // Undo the allocation
            for (int i = 0; i < numResources; ++i) {
                available[i] += request[i];
                allocation[process][i] -= request[i];
                need[process][i] += request[i];
            }
            return false;
        }
        return true;
    }
};

int main() {
    int numProcesses, numResources;
    cout << "Enter number of processes: ";
    cin >> numProcesses;
    cout << "Enter number of resources: ";
    cin >> numResources;

    BankersAlgorithm banker(numProcesses, numResources);

    int resources[MAX_RESOURCES];
    cout << "Enter the available resources: ";
    for (int i = 0; i < numResources; ++i) {
        cin >> resources[i];
    }
    banker.setAvailable(resources);

    cout << "Enter allocation matrix: " << endl;
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            cin >> resources[j];
        }
        banker.setAllocation(i, resources);
    }

    cout << "Enter max need matrix: " << endl;
    for (int i = 0; i < numProcesses; ++i) {
        for (int j = 0; j < numResources; ++j) {
            cin >> resources[j];
        }
        banker.setMaxNeed(i, resources);
    }

    banker.calculateNeed();

    if (banker.isSafe()) {
        cout << "Safe state achieved!" << endl;
        int process;
        while (true) {
            cout << "Enter process number (-1 to exit): ";
            cin >> process;
            if (process == -1)
                break;
            int request[MAX_RESOURCES];
            cout << "Enter the request for process " << process << ": ";
            for (int i = 0; i < numResources; ++i) {
                cin >> request[i];
            }
            if (banker.requestResources(process, request)) {
                cout << "Request granted." << endl;
            } else {
                cout << "Request denied. Deadlock may occur." << endl;
            }
        }
    } else {
        cout << "Unsafe state! Deadlock detected." << endl;
    }

    return 0;
}
