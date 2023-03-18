#include <iostream>
#include "jthread.hpp"
#include <chrono>
#include <cstdlib>
#include <queue>
#include <mutex>
#include <pthread.h>

//int max_size = 100;
std::queue<int> tasktable;
std::queue<int> beertable;
pthread_mutex_t myMutex;
pthread_mutex_t beer_mutex;
pthread_mutex_t task_mutex;
pthread_cond_t	beer_is_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t	task_is_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t	beer_is_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t	task_is_full = PTHREAD_COND_INITIALIZER;


void project_manager(int id) {
    while(true){
        pthread_mutex_lock(&task_mutex);
        if(tasktable.size() < 100){
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
            std::cout<<"dodano task\n";
            tasktable.push(rand() % 10 + 1);
            pthread_cond_signal(&task_is_empty);
        }
        else{
            std::cout<<"czekam na wykonanie taska: "<<id<<std::endl;
            pthread_cond_wait(&task_is_full, &task_mutex);
        }
        pthread_mutex_unlock(&task_mutex);
    }
}

void beer_manager(int id) {
    while(true){
        pthread_mutex_lock(&beer_mutex);
        if(beertable.size() < 100){
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 1));
            std::cout<<"dodano piwo\n";
            beertable.push(rand() % 10 + 1);
            pthread_cond_signal(&beer_is_empty);
        }
        else{
            std::cout<<"czekam na wypicie piwa: "<<id<<std::endl;
            pthread_cond_wait(&beer_is_full, &beer_mutex);
        }
        pthread_mutex_unlock(&beer_mutex);
    }
}

void worker(int id) {
    while(true){
        pthread_mutex_lock(&myMutex);
        if (beertable.size() > 0) {
            if(tasktable.size() > 0){
                std::this_thread::sleep_for(std::chrono::milliseconds(1*beertable.front()*tasktable.front()));
                std::cout<<"wykonano task\n";
                beertable.pop();
                tasktable.pop();
                pthread_cond_signal(&task_is_full);
                pthread_cond_signal(&beer_is_full);
            }
            else{
                std::cout<<"czekam na taski: "<<id<<std::endl;
                pthread_cond_wait(&task_is_empty, &myMutex);
            }
          
        }
        else{
            std::cout<<"czekam na piwo: "<<id<<std::endl;
			pthread_cond_wait(&beer_is_empty, &myMutex);
        }
        pthread_mutex_unlock(&myMutex);
    }
}


int main(int argc, char *argv[]) {

    std::vector<std::jthread> workers;
    std::vector<std::jthread> project_managers;
    std::vector<std::jthread> beer_managers;

    for(int i = 0; i<*argv[0]; i++){
        workers.push_back(std::jthread(worker, i));
    }

    for(int i = 0; i<*argv[1]; i++){
        project_managers.push_back(std::jthread(project_manager, i));
    }

    for(int i = 0; i<*argv[2]; i++){
        beer_managers.push_back(std::jthread(beer_manager, i));
    }

    for(int i = 0; i<*argv[0]; i++){
        workers[i].join();
    }

    for(int i = 0; i<*argv[1]; i++){
        project_managers[i].join();
    }

    for(int i = 0; i<*argv[2]; i++){
        beer_managers[i].join();
    }
    
}