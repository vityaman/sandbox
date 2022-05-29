#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <emmintrin.h>
#include "decor.c"

static size_t count_not_negatives_impl(int16_t* array, size_t size) {
    size_t roundedSize = size / 8 * 8;
    __m128i r_sum      = _mm_setzero_si128(),
            r_neg_ones = _mm_cmpeq_epi16(r_sum, r_sum),
            r_part;              
    int16_t* end = array + roundedSize;              
    for (int16_t* current = array; current < end; current += 8) {   
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
    for (size_t i = roundedSize; i < size; ++i) {
        if (array[i] >= 0) {
            ++count;
        }
    }
    return count;
}

static PyObject* count_not_negatives_decor(PyObject *self, PyObject *args) {
    return array_int16t_to_sizet_decor(self, args, count_not_negatives_impl);
}
