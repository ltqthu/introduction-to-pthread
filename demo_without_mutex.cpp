#include <cstdio>
#include <pthread.h>
#include <cstdlib>

const int NUM_THREADS = 10000;
static int s_count = 0;

void* add(void* args)
{
    for (int i = 0; i < 10000; i++)
    {
        s_count++;
    }
}

int main()
{
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        int flag = pthread_create(&threads[i], nullptr, add, nullptr);
        if (flag)
        {
            printf("error: unable to create thread, error code: %d\n", flag);
            exit(-1);
        }
    }
    for (int i = 0; i <NUM_THREADS; i++)
    {
        int flag = pthread_join(threads[i], nullptr);
        if (flag)
        {
            printf("error: threads[%d] pthread_join error flag: %d", threads[i], flag);
        }
    }
    printf("s_count=%d\n", s_count);
    getchar();
    pthread_exit(nullptr);
}