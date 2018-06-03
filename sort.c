#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
int array[] = {1, 9, 5, 3, 4, 8, 2, 7, 6, 0};
int size = 10;

void *sort(void *arg) {
    for (int i = size / 2; i < size - 1; i++) {
        int index = i;
        for (int j = i + 1; j < size; j++) {
            if (array[index] > array[j]) {
                index = j;
            }
        }
        if (index != i) {
            int temp = array[index];
            array[index] = array[i];
            array[i] = temp;
        }
    }
    return NULL;
}

void master() {
    for (int i = 0; i < size / 2 - 1; i++) {
        int index = i;
        for (int j = i + 1; j < size / 2; j++) {
            if (array[index] > array[j]) {
                index = j;
            }
        }
        if (index != i) {
            int temp = array[index];
            array[index] = array[i];
            array[i] = temp;
        }
    }

    return;
}

void merge_sort() {
    int a = 0, b = size / 2, copy[10], pointer = 0;
    for (int i = 0; i < size; i++)
        copy[i] = array[i];
    while (pointer < size) {

        if (a == size / 2 || b < size && copy[a] > copy[b]) {
            array[pointer] = copy[b];
            b++;
        } else {
            array[pointer] = copy[a];
            a++;
        }

        pointer++;
    }
}
int main() {
    pthread_t worker_tid;
    pthread_create(&worker_tid, NULL, sort, NULL);
    master();
    pthread_join(worker_tid, NULL);
    merge_sort();
    for (int i = 0; i < size; i++)
        printf("%d,", array[i]);
    printf("\n");
    return 0;
}
