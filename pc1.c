#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

char buffer1[4], buffer2[4];
int flag1 = 0, flag2 = 0, size1 = 0, size2 = 0;
int times = 20;

void *producer(void *arg) {
    char resource[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    int pr = 0, pb = 0;
    while (times > 0) {
        if (flag1 == 0 && size1 < 4) {
            buffer1[pb] = resource[pr];
            pb = (pb + 1) % 4;
            pr = (pr + 1) % 8;
            size1++;
            printf("hello\n");
            flag1 = 1;
            times--;
        } else
            sleep(0.1);
    }
}

void *computer(void *arg) {
    int pb1 = 0, pb2 = 0;
    while (times > 0) {
        if (flag1 == 1 && flag2 == 0 && size1 > 0 && size2 < 4) {
            buffer2[pb2] = buffer1[pb1] + 'A' - 'a';
            pb1 = (pb1 + 1) % 4;
            pb2 = (pb2 + 1) % 4;
            flag1 = 0;
            flag2 = 1;
            size1--;
            size2++;
        } else
            sleep(0.1);
    }
}

void *comsumer(void *arg) {
    int pb = 0;
    while (times > 0) {
        if (flag2 = 1 && size2 > 0) {
            printf("%c\n", buffer2[pb]);
            pb = (pb + 1) % 4;
            size2--;
            flag2 = 0;
        } else
            sleep(0.1);
    }
}
int main() {
    pthread_t proceduce,compute,comsume;
    pthread_create(&proceduce, NULL, producer, NULL);
    pthread_create(&compute, NULL, computer, NULL);
    pthread_create(&comsume, NULL, comsumer, NULL);

    pthread_join(proceduce, NULL);
	pthread_join(compute, NULL);
	pthread_join(comsume, NULL);
    printf("finished\n");
    return 0;
}
