#include <iostream>
#include "jthread.hpp"
#include <chrono>
#include <cstdlib>
#include <queue>
#include <mutex>

std::queue<int> timetable;
std::queue<int> beertable;
std::mutex myMutex;

void project_manager(int id) {
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        timetable.push(rand() % 10 + 1);
        std::cout << "PM id: " << id << std::endl;
    }
}

void beer_manager(int id) {
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        beertable.push(rand() % 10 + 1);
        std::cout << "Beer id: " << id << std::endl;
    }
}

void worker(int id) {
    while(true){
        if (beertable.size() > 0 && timetable.size() > 0) {
          // std::unique_lock<std::mutex> ul(myMutex);
          std::cout << "Worker:" << beertable.front() * timetable.front() << " id: " << id << std::endl;
          beertable.pop();
          timetable.pop();
          // ul.unlock();
        }
    }
}

void printQueue() {
  while (true) {
    std::cout << timetable.size() << std::endl;
  }
}

int main() {
    std::jthread worker_1(worker, 1);
    std::jthread worker_2(worker, 2);
    std::jthread worker_3(worker, 3);

    std::jthread pm_1(project_manager, 1);
    std::jthread pm_2(project_manager, 2);
    std::jthread pm_3(project_manager, 3);

    std::jthread bm_1(beer_manager, 1);
    std::jthread bm_2(beer_manager, 2);
    std::jthread bm_3(beer_manager, 3);

    // std::jthread pr(printQueue);

    // pr.join();

    pm_1.join();
    pm_2.join();
    pm_3.join();

    bm_1.join();
    bm_2.join();
    bm_3.join();

    worker_1.join();
    worker_2.join();
    worker_3.join();

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