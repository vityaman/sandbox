#include <Python.h>

size_t do_count(int16_t* input, size_t size) {
  return 0; // TODO: implement
}

static PyObject* count_geq0(PyObject *self, PyObject *args) {
  PyObject* input_obj;
  Py_buffer input_buf;
  // Try to read one argument (O = a PyObject)
  if (!PyArg_ParseTuple(args, "O", &input_obj))
      return NULL;

  // Try to read the object as a Py_buffer (e.g. array.array)
  if (PyObject_GetBuffer(input_obj, &input_buf, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1)
      return NULL;

  // Check that the buffer has one dimension and contains int16_t ("h")
  if (input_buf.ndim != 1 || strcmp(input_buf.format, "h") != 0) {
      PyErr_SetString(PyExc_TypeError, "Expected a flat array of 16-bit integers");
      PyBuffer_Release(&input_buf);
      return NULL;
  }

  size_t result = do_count(input_buf.buf, input_buf.shape[0]);

  PyBuffer_Release(&input_buf);
  return PyLong_FromSize_t(result);
}

PyMODINIT_FUNC PyInit_simdcount(void) {
    static PyMethodDef module_methods[] = {
        {"count_geq0", (PyCFunction)count_geq0, METH_VARARGS, "Count >= 0"},
        {NULL}
    };
    static struct PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "simdcount",
        "This module provides a SIMD implementation of count_geq0",
        -1,
        module_methods
    };
    return PyModule_Create(&module_def);
}
