#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

const int BUFFER_SIZE = 5;
];
int front = 0, rear = 0;

mutex mtx;
condition_variable full, empty;

void produce(int item) {
    // produce an item
    // For simplicity, we'll just use the item passed as an argument
    // In a real scenario, you'd generate the item to be produced

    unique_lock<mutex> lock(mtx);
    empty.wait(lock, [] { return (rear + 1) % BUFFER_SIZE != front; }); // wait until buffer is not full
    buffer[rear] = item; // add item to buffer
    rear = (rear + 1) % BUFFER_SIZE;
    cout << "Produced: " << item << endl;
    full.notify_all(); // signal that buffer is not empty anymore
}

int consume() {
    int item;
    unique_lock<mutex> lock(mtx);
    full.wait(lock, [] { return front != rear; }); // wait until buffer is not empty
    item = buffer[front]; // remove item from buffer
    front = (front + 1) % BUFFER_SIZE;
    cout << "Consumed: " << item << endl;
    empty.notify_all(); // signal that buffer is not full anymore
    return item;
}

void producer() {
    for (int i = 0; i < 7; ++i) {
        produce(i);
    }
}

void consumer() {
    for (int i = 0; i < 7; ++i) {
        consume();
    }
}

int main() {
    thread producerThread(producer);
    thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
