#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>
#include <list>
#include <vector>
#include <time.h>

using namespace std;

vector<pthread_mutex_t> forks(5);

time_t my_time;

void think(int num) {
	cout << "Philosopher: " << num << " thinking..." << endl;
	sleep(3 + rand() % 4);
}

void* live(void* data) {

	int spagetti_count = 2;
	int num = *((int*)data);
	int times_before_dead = 4;
	int chance = 4;

	while (1) {
		int temp = rand() % 2;
		int first = (num + temp) % 5;
		int second = (num + (1 - temp)) % 5;
		int check = 1;

		cout << ctime(&my_time) << " Philosopher: " << num << " trying to take fork " << first << " " << endl;

		if (pthread_mutex_trylock(&forks[first]) == 0) {

			sleep(rand() % 3);
			cout << ctime(&my_time) << " Philosopher: " << num << " successfully taked fork " << first << " " << endl;
			chance = 4;

			do {

				if (rand() % chance == 0) {
					check = 0;
					cout << ctime(&my_time) << " Philosopher: " << num << " put back fork " << first << " " << endl;
					pthread_mutex_unlock(&forks[first]);
					if (times_before_dead == 0) {
						cout << ctime(&my_time) << " Philosopher: " << num << " is dead " << endl;
						return NULL;
					}
					times_before_dead--;
					sleep(rand() % 5);
				}
				else {

					cout << ctime(&my_time) << " Philosopher: " << num << " trying to take fork " << second << " " << endl;

					if ((pthread_mutex_trylock(&forks[second]) == 0)) {
						check = 0;

						if (spagetti_count == 0) {
							cout << ctime(&my_time) << " Philosopher: " << num << " end all his spagetti " << endl;
							pthread_mutex_unlock(&forks[first]);
							pthread_mutex_unlock(&forks[second]);
							return NULL;
						}
						cout << ctime(&my_time) << " Philosopher: " << num << " is eating " << endl;
						pthread_mutex_unlock(&forks[first]);
						pthread_mutex_unlock(&forks[second]);
						spagetti_count--;
						sleep(1 + rand() % 2);
						think(num);

					}
					else {
						cout << ctime(&my_time) << " Philosopher: " << num << " can't take fork " << second << " " << endl;
						chance--;
						sleep(2 + rand() % 4);

					}
				}
			} while (check);

		}
		else {

			cout << ctime(&my_time) << " Philosopher: " << num << " can't take fork " << first << " " << endl;

			if (times_before_dead == 0) {
				cout << ctime(&my_time) << " Philosopher: " << num << " is dead " << endl;
				return NULL;
			}
			times_before_dead--;
			sleep(rand() % 5);
		}
	}
	return NULL;
}

int main() {

	srand(time(NULL));
	time_t my_time = time(NULL);
	vector<pthread_t> philosopher(5);
	int nums[5] = { 0,1,2,3,4 };
	for (int i = 0; i < 5; i++) {
		pthread_create(&philosopher[i], NULL, live, (void*)&nums[i]);
	}

	for (int i = 0; i < 5; i++) {

		pthread_join(philosopher[i], NULL);
	}

	return 0;
}
