/*
===============================================================================

	PythonMethodProcess.h

	A time-slice process that calls a method of a Python instance on each
	update: the scripts make one, give it the instance (SetInstance) and the
	method's name (SetFunction), and set its delay and priority.

	0x38 bytes. Evidence: the SWIG wrapper new_PythonMethodProcess allocates
	0x38 bytes and expands the constructor: TimeSliceProcess's constructor
	(0x46F380), the vtable 0x894688 (TimeSliceProcess's 2 slots: Update,
	0x46F930, and the scalar deleting destructor 0x5FCCA0) and 0x30 and 0x34
	cleared. Update runs TimeSliceProcess's, then calls the method named at
	0x34 on the instance at 0x30 with the time (PyEmbed::CallMethod) and
	prints any Python error. The destructor (0x5FCCC0, AppWrap's copy) frees
	the name and runs TimeSliceProcess's; the wrapper
	PythonMethodProcess_SetFunction expands SetFunction, which replaces the
	name with a heap copy.

===============================================================================
*/

#ifndef __PYTHONMETHODPROCESS_H__
#define __PYTHONMETHODPROCESS_H__

#include <string.h>

#include "TimeSliceProcess.h"
#include "Python.h"

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x005fcca0 ??_GPythonMethodProcess@@UAEPAXI@Z
class PythonMethodProcess : public TimeSliceProcess {
public:
					PythonMethodProcess( void ) { instance = NULL; function = NULL; }
					// FUNCTION: STBC 0x005fccc0 ??1PythonMethodProcess@@UAE@XZ
	virtual			~PythonMethodProcess( void ) { delete function; }

	// TimeSliceProcess
	virtual void	Update( double time );

	void			SetInstance( PyObject *instance_ ) { instance = instance_; }
	void			SetFunction( char *function_ ) {
						delete function;
						function = new char[strlen( function_ ) + 1];
						strcpy( function, function_ );
					}

protected:
	PyObject *		instance;				// 0x30 the instance whose method is called (the scripts' SetInstance)
	char *			function;				// 0x34 owned: the method's name
};

// The Python method PythonMethodProcess.SetInstance (installed by hand).
PyObject *		PythonMethodProcess_SetInstance( PyObject *self, PyObject *args );

#endif /* !__PYTHONMETHODPROCESS_H__ */
