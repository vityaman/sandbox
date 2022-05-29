#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <emmintrin.h>

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
    PyObject* argument;
    Py_buffer array;
    int argumentSuccessfullyParsed = PyArg_ParseTuple(args, "O", &argument);
    if (!argumentSuccessfullyParsed) {
        PyErr_SetString(PyExc_TypeError, "Invalid argument: expected Object");
        return NULL;
    }
    int arraySuccessfullyParsed = PyObject_GetBuffer(
        argument, &array, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT
    ) == 0;
    if (!arraySuccessfullyParsed) {
        PyErr_SetString(PyExc_TypeError, "Invalid argument: expected array");
        return NULL;
    }
    int arrayIsOneDimentionalAndContainsInt16 
            = ((array.ndim == 1) && (strcmp(array.format, "h") == 0));
    if (!arrayIsOneDimentionalAndContainsInt16) {
        PyErr_SetString(PyExc_TypeError, "Expected a flat array of 16-bit integers");
        PyBuffer_Release(&array);
        return NULL;
    }
    size_t result = count_not_negatives_impl(array.buf, array.shape[0]);
    PyBuffer_Release(&array);
    return PyLong_FromSize_t(result);
}

