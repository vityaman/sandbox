#include <stdio.h>
#include <emmintrin.h>

#define SIZE_LIMIT 100000000000
#define TRIES 30

// Solution 1: Basic for-loop linear scan
// Twice faster than Solution 2
// By 3.5 slower than Solution 3
size_t solution_basic(int16_t* input, size_t size) {
    size_t count = 0;
    int16_t* end = input + size;
    while(input < end) {
        if (*input >= 0) {
            ++count;
        }
        ++input;
    }
    return count;
}

// Solution 2: First solution, than I've made
// The problem here is _mm_store_si128((__m128i*)res, r_part);
// as it stores data in RAM, that ruins performance as
// access to memory is an expensive operation
size_t solution_half_simd(int16_t* input, size_t size) {
    int16_t* res = malloc(sizeof(int16_t) * size);
    int16_t* result = res;
    __m128i r_zeros    = _mm_setzero_si128(),
            r_neg_ones = _mm_cmpeq_epi16(r_zeros, r_zeros),
            r_part;
    int16_t* end = input + size;
    for (int16_t* current = input; current < end; current += 8, res += 8) {
        r_part      = _mm_load_si128((__m128i*)current);
        r_part      = _mm_cmpgt_epi16(r_part, r_neg_ones);
        _mm_store_si128((__m128i*)res, r_part);
    }
    size_t count = 0;
    for (size_t i = 0; i < size; ++i) {
        count -= result[i];
    }
    free(result);
    return count;
}

// Solution 3: My best attempt 
// result is stored in register, so I don't
// need to touch RAM unless process is complete
size_t solution_max_simd(int16_t* input, size_t size) {
    __m128i r_sum      = _mm_setzero_si128(),
            r_neg_ones = _mm_cmpeq_epi16(r_sum, r_sum),
            r_part;                            
    int16_t* end = input + size;
    for (int16_t* current = input; current < end; current += 8) {   
	r_part     = _mm_load_si128((__m128i*) current);  
        r_part     = _mm_cmpgt_epi16(r_part, r_neg_ones);
        r_sum      = _mm_add_epi16(r_sum, r_part);
    }
    size_t count = 0;
    int16_t* sum = malloc(sizeof(int16_t) * 8);
    _mm_store_si128((__m128i*) sum, r_sum);       
    for (size_t i = 0; i < 8; ++i) {
        count -= sum[i];
    }
    free(sum);
    return count;
}

// Stress test for given solution
void check(size_t (*calc)(int16_t*, size_t), int16_t* input, size_t size) {
    u_int64_t elapsed_us = 0;
    for (int t = 0; t < TRIES; ++t) {
        struct timeval tstart, tend;
        gettimeofday(&tstart, NULL);
        int32_t cnt = calc(input, size);
        gettimeofday(&tend, NULL);
        elapsed_us += (tend.tv_sec - tstart.tv_sec) * 1000000 + tend.tv_usec - tstart.tv_usec;
    }
    elapsed_us /= TRIES;
    printf("t=%llu us\n", elapsed_us);
}

int main() {
    for (size_t size = 8; size < SIZE_LIMIT; size *= 2) {
	int16_t* input = malloc(sizeof(int16_t) * size); 	
    for (int i = 0; i < size; ++i) {
        input[i] = i % 2 == 0 ? i : -i;
        printf("\n`size=%d\nfor-loop:\t", size);
        check(solution_basic, input, size);
        printf("simd-mistake:\t");
        check(solution_half_simd, input, size);
        printf("simd-better:\t");
        check(solution_max_simd, input, size);
        free(input);
    }
    return 0;
}