#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "decor.c"

size_t naive_count_not_negatives_impl(int16_t* input, size_t size) {
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

static PyObject* naive_count_not_negatives_decor(PyObject *self, PyObject *args) {
    return array_int16t_to_sizet_decor(self, args, naive_count_not_negatives_decor);
}
