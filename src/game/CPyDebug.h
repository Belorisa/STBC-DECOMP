/*
===============================================================================

	CPyDebug.h

	The scripts' debug printer: Print does nothing in the release build.

	4 bytes, allocated with operator new (the SWIG wrapper new_CPyDebug).
	The constructor (0x4372E0) keeps a copy of the name it is given, NULL
	for none; the destructor (0x437340) frees it.

===============================================================================
*/

#ifndef __CPYDEBUG_H__
#define __CPYDEBUG_H__

#include "Python.h"

class CPyDebug {
public:
					CPyDebug( char *name_ );
					~CPyDebug( void );

	void	Print( char * );
	char *	GetName( void ) { return name; }

protected:
	char *			name;			// 0x00 owned copy, or NULL (invented name)
};

// The save's handlers for the shadow instances: the name goes into the
// instance's state, or None (invented names, after
// ArtificialIntelligence_SaveInstance; UtopiaModule files them).
bool		CPyDebug_SaveInstance( PyObject *instance, PyObject **state );
bool		CPyDebug_LoadInstance( PyObject *instance, PyObject *state );

#endif /* !__CPYDEBUG_H__ */
