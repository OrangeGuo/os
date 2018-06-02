#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int n = 4;
float sum = 0;
float result[100];
void *compute(void *arg) {
    /* printf("niaho,%d\n", *(int *)(arg)); */
    float *sum;
    int index = *(int *)arg;
    sum = &result[index];
    *sum = (float)1 / (2 * index + 1);
    /* printf("%f\n", *sum); */

    if ((index + 1) % 2 == 0) {
        *sum = *sum * (-1);
    }
    return NULL;
}

int main() {
    int flags[100];
    pthread_t worker_tid[100];
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        flags[i] = i;
        pthread_create(&worker_tid[i], NULL, compute, &flags[i]);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(worker_tid[i], NULL);
        sum += result[i];
    }

    printf("sum=%f\n", sum);
    return 0;
}
