#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>
#include <vector>
#include <sys/wait.h>
#include <sys/shm.h>
#include <fstream>
#include <cstring>
#include <fcntl.h> 

#define lockname "/locksem"
#define readyname "/readysem"

using namespace std;

pid_t pid[4];
int status;
char* sh_mem;
int segment_id;
vector<string> answer(12);
sem_t* lock, * ready;

void child(int id) {
    fstream file;
    file.open("sovn.txt", fstream::in);
    for (int i = id; i < 13; i += 4) {
        file.seekg(file.beg);
        string temp;
        int current_line = 0;
        while (current_line != i) {
            getline(file, temp);
            current_line++;
        }
        sem_wait(lock);
        strcpy(sh_mem, temp.c_str());
        sem_post(ready);
    }
}

void batya() {
    for (int i = 0; i < 12; i++) {
        sem_wait(ready);
        string temp = sh_mem;
        string number;
        for (char x : temp) {
            if (x == ' ') {
                break;
            }
            number += x;
        }
        int num = stoi(number);
        answer[num - 1] = temp;
        sem_post(lock);
    }
}

int main() {

    lock = sem_open(lockname, O_CREAT, 0666, 1);
    ready = sem_open(readyname, O_CREAT, 0666, 0);

    segment_id = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    sh_mem = (char*)shmat(segment_id, 0, 0);
    for (int i = 0; i < 4; i++) {
        pid[i] = fork();
        if (pid[i] == 0) {
            child(4 - i);
            exit(0);
        }
    }
    batya();
    for (int i = 0; i < 4; i++) {
        wait(&status);
    }

    shmdt(sh_mem);
    shmctl(segment_id, IPC_RMID, 0);
    sem_unlink(lockname);
    sem_unlink(readyname);
    for (string a : answer) {
        cout << a << endl;
    }

    return 0;
}