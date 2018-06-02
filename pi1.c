#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int n = 1;
float a_sum = 0;
float b_sum = 0;
void *compute(void *arg) {
    /* printf("niaho,%d\n", *(int *)(arg)); */
    float *sum, flag = -1;
    int a = 1, b = n;
    if (*(int *)arg == 0) {
        sum = &a_sum;
        *sum = 1;
    } else {
        sum = &b_sum;
        *sum = (float)1 / (2 * n + 1);
        /* printf("%f\n", *sum); */

        a += n;
        b += n;
        if ((n + 1) % 2 == 0) {
            flag = 1;
            *sum = *sum * (-1);
        }
    }
    for (int i = a; i < b; i++) {
        *sum = *sum + (flag)*1 / (i * 2 + 1);
        flag *= -1;
    }
    return NULL;
}

int main() {
    int flag_a = 1, flag_b = 0;
    pthread_t worker_tid;
    pthread_create(&worker_tid, NULL, compute, &flag_a);

    compute(&flag_b);
    pthread_join(worker_tid, NULL);
    printf("a_sum=%f,b_sum=%f,sum=%f\n", a_sum, b_sum, a_sum + b_sum);
    return 0;
}
