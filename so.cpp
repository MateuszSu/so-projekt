#include <iostream>
#include "jthread.hpp"
#include <chrono>

void worker(int id, int duration) {
    std::cout << "Thread " << id << " started" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    std::cout << "Thread " << id << " finished" << std::endl;
}

int main() {
    // Create two jthreads
    std::jthread thread1(worker, 1, 500);
    std::jthread thread2(worker, 2, 1000);

    // Wait for the threads to finish
    thread1.join();
    thread2.join();

    std::cout << "All threads finished" << std::endl;

    return 0;
}