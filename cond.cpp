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
            printf("employee_work(): Ա�� %ld, ���������ﵽ %d, ֪ͨ�ϰ���Ϣ\n", (long)t, count);
            /* pthread_cond_signal �����������ڵȴ���Ӧ�����������̣߳���ʱ��������������ס�ġ�
              ִ��֮������� pthread_mutex_unlock ������������
              ���ж���߳��ڵȴ�������������ô������ pthread_cond_broadcast ���� pthread_cond_signal��
              �����ڵ��� pthread_cond_signal ֮ǰ���� pthread_cond_wait��
              */
            pthread_cond_signal(&cond);
        }

        printf("employee_work(): Ա�� %ld, ��ʱȫԱ��������Ϊ %d \n", (long)t, count);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    pthread_exit(NULL);
}

void* boss_rest(void* t) {
    printf("boss_rest(): �ϰ� %ld ��ʼ�ȴ���Ϣ...\n", (long)t);
    pthread_mutex_lock(&mutex);

    while (count < NUMS_TO_REST) {
        printf("boss_rest(): �ϰ� %ld Ա����������Ϊ %d. �����ȴ�...\n", (long)t, count);
        // pthread_cond_wait �����Զ���ԭ�ӵؽ�����������
        pthread_cond_wait(&cond, &mutex);
    }

    printf("boss_rest(): �ϰ� %ld ��Ϣ����Ա���Ǻúøɻ\n", (long)t, count);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int i, rc;
    pthread_t th[EMPLOYEE_NUMS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&mutex, NULL); // ��ʼ��������
    pthread_cond_init(&cond, NULL); // ��ʼ����������

    // �����ϰ��߳�
    pthread_create(&th[0], &attr, boss_rest, (void*)1l);

    // ����Ա���߳�
    for (long i = 1; i < 3; i++) {
        pthread_create(&th[i], &attr, employee_work, (void*)i);
    }

    // �ȴ�����Ա���߳̽���
    for (i = 0; i < EMPLOYEE_NUMS; i++) {
        pthread_join(th[i], NULL);
    }

    printf("main(): ��������Ϊ %d, ����ȫ�����", EMPLOYEE_NUMS, count);

    // �����˳� 
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);
}
