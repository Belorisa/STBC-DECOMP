static char operator_doc[] = "\
Operator interface.\n\
\n\
This module exports a set of functions implemented in C corresponding\n\
to the intrinsic operators of Python.  For example, operator.add(x, y)\n\
is equivalent to the expression x+y.  The function names are those\n\
used for special class methods; variants without leading and trailing\n\
'__' are also provided for convenience.\n\
";

/*

     Copyright 

       Copyright 1996 Digital Creations, L.C., 910 Princess Anne
       Street, Suite 300, Fredericksburg, Virginia 22401 U.S.A. All
       rights reserved.  Copyright in this software is owned by DCLC,
       unless otherwise indicated. Permission to use, copy and
       distribute this software is hereby granted, provided that the
       above copyright notice appear in all copies and that both that
       copyright notice and this permission notice appear. Note that
       any product, process or technology described in this software
       may be the subject of other Intellectual Property rights
       reserved by Digital Creations, L.C. and are not licensed
       hereunder.

     Trademarks 

       Digital Creations & DCLC, are trademarks of Digital Creations, L.C..
       All other trademarks are owned by their respective companies. 

     No Warranty 

       The software is provided "as is" without warranty of any kind,
       either express or implied, including, but not limited to, the
       implied warranties of merchantability, fitness for a particular
       purpose, or non-infringement. This software could include
       technical inaccuracies or typographical errors. Changes are
       periodically made to the software; these changes will be
       incorporated in new editions of the software. DCLC may make
       improvements and/or changes in this software at any time
       without notice.

     Limitation Of Liability 

       In no event will DCLC be liable for direct, indirect, special,
       incidental, economic, cover, or consequential damages arising
       out of the use of or inability to use this software even if
       advised of the possibility of such damages. Some states do not
       allow the exclusion or limitation of implied warranties or
       limitation of liability for incidental or consequential
       damages, so the above limitation or exclusion may not apply to
       you.


    If you have questions regarding this software,
    contact:
   
      Jim Fulton, jim@digicool.com
      Digital Creations L.C.  
   
      (540) 371-6909

    Modifications
    
      Renamed and slightly rearranged by Guido van Rossum

*/

#include "Python.h"

#define spam1(OP,AOP) static PyObject *OP(s,a) PyObject *s, *a; { \
  PyObject *a1; \
  if(! PyArg_ParseTuple(a,"O",&a1)) return NULL; \
  return AOP(a1); }

#define spam2(OP,AOP) static PyObject *OP(s,a) PyObject *s, *a; { \
  PyObject *a1, *a2; \
  if(! PyArg_ParseTuple(a,"OO",&a1,&a2)) return NULL; \
  return AOP(a1,a2); }

#define spamoi(OP,AOP) static PyObject *OP(s,a) PyObject *s, *a; { \
  PyObject *a1; int a2; \
  if(! PyArg_ParseTuple(a,"Oi",&a1,&a2)) return NULL; \
  return AOP(a1,a2); }

#define spam2n(OP,AOP) static PyObject *OP(s,a) PyObject *s, *a; { \
  PyObject *a1, *a2; \
  if(! PyArg_ParseTuple(a,"OO",&a1,&a2)) return NULL; \
  if(-1 == AOP(a1,a2)) return NULL; \
  Py_INCREF(Py_None); \
  return Py_None; }

#define spam3n(OP,AOP) static PyObject *OP(s,a) PyObject *s, *a; { \
  PyObject *a1, *a2, *a3; \
  if(! PyArg_ParseTuple(a,"OOO",&a1,&a2,&a3)) return NULL; \
  if(-1 == AOP(a1,a2,a3)) return NULL; \
  Py_INCREF(Py_None); \
  return Py_None; }

#define spami(OP,AOP) static PyObject *OP(s,a) PyObject *s, *a; { \
  PyObject *a1; long r; \
  if(! PyArg_ParseTuple(a,"O",&a1)) return NULL; \
  if(-1 == (r=AOP(a1))) return NULL; \
  return PyInt_FromLong(r); }

#define spami2(OP,AOP) static PyObject *OP(s,a) PyObject *s, *a; { \
  PyObject *a1, *a2; long r; \
  if(! PyArg_ParseTuple(a,"OO",&a1,&a2)) return NULL; \
  if(-1 == (r=AOP(a1,a2))) return NULL; \
  return PyInt_FromLong(r); }

