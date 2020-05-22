# pthread_create

`pthread_create`用来创建线程。

## 函数声明


```cpp
#include <pthread.h>
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
                   void *(*start_routine) (void *), void *arg);
```

## 参数

第一个参数为指向线程标识符的指针。

第二个参数用来设置线程属性，可使用默认值NULL。

第三个参数是线程运行函数的起始地址，一旦线程被创建就会执行。

最后一个参数是线程运行函数的参数，它必须通过把引用作为指针强制转换为 void 类型进行传递。如果没有传递参数，则使用 NULL。

## 返回值

若成功则返回0，否则返回错误码。

# pthread_join

`pthread_join`用来等待线程结束。`pthread_join`是一个线程阻塞的函数，调用它的函数将一直等待到被等待的线程结束为止，当函数返回时，被等待线程的资源被收回。

## 函数声明

```cpp
#include <pthread.h>
int pthread_join(pthread_t thread, void **status);
```

## 参数

第一个参数为被等待的线程标识符。

第二个参数为一个用户定义的指针，它可以用来存储被等待线程的返回值。

## 返回值

若成功则返回0，否则返回错误码。

## 示例一：有pthread_join的效果

```cpp
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
    pthread_join(thread_id, nullptr);
    printf("The count is = %d\n", s_count);
    getchar();
    return 0;
}
```

程序输出为：

```
thread_count is = 1
thread_count is = 2
thread_count is = 3
thread_count is = 4
thread_count is = 5
The count is = 5
```

主线程等待子线程执行完毕。

## 示例二：没有pthread_join的效果

```cpp
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
```

程序输出为：

```
The count is = 0
thread_count is = 1
thread_count is = 2
thread_count is = 3
thread_count is = 4
thread_count is = 5
```

## 示例三：向线程传递参数

```cpp
#include <cstdio>
#include <pthread.h>
#include <cstdlib>

#define NUM_THREADS     5

struct thread_data {
    int  thread_id;
    char* message;
};

void* say_hello(void* threadarg)
{
    thread_data* my_data = (thread_data*) threadarg;
    printf("say_hello(): %d %s\n", my_data->thread_id, my_data->message);
    pthread_exit(nullptr);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    thread_data td[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("main(): creating thread %d \n", i);
        td[i].thread_id = i;
        td[i].message = (char*)"hello";
        int rc = pthread_create(&threads[i], nullptr, say_hello, (void*)& td[i]);
        if (rc) 
        {
            printf("error: unable to create thread, error code: %d\n", rc);
            exit(-1);
        }
    }
    getchar();
    pthread_exit(nullptr);
}
```

程序输出结果：

```
main(): creating thread 0 
main(): creating thread 1 
say_hello(): 0 hello
main(): creating thread 2 
say_hello(): 1 hello
main(): creating thread 3 
say_hello(): 2 hello
main(): creating thread 4 
say_hello(): 3 hello
say_hello(): 4 hello
```

## 示例四：不加锁的后果

```cpp
#include <cstdio>
#include <pthread.h>
#include <cstdlib>

const int NUM_THREADS = 100;
static int s_count = 0;

void* add(void* args)
{
    for (int i=0; i<100; i++)
    {
        s_count++;
    }
}

int main()
{
    pthread_t threads[NUM_THREADS];
    for (int i=0; i< NUM_THREADS; i++)
    {
        int nc = pthread_create(&threads[i], nullptr, add, nullptr);
        if (nc)
        {
            printf("error: unable to create thread, error code: %d\n", nc);
            exit(-1);
        }
    }
    printf("s_count=%d\n", s_count);
    getchar();
    pthread_exit(nullptr);
}
```

程序输出结果：

```
s_count=9800
```

