/*
===============================================================================

	SWIGRuntime.h

	The part of the SWIG 1.3.6 runtime that the game's own Python glue
	calls: AppWrap.cpp is built with SWIG_GLOBAL, so its runtime functions
	have external C linkage (SWIG_NOINCLUDE's declarations), and PyEmbed
	converts shadow instances to their C++ objects with them. The type
	information itself is defined by the runtime in AppWrap.cpp.

===============================================================================
*/

#ifndef __SWIGRUNTIME_H__
#define __SWIGRUNTIME_H__

#include "Python.h"

struct swig_type_info;

extern "C" {
swig_type_info *	SWIG_TypeQuery( const char *name );
int					SWIG_ConvertPtr( PyObject *obj, void **ptr, swig_type_info *ty, int flags );
}

#endif /* !__SWIGRUNTIME_H__ */