// FUNCTION: STBC 0x0076cf10 _isCallable
spami(isCallable       , PyCallable_Check)
// FUNCTION: STBC 0x0076cf50 _isNumberType
spami(isNumberType     , PyNumber_Check)
// FUNCTION: STBC 0x0076cf90 _truth
spami(truth            , PyObject_IsTrue)
// FUNCTION: STBC 0x0076cfd0 _op_add
spam2(op_add           , PyNumber_Add)
// FUNCTION: STBC 0x0076d010 _op_sub
spam2(op_sub           , PyNumber_Subtract)
// FUNCTION: STBC 0x0076d050 _op_mul
spam2(op_mul           , PyNumber_Multiply)
// FUNCTION: STBC 0x0076d090 _op_div
spam2(op_div           , PyNumber_Divide)
// FUNCTION: STBC 0x0076d0d0 _op_mod
spam2(op_mod           , PyNumber_Remainder)
// FUNCTION: STBC 0x0076d110 _op_neg
spam1(op_neg           , PyNumber_Negative)
// FUNCTION: STBC 0x0076d140 _op_pos
spam1(op_pos           , PyNumber_Positive)
// FUNCTION: STBC 0x0076d170 _op_abs
spam1(op_abs           , PyNumber_Absolute)
// FUNCTION: STBC 0x0076d1a0 _op_inv
spam1(op_inv           , PyNumber_Invert)
// FUNCTION: STBC 0x0076d1d0 _op_lshift
spam2(op_lshift        , PyNumber_Lshift)
// FUNCTION: STBC 0x0076d210 _op_rshift
spam2(op_rshift        , PyNumber_Rshift)
// FUNCTION: STBC 0x0076d250 _op_not_
spami(op_not_          , PyObject_Not)
// FUNCTION: STBC 0x0076d290 _op_and_
spam2(op_and_          , PyNumber_And)
// FUNCTION: STBC 0x0076d2d0 _op_xor
spam2(op_xor           , PyNumber_Xor)
// FUNCTION: STBC 0x0076d310 _op_or_
spam2(op_or_           , PyNumber_Or)
// FUNCTION: STBC 0x0076d350 _isSequenceType
spami(isSequenceType   , PySequence_Check)
// FUNCTION: STBC 0x0076d390 _op_concat
spam2(op_concat        , PySequence_Concat)
// FUNCTION: STBC 0x0076d3d0 _op_repeat
spamoi(op_repeat       , PySequence_Repeat)
// FUNCTION: STBC 0x0076d410 _op_contains
spami2(op_contains     , PySequence_Contains)
// FUNCTION: STBC 0x0076d460 _indexOf
spami2(indexOf         , PySequence_Index)
// FUNCTION: STBC 0x0076d4b0 _countOf
spami2(countOf         , PySequence_Count)
// FUNCTION: STBC 0x0076d500 _isMappingType
spami(isMappingType    , PyMapping_Check)
// FUNCTION: STBC 0x0076d540 _op_getitem
spam2(op_getitem       , PyObject_GetItem)
// FUNCTION: STBC 0x0076d580 _op_delitem
spam2n(op_delitem       , PyObject_DelItem)
// FUNCTION: STBC 0x0076d5d0 _op_setitem
spam3n(op_setitem      , PyObject_SetItem)

// FUNCTION: STBC 0x0076d640
static PyObject*
op_getslice(s,a)
        PyObject *s, *a;
{
        PyObject *a1;
        int a2,a3;

        if (!PyArg_ParseTuple(a,"Oii",&a1,&a2,&a3))
                return NULL;
        return PySequence_GetSlice(a1,a2,a3);
}

// FUNCTION: STBC 0x0076d690
static PyObject*
op_setslice(s,a)
        PyObject *s, *a;
{
        PyObject *a1, *a4;
        int a2,a3;

        if (!PyArg_ParseTuple(a,"OiiO",&a1,&a2,&a3,&a4))
                return NULL;

        if (-1 == PySequence_SetSlice(a1,a2,a3,a4))
                return NULL;

        Py_INCREF(Py_None);
        return Py_None;
}

