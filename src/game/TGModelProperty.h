/*
===============================================================================

	TGModelProperty.h

	A named, reference-counted property of a model.

	0x1C bytes, allocated from the pools. Evidence: the constructor
	(0x6EC640) calls TGObject's, stores the vtable 0x8961AC, sets the
	reference count at 0x08 to 1 and the TGString at 0x0C to
	"Uninitialized"; IncRefCount, DecRefCount and GetRefCount
	(0x6EC780-0x6EC7C0) work on 0x08. SubsystemProperty's members start at
	0x1C. GetObjType returns 0x601.

	The vtable has TGObject's 12 slots and five of its own: slot 12 writes
	the script line that creates the property, slot 13 is WriteInit and
	slot 14 Edit (the SWIG wrappers TGModelProperty_WriteInit and
	TGModelProperty_Edit call them), slot 15 shows or hides the property
	on a model, and slot 16 copies another property's settings
	(Duplicate calls it on the new property).

===============================================================================
*/

#ifndef __TGMODELPROPERTY_H__
#define __TGMODELPROPERTY_H__

#include "TGObject.h"
#include "TGString.h"

class TGStream;
class ModelPropertyViewer;
class NiNode;

// The window handle Edit opens its dialog under (windef.h's declaration).
struct HWND__;
typedef struct HWND__ *HWND;

// The type TGModelProperty::Cast (0x6EC5C0) checks.
#define TGMODELPROPERTY_TYPE		0x601

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x006ec6c0 ??_GTGModelProperty@@UAEPAXI@Z @game/TGModelProperty
class TGModelProperty : public TGObject {
public:
					TGModelProperty( long id = 0 );
	virtual			~TGModelProperty( void );

	// TGObject
	// FUNCTION: STBC 0x006ed220 ?GetObjType@TGModelProperty@@UAEHXZ
	virtual int		GetObjType( void ) { return TGMODELPROPERTY_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	// The copies kept from BlinkingLightProperty's unit, the first property
	// unit that does not override them.
	// FUNCTION: STBC 0x0067c560 ?FixUpPointers@TGModelProperty@@UAE_NXZ @game/BlinkingLightProperty
	virtual bool	FixUpPointers( void ) { return true; }
	// FUNCTION: STBC 0x0067c570 ?PostFixUp@TGModelProperty@@UAEXXZ @game/BlinkingLightProperty
	virtual void	PostFixUp( void ) {}
	// FUNCTION: STBC 0x006ed250 ?GetClassName@TGModelProperty@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGModelProperty"; }
	// FUNCTION: STBC 0x006ed260 ?GetSwigTypeName@TGModelProperty@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGModelProperty"; }
	// FUNCTION: STBC 0x006ed270 ?GetPythonClassName@TGModelProperty@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGModelPropertyPtr"; }

	// Writes the script line that creates the property, then WriteInit's
	// lines (invented name).
	virtual void	WriteScript( TGStream *stream );
	// Writes the script lines that set the property's values on the
	// variable. The copy is kept from EngineGlowProperty's unit, the first
	// property unit that does not override it.
	// FUNCTION: STBC 0x0067ddd0 ?WriteInit@TGModelProperty@@UAEXPAVTGStream@@PAD@Z @game/EngineGlowProperty
	virtual void	WriteInit( TGStream *stream, char *variableName ) {}
	// Opens the property's editor dialog; false when there is none.
	virtual bool	Edit( HWND parent );
	// Shows or hides the property on a model (the debug geometry of the
	// subsystem properties).
	virtual void	VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent );
	// Takes the settings of another property of the same type (invented name).
	virtual bool	Copy( TGModelProperty *property );

	int	GetRefCount( void );
	void	DecRefCount( void );
	void	IncRefCount( void );
	static TGModelProperty *	Cast( TGModelProperty * );
	static TGModelProperty *	Create( char * );
	TGModelProperty *	Duplicate( void );
	// The SWIG wrapper TGModelProperty_GetName calls the copy kept from
	// STSubsystemMenu's unit.
	// FUNCTION: STBC 0x005378d0 ?GetName@TGModelProperty@@QAE?AVTGString@@XZ @game/STSubsystemMenu
	TGString	GetName( void ) { return name; }

	// The SWIG wrapper TGModelProperty_SetName calls this copy, and it
	// forwards to the TGString overload, which every Create calls with the
	// name it was given.
	// FUNCTION: STBC 0x005ddc10 ?SetName@TGModelProperty@@QAEXPAD@Z @game/AppWrap
	void		SetName( char *name_ ) { SetName( TGString( name_ ) ); }
	// FUNCTION: STBC 0x005ddc40 ?SetName@TGModelProperty@@QAEXVTGString@@@Z @game/AppWrap
	void		SetName( TGString name_ ) { name = name_; }
	// FUNCTION: STBC 0x005dde50 ?SetNameW@TGModelProperty@@QAEXVTGString@@@Z @game/AppWrap
	void		SetNameW( TGString name_ ) { SetName( name_ ); }

protected:
	int				refCount;		// 0x08 the property is deleted when it drops to 0
	TGString		name;			// 0x0C (invented name)
};

/*
================
TGModelProperty::IsTypeOf
================
*/
// FUNCTION: STBC 0x006ed230
inline bool TGModelProperty::IsTypeOf( int type ) {
	if ( type == TGMODELPROPERTY_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

#endif /* !__TGMODELPROPERTY_H__ */
