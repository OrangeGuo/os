#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define CAPACITY 4
int buffer1[CAPACITY];
int buffer2[CAPACITY];
int in_1;
int out_1;
int in_2;
int out_2;

int get_item_from_buffer1() {
    int item;

    item = buffer1[out_1];
    out_1 = (out_1 + 1) % CAPACITY;
    return item;
}
int get_item_from_buffer2() {
    int item;

    item = buffer2[out_2];
    out_2 = (out_2 + 1) % CAPACITY;
    return item;
}

void put_item_into_buffer1(int item) {
    buffer1[in_1] = item;
    in_1 = (in_1 + 1) % CAPACITY;
}
void put_item_into_buffer2(int item) {
    printf(" %c->%c\n", item + 'a' - 'A', item);
    buffer2[in_2] = item;
    in_2 = (in_2 + 1) % CAPACITY;
}

pthread_mutex_t mutex1, mutex2;
pthread_cond_t wait_empty_buffer1, wait_empty_buffer2;
pthread_cond_t wait_full_buffer1, wait_full_buffer2;

#define ITEM_COUNT (CAPACITY * 2)
void *consume(void *arg) {
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutex2);
        while (in_2 == out_2)
            pthread_cond_wait(&wait_full_buffer2, &mutex2);

        item = get_item_from_buffer2();

        printf(" consume item: %c \n", item);
        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}
void *compute(void *arg) {
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {

        pthread_mutex_lock(&mutex1);
        while (in_1 == out_1)
            pthread_cond_wait(&wait_full_buffer1, &mutex1);
        item = get_item_from_buffer1();

        pthread_cond_signal(&wait_empty_buffer1);
        pthread_mutex_unlock(&mutex1);

        pthread_mutex_lock(&mutex2);
        while ((in_2 + 1) % CAPACITY == out_2)
            pthread_cond_wait(&wait_empty_buffer2, &mutex2);

        put_item_into_buffer2(item + 'A' - 'a');
        pthread_cond_signal(&wait_full_buffer2);
        pthread_mutex_unlock(&mutex2);
        /* printf("call %d\n", i + 1); */
    }
    return NULL;
}

void produce() {
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        pthread_mutex_lock(&mutex1);
        while ((in_1 + 1) % CAPACITY == out_1)
            pthread_cond_wait(&wait_empty_buffer1, &mutex1);

        item = i + 'a';
        printf("produce item: %c\n", item);
        put_item_into_buffer1(item);

        pthread_cond_signal(&wait_full_buffer1);
        pthread_mutex_unlock(&mutex1);
    }
}

// The program contains a bug. Can you find it?
int main() {
    pthread_t computer_tid, consumer_tid;

    pthread_mutex_init(&mutex1, NULL);
    pthread_cond_init(&wait_empty_buffer1, NULL);
    pthread_cond_init(&wait_full_buffer1, NULL);

    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&wait_empty_buffer2, NULL);
    pthread_cond_init(&wait_full_buffer2, NULL);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&computer_tid, NULL, compute, NULL);
    produce();
    pthread_join(computer_tid, NULL);
    pthread_join(consumer_tid, NULL);
    pthread_cond_destroy(&wait_full_buffer1);
    pthread_cond_destroy(&wait_empty_buffer1);
    pthread_mutex_destroy(&mutex1);
    pthread_cond_destroy(&wait_full_buffer2);
    pthread_cond_destroy(&wait_empty_buffer2);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
