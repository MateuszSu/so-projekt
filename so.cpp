#include <iostream>
#include "jthread.hpp"
#include <chrono>
#include <cstdlib>

int task_time = 5;
int beer = 5;

void project_manager() {
    while(true){
        if(task_time==0){
        task_time = rand() % 10 + 1;
        std::cout << "\nProject manager is sleeping for 2000 seconds";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }
}
void worker() {
    while(true){
        std::cout << "\nWorker is sleeping for " << 1000*task_time*beer << " seconds";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000*task_time*beer));
        task_time = 0;
        beer = 0;
    }
}
void beer_manager() {
    while(true){
        if(beer==0){
        beer = rand() % 10 + 1;
        std::cout << "\nBeer manager is sleeping for " << 1000*beer << " seconds";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000*beer));
        }
    }
    
}

int main() {
    // Create two jthreads
    std::jthread thread1(worker);
    std::jthread thread2(project_manager);
    std::jthread thread3(beer_manager);

    // Wait for the threads to finish
    thread1.join();
    thread2.join();
    thread3.join();

    std::cout << "All threads finished" << std::endl;

    return 0;
}
/*
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
using namespace std;

std::mutex g_mutex;
bool g_ready = false;
int g_data = 0;

int produceData() {
  int randomNumber = rand() % 1000;
  std::cout << "produce data: " << randomNumber << "\n";
  return randomNumber;
}

void consumeData(int data) { std::cout << "receive data: " << data << "\n"; }

// consumer thread function
void consumer() {
  while (true) {
    while (!g_ready) {
      // sleep for 1 second
      std::this_thread::sleep_for (std::chrono::seconds(1));
    }
    std::unique_lock<std::mutex> ul(g_mutex);
    consumeData(g_data);
    g_ready = false;
  }
}

// producer thread function
void producer() {
  while (true) {
    std::unique_lock<std::mutex> ul(g_mutex);

    g_data = produceData();
    g_ready = true;
    ul.unlock();
    while (g_ready) {
      // sleep for 1 second
      std::this_thread::sleep_for (std::chrono::seconds(1));
    }
  }
}

void consumerThread() { consumer(); }

void producerThread() { producer(); }

int main() {
  std::thread t1(consumerThread);
  std::thread t2(producerThread);
  t1.join();
  t2.join();
  return 0;
}
*/