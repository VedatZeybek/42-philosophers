# include "stdlib.h"
# include "pthread.h"
# include "stdio.h"
# include <unistd.h> 
# include <sys/time.h>
# include "error.h"


void	*to_upper(void *arg)
{


	usleep(5000 * 1000);
	printf("seelaaaam\n");

	return (NULL);
}

void	*to_lower(void *arg)
{


	usleep(10000 * 1000);
	printf("seelaaaam\n");

	return (NULL);
}

int main()
{


	pthread_t	death_thread;
	pthread_t	death_thread2;

	void *arg;

	pthread_create(&death_thread, NULL,  to_upper, arg);
	pthread_join(death_thread, NULL);


	
	
	pthread_create(&death_thread2, NULL,  to_lower, arg);

	perror("1");
	pthread_join(death_thread2, NULL);
	perror("2");


}