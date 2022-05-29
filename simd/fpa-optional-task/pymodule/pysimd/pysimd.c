#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "count_not_negatives.c"
#include "naive_count_not_negatives.c"


PyMODINIT_FUNC PyInit_pysimd(void) {
    static PyMethodDef methods[] = {
        {
            "count_not_negatives", 
            (PyCFunction)count_not_negatives_decor, 
            METH_VARARGS,    
            "Counts not negative elements in the given array using SIMD."
        },
        {
            "naive_count_not_negatives", 
            (PyCFunction)naive_count_not_negatives_decor, 
            METH_VARARGS,    
            "Counts not negative elements in the given array using naive for-loop."
        },
        {NULL}
    };
    static struct PyModuleDef module = {
        PyModuleDef_HEAD_INIT,
        "pysimd",
        "This module provides a SIMD implementation of count_not_negatives",
        -1,
        methods
    };
    return PyModule_Create(&module);
}
