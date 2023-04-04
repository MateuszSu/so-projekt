#include <iostream>
#include "jthread.hpp"
#include <chrono>
#include <cstdlib>
#include <queue>
#include <mutex>
#include <pthread.h>
#include <ncurses.h>

std::queue<int> tasktable;
std::queue<int> beertable;
pthread_mutex_t myMutex;
pthread_mutex_t beer_mutex;
pthread_mutex_t task_mutex;
pthread_cond_t	beer_is_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t	task_is_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t	beer_is_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t	task_is_full = PTHREAD_COND_INITIALIZER;
int no_beer_managers;
int no_project_managers;
int no_workers;

void project_manager(int id) {
    while(true){
        pthread_mutex_lock(&task_mutex);
        if(tasktable.size() < 100){
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000 + 1));
            move(id,0);
            printw("status pm'a %d: dodano task                                          ", id);
            refresh();
            tasktable.push(rand() % 100 + 100);
            pthread_cond_signal(&task_is_empty);
        }
        else{
            move(id,0);
            printw("status pm'a %d: już zrobilem za dużo tasków                                          ", id);
            refresh();
            pthread_cond_wait(&task_is_full, &task_mutex);
        }
        pthread_mutex_unlock(&task_mutex);
    }
}

void beer_manager(int id) {
    while(true){
        pthread_mutex_lock(&beer_mutex);
        if(beertable.size() < 100){
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 4000 + 1));
            move(id+no_project_managers,0);
            printw("status bm'a %d: dodano piwo                                          ", id);
            refresh();
            beertable.push(rand() % 100 + 100);
            pthread_cond_signal(&beer_is_empty);

        }
        else{
            move(id+no_project_managers,0);
            printw("status bm'a %d: już polalem za dużo piw                                          ", id);
            refresh();
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
                std::this_thread::sleep_for(std::chrono::milliseconds((beertable.front()*tasktable.front()))/4);
                move(id+no_project_managers+no_beer_managers,0);
                printw("status pracownika %d: zrobil taska                                          ", id);
                refresh();
                beertable.pop();
                tasktable.pop();
                pthread_cond_signal(&task_is_full);
                pthread_cond_signal(&beer_is_full);
                refresh();
            }
            else{
                move(id+no_project_managers+no_beer_managers,0);
                printw("status pracownika %d: czeka na taska                                          ", id);
                refresh();
                pthread_cond_wait(&task_is_empty, &myMutex);
            }
          
        }
        else{
            move(id+no_workers+no_beer_managers,0);
            printw("status pracownika %d: czeka na piwo                                          ", id);
            refresh();
			pthread_cond_wait(&beer_is_empty, &myMutex);
        }
        pthread_mutex_unlock(&myMutex);
    }
}

int main(int argc, char **argv) {
    initscr();

    no_workers = std::stoi(std::string(argv[1]));
    no_project_managers = std::stoi(std::string(argv[2]));
    no_beer_managers = std::stoi(std::string(argv[3]));

    std::vector<std::jthread> workers;
    std::vector<std::jthread> project_managers;
    std::vector<std::jthread> beer_managers;

    for(int i = 0; i<std::stoi(std::string(argv[1])); i++){
        workers.push_back(std::jthread(worker, i));
    }

    for(int i = 0; i<std::stoi(std::string(argv[2])); i++){
        project_managers.push_back(std::jthread(project_manager, i));
    }

    for(int i = 0; i<std::stoi(std::string(argv[3])); i++){
        beer_managers.push_back(std::jthread(beer_manager, i));
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