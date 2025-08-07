#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    sem_t *sem;
    pid_t pid;

    sem = sem_open("/shared_resource", O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED)
    {
        sem = sem_open("/shared_resource", 0);
        if (sem == SEM_FAILED)
        {
            perror("sem_open");
            exit(1);
        }
    }
    pid = fork();
    if (pid == 0)
    {
        // Child process (Consumer)
        for (int i = 0; i < 3; i++) 
        {
            printf("Consumer: Kaynak için bekleniyor...\n");
            sem_wait(sem);
            
            printf("Consumer: Kaynak kullanılıyor (%d)\n", i+1);
            sleep(2);
            
            printf("Consumer: Kaynak serbest bırakılıyor\n");
            sem_post(sem);
            
            sleep(1);
        }
        sem_close(sem);
        exit(0);
    }
    else if (pid > 0)
    {
        // Parent process (Producer)
        for (int i = 0; i < 3; i++) 
        {
            printf("Producer: Kaynak için bekleniyor...\n");
            sem_wait(sem);
            
            printf("Producer: Kaynak kullanılıyor (%d)\n", i+1);
            sleep(1);
            
            printf("Producer: Kaynak serbest bırakılıyor\n");
            sem_post(sem);
            
            sleep(1);
        }
        wait(NULL);
        sem_close(sem);
        sem_unlink("/shared_resource");
    } 
    else 
    {
        perror("fork");
        sem_close(sem);
        sem_unlink("/shared_resource");
        exit(1);
    }   
    return 0;
}