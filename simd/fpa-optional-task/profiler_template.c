#include <stdio.h>
#include <emmintrin.h>

#define SIZE 65520
#define TRIES 10

int main() {
    int16_t* input = malloc(sizeof(int16_t) * SIZE);
    for (int i = 0; i < SIZE; ++i)
        input[i] = i % 2 == 0 ? i : -i;
    
    u_int64_t elapsed_us = 0;
    int32_t cnt;
    for (int t = 0; t < TRIES; ++t)
    {
        struct timeval tstart, tend;
        gettimeofday(&tstart, NULL);

        cnt = calculate(input, input + SIZE);

        gettimeofday(&tend, NULL);
        elapsed_us += (tend.tv_sec - tstart.tv_sec) * 1000000 + tend.tv_usec - tstart.tv_usec;
    }
    elapsed_us /= TRIES;

    printf("Average: %llu us\n", cnt, elapsed_us);

    // Repeat for is_not_negative
    // ...
}