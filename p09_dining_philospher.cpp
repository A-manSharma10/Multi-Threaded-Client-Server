#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

class DiningPhilosophers {
private:
    enum { THINKING, HUNGRY, EATING } state[5];
    condition_variable self[5];
    mutex mtx;

public:
    void pickup(int i) {
        unique_lock<mutex> lock(mtx);
        state[i] = HUNGRY;
        test(i);
        if (state[i] != EATING)
            self[i].wait(lock);
    }

    void putdown(int i) {
        unique_lock<mutex> lock(mtx);
        state[i] = THINKING;
        // Test left and right neighbors
        test((i + 4) % 5);
        test((i + 1) % 5);
    }

private:
    void test(int i) {
        if ((state[(i + 4) % 5] != EATING) &&
            (state[i] == HUNGRY) &&
            (state[(i + 1) % 5] != EATING)) {
            state[i] = EATING;
            self[i].notify_one();
        }
    }
};

void philosopher(int id, DiningPhilosophers& table) {
    for (int i = 0; i < 3; ++i) { // Each philosopher eats 3 times
        table.pickup(id);
        cout << "Philosopher " << id << " is eating." << endl;
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulating eating
        table.putdown(id);
        cout << "Philosopher " << id << " finished eating." << endl;
        this_thread::sleep_for(chrono::milliseconds(100)); // Simulating thinking
    }
}

int main() {
    DiningPhilosophers table;

    thread philosophers[5];

    // Create philosophers
    for (int i = 0; i < 5; ++i) {
        philosophers[i] = thread(philosopher, i, ref(table));
    }

    // Join threads
    for (int i = 0; i < 5; ++i) {
        philosophers[i].join();
    }

    return 0;
}
