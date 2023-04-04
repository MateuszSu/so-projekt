#include <iostream>
#include "jthread.hpp"
#include <chrono>
#include <cstdlib>
#include <queue>
#include <mutex>
#include <pthread.h>

std::queue<int> taskQueue;
std::queue<int> beerQueue;
pthread_mutex_t workerMutex;
pthread_mutex_t beerMutex;
pthread_mutex_t taskMutex;
pthread_cond_t	beerIsEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t	taskIsEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t	beerIsFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t	taskIsFull = PTHREAD_COND_INITIALIZER;

void project_manager(int id) {
    while(true){
        pthread_mutex_lock(&taskMutex);
        if(taskQueue.size() < 100){
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
            std::cout<<"dodano task\n";
            taskQueue.push(rand() % 10 + 1);
            pthread_cond_signal(&taskIsEmpty);
        }
        else{
            std::cout<<"czekam na wykonanie taska: "<<id<<std::endl;
            pthread_cond_wait(&taskIsFull, &taskMutex);
        }
        pthread_mutex_unlock(&taskMutex);
    }
}

void beer_manager(int id) {
    while(true){
        pthread_mutex_lock(&beerMutex);
        if(beerQueue.size() < 100){
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
            std::cout<<"dodano piwo\n";
            beerQueue.push(rand() % 10 + 1);
            pthread_cond_signal(&beerIsEmpty);
        }
        else{
            std::cout<<"czekam na wypicie piwa: "<<id<<std::endl;
            pthread_cond_wait(&beerIsFull, &beerMutex);
        }
        pthread_mutex_unlock(&beerMutex);
    }

}

void worker(int id) {
    while(true){
        pthread_mutex_lock(&workerMutex);
        if (!beerQueue.empty()) {
            if(!taskQueue.empty()){
                std::this_thread::sleep_for(std::chrono::milliseconds(1 * beerQueue.front() * taskQueue.front()));
                std::cout<<"wykonano task\n";
                beerQueue.pop();
                taskQueue.pop();
                pthread_cond_signal(&taskIsFull);
                pthread_cond_signal(&beerIsFull);
            }
            else{
                std::cout<<"czekam na taski: "<<id<<std::endl;
                pthread_cond_wait(&taskIsEmpty, &workerMutex);
            }
          
        }
        else{
            std::cout<<"czekam na piwo: "<<id<<std::endl;
			pthread_cond_wait(&beerIsEmpty, &workerMutex);
        }
        pthread_mutex_unlock(&workerMutex);
    }
}

int main(int argc, char **argv) {
    std::vector<std::jthread> workers;
    std::vector<std::jthread> project_managers;
    std::vector<std::jthread> beer_managers;

    for(int i = 0; i<std::stoi(std::string(argv[1])); i++){
        workers.emplace_back(worker, i);
    }

    for(int i = 0; i<std::stoi(std::string(argv[2])); i++){
        project_managers.emplace_back(project_manager, i);
    }

    for(int i = 0; i<std::stoi(std::string(argv[3])); i++){
        beer_managers.emplace_back(beer_manager, i);
    }

    for(int i = 0; i<std::stoi(std::string(argv[1])); i++){
        workers[i].join();
    }

    for(int i = 0; i<std::stoi(std::string(argv[2])); i++){
        project_managers[i].join();
    }

    for(int i = 0; i<std::stoi(std::string(argv[3])); i++){
        beer_managers[i].join();
    }
}