/* -*- Mode: C; c-basic-offset: 4 -*-
    Gimp-Python - allows the writing of Gimp plugins in Python.
    Copyright (C) 1997-2002  James Henstridge <james@daa.com.au>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "pygimp.h"
#include <sysmodule.h>
#include <structmember.h>

/* maximum bits per pixel ... */
#define MAX_BPP 4

/* part of Hans Breuer's pygimp on win32 patch */
#if defined(_MSC_VER)
/* reduce strict checking in glibconfig.h */
#  pragma warning(default:4047)
#endif

PyObject *pygimp_error;

#ifndef PG_DEBUG
# define PG_DEBUG 0
#endif


/* End of code for pdbFunc objects */
/* -------------------------------------------------------- */

GimpPlugInInfo PLUG_IN_INFO = {
    NULL, /* init_proc */
    NULL, /* quit_proc */
    NULL, /* query_proc */
    NULL  /* run_proc */
};

static PyObject *callbacks[] = {
    NULL, NULL, NULL, NULL
};

static void
pygimp_init_proc(void)
{
    PyObject *r;
    r = PyObject_CallFunction(callbacks[0], "()");
    if (!r) {
	PyErr_Print();
	PyErr_Clear();
	return;
    }
    Py_DECREF(r);
}

static void
pygimp_quit_proc(void)
{
    PyObject *r;
    r = PyObject_CallFunction(callbacks[1], "()");
    if (!r) {
	PyErr_Print();
	PyErr_Clear();
	return;
    }
    Py_DECREF(r);
}

static void
pygimp_query_proc(void)
{
    PyObject *r;
    r = PyObject_CallFunction(callbacks[2], "()");
    if (!r) {
	PyErr_Print();
	PyErr_Clear();
	return;
    }
    Py_DECREF(r);
}

static void
pygimp_run_proc(char *name, int nparams, GimpParam *params,
		int *nreturn_vals, GimpParam **return_vals)
{
    PyObject *args, *ret;
    GimpParamDef *pd, *rv;
    GimpPDBProcType t;
    char *b, *h, *a, *c, *d;
    int np, nrv;

    gimp_procedural_db_proc_info(name, &b, &h, &a, &c, &d, &t, &np, &nrv,
			 &pd, &rv);
    g_free(b); g_free(h); g_free(a); g_free(c); g_free(d); g_free(pd);

#if PG_DEBUG > 0
    fprintf(stderr, "Params for %s:", name);
    print_GParam(nparams, params);
#endif
    args = pygimp_param_to_tuple(nparams, params);
    if (args == NULL) {
	PyErr_Clear();
	*nreturn_vals = 1;
	*return_vals = g_new(GimpParam, 1);
	(*return_vals)[0].type = GIMP_PDB_STATUS;
	(*return_vals)[0].data.d_status = GIMP_PDB_CALLING_ERROR;
	return;
    }
    ret = PyObject_CallFunction(callbacks[3], "(sO)", name, args);
    Py_DECREF(args);
    if (ret == NULL) {
	PyErr_Print();
	PyErr_Clear();
	*nreturn_vals = 1;
	*return_vals = g_new(GimpParam, 1);
	(*return_vals)[0].type = GIMP_PDB_STATUS;
	(*return_vals)[0].data.d_status = GIMP_PDB_EXECUTION_ERROR;
	return;
    }
    *return_vals = pygimp_param_from_tuple(ret, rv, nrv);
    g_free(rv);
    if (*return_vals == NULL) {
	PyErr_Clear();
	*nreturn_vals = 1;
	*return_vals = g_new(GimpParam, 1);
	(*return_vals)[0].type = GIMP_PDB_STATUS;
	(*return_vals)[0].data.d_status = GIMP_PDB_EXECUTION_ERROR;
	return;
    }
    Py_DECREF(ret);
    *nreturn_vals = nrv + 1;
    (*return_vals)[0].type = GIMP_PDB_STATUS;
    (*return_vals)[0].data.d_status = GIMP_PDB_SUCCESS;
}

