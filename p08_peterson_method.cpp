#include <iostream>
#include <fstream>

using namespace std;

int turn;
bool flag[2];

void critical_section(int process) {
    ofstream file;
    file.open("source.txt", ios::app); 
    if (file.is_open()) {
        file << "Process " << process << " is in critical section.\n";
        file.close();
    } else {
        cout << "Error opening file." << endl;
    }
}

void peterson(int process) {
    int other = 1 - process;
    flag[process] = true;
    turn = process;
    while (flag[other] && turn == process);
    critical_section(process);
    flag[process] = false;
}

int main() {
    flag[0] = false;
    flag[1] = false;

    // Simulate two processes
    peterson(0);
    peterson(1);

    return 0;
}
