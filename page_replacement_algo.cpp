#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

class PageReplacement {
public:
    virtual int replacePage(int pages[], int n, int capacity) = 0;

    bool checkBeladyAnomaly(int pages[], int n) {
        int previousPageFaults = -1;
        for (int capacity = 1; capacity <= n; capacity++) {
            int currentPageFaults = replacePage(pages, n, capacity);
            if (previousPageFaults != -1 && currentPageFaults > previousPageFaults) {
                return true;
            }
            previousPageFaults = currentPageFaults;
        }
        return false;
    }
};

class FIFO : public PageReplacement {
public:
    int replacePage(int pages[], int n, int capacity) override {
        list<int> pageList;
        int pageFaults = 0;

        for (int i = 0; i < n; i++) {
            auto it = find(pageList.begin(), pageList.end(), pages[i]);
            if (it == pageList.end()) { // Page fault
                if (pageList.size() == capacity) {
                    pageList.pop_front();
                }
                pageList.push_back(pages[i]);
                pageFaults++;
            }
        }
        return pageFaults;
    }
};

class LRU : public PageReplacement {
public:
    int replacePage(int pages[], int n, int capacity) override {
        list<int> pageList;
        int pageFaults = 0;

        for (int i = 0; i < n; i++) {
            auto it = find(pageList.begin(), pageList.end(), pages[i]);
            if (it == pageList.end()) { // Page fault
                if (pageList.size() == capacity) {
                    pageList.pop_back();
                }
                pageList.push_front(pages[i]);
                pageFaults++;
            } else {
                pageList.erase(it);
                pageList.push_front(pages[i]);
            }
        }
        return pageFaults;
    }
};

class Optimal : public PageReplacement {
public:
    int replacePage(int pages[], int n, int capacity) override {
        list<int> pageList;
        int pageFaults = 0;

        for (int i = 0; i < n; i++) {
            auto it = find(pageList.begin(), pageList.end(), pages[i]);
            if (it == pageList.end()) { // Page fault
                if (pageList.size() == capacity) {
                    int farthest = i + 1, val = -1;
                    for (int page : pageList) {
                        int j;
                        for (j = i + 1; j < n; j++) {
                            if (page == pages[j]) break;
                        }
                        if (j > farthest) {
                            farthest = j;
                            val = page;
                        }
                    }
                    if (val == -1) val = pageList.front();
                    pageList.remove(val);
                }
                pageList.push_back(pages[i]);
                pageFaults++;
            }
        }
        return pageFaults;
    }
};

int main() {
    int choice, capacity,n;

    cout << "Enter the number of pages: ";
    cin >> n;
    int pages[n];

    cout << "Enter the pages: ";
    for (int i = 0; i < n; i++) {
        cin >> pages[i];
    }

    cout << "Enter the capacity of the memory: ";
    cin >> capacity;

    PageReplacement* algo;
    int fifoFaults, lruFaults, optimalFaults;

    while (true) {
        cout << "\nChoose Page Replacement Algorithm:\n";
        cout << "1. FIFO\n2. LRU\n3. Optimal\n4. Compare All\n5. Exit\n";
        cin >> choice;

        switch (choice) {
            case 1:
                algo = new FIFO();
                fifoFaults = algo->replacePage(pages, n, capacity);
                cout << "FIFO Page Faults: " << fifoFaults << endl;
                if (algo->checkBeladyAnomaly(pages, n)) {
                    cout << "Belady's Anomaly detected in FIFO.\n";
                } else {
                    cout << "No Belady's Anomaly in FIFO.\n";
                }
                delete algo;
                break;
            case 2:
                algo = new LRU();
                lruFaults = algo->replacePage(pages, n, capacity);
                cout << "LRU Page Faults: " << lruFaults << endl;
                if (algo->checkBeladyAnomaly(pages, n)) {
                    cout << "Belady's Anomaly detected in LRU.\n";
                } else {
                    cout << "No Belady's Anomaly in LRU.\n";
                }
                delete algo;
                break;
            case 3:
                algo = new Optimal();
                optimalFaults = algo->replacePage(pages, n, capacity);
                cout << "Optimal Page Faults: " << optimalFaults << endl;
                if (algo->checkBeladyAnomaly(pages, n)) {
                    cout << "Belady's Anomaly detected in Optimal.\n";
                } else {
                    cout << "No Belady's Anomaly in Optimal.\n";
                }
                delete algo;
                break;
            case 4:
                algo = new FIFO();
                fifoFaults = algo->replacePage(pages, n, capacity);
                cout << "FIFO Page Faults: " << fifoFaults << endl;
                if (algo->checkBeladyAnomaly(pages, n)) {
                    cout << "Belady's Anomaly detected in FIFO.\n";
                } else {
                    cout << "No Belady's Anomaly in FIFO.\n";
                }
                delete algo;

                algo = new LRU();
                lruFaults = algo->replacePage(pages, n, capacity);
                cout << "LRU Page Faults: " << lruFaults << endl;
                if (algo->checkBeladyAnomaly(pages, n)) {
                    cout << "Belady's Anomaly detected in LRU.\n";
                } else {
                    cout << "No Belady's Anomaly in LRU.\n";
                }
                delete algo;

                algo = new Optimal();
                optimalFaults = algo->replacePage(pages, n, capacity);
                cout << "Optimal Page Faults: " << optimalFaults << endl;
                if (algo->checkBeladyAnomaly(pages, n)) {
                    cout << "Belady's Anomaly detected in Optimal.\n";
                } else {
                    cout << "No Belady's Anomaly in Optimal.\n";
                }
                delete algo;

                if (fifoFaults <= lruFaults && fifoFaults <= optimalFaults) {
                    cout << "Best Algorithm: FIFO\n";
                } else if (lruFaults <= fifoFaults && lruFaults <= optimalFaults) {
                    cout << "Best Algorithm: LRU\n";
                } else {
                    cout << "Best Algorithm: Optimal\n";
                }
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid choice! Please choose again.\n";
        }
    }

    return 0;
}
