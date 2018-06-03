#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value) {
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    sema->value--;
    while (sema->value < 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

void sema_destroy(sema_t *sema) {
    pthread_cond_destroy(&sema->cond);
    pthread_mutex_destroy(&sema->mutex);
}

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

/* pthread_mutex_t mutex1, mutex2; */
/* pthread_cond_t wait_empty_buffer1, wait_empty_buffer2; */
/* pthread_cond_t wait_full_buffer1, wait_full_buffer2; */
sema_t mutex_sema1, mutex_sema2;
sema_t empty_buffer1_sema, empty_buffer2_sema;
sema_t full_buffer1_sema, full_buffer2_sema;

#define ITEM_COUNT (CAPACITY * 2)
void *consume(void *arg) {
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        /* pthread_mutex_lock(&mutex2); */
        /* while (in_2 == out_2) */
        /*     pthread_cond_wait(&wait_full_buffer2, &mutex2); */
        sema_wait(&full_buffer2_sema);
        sema_wait(&mutex_sema1);

        item = get_item_from_buffer2();
        printf(" consume item: %c \n", item);

        sema_signal(&mutex_sema1);
        sema_signal(&empty_buffer2_sema);
        /* pthread_cond_signal(&wait_empty_buffer2); */
        /* pthread_mutex_unlock(&mutex2); */
    }
    return NULL;
}
void *compute(void *arg) {
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {

        /* pthread_mutex_lock(&mutex1); */
        /* while (in_1 == out_1) */
        /*     pthread_cond_wait(&wait_full_buffer1, &mutex1); */
        sema_wait(&full_buffer1_sema);
        sema_wait(&mutex_sema1);

        /* pthread_cond_signal(&wait_empty_buffer1); */
        /* pthread_mutex_unlock(&mutex1); */

        /* pthread_mutex_lock(&mutex2); */
        /* while ((in_2 + 1) % CAPACITY == out_2) */
        /*     pthread_cond_wait(&wait_empty_buffer2, &mutex2); */

        sema_wait(&empty_buffer2_sema);
        sema_wait(&mutex_sema2);
        item = get_item_from_buffer1();
        put_item_into_buffer2(item + 'A' - 'a');
        sema_signal(&mutex_sema1);
        sema_signal(&empty_buffer1_sema);
        sema_signal(&mutex_sema2);
        sema_signal(&full_buffer2_sema);
        /* pthread_cond_signal(&wait_full_buffer2); */
        /* pthread_mutex_unlock(&mutex2); */
        /* printf("call %d\n", i + 1); */
    }
    return NULL;
}

void produce() {
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) {
        /* pthread_mutex_lock(&mutex1); */
        /* while ((in_1 + 1) % CAPACITY == out_1) */
        /* pthread_cond_wait(&wait_empty_buffer1, &mutex1); */
        sema_wait(&empty_buffer1_sema);
        sema_wait(&mutex_sema1);
        item = i + 'a';
        printf("produce item: %c\n", item);
        put_item_into_buffer1(item);

        /* pthread_cond_signal(&wait_full_buffer1); */
        /* pthread_mutex_unlock(&mutex1); */
        sema_signal(&mutex_sema1);
        sema_signal(&full_buffer1_sema);
    }
}

// The program contains a bug. Can you find it?
int main() {
    pthread_t computer_tid, consumer_tid;

    /* pthread_mutex_init(&mutex1, NULL); */
    /* pthread_cond_init(&wait_empty_buffer1, NULL); */
    /* pthread_cond_init(&wait_full_buffer1, NULL); */

    /* pthread_mutex_init(&mutex2, NULL); */
    /* pthread_cond_init(&wait_empty_buffer2, NULL); */
    /* pthread_cond_init(&wait_full_buffer2, NULL); */

    sema_init(&mutex_sema1, 1);
    sema_init(&mutex_sema2, 1);
    sema_init(&empty_buffer1_sema, 1);
    sema_init(&empty_buffer2_sema, 1);
    sema_init(&full_buffer1_sema, 0);
    sema_init(&full_buffer2_sema, 0);
    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&computer_tid, NULL, compute, NULL);
    produce();
    pthread_join(computer_tid, NULL);
    pthread_join(consumer_tid, NULL);
    sema_destroy(&mutex_sema1);
    sema_destroy(&mutex_sema2);
    sema_destroy(&empty_buffer1_sema);
    sema_destroy(&empty_buffer2_sema);
    sema_destroy(&full_buffer1_sema);
    sema_destroy(&full_buffer2_sema);
    /* pthread_cond_destroy(&wait_full_buffer1); */
    /* pthread_cond_destroy(&wait_empty_buffer1); */
    /* pthread_mutex_destroy(&mutex1); */
    /* pthread_cond_destroy(&wait_full_buffer2); */
    /* pthread_cond_destroy(&wait_empty_buffer2); */
    /* pthread_mutex_destroy(&mutex2); */

    return 0;
}
