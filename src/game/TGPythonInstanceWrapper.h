/*
===============================================================================

	TGPythonInstanceWrapper.h

	An event handler object that stands for a Python instance: its slot 8,
	which asks an object for its Python shadow, hands back the wrapped
	instance (with a new reference) instead of making one.

	0x18 bytes. Evidence: the SWIG wrappers new_TGPythonInstanceWrapper and
	TGPythonInstanceWrapper_Create allocate 0x18 bytes from the pool and
	call the constructor 0x6DFC90, which runs TGEventHandlerObject's with ID
	0, clears 0x14 and stores the vtable 0x895F78 (TGEventHandlerObject's 21
	slots: the destructor, GetObjType, IsTypeOf, WriteToStream, slot 8, the
	class strings and GetClassHandlers overridden; WriteToStream is 0x6DFD40,
	beside the constructor). GetObjType returns 0x10B, the SWIG constant
	CT_TG_PYTHON_INSTANCE_WRAPPER. The class strings are
	"TGPythonInstanceWrapper", "_p_TGPythonInstanceWrapper" and
	"TGPythonInstanceWrapperPtr"; the destructor is the implicit one (0x6D4B10
	only jumps to TGEventHandlerObject's). The copies of the inline virtuals
	and of the deleting destructor (0x6D4A40-0x6D4B10) sit in a unit not
	reconstructed yet.

===============================================================================
*/

#ifndef __TGPYTHONINSTANCEWRAPPER_H__
#define __TGPYTHONINSTANCEWRAPPER_H__

#include "TGEventHandlerObject.h"

// GetObjType's value for a TGPythonInstanceWrapper (the SWIG constant CT_TG_PYTHON_INSTANCE_WRAPPER).
#define TGPYTHONINSTANCEWRAPPER_TYPE	0x10B

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006d4ae0 ??_GTGPythonInstanceWrapper@@UAEPAXI@Z @game/TGPythonInstanceWrapper
// FUNCTION: STBC 0x006d4b10 ??1TGPythonInstanceWrapper@@UAE@XZ @game/TGPythonInstanceWrapper
class TGPythonInstanceWrapper : public TGEventHandlerObject {
public:
					TGPythonInstanceWrapper( void );
	// FUNCTION: STBC 0x006d4a20 ??0TGPythonInstanceWrapper@@QAE@J@Z @game/TGEventModule
					TGPythonInstanceWrapper( long id ) : TGEventHandlerObject( id ) {}

	// TGObject
	// FUNCTION: STBC 0x006d4a40 ?GetObjType@TGPythonInstanceWrapper@@UAEHXZ @game/TGPythonInstanceWrapper
	virtual int		GetObjType( void ) { return TGPYTHONINSTANCEWRAPPER_TYPE; }
	// FUNCTION: STBC 0x006d4a50 ?IsTypeOf@TGPythonInstanceWrapper@@UAE_NH@Z @game/TGPythonInstanceWrapper
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	// The wrapped instance, with a new reference.
	// FUNCTION: STBC 0x006d4ad0 ?VirtualSlot08@TGPythonInstanceWrapper@@UAEPAU_object@@XZ @game/TGPythonInstanceWrapper
	virtual PyObject *	VirtualSlot08( void ) { Py_INCREF( instance ); return instance; }
	// FUNCTION: STBC 0x006d4a90 ?GetClassName@TGPythonInstanceWrapper@@UAEPADXZ @game/TGPythonInstanceWrapper
	virtual char *	GetClassName( void ) { return "TGPythonInstanceWrapper"; }
	// FUNCTION: STBC 0x006d4aa0 ?GetSwigTypeName@TGPythonInstanceWrapper@@UAEPADXZ @game/TGPythonInstanceWrapper
	virtual char *	GetSwigTypeName( void ) { return "_p_TGPythonInstanceWrapper"; }
	// FUNCTION: STBC 0x006d4ab0 ?GetPythonClassName@TGPythonInstanceWrapper@@UAEPADXZ @game/TGPythonInstanceWrapper
	virtual char *	GetPythonClassName( void ) { return "TGPythonInstanceWrapperPtr"; }

	// TGEventHandlerObject
	// FUNCTION: STBC 0x006d4ac0 ?GetClassHandlers@TGPythonInstanceWrapper@@UAEPAVTGEventHandlerTable@@XZ @game/TGPythonInstanceWrapper
	virtual TGEventHandlerTable *	GetClassHandlers( void ) { return &classHandlers; }

	// The state functions of the shadow instances (PyEmbed::AddGetStateFunction):
	// a pickled instance that owns its wrapper saves the wrapper beside the
	// pickle, and a loaded one gets it back (invented names).
	static bool		GetState( PyObject *instance, PyObject **state );
	static bool		SetState( PyObject *instance, PyObject *state );
	// Their stream callbacks: write the wrapper, read it back into the
	// containing instance (invented names).
	static bool		WriteWrapper( PyObject *instance, TGStream *stream );
	static bool		ReadWrapper( PyObject *containingInstance, TGStream *stream );

	static TGEventHandlerTable *	GetClassTable( void ) { return &classHandlers; }

	// Declared for the SWIG wrapper that calls it.
	static TGPythonInstanceWrapper *	Create( void ) { return new TGPythonInstanceWrapper; }	// the SWIG wrapper TGPythonInstanceWrapper_Create expands it

protected:
	friend PyObject *	TGPythonInstanceWrapper_SetPyWrapper( PyObject *self, PyObject *args );

	PyObject *		instance;				// 0x14 the wrapped Python instance; NULL after construction (invented name)

	static TGEventHandlerTable	classHandlers;		// 0x998D90 (invented name)
};

/*
================
TGPythonInstanceWrapper::IsTypeOf
================
*/
inline bool TGPythonInstanceWrapper::IsTypeOf( int type ) {
	if ( type == TGPYTHONINSTANCEWRAPPER_TYPE ) {
		return true;
	}
	return TGEventHandlerObject::IsTypeOf( type );
}

// The Python method SetPyWrapper of the shadow class: sets the instance a
// wrapper wraps (PyEmbed::AddClassMethod from TGEventModule's startup).
PyObject *	TGPythonInstanceWrapper_SetPyWrapper( PyObject *self, PyObject *args );

#endif /* !__TGPYTHONINSTANCEWRAPPER_H__ */
