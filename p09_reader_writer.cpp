#include <iostream>

using namespace std;

int readers = 0;
bool writing = false;

void reader(int id) {
    while (true) {
        while (writing); // Wait if a writer is writing
        ++readers;
        // Reading section
        cout << "Reader " << id << " is reading." << endl;
        --readers;
    }
}

void writer(int id) {
    while (true) {
        while (writing || readers > 0); // Wait if a reader is reading or a writer is writing
        writing = true;
        // Writing section
        cout << "Writer " << id << " is writing." << endl;
        writing = false;
    }
}

int main() {
    for (int i = 0; i < 2; ++i) { // Run two reader threads
        reader(i + 1);
    }

    writer(1); // Run the writer thread

    return 0;
}
