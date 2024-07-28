#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

atomic_int x = 0, y = 0;
int r1 = 0, r2 = 0;

void *thread1(void *arg)
{
    atomic_store_explicit(&x, 1, memory_order_relaxed);
    r1 = atomic_load_explicit(&y, memory_order_relaxed);
    return NULL;
}

void *thread2(void *arg)
{
    atomic_store_explicit(&y, 1, memory_order_relaxed);
    r2 = atomic_load_explicit(&x, memory_order_relaxed);
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;
    int iterations = 1000000;
    int count[4] = {0};  // To count occurrences of each outcome

    for (int i = 0; i < iterations; i++) {
        x = 0;
        y = 0;
        r1 = 0;
        r2 = 0;

        pthread_create(&t1, NULL, thread1, NULL);
        pthread_create(&t2, NULL, thread2, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        count[r1 * 2 + r2]++;
    }

    printf("Results after %d iterations\n", iterations);
    printf("r1 = 0, r2 = 0: %d times\n", count[0]);
    printf("r1 = 0, r2 = 1: %d times\n", count[1]);
    printf("r1 = 1, r2 = 0: %d times\n", count[2]);
    printf("r1 = 1, r2 = 1: %d times\n", count[3]);

    return 0;
}
