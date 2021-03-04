#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

// flags -lpthread -lm

#define N 1000

static pthread_mutex_t mutex;

float pi = 0.0;

void *f_thread(void* arg)
{
	
	long int num = (long int) arg + 1;
	float val = 1.0/(num * num);
	
	pthread_mutex_lock(&mutex);
	pi += val;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid[N];
	long j;
	
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		fprintf(stderr, "Erreur thread mutex");
		exit(-1);
	}
	
	for (j = 0 ; j < N ; j ++)
	{
		if (pthread_create(&tid[j], 0, f_thread, (void*) j))
		{
			fprintf(stderr, "Erreur thread");
			exit(-1);
		}
	}
	
	for (j = 0 ; j < N ; j ++)
	{
		if (pthread_join(tid[j], NULL) != 0)
		{
			fprintf(stderr, "Erreur join");
			exit(-1);
		}
	}
	
	printf("Pi = %f\n", sqrt(pi*6));
	
	pthread_exit(EXIT_SUCCESS);
}
