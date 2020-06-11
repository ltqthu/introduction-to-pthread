#include <cstdio>
#include <unistd.h>
#include <pthread.h>

static int s_count = 0;

void* thread_count(void* param)
{
    for (int i = 0; i < 5; i++)
    {
        s_count++;
        printf("thread_count is = %d\n", s_count);
        sleep(1);
    }
    return nullptr;
}

int main()
{
    pthread_t thread_id;
    pthread_create(&thread_id, nullptr, thread_count, nullptr);
    //pthread_join(thread_id, nullptr);
    printf("The count is = %d\n", s_count);
    getchar();
    return 0;
}