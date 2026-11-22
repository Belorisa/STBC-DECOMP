/*
===============================================================================

	PyEmbedModule.h

	The module that starts and ends the embedded Python interpreter: at
	startup it builds the SWIG import table and initialises Python, then
	imports App; at the end it releases App, finalises Python and frees
	what the embedding filed. The class, its unit and its names are
	invented.

	Evidence: the one instance at 0x99AC18 is built by 0x6F6C40 (TGModule's
	constructor with the priority 0xA0000000, then the vtable 0x896360).
	The vtable's slots 0, 1, 4 and 5 are 0x6F6C90 (swig_main, then
	Py_Initialize), 0x6F6CA0 (PyEmbed::LoadAppModule), 0x6F6CB0
	(PyEmbed::ReleaseAppModule) and 0x6F6CC0 (Py_Finalize, then
	PyEmbed::FreeRegistrations); slots 2, 3, 6 and 7 are TGModule's empty
	ones. The object and its unit's <iostream> statics (0x99AB5C on) lie
	in .bss below PyEmbed.cpp's data, and its code just before
	PyEmbed.cpp's, so it is a unit of its own.

===============================================================================
*/

#ifndef __PYEMBEDMODULE_H__
#define __PYEMBEDMODULE_H__

#include "TGModule.h"

// FUNCTION: STBC 0x006f6c80 ??1PyEmbedModule@@QAE@XZ
class PyEmbedModule : public TGModule {		// invented name
public:
	// FUNCTION: STBC 0x006f6c40 ??0PyEmbedModule@@QAE@XZ
					PyEmbedModule( void ) : TGModule( 0xA0000000 ) {}

	// TGModule
	virtual void	VirtualSlot00( void );		// the startup: Python itself
	virtual void	VirtualSlot01( void );		// imports App
	virtual void	VirtualSlot04( void );		// releases App
	virtual void	VirtualSlot05( void );		// the end: Python itself
};

#endif /* !__PYEMBEDMODULE_H__ */
