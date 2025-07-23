#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

// Structure to pass data to threads
typedef struct {
    int thread_id;
    int* shared_counter;
    pthread_mutex_t* mutex;
} thread_data_t;

// 1. usleep example - sleep in microseconds
void usleep_example() {
    printf("=== usleep() example ===\n");
    printf("Printing messages every 500ms for 3 seconds...\n");
    
    for (int i = 0; i < 6; i++) {
        printf("Step %d\n", i + 1);
        usleep(500000); // 500,000 microseconds = 500 milliseconds = 0.5 seconds
    }
    printf("usleep example completed!\n\n");
}

// 2. gettimeofday example - time measurement
void gettimeofday_example() {
    printf("=== gettimeofday() example ===\n");
    struct timeval start, end;
    
    // Get start time
    gettimeofday(&start, NULL);
    printf("Operation starting...\n");
    
    // Simulate some operation (1 second sleep)
    sleep(1);
    
    // Get end time
    gettimeofday(&end, NULL);
    
    // Calculate elapsed time
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    
    printf("Operation completed. Elapsed time: %.6f seconds\n", elapsed);
    printf("Start: %ld.%06ld\n", start.tv_sec, start.tv_usec);
    printf("End: %ld.%06ld\n\n", end.tv_sec, end.tv_usec);
}

// Thread function for detached thread
void* detached_thread_func(void* arg) {
    int thread_id = *(int*)arg;
    printf("Detached Thread %d started\n", thread_id);
    
    for (int i = 0; i < 3; i++) {
        printf("Detached Thread %d - Step %d\n", thread_id, i + 1);
        usleep(300000); // 300ms sleep
    }
    
    printf("Detached Thread %d completed\n", thread_id);
    return NULL;
}

// Thread function for joinable thread
void* joinable_thread_func(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    int thread_id = data->thread_id;
    printf("Joinable Thread %d started\n", thread_id);
    
    // Protected counter increment with mutex
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(data->mutex);
        (*(data->shared_counter))++;
        printf("Thread %d: Counter = %d\n", thread_id, *(data->shared_counter));
        pthread_mutex_unlock(data->mutex);
        usleep(100000); // 100ms sleep
    }
    
    printf("Joinable Thread %d completed\n", thread_id);
    return (void*)(long)(thread_id * 100); // Return value
}

// 3. pthread_create and pthread_detach example
void pthread_detach_example() {
    printf("=== pthread_create() and pthread_detach() example ===\n");
    
    pthread_t detached_threads[2];
    int thread_ids[2] = {1, 2};
    
    // Create two detached threads
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&detached_threads[i], NULL, detached_thread_func, &thread_ids[i]) != 0) {
            printf("Failed to create Thread %d!\n", i + 1);
            return;
        }
        
        // Detach the thread - main thread won't wait for these threads
        if (pthread_detach(detached_threads[i]) != 0) {
            printf("Failed to detach Thread %d!\n", i + 1);
            return;
        }
        printf("Thread %d created as detached\n", i + 1);
    }
    
    // Wait a bit for detached threads to complete
    printf("Main thread waiting for detached threads to complete...\n");
    sleep(2);
    printf("pthread_detach example completed!\n\n");
}

// 4. pthread_join example
void pthread_join_example() {
    printf("=== pthread_join() example ===\n");
    
    pthread_t joinable_threads[3];
    thread_data_t thread_data[3];
    void* thread_results[3];
    int shared_counter = 0;
    pthread_mutex_t mutex;
    
    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) 
	{
        printf("Failed to initialize mutex!\n");
        return;
    }
    
    // Prepare thread data
    int thread_ids[3] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        thread_data[i].thread_id = thread_ids[i];
        thread_data[i].shared_counter = &shared_counter;
        thread_data[i].mutex = &mutex;
    }
    
    // Create three joinable threads
    for (int i = 0; i < 3; i++) {
        if (pthread_create(&joinable_threads[i], NULL, joinable_thread_func, &thread_data[i]) != 0) {
            printf("Failed to create Thread %d!\n", thread_ids[i]);
            pthread_mutex_destroy(&mutex);
            return;
        }
        printf("Joinable Thread %d created\n", thread_ids[i]);
    }
    
    // Wait for all threads to complete
    printf("Main thread waiting for all joinable threads to complete...\n");
    for (int i = 0; i < 3; i++) {
        if (pthread_join(joinable_threads[i], &thread_results[i]) != 0) {
            printf("Failed to join Thread %d!\n", thread_ids[i]);
            continue;
        }
        printf("Thread %d completed, return value: %ld\n", 
               thread_ids[i], (long)thread_results[i]);
    }
    
    printf("All joinable threads completed!\n");
    printf("Final counter value: %d\n", shared_counter);
    
    // Clean up mutex
    pthread_mutex_destroy(&mutex);
    printf("\n");
}

