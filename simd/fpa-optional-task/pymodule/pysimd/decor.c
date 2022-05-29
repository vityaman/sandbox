#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* array_int16t_to_sizet_decor(
    PyObject *self, PyObject *args, size_t (*f)(int16_t*, size_t)
) {
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
    size_t result = f(array.buf, array.shape[0]);
    PyBuffer_Release(&array);
    return PyLong_FromSize_t(result);
}