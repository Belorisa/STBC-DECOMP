/*
===============================================================================

	NiStencilProperty.h

	The stencil test: enable, function, reference and mask, and the actions
	on stencil failure, depth failure and pass.

	Evidence: the vtable at 0x89A178 (NiProperty's 14 slots, none added;
	Type returns 14); CreateClone (0x807830) allocates 0x34 bytes and
	the constructor sets the bool at 0x18 false, the function at 0x1C to 4,
	the reference at 0x20 to 0, the mask at 0x24 to 0xFFFFFFFF and the
	actions at 0x28, 0x2C and 0x30 to 0, 0 and 3; the renderer indexes its
	Direct3D tables with the function (NEVER .. ALWAYS, 0-7) and the
	actions. The NiRTTI is 0x9A2438, named "NiStencilProperty". The enum
	names follow the Gamebryo lineage, whose defaults (TEST_GREATER,
	ACTION_KEEP, ACTION_INCREMENT) are these.

===============================================================================
*/

#ifndef __NISTENCILPROPERTY_H__
#define __NISTENCILPROPERTY_H__

#include "NiBool.h"
#include "NiProperty.h"
#include "NiStreamMacros.h"

/*
================
NiStencilPropertyLoadRecord

The record CreateObject hands the stream: NiObjectNET's link IDs under a
vtable of its own (0x899F78). The class name is invented.
================
*/
// FUNCTION: STBC 0x008027f0 ??_GNiStencilPropertyLoadRecord@@UAEPAXI@Z @netimmerse/NiStencilProperty
class NiStencilPropertyLoadRecord : public NiObjectNETLoadRecord {
};

class NiStencilProperty : public NiProperty {
public:
	// FUNCTION: STBC 0x808150 ?GetRTTI@NiStencilProperty@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	virtual NiObject *		CreateClone();

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	// FUNCTION: STBC 0x8081A0 ?Type@NiStencilProperty@@UAEHXZ
	virtual int				Type() { return STENCIL; }
	static int				GetType() { return STENCIL; }

	enum {
		STENCIL				= 14
	};

	enum TestFunction {
		TEST_NEVER,
		TEST_LESS,
		TEST_EQUAL,
		TEST_LESSEQUAL,
		TEST_GREATER,
		TEST_NOTEQUAL,
		TEST_GREATEREQUAL,
		TEST_ALWAYS,
		TEST_MAX
	};

	enum Action {
		ACTION_KEEP,
		ACTION_ZERO,
		ACTION_REPLACE,
		ACTION_INCREMENT,
		ACTION_DECREMENT,
		ACTION_INVERT,
		ACTION_MAX
	};

							NiStencilProperty() {
								enable = false;
								function = TEST_GREATER;
								reference = 0;
								mask = 0xFFFFFFFF;
								failAction = ACTION_KEEP;
								zFailAction = ACTION_KEEP;
								passAction = ACTION_INCREMENT;
							}
							// written out: it stores the vptr (0x808190), which the
							// compiler's own destructor would not
	// FUNCTION: STBC 0x808160 ??_GNiStencilProperty@@UAEPAXI@Z
	// FUNCTION: STBC 0x808190 ??1NiStencilProperty@@UAE@XZ
	virtual					~NiStencilProperty() {}

	// accessors (Gamebryo lineage names)
	NiBool					GetStencilOn() const { return enable; }
	TestFunction			GetStencilFunction() const { return function; }
	unsigned int			GetStencilReference() const { return reference; }
	unsigned int			GetStencilMask() const { return mask; }
	Action					GetStencilFailAction() const { return failAction; }
	Action					GetStencilPassZFailAction() const { return zFailAction; }
	Action					GetStencilPassAction() const { return passAction; }

	// the stencil property of objects without one
	// FUNCTION: STBC 0x820DE0 ?GetDefault@NiStencilProperty@@SA?AV?$NiPointer@VNiStencilProperty@@@@XZ
	static NiPointer<NiStencilProperty>	GetDefault() { return defaultProperty; }

	// stream registration
	static void				CreateObject( NiStream &stream, NiObject *&object );
	static int				Register();

	static const NiRTTI		rtti;

	// the default property, which the file-scope init object creates
	static NiPointer<NiStencilProperty>	defaultProperty;	// name invented

protected:
	void					CopyMembers( NiStencilProperty *dest );

	// "prefix = TEST_..." and "prefix = ACTION_..." in a new block
	static char *			GetViewerString( const char *prefix, TestFunction function );
	static char *			GetViewerString( const char *prefix, Action action );

	NiBool					enable;
	TestFunction			function;
	unsigned int			reference;
	unsigned int			mask;
	Action					failAction;		// the stencil test fails
	Action					zFailAction;	// the stencil test passes, the depth test fails
	Action					passAction;		// both pass
};

NiRegisterStream( NiStencilProperty );

#endif /* !__NISTENCILPROPERTY_H__ */
