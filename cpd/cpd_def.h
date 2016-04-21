/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

// This part of the code is greatly inspired by Pure Data and libPD, and sometimes
// directly copied. None of the authors of Pure Data and libPD is responsible for these
// experiments but you must be aware of their unintended contribution.

#ifndef cpd_def_h
#define cpd_def_h

#ifdef _WIN32
#ifdef PD_INTERNAL
#define CPD_EXTERN __declspec(dllexport) extern
#else
#define CPD_EXTERN __declspec(dllimport) extern
#endif
#else
#define CPD_EXTERN extern
#endif

#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) && !defined(__cplusplus)
#define CPD_EXTERN_STRUCT extern struct
#else
#define CPD_EXTERN_STRUCT struct
#endif

CPD_EXTERN_STRUCT _symbol;
CPD_EXTERN_STRUCT _symbol;
CPD_EXTERN_STRUCT _binbuf;
CPD_EXTERN_STRUCT _internal;
CPD_EXTERN_STRUCT _gpointer;
CPD_EXTERN_STRUCT _glist;
CPD_EXTERN_STRUCT _text;
CPD_EXTERN_STRUCT _iemgui;
CPD_EXTERN_STRUCT _atom;

#endif // cpd_def_h