// 5. pthread_mutex example
void mutex_example() {
    printf("=== pthread_mutex example ===\n");
    
    pthread_mutex_t local_mutex;
    
    // Initialize mutex
    if (pthread_mutex_init(&local_mutex, NULL) != 0) {
        printf("Failed to initialize mutex!\n");
        return;
    }
    printf("Mutex successfully initialized\n");
    
    // Lock mutex
    printf("Locking mutex...\n");
    pthread_mutex_lock(&local_mutex);
    printf("Mutex locked - critical section\n");
    
    // Simulate some operation in critical section
    usleep(500000); // 500ms sleep
    
    // Unlock mutex
    printf("Unlocking mutex...\n");
    pthread_mutex_unlock(&local_mutex);
    printf("Mutex unlocked\n");
    
    // Destroy mutex
    if (pthread_mutex_destroy(&local_mutex) != 0) {
        printf("Failed to destroy mutex!\n");
        return;
    }
    printf("Mutex successfully destroyed\n\n");
}

// Advanced example: Producer-Consumer with mutex and multiple threads
void producer_consumer_example() {
    printf("=== Advanced Producer-Consumer example ===\n");
    
    pthread_t threads[4];
    thread_data_t thread_data[4];
    int buffer = 0;  // Shared buffer
    pthread_mutex_t buffer_mutex;
    
    // Initialize mutex
    if (pthread_mutex_init(&buffer_mutex, NULL) != 0) {
        printf("Failed to initialize buffer mutex!\n");
        return;
    }
    
    // Prepare thread data for producers and consumers
    for (int i = 0; i < 4; i++) 
	{
        thread_data[i].thread_id = i + 1;
        thread_data[i].shared_counter = &buffer;
        thread_data[i].mutex = &buffer_mutex;
    }
    
    // Create producer and consumer threads
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, joinable_thread_func, &thread_data[i]) != 0) {
            printf("Failed to create Thread %d!\n", i + 1);
            pthread_mutex_destroy(&buffer_mutex);
            return;
        }
        printf("Thread %d created for producer-consumer example\n", i + 1);
    }
    
    // Wait for all threads
    for (int i = 0; i < 4; i++) {
        void* result;
        pthread_join(threads[i], &result);
        printf("Thread %d finished with result: %ld\n", i + 1, (long)result);
    }
    
    printf("Final buffer value: %d\n", buffer);
    pthread_mutex_destroy(&buffer_mutex);
    printf("Producer-Consumer example completed!\n\n");
}

// Main function running all examples
int main() {
    printf("POSIX Functions Examples\n");
    printf("========================\n\n");
    
    // 1. usleep example
    //usleep_example();
    
    // 2. gettimeofday example
    //gettimeofday_example();
    
    // 3. mutex example
   // mutex_example();
    
    // 4. pthread_detach example
    //pthread_detach_example();
    
    // 5. pthread_join example
    pthread_join_example();
    
    // 6. Advanced producer-consumer example
    //producer_consumer_example();
    
    printf("All examples completed!\n");
    return 0;
}

/*
COMPILATION COMMAND:
gcc -o posix_examples posix_examples.c -lpthread

FUNCTION DESCRIPTIONS:

1. usleep(microseconds):
   - Suspends program execution for specified microseconds
   - 1 second = 1,000,000 microseconds

2. gettimeofday(struct timeval *tv, struct timezone *tz):
   - Gets current time with microsecond precision
   - Used for time measurements

3. pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
                  void *(*start_routine)(void*), void *arg):
   - Creates a new thread
   - start_routine: function the thread will execute
   - arg: parameter passed to the function

4. pthread_detach(pthread_t thread):
   - Detaches thread from main thread
   - No need to call pthread_join for this thread
   - Resources are automatically cleaned when thread completes

5. pthread_join(pthread_t thread, void **retval):
   - Waits for specified thread to complete
   - Gets thread's return value
   - Cleans up thread resources

6. pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr):
   - Initializes mutex and prepares it for use

7. pthread_mutex_destroy(pthread_mutex_t *mutex):
   - Destroys mutex and frees its resources

8. pthread_mutex_lock(pthread_mutex_t *mutex):
   - Locks mutex (enters critical section)
   - If mutex is already locked, waits until it's unlocked

9. pthread_mutex_unlock(pthread_mutex_t *mutex):
   - Unlocks mutex (exits critical section)
*/