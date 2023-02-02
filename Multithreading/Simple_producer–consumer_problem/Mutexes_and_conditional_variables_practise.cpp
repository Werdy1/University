#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <list>
#include <vector>

using namespace std;

list<int> buffer;
pthread_cond_t not_full;
pthread_cond_t not_empty;
pthread_mutex_t mlock;
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
		pthread_mutex_lock(&mlock);
		while (buffer.size() == bsize) {
			pthread_cond_wait(&not_full, &mlock);
		}
		append_to_buffer(item);
		pthread_cond_signal(&not_empty);
		pthread_mutex_unlock(&mlock);
		sleep(4);
	}
	return NULL;

}

void* consumer(void* data) {

	while (1) {
		int item;
		pthread_mutex_lock(&mlock);
		while (buffer.size() == 0) {
			pthread_cond_wait(&not_empty, &mlock);
		}
		item = receive_from_buffer();
		pthread_cond_signal(&not_full);
		pthread_mutex_unlock(&mlock);
		consume(item);
		sleep(3);
	}
	return NULL;
}

int main() {

	srand(time(NULL));
	int  n_prod, n_cons;
	cout << "Enter size of buffer:" << endl;
	cin >> bsize;
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
