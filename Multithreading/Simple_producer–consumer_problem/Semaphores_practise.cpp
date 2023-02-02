#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>
#include <list>
#include <vector>

using namespace std;

sem_t lock, empty, full;
list<int> buffer;
int bsize;
int produce() {
	return 100 + rand() % 900;
}
void append_to_buffer(int item) {
	buffer.push_back(item);
}

void consume(int item) {
	cout << "Consumed item: " << item << " Thread: " << pthread_self() << " Time: " << time(NULL) << endl;
}

int receive_from_buffer() {
	if (buffer.size() > 0) {
		int temp = buffer.back();
		buffer.pop_back();
		return temp;
	}
	else {
		return -1;
	}

}

void* producer(void* data) {

	while (1) {
		int item = produce();
		sem_wait(&empty);
		sem_wait(&lock);
		append_to_buffer(item);
		sem_post(&lock);
		sem_post(&full);
		sleep(10);
	}
	return NULL;

}

void* consumer(void* data) {

	while (1) {
		int item;
		sem_wait(&full);
		sem_wait(&lock);
		item = receive_from_buffer();
		sem_post(&lock);
		sem_post(&empty);
		consume(item);
		sleep(6);
	}
	return NULL;
}

int main() {

	srand(time(NULL));
	int  n_prod, n_cons;
	cout << "Enter size of buffer:" << endl;
	cin >> bsize;
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, bsize);
	sem_init(&lock, 0, 1);
	cout << "Enter number of producers:" << endl;
	cin >> n_prod;
	vector<pthread_t> prod(n_prod);
	cout << "Enter number of consumers:" << endl;
	cin >> n_cons;
	vector<pthread_t> cons(n_cons);

	for (int i = 0; i < n_prod; i++) {

		pthread_create(&prod[i], NULL, producer, NULL);
	}

	for (int i = 0; i < n_cons; i++) {

		pthread_create(&cons[i], NULL, consumer, NULL);
	}

	for (int i = 0; i < n_prod; i++) {

		pthread_join(prod[i], NULL);
	}

	for (int i = 0; i < n_cons; i++) {

		pthread_join(cons[i], NULL);
	}

	return 0;
}