// FUNCTION: STBC 0x0076d700
static PyObject*
op_delslice(s,a)
        PyObject *s, *a;
{
        PyObject *a1;
        int a2,a3;

        if(! PyArg_ParseTuple(a,"Oii",&a1,&a2,&a3))
                return NULL;

        if (-1 == PySequence_DelSlice(a1,a2,a3))
                return NULL;

        Py_INCREF(Py_None);
        return Py_None;
}

#undef spam1
#undef spam2
#ifdef HAVE_OLD_CPP
#define spam1(OP,DOC) {"OP", OP, 1, DOC},
#define spam2(OP,ALTOP,DOC) {"OP", op_/**/OP, 1, DOC}, \
			   {"ALTOP", op_/**/OP, 1, DOC}, 
#else
#define spam1(OP,DOC) {#OP, OP, 1, DOC},
#define spam2(OP,ALTOP,DOC) {#OP, op_##OP, 1, DOC}, \
			   {#ALTOP, op_##OP, 1, DOC}, 
#endif

// DATA: STBC 0x0096a078
static struct PyMethodDef operator_methods[] = {

spam1(isCallable,
 "isCallable(a) -- Same as callable(a).")
spam1(isNumberType,
 "isNumberType(a) -- Return 1 if a has a numeric type, and zero otherwise.")
spam1(isSequenceType,
 "isSequenceType(a) -- Return 1 if a has a sequence type, and zero otherwise.")
spam1(truth,
 "truth(a) -- Return 1 if a is true, and 0 otherwise.")
spam2(contains,sequenceIncludes,
 "contains(a, b) -- Same as b in a (note reversed operands).")
spam1(indexOf,
 "indexOf(a, b) -- Return the first index of b in a.")
spam1(countOf,
 "countOf(a, b) -- Return the number of times b occurs in a.")
spam1(isMappingType,
 "isMappingType(a) -- Return 1 if a has a mapping type, and zero otherwise.")

spam2(add,__add__, "add(a, b) -- Same as a + b.")
spam2(sub,__sub__, "sub(a, b) -- Same as a - b.")
spam2(mul,__mul__, "mul(a, b) -- Same as a * b.")
spam2(div,__div__, "div(a, b) -- Same as a / b.")
spam2(mod,__mod__, "mod(a, b) -- Same as a % b.")
spam2(neg,__neg__, "neg(a) -- Same as -a.")
spam2(pos,__pos__, "pos(a) -- Same as +a.")
spam2(abs,__abs__, "abs(a) -- Same as abs(a).")
spam2(inv,__inv__, "inv(a) -- Same as ~a.")
spam2(lshift,__lshift__, "lshift(a, b) -- Same as a << b.")
spam2(rshift,__rshift__, "rshift(a, b) -- Same as a >> b.")
spam2(not_,__not__, "not_(a) -- Same as not a.")
spam2(and_,__and__, "and_(a, b) -- Same as a & b.")
spam2(xor,__xor__, "xor(a, b) -- Same as a ^ b.")
spam2(or_,__or__, "or_(a, b) -- Same as a | b.")
spam2(concat,__concat__,
 "concat(a, b) -- Same as a + b, for a and b sequences.")
spam2(repeat,__repeat__,
 "repeat(a, b) -- Return a * b, where a is a sequence, and b is an integer.")
spam2(getitem,__getitem__,
 "getitem(a, b) -- Same as a[b].")
spam2(setitem,__setitem__,
 "setitem(a, b, c) -- Same as a[b] = c.")
spam2(delitem,__delitem__,
 "delitem(a, b) -- Same as del a[b].")
spam2(getslice,__getslice__,
 "getslice(a, b, c) -- Same as a[b:c].")
spam2(setslice,__setslice__,
"setslice(a, b, c, d) -- Same as a[b:c] = d.")
spam2(delslice,__delslice__,
"delslice(a, b, c) -- Same as del a[b:c].")

	{NULL,		NULL}		/* sentinel */

};


/* Initialization function for the module (*must* be called initoperator) */

// FUNCTION: STBC 0x0076d770 _initoperator
DL_EXPORT(void)
initoperator()
{
        /* Create the module and add the functions */
        Py_InitModule4("operator", operator_methods, operator_doc,
		       (PyObject*)NULL, PYTHON_API_VERSION);
}
