#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 10

pthread_mutex_t mutex;
int current_turn = 0;

const char *philosopher_names[NUM_PHILOSOPHERS] = {
	"Socrates", "Plato", "Aristotle", "Descatres", "Kant",
	"Nietzsche", "Hume", "Confucius", "Spinoza", "Laozi"
};

void	*philosopher_thread(void *arg)
{
	int index= *(int *)arg;
	free(arg);

	while (1)
	{
		pthread_mutex_lock(&mutex);
		
		if (current_turn == index)
		{
			printf("Philosopher %d: My name is %s.\n", index + 1, philosopher_names[index]);
			current_turn++;
			pthread_mutex_unlock(&mutex);
			break;
		}
		pthread_mutex_unlock(&mutex);
	}

	return (NULL);
}

int	main(void)
{
	pthread_t threads[NUM_PHILOSOPHERS];
	pthread_mutex_init(&mutex, NULL);

	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		int *index = malloc(sizeof(int));
		if (index == NULL)
		{
			perror("failed to allocate memory");
			exit(EXIT_FAILURE);
		}
		*index = i;

		if (pthread_create(&threads[i], NULL, philosopher_thread, index) != 0)
		{
			perror("failed to create thread");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			perror("failed to join threads");
			exit(EXIT_FAILURE);
		}
	}
	pthread_mutex_destroy(&mutex);
	return (0);
}
