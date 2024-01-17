// Compile with `gcc foo.c -Wall -std=gnu99 -lpthread`, or use the makefile
// The executable will be named `foo` if you use the makefile, or `a.out` if you use gcc directly

#include <pthread.h>
#include <stdio.h>

int i = 0;

int last_operation = 0; // 0 = add, 1 = sub
int current_sequence = 0;
int longest_sequence_add = 0;
int longest_sequence_sub = 0;

pthread_mutex_t lock;

void update_longest_sequence(int* sequence_operation) {
    // Determine which operation the current sequence belongs to
    // based on the pointer to the sequence variable.
    // 0 = add, 1 = sub. 
    int belonging_operation = 0;
    if (sequence_operation == &longest_sequence_sub) {
        belonging_operation = 1;
    }

    if (last_operation == belonging_operation) {
        current_sequence++;
    }
    else {   
        if (current_sequence > *sequence_operation) {
            *sequence_operation = current_sequence;
        }
        current_sequence = 0;
    }
}

// Note the return type: void*
void* incrementingThreadFunction(){
    for (int j = 0; j < 1000000; j++) {
        pthread_mutex_lock(&lock);
        i++;

        update_longest_sequence(&longest_sequence_add);

        last_operation = 0;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* decrementingThreadFunction(){
    for (int k = 0; k < 1000000-1; k++) {
        pthread_mutex_lock(&lock);
        i--;

        update_longest_sequence(&longest_sequence_sub);

        last_operation = 1;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    pthread_mutex_init(&lock, NULL);

    //Create threads
    pthread_t incrementingThread;
    pthread_create(&incrementingThread, NULL, incrementingThreadFunction, "incrementing thread");

    pthread_t decrementingThread;
    pthread_create(&decrementingThread, NULL, decrementingThreadFunction, "decrementing thread");

    pthread_join(incrementingThread, NULL);
    pthread_join(decrementingThread, NULL);
    
    printf("The magic number is: %d\n", i);
    printf("Longest sequence add: %d\n", longest_sequence_add);
    printf("Longest sequence sub: %d\n", longest_sequence_sub);

    pthread_mutex_destroy(&lock);
    return 0;
}
