#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/resource.h>
typedef struct s_thread_info{
	void* n_count;
	int add_amount;
}thread_info;
void add(long long* pointer, long long value) {
	long long sum = *pointer + value;
	*pointer = sum;
}
void thread_action(void* arg){
	thread_info t = *(thread_info*)arg;
	long long* pointer = t.n_count;
	int value = t.add_amount;
	add(pointer, value);	
	add(pointer, -value);
}

long long counter = 0;
struct timespec timer;
static struct option long_options[] = {
		{ "--threads", required_argument, 0, 'a' },
		{ "--iter", required_argument, 0, 'b' },
		{ "--iterations", required_argument, 0, 'b' },
		{0,0,0,0}
};


int main(int argc, char** argv){
	int threads, iterations;
	threads = iterations = 1;
	int i = 0;
	thread_info a;
	a.n_count = &counter;
	a.add_amount = 1;
	long operations;
	double per_op;
	long long time_init, time_finish;
	char option;
	while ((option = (char)getopt_long(argc, argv, "", long_options, &i)) != -1){
		switch (option){
		case 'a':
			if (threads=atoi(optarg) == 0){
				fprintf(stderr, "Argument must be positive integer\n");
			}
			break;
		case 'b':
			if (iterations = atoi(optarg) == 0){
				fprintf(stderr, "Argument must be positive integer\n");
			}
			break;
		}
	}
	pthread_t *tids = malloc(threads*sizeof(pthread_t));
	
	if (tids == NULL){
		fprintf(stderr, "Error: memory not allocated\n");
		exit(1);
	}
	clock_gettime(CLOCK_MONOTONIC, timer);
	time_init = time.tv_sec * 1000000000 + time.tv_nsec;
	for (int a = 0, create_check = 0; a < threads; a++){
		create_check = pthread_create(tids + a, 0, &add, &a);
		if (create_check){
			fprintf(stderr, "Error: unable to create thread\n");
		}
	}
	for (int a = 0; a < threads; a++){
		pthread_join(tids[i], 0);
	}
	free(tids);
	free(a);
	clock_gettime(CLOCK_MONOTONIC, timer);
	time_finish = time.tv_sec * 1000000000 + time.tv_nsec - time_init;
	operations = threads* iterations * 2;
	fprintf(stdout, "%d threads x %d 10000 iterations x (add + subtract) = %l operations\n", threads, iterations, operations);
	if (counter != 0){
		fprintf(stderr, "Error: final count = %lld\n", counter);
	}
	fprintf(stdout, "elapsed time: %lld\n", time_finish);
	per_op = time_finish / operations;
	fprintf(stdout, "per operation: %f", per_op);
	return 0;
}