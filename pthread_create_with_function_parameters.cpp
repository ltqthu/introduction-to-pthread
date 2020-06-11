#include <cstdio>
#include <pthread.h>
#include <cstdlib>

#define NUM_THREADS    5

struct thread_struct {
    int  thread_id;
    char* message;
};

void* recv_message(void* threadarg)
{
    thread_struct* p_thread_struct = (thread_struct*)threadarg;
    printf("recv_message(): %d %s\n", p_thread_struct->thread_id, p_thread_struct->message);
    pthread_exit(nullptr);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    thread_struct td[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("main(): creating thread %d \n", i);
        td[i].thread_id = i;
        td[i].message = (char*)"hello pthread!";
        int rc = pthread_create(&threads[i], nullptr, recv_message, (void*)& td[i]);
        if (rc)
        {
            printf("error: unable to create thread, error code: %d\n", rc);
            exit(-1);
        }
    }
    getchar();
    pthread_exit(nullptr);
}