#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EMPLOYEE_NUMS  3
#define WORKLOAD_PER_EMPLOYEE 10
#define NUMS_TO_REST 20

int count = 0;
pthread_mutex_t mutex;
pthread_cond_t  cond;

void* employee_work(void* t) {
    for (int i = 0; i < WORKLOAD_PER_EMPLOYEE; i++) {
        pthread_mutex_lock(&mutex);
        if (++count == NUMS_TO_REST) {
            printf("employee_work(): 员工 %ld, 工作总量达到 %d, 通知老板休息\n", (long)t, count);
            /* pthread_cond_signal 用来唤醒正在等待对应条件变量的线程，此时互斥锁必须是锁住的。
              执行之后必须用 pthread_mutex_unlock 解锁互斥锁。
              若有多个线程在等待条件变量，那么必须用 pthread_cond_broadcast 代替 pthread_cond_signal。
              必须在调用 pthread_cond_signal 之前调用 pthread_cond_wait。
              */
            pthread_cond_signal(&cond);
        }

        printf("employee_work(): 员工 %ld, 此时全员工作总量为 %d \n", (long)t, count);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    pthread_exit(NULL);
}

void* boss_rest(void* t) {
    printf("boss_rest(): 老板 %ld 开始等待休息...\n", (long)t);
    pthread_mutex_lock(&mutex);

    while (count < NUMS_TO_REST) {
        printf("boss_rest(): 老板 %ld 员工工作总量为 %d. 继续等待...\n", (long)t, count);
        // pthread_cond_wait 总是自动且原子地解锁互斥锁。
        pthread_cond_wait(&cond, &mutex);
    }

    printf("boss_rest(): 老板 %ld 休息啦，员工们好好干活！\n", (long)t, count);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int i, rc;
    pthread_t th[EMPLOYEE_NUMS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&mutex, NULL); // 初始化互斥锁
    pthread_cond_init(&cond, NULL); // 初始化条件变量

    // 创建老板线程
    pthread_create(&th[0], &attr, boss_rest, (void*)1l);

    // 创建员工线程
    for (long i = 1; i < 3; i++) {
        pthread_create(&th[i], &attr, employee_work, (void*)i);
    }

    // 等待所有员工线程结束
    for (i = 0; i < EMPLOYEE_NUMS; i++) {
        pthread_join(th[i], NULL);
    }

    printf("main(): 工作总量为 %d, 工作全部完成", EMPLOYEE_NUMS, count);

    // 清理并退出 
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}
