#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM 10
int fd[2];
pthread_mutex_t mutex_read, mutex_write;
pthread_cond_t list[NUM];

void *consume(void *arg) {
    int id = *(int *)arg;
    char buf[16];
    if (id) {
        pthread_mutex_lock(&mutex_read);
        pthread_cond_wait(&list[id], &mutex_read);
        read(fd[0], buf, sizeof(buf));
        printf("receive %s\n", buf);
        pthread_mutex_unlock(&mutex_read);
        pthread_mutex_lock(&mutex_write);
        printf("rend %d\n", id + 1);
        char num[3];
        sprintf(num, "%d", id + 1);
        write(fd[1], num, 3);
        pthread_cond_signal(&list[(id + 1) % NUM]);
        pthread_mutex_unlock(&mutex_write);
    } else {
        pthread_mutex_lock(&mutex_write);
        printf("send %d\n", id + 1);
        char num[3];
        sprintf(num, "%d", id + 1);
        write(fd[1], num, 3);
        pthread_cond_signal(&list[(id + 1) % NUM]);
        pthread_mutex_unlock(&mutex_write);
        pthread_mutex_lock(&mutex_read);
        pthread_cond_wait(&list[id], &mutex_read);
        read(fd[0], buf, sizeof(buf));
        printf("receive %s\n", buf);
        pthread_mutex_unlock(&mutex_read);
    }
    return NULL;
}

int main() {
    int tag[NUM];
    pthread_t T_id[NUM];
    pipe(fd);
    pthread_mutex_init(&mutex_read, NULL);
    pthread_mutex_init(&mutex_write, NULL);
    for (int i = 0; i < NUM; i++) {
        tag[i] = i;
        pthread_create(&T_id[i], NULL, consume, &tag[i]);
    }
    for (int i = 0; i < NUM; i++)
        pthread_join(T_id[i], NULL);

    for (int i = 0; i < NUM; i++) {
        pthread_cond_destroy(&list[i]);
    }
    pthread_mutex_destroy(&mutex_write);
    pthread_mutex_destroy(&mutex_read);
    return 0;
}