static PyObject *
pygimp_main(PyObject *self, PyObject *args)
{
    PyObject *av;
    int argc, i;
    char **argv;
    PyObject *ip, *qp, *query, *rp;
	
    if (!PyArg_ParseTuple(args, "OOOO:main", &ip, &qp, &query, &rp))
	return NULL;

#define Arg_Check(v) (PyCallable_Check(v) || (v) == Py_None)

    if (!Arg_Check(ip) || !Arg_Check(qp) || !Arg_Check(query) ||
	!Arg_Check(rp)) {
	PyErr_SetString(pygimp_error, "arguments must be callable.");
	return NULL;
    }
    if (query == Py_None) {
	PyErr_SetString(pygimp_error, "a query procedure must be provided.");
	return NULL;
    }

    if (ip != Py_None) {
	callbacks[0] = ip;
	PLUG_IN_INFO.init_proc = pygimp_init_proc;
    }
    if (qp != Py_None) {
	callbacks[1] = qp;
	PLUG_IN_INFO.quit_proc = pygimp_quit_proc;
    }
    if (query != Py_None) {
	callbacks[2] = query;
	PLUG_IN_INFO.query_proc = pygimp_query_proc;
    }
    if (rp != Py_None) {
	callbacks[3] = rp;
	PLUG_IN_INFO.run_proc = pygimp_run_proc;
    }

    av = PySys_GetObject("argv");

    argc = PyList_Size(av);
    argv = g_new(char *, argc);

    for (i = 0; i < argc; i++)
	argv[i] = g_strdup(PyString_AsString(PyList_GetItem(av, i)));

#ifdef G_OS_WIN32
    {
	extern void set_gimp_PLUG_IN_INFO_PTR(GimpPlugInInfo *);
	set_gimp_PLUG_IN_INFO_PTR(&PLUG_IN_INFO);
    }
#endif

    gimp_main(argc, argv);

    if (argv != NULL) {
	for (i = 0; i < argc; i++)
	    if (argv[i] != NULL)
		g_free(argv[i]);
	g_free(argv);
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_quit(PyObject *self, PyObject *args)
{

    if (!PyArg_ParseTuple(args, ":quit"))
	return NULL;
    gimp_quit();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_set_data(PyObject *self, PyObject *args)
{
    char *id, *data;
    int bytes, nreturn_vals;
    GimpParam *return_vals;

    if (!PyArg_ParseTuple(args, "ss#:set_data", &id, &data, &bytes))
	return NULL;
    return_vals = gimp_run_procedure("gimp_procedural_db_set_data",
				     &nreturn_vals, GIMP_PDB_STRING, id, GIMP_PDB_INT32, bytes,
				     GIMP_PDB_INT8ARRAY, data, GIMP_PDB_END);
    if (return_vals[0].data.d_status != GIMP_PDB_SUCCESS) {
	PyErr_SetString(pygimp_error, "error occurred while storing");
	return NULL;
    }
    gimp_destroy_params(return_vals, nreturn_vals);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_get_data(PyObject *self, PyObject *args)
{
    char *id;
    int nreturn_vals;
    GimpParam *return_vals;
    PyObject *s;

    if (!PyArg_ParseTuple(args, "s:get_data", &id))
	return NULL;

    return_vals = gimp_run_procedure("gimp_procedural_db_get_data",
				     &nreturn_vals, GIMP_PDB_STRING, id, GIMP_PDB_END);

    if (return_vals[0].data.d_status != GIMP_PDB_SUCCESS) {
	PyErr_SetString(pygimp_error, "no data for id");
	return NULL;
    }
    s = PyString_FromStringAndSize((char *)return_vals[2].data.d_int8array,
				   return_vals[1].data.d_int32);
    gimp_destroy_params(return_vals, nreturn_vals);
    return s;
}

static PyObject *
pygimp_progress_init(PyObject *self, PyObject *args)
{
    char *msg = NULL;
    if (!PyArg_ParseTuple(args, "|s:progress_init", &msg))
	return NULL;
    gimp_progress_init(msg);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_progress_update(PyObject *self, PyObject *args)
{
    double p;
    if (!PyArg_ParseTuple(args, "d:progress_update", &p))
	return NULL;
    gimp_progress_update(p);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_query_images(PyObject *self, PyObject *args)
{
    gint32 *imgs;
    int nimgs, i;
    PyObject *ret;
    if (!PyArg_ParseTuple(args, ":query_images"))
	return NULL;
    imgs = gimp_image_list(&nimgs);
    ret = PyList_New(nimgs);
    for (i = 0; i < nimgs; i++)
	PyList_SetItem(ret, i, (PyObject *)pygimp_image_new(imgs[i]));
    return ret;
}

static PyObject *
pygimp_install_procedure(PyObject *self, PyObject *args)
{
    char *name, *blurb, *help, *author, *copyright, *date, *menu_path,
	*image_types, *n, *d;
    GimpParamDef *params, *return_vals;
    int type, nparams, nreturn_vals, i;
    PyObject *pars, *rets;

    if (!PyArg_ParseTuple(args, "sssssszziOO:install_procedure",
			  &name, &blurb, &help,
			  &author, &copyright, &date, &menu_path, &image_types,
			  &type, &pars, &rets))
	return NULL;
    if (!PySequence_Check(pars) || !PySequence_Check(rets)) {
	PyErr_SetString(PyExc_TypeError,
			"last two args must be sequences");
	return NULL;
    }
    nparams = PySequence_Length(pars);
    nreturn_vals = PySequence_Length(rets);
    params = g_new(GimpParamDef, nparams);
    for (i = 0; i < nparams; i++) {
	if (!PyArg_ParseTuple(PySequence_GetItem(pars, i), "iss",
			      &(params[i].type), &n, &d)) {
	    g_free(params);
	    return NULL;
	}
	params[i].name = g_strdup(n);
	params[i].description = g_strdup(d);
    }
    return_vals = g_new(GimpParamDef, nreturn_vals);
    for (i = 0; i < nreturn_vals; i++) {
	if (!PyArg_ParseTuple(PySequence_GetItem(rets, i), "iss",
			      &(return_vals[i].type), &n, &d)) {
	    g_free(params); g_free(return_vals);
	    return NULL;
	}
	return_vals[i].name = g_strdup(n);
	return_vals[i].description = g_strdup(d);
    }
    gimp_install_procedure(name, blurb, help, author, copyright, date,
			   menu_path, image_types, type, nparams, nreturn_vals, params,
			   return_vals);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_install_temp_proc(PyObject *self, PyObject *args)
{
    char *name, *blurb, *help, *author, *copyright, *date, *menu_path,
	*image_types, *n, *d;
    GimpParamDef *params, *return_vals;
    int type, nparams, nreturn_vals, i;
    PyObject *pars, *rets;

    if (!PyArg_ParseTuple(args, "sssssszziOO:install_temp_proc",
			  &name, &blurb, &help,
			  &author, &copyright, &date, &menu_path, &image_types,
			  &type, &pars, &rets))
	return NULL;
    if (!PySequence_Check(pars) || !PySequence_Check(rets)) {
	PyErr_SetString(PyExc_TypeError,
			"last two args must be sequences");
	return NULL;
    }
    nparams = PySequence_Length(pars);
    nreturn_vals = PySequence_Length(rets);
    params = g_new(GimpParamDef, nparams);
    for (i = 0; i < nparams; i++) {
	if (!PyArg_ParseTuple(PySequence_GetItem(pars, i), "iss",
			      &(params[i].type), &n, &d)) {
	    g_free(params);
	    return NULL;
	}
	params[i].name = g_strdup(n);
	params[i].description = g_strdup(d);
    }
    return_vals = g_new(GimpParamDef, nreturn_vals);
    for (i = 0; i < nreturn_vals; i++) {
	if (!PyArg_ParseTuple(PySequence_GetItem(rets, i), "iss",
			      &(return_vals[i].type), &n, &d)) {
	    g_free(params); g_free(return_vals);
	    return NULL;
	}
	return_vals[i].name = g_strdup(n);
	return_vals[i].description = g_strdup(d);
    }
    gimp_install_temp_proc(name, blurb, help, author, copyright, date,
			   menu_path, image_types, type, nparams, nreturn_vals, params,
			   return_vals, pygimp_run_proc);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_uninstall_temp_proc(PyObject *self, PyObject *args)
{
    char *name;

    if (!PyArg_ParseTuple(args, "s:uninstall_temp_proc", &name))
	return NULL;
    gimp_uninstall_temp_proc(name);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_register_magic_load_handler(PyObject *self, PyObject *args)
{
    char *name, *extensions, *prefixes, *magics;
    if (!PyArg_ParseTuple(args, "ssss:register_magic_load_handler",
			  &name, &extensions, &prefixes, &magics))
	return NULL;
    gimp_register_magic_load_handler(name, extensions, prefixes, magics);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_register_load_handler(PyObject *self, PyObject *args)
{
    char *name, *extensions, *prefixes;
    if (!PyArg_ParseTuple(args, "sss:register_load_handler",
			  &name, &extensions, &prefixes))
	return NULL;
    gimp_register_load_handler(name, extensions, prefixes);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_register_save_handler(PyObject *self, PyObject *args)
{
    char *name, *extensions, *prefixes;
    if (!PyArg_ParseTuple(args, "sss:register_save_handler",
			  &name, &extensions, &prefixes))
	return NULL;
    gimp_register_save_handler(name, extensions, prefixes);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_gamma(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":gamma"))
	return NULL;
    return PyFloat_FromDouble(gimp_gamma());
}

static PyObject *
pygimp_install_cmap(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":install_cmap"))
	return NULL;
    return PyInt_FromLong(gimp_install_cmap());
}

static PyObject *
pygimp_gtkrc(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":gtkrc"))
	return NULL;
    return PyString_FromString(gimp_gtkrc());
}

static PyObject *
pygimp_get_background(PyObject *self, PyObject *args)
{
    GimpRGB colour;
    guchar r, g, b;
    if (!PyArg_ParseTuple(args, ":get_background"))
	return NULL;
    gimp_palette_get_background(&colour);
    gimp_rgb_get_uchar(&colour, &r, &g, &b);
    return Py_BuildValue("(iii)", (int)r, (int)g, (int)b);
}

static PyObject *
pygimp_get_foreground(PyObject *self, PyObject *args)
{
    GimpRGB colour;
    guchar r, g, b;
    if (!PyArg_ParseTuple(args, ":get_foreground"))
	return NULL;
    gimp_palette_get_foreground(&colour);
    gimp_rgb_get_uchar(&colour, &r, &g, &b);
    return Py_BuildValue("(iii)", (int)r, (int)g, (int)b);
}

static PyObject *
pygimp_set_background(PyObject *self, PyObject *args)
{
    GimpRGB colour;
    int r, g, b;
    if (!PyArg_ParseTuple(args, "(iii):set_background", &r, &g, &b)) {
	PyErr_Clear();
	if (!PyArg_ParseTuple(args, "iii:set_background", &r, &g, &b))
	    return NULL;
    }
    gimp_rgb_set_uchar(&colour, r, g, b);
    gimp_palette_set_background(&colour);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_set_foreground(PyObject *self, PyObject *args)
{
    GimpRGB colour;
    int r, g, b;
    if (!PyArg_ParseTuple(args, "(iii):set_foreground", &r, &g, &b)) {
	PyErr_Clear();
	if (!PyArg_ParseTuple(args, "iii:set_foreground", &r, &g, &b))
	    return NULL;
    }
    gimp_rgb_set_uchar(&colour, r, g, b);
    gimp_palette_set_foreground(&colour);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_gradients_get_list(PyObject *self, PyObject *args)
{
    char **list;
    int num, i;
    PyObject *ret;
    if (!PyArg_ParseTuple(args, ":gradients_get_list"))
	return NULL;
    list = gimp_gradients_get_list(&num);
    ret = PyList_New(num);
    for (i = 0; i < num; i++)
	PyList_SetItem(ret, i, PyString_FromString(list[i]));
    g_free(list);
    return ret;
}

static PyObject *
pygimp_gradients_get_gradient(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":gradients_get_gradient"))
	return NULL;
    return PyString_FromString(gimp_gradients_get_gradient());
}

static PyObject *
pygimp_gradients_set_gradient(PyObject *self, PyObject *args)
{
    char *actv;
    if (!PyArg_ParseTuple(args, "s:gradients_set_gradient", &actv))
	return NULL;
    gimp_gradients_set_gradient(actv);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_gradients_sample_uniform(PyObject *self, PyObject *args)
{
    int num, i, j;
    double *samp;
    PyObject *ret;
    if (!PyArg_ParseTuple(args, "i:gradients_sample_uniform", &num))
	return NULL;
    samp = gimp_gradients_sample_uniform(num);
    ret = PyList_New(num);
    for (i = 0, j = 0; i < num; i++, j += 4)
	PyList_SetItem(ret, i, Py_BuildValue("(dddd)", samp[j],
					     samp[j+1], samp[j+2], samp[j+3]));
    g_free(samp);
    return ret;
}

static PyObject *
pygimp_gradients_sample_custom(PyObject *self, PyObject *args)
{
    int num, i, j;
    double *pos, *samp;
    PyObject *ret, *item;
    if (!PyArg_ParseTuple(args, "O:gradients_sample_custom", &ret))
	return NULL;
    if (!PySequence_Check(ret)) {
	PyErr_SetString(PyExc_TypeError,
			"second arg must be a sequence");
	return NULL;
    }
    num = PySequence_Length(ret);
    pos = g_new(gdouble, num);
    for (i = 0; i < num; i++) {
	item = PySequence_GetItem(ret, i);
	if (!PyFloat_Check(item)) {
	    PyErr_SetString(PyExc_TypeError,
			    "second arg must be a sequence of floats");
	    g_free(pos);
	    return NULL;
	}
	pos[i] = PyFloat_AsDouble(item);
    }
    samp = gimp_gradients_sample_custom(num, pos);
    g_free(pos);
    ret = PyList_New(num);
    for (i = 0, j = 0; i < num; i++, j += 4)
	PyList_SetItem(ret, i, Py_BuildValue("(dddd)", samp[j],
					     samp[j+1], samp[j+2], samp[j+3]));
    g_free(samp);
    return ret;
}

static PyObject *
pygimp_delete(PyObject *self, PyObject *args)
{
    PyGimpImage *img;

    if (!PyArg_ParseTuple(args, "O:delete", &img))
	return NULL;
    if (pygimp_image_check(img)) gimp_image_delete(img->ID);
    else if (pygimp_layer_check(img)) gimp_layer_delete(img->ID);
    else if (pygimp_channel_check(img)) gimp_channel_delete(img->ID);
    else if (pygimp_display_check(img)) gimp_display_delete(img->ID);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pygimp_displays_flush(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":flush"))
	return NULL;
    gimp_displays_flush();
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pygimp_tile_cache_size(PyObject *self, PyObject *args)
{
    unsigned long k;
    if (!PyArg_ParseTuple(args, "l:tile_cache_size", &k))
	return NULL;
    gimp_tile_cache_size(k);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pygimp_tile_cache_ntiles(PyObject *self, PyObject *args)
{
    unsigned long n;
    if (!PyArg_ParseTuple(args, "l:tile_cache_ntiles", &n))
	return NULL;
    gimp_tile_cache_ntiles(n);
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *
pygimp_tile_width(PyObject *self, PyObject *args)
{
    if (PyArg_ParseTuple(args, ":tile_width"))
	return NULL;
    return PyInt_FromLong(gimp_tile_width());
}


static PyObject *
pygimp_tile_height(PyObject *self, PyObject *args)
{
    if (PyArg_ParseTuple(args, ":tile_height"))
	return NULL;
    return PyInt_FromLong(gimp_tile_height());
}

void gimp_extension_ack     (void);
void gimp_extension_process (guint timeout);

static PyObject *
pygimp_extension_ack(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":extension_ack"))
	return NULL;
    gimp_extension_ack();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_extension_process(PyObject *self, PyObject *args)
{
    int timeout;

    if (!PyArg_ParseTuple(args, "i:extension_process", &timeout))
	return NULL;
    gimp_extension_process(timeout);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
new_parasite(PyObject *self, PyObject *args)
{
    char *name, *data;
    int flags, size;
    if (!PyArg_ParseTuple(args, "sis#:parasite", &name, &flags,
			  &data, &size))
	return NULL;
    return pygimp_parasite_new(gimp_parasite_new(name, flags, size, data));
}

static PyObject *
pygimp_parasite_find(PyObject *self, PyObject *args)
{
    char *name;
    if (!PyArg_ParseTuple(args, "s:parasite_find", &name))
	return NULL;
    return pygimp_parasite_new(gimp_parasite_find(name));
}

static PyObject *
pygimp_parasite_attach(PyObject *self, PyObject *args)
{
    PyGimpParasite *parasite;
    if (!PyArg_ParseTuple(args, "O!:parasite_attach",
			  &PyGimpParasite_Type, &parasite))
	return NULL;
    gimp_parasite_attach(parasite->para);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_attach_new_parasite(PyObject *self, PyObject *args)
{
    char *name, *data;
    int flags, size;
    if (!PyArg_ParseTuple(args, "sis#:attach_new_parasite", &name, &flags,
			  &data, &size))
	return NULL;
    gimp_attach_new_parasite(name, flags, size, data);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_parasite_detach(PyObject *self, PyObject *args)
{
    char *name;
    if (!PyArg_ParseTuple(args, "s:parasite_detach", &name))
	return NULL;
    gimp_parasite_detach(name);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pygimp_default_display(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":default_display"))
	return NULL;
    return pygimp_display_new(gimp_default_display());
}

static PyObject *
id2image(PyObject *self, PyObject *args)
{
    int id;
    if (!PyArg_ParseTuple(args, "i:_id2image", &id))
	return NULL;
    if (id >= 0)
	return pygimp_image_new(id);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
id2drawable(PyObject *self, PyObject *args)
{
    int id;
    if (!PyArg_ParseTuple(args, "i:_id2drawable", &id))
	return NULL;
    if (id >= 0)
	return pygimp_drawable_new(NULL, id);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
id2display(PyObject *self, PyObject *args)
{
    int id;
    if (!PyArg_ParseTuple(args, "i:_id2display", &id))
	return NULL;
    if (id >= 0)
	return pygimp_display_new(id);
    Py_INCREF(Py_None);
    return Py_None;
}

/* List of methods defined in the module */

static struct PyMethodDef gimp_methods[] = {
    {"main",	(PyCFunction)pygimp_main,	METH_VARARGS},
    {"quit",	(PyCFunction)pygimp_quit,	METH_VARARGS},
    {"set_data",	(PyCFunction)pygimp_set_data,	METH_VARARGS},
    {"get_data",	(PyCFunction)pygimp_get_data,	METH_VARARGS},
    {"progress_init",	(PyCFunction)pygimp_progress_init,	METH_VARARGS},
    {"progress_update",	(PyCFunction)pygimp_progress_update,	METH_VARARGS},
    {"query_images",	(PyCFunction)pygimp_query_images,	METH_VARARGS},
    {"install_procedure",	(PyCFunction)pygimp_install_procedure,	METH_VARARGS},
    {"install_temp_proc",	(PyCFunction)pygimp_install_temp_proc,	METH_VARARGS},
    {"uninstall_temp_proc",	(PyCFunction)pygimp_uninstall_temp_proc,	METH_VARARGS},
    {"register_magic_load_handler",	(PyCFunction)pygimp_register_magic_load_handler,	METH_VARARGS},
    {"register_load_handler",	(PyCFunction)pygimp_register_load_handler,	METH_VARARGS},
    {"register_save_handler",	(PyCFunction)pygimp_register_save_handler,	METH_VARARGS},
    {"gamma",	(PyCFunction)pygimp_gamma,	METH_VARARGS},
    {"install_cmap",	(PyCFunction)pygimp_install_cmap,	METH_VARARGS},
    {"gtkrc",	(PyCFunction)pygimp_gtkrc,	METH_VARARGS},
    {"get_background",	(PyCFunction)pygimp_get_background,	METH_VARARGS},
    {"get_foreground",	(PyCFunction)pygimp_get_foreground,	METH_VARARGS},
    {"set_background",	(PyCFunction)pygimp_set_background,	METH_VARARGS},
    {"set_foreground",	(PyCFunction)pygimp_set_foreground,	METH_VARARGS},
    {"gradients_get_list",	(PyCFunction)pygimp_gradients_get_list,	METH_VARARGS},
    {"gradients_get_gradient",	(PyCFunction)pygimp_gradients_get_gradient,	METH_VARARGS},
    {"gradients_set_gradient",	(PyCFunction)pygimp_gradients_set_gradient,	METH_VARARGS},
    {"gradients_sample_uniform",	(PyCFunction)pygimp_gradients_sample_uniform,	METH_VARARGS},
    {"gradients_sample_custom",	(PyCFunction)pygimp_gradients_sample_custom,	METH_VARARGS},
    {"delete", (PyCFunction)pygimp_delete, METH_VARARGS},
    {"displays_flush", (PyCFunction)pygimp_displays_flush, METH_VARARGS},
    {"tile_cache_size", (PyCFunction)pygimp_tile_cache_size, METH_VARARGS},
    {"tile_cache_ntiles", (PyCFunction)pygimp_tile_cache_ntiles, METH_VARARGS},
    {"tile_width", (PyCFunction)pygimp_tile_width, METH_VARARGS},
    {"tile_height", (PyCFunction)pygimp_tile_height, METH_VARARGS},
    {"extension_ack", (PyCFunction)pygimp_extension_ack, METH_VARARGS},
    {"extension_process", (PyCFunction)pygimp_extension_process, METH_VARARGS},
    {"parasite",           (PyCFunction)new_parasite,            METH_VARARGS},
    {"parasite_find",      (PyCFunction)pygimp_parasite_find,      METH_VARARGS},
    {"parasite_attach",    (PyCFunction)pygimp_parasite_attach,    METH_VARARGS},
    {"attach_new_parasite",(PyCFunction)pygimp_attach_new_parasite,METH_VARARGS},
    {"parasite_detach",    (PyCFunction)pygimp_parasite_detach,    METH_VARARGS},
    {"default_display",  (PyCFunction)pygimp_default_display,  METH_VARARGS},
    {"_id2image", (PyCFunction)id2image, METH_VARARGS},
    {"_id2drawable", (PyCFunction)id2drawable, METH_VARARGS},
    {"_id2display", (PyCFunction)id2display, METH_VARARGS},
    {NULL,	 (PyCFunction)NULL, 0, NULL}		/* sentinel */
};


/* Initialization function for the module (*must* be called initgimp) */

static char gimp_module_documentation[] = 
"This module provides interfaces to allow you to write gimp plugins"
;

void
initgimp()
{
    PyObject *m, *d;
    PyObject *i;

    PyGimpPDB_Type.ob_type = &PyType_Type;
    PyGimpPDB_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpPDB_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpPDB_Type) < 0)
	return;

    PyGimpPDBFunction_Type.ob_type = &PyType_Type;
    PyGimpPDBFunction_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpPDBFunction_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpPDBFunction_Type) < 0)
	return;

    PyGimpImage_Type.ob_type = &PyType_Type;
    PyGimpImage_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpImage_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpImage_Type) < 0)
	return;

    PyGimpDisplay_Type.ob_type = &PyType_Type;
    PyGimpDisplay_Type.ob_type = &PyType_Type;
    PyGimpDisplay_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpDisplay_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpDisplay_Type) < 0)
	return;

    PyGimpLayer_Type.ob_type = &PyType_Type;
    PyGimpLayer_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpLayer_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpLayer_Type) < 0)
	return;

    PyGimpChannel_Type.ob_type = &PyType_Type;
    PyGimpChannel_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpChannel_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpChannel_Type) < 0)
	return;

    PyGimpTile_Type.ob_type = &PyType_Type;
    PyGimpTile_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpTile_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpTile_Type) < 0)
	return;

    PyGimpPixelRgn_Type.ob_type = &PyType_Type;
    PyGimpPixelRgn_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpPixelRgn_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpPixelRgn_Type) < 0)
	return;

    PyGimpParasite_Type.ob_type = &PyType_Type;
    PyGimpParasite_Type.tp_alloc = PyType_GenericAlloc;
    PyGimpParasite_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyGimpParasite_Type) < 0)
	return;

    /* Create the module and add the functions */
    m = Py_InitModule4("gimp", gimp_methods,
		       gimp_module_documentation,
		       (PyObject*)NULL,PYTHON_API_VERSION);

    /* Add some symbolic constants to the module */
    d = PyModule_GetDict(m);
    pygimp_error = PyErr_NewException("gimp.error", PyExc_RuntimeError, NULL);
    PyDict_SetItemString(d, "error", pygimp_error);

    PyDict_SetItemString(d, "pdb", pygimp_pdb_new());

    /* export the types used in gimpmodule */
    PyDict_SetItemString(d, "Image", (PyObject *)&PyGimpImage_Type);
    PyDict_SetItemString(d, "Drawable", (PyObject *)&PyGimpDrawable_Type);
    PyDict_SetItemString(d, "Layer", (PyObject *)&PyGimpLayer_Type);
    PyDict_SetItemString(d, "Channel", (PyObject *)&PyGimpChannel_Type);
    PyDict_SetItemString(d, "Display", (PyObject *)&PyGimpDisplay_Type);
    PyDict_SetItemString(d, "Tile", (PyObject *)&PyGimpTile_Type);
    PyDict_SetItemString(d, "PixelRgn", (PyObject *)&PyGimpPixelRgn_Type);
    PyDict_SetItemString(d, "Parasite", (PyObject *)&PyGimpParasite_Type);

    PyDict_SetItemString(d, "version",
			 i=Py_BuildValue("(iii)",
					 gimp_major_version,
					 gimp_minor_version,
					 gimp_micro_version));
    Py_DECREF(i);
	
    /* Check for errors */
    if (PyErr_Occurred())
	Py_FatalError("can't initialize module gimp");
}
