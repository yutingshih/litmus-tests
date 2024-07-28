#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

atomic_int x = 0, y = 0;
int r1 = 0, r2 = 0, r3 = 0, r4 = 0;

void *thread1(void *arg)
{
    atomic_store_explicit(&x, 1, memory_order_relaxed);
    return NULL;
}

void *thread2(void *arg)
{
    atomic_store_explicit(&y, 1, memory_order_relaxed);
    return NULL;
}

void *thread3(void *arg)
{
    r1 = atomic_load_explicit(&x, memory_order_relaxed);
    r2 = atomic_load_explicit(&y, memory_order_relaxed);
    return NULL;
}

void *thread4(void *arg)
{
    r3 = atomic_load_explicit(&y, memory_order_relaxed);
    r4 = atomic_load_explicit(&x, memory_order_relaxed);
    return NULL;
}

int main(void)
{
    pthread_t t1, t2, t3, t4;
    int iterations = 1000000;
    int count[16] = {0};  // To count occurrences of each outcome

    for (int i = 0; i < iterations; i++) {
        x = 0;
        y = 0;
        r1 = 0;
        r2 = 0;
        r3 = 0;
        r4 = 0;

        pthread_create(&t1, NULL, thread1, NULL);
        pthread_create(&t2, NULL, thread2, NULL);
        pthread_create(&t3, NULL, thread3, NULL);
        pthread_create(&t4, NULL, thread4, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);
        pthread_join(t4, NULL);

        count[r1 * 8 + r2 * 4 + r3 * 2 + r4]++;
    }

    printf("Results after %d iterations\n", iterations);
    for (int i = 0; i < 16; i++) {
        printf("r1 = %d, r2 = %d, r3 = %d, r4 = %d: %d times\n",
               (i & 8) >> 3, (i & 4) >> 2, (i & 2) >> 1, i & 1, count[i]);
    }

    return 0;
}
