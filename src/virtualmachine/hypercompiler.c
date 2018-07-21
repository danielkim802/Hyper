#include "hypercompiler.h"

int compile(char* filename) {
	Py_Initialize();
	PyObject* sys = PyImport_ImportModule("sys");
	PyObject* path = PyObject_GetAttrString(sys, "path");
	PyList_Append(path, PyString_FromString("../compiler"));

	PyObject* name = PyString_FromString("hypercompiler");
	PyObject* module = PyImport_Import(name);
	PyObject* dict = PyModule_GetDict(module);
	PyObject* class = PyDict_GetItemString(dict, "Compiler");
	PyObject* args = PyTuple_New(1);
	PyTuple_SetItem(args, 0, PyString_FromString(filename));
	PyObject* object = PyObject_CallObject(class, args);
	if (object == NULL)
		return 0;

	PyObject* funcname = PyString_FromString("compile");
	PyObject* func = PyObject_GetAttr(object, funcname);
	PyObject* res = PyObject_CallObject(func, NULL);
	if (res == NULL)
		return 0;
	return 1;
}