#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <math.h>
#include <stdlib.h>

// Function to calculate sum
static PyObject* sum_values(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list)) return NULL;

    Py_ssize_t len = PyList_Size(list);
    double sum = 0.0;
    for (Py_ssize_t i = 0; i < len; i++) {
        sum += PyFloat_AsDouble(PyList_GetItem(list, i));
    }
    return PyFloat_FromDouble(sum);
}

// Function to calculate mean
static PyObject* mean_values(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list)) return NULL;

    Py_ssize_t len = PyList_Size(list);
    double sum = 0.0;
    for (Py_ssize_t i = 0; i < len; i++) {
        sum += PyFloat_AsDouble(PyList_GetItem(list, i));
    }
    double mean = sum / len;
    return PyFloat_FromDouble(mean);
}

// Function to calculate standard deviation
static PyObject* std_dev(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list)) return NULL;

    Py_ssize_t len = PyList_Size(list);
    double sum = 0.0, sq_diff = 0.0;
    for (Py_ssize_t i = 0; i < len; i++) {
        sum += PyFloat_AsDouble(PyList_GetItem(list, i));
    }
    double mean = sum / len;

    for (Py_ssize_t i = 0; i < len; i++) {
        double val = PyFloat_AsDouble(PyList_GetItem(list, i));
        sq_diff += (val - mean) * (val - mean);
    }
    double std = sqrt(sq_diff / (len - 1)); // Sample standard deviation
    return PyFloat_FromDouble(std);
}

// Function to calculate mode (most frequent number)
static PyObject* mode_value(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list)) return NULL;

    Py_ssize_t len = PyList_Size(list);
    if (len == 0) return NULL;

    double mode = 0.0;
    int max_count = 0;

    for (Py_ssize_t i = 0; i < len; i++) {
        double val_i = PyFloat_AsDouble(PyList_GetItem(list, i));
        int count = 0;
        for (Py_ssize_t j = 0; j < len; j++) {
            double val_j = PyFloat_AsDouble(PyList_GetItem(list, j));
            if (val_i == val_j) count++;
        }
        if (count > max_count) {
            max_count = count;
            mode = val_i;
        }
    }
    return PyFloat_FromDouble(mode);
}

// Function to count values
static PyObject* count_values(PyObject *self, PyObject *args) {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "O", &list)) return NULL;

    Py_ssize_t len = PyList_Size(list);
    return PyLong_FromSsize_t(len);
}

// Method definitions
static PyMethodDef StatsMethods[] = {
    {"sum_values", sum_values, METH_VARARGS, "Calculate sum of values"},
    {"mean_values", mean_values, METH_VARARGS, "Calculate mean"},
    {"std_dev", std_dev, METH_VARARGS, "Calculate standard deviation"},
    {"mode_value", mode_value, METH_VARARGS, "Calculate mode"},
    {"count_values", count_values, METH_VARARGS, "Count total values"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef statsmodule = {
    PyModuleDef_HEAD_INIT,
    "q3stats",     // name in Python
    "Custom statistics C extension",
    -1,
    StatsMethods
};

// Module init
PyMODINIT_FUNC PyInit_q3stats(void) {
    return PyModule_Create(&statsmodule);
}
