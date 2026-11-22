/*
===============================================================================

	NiObject.h

	The root of the NetImmerse class hierarchy: reference counting, run-time
	type information, cloning and streaming.

	Evidence: the vtable at 0x898B94 (12 slots); the constructor (0x7D87A0)
	and destructor (0x7D87F0) keep a count of live objects; the viewer
	strings name the reference count ("m_iRefCount"); every smart pointer
	release calls slot 10, the scalar deleting destructor, when the count
	reaches zero.

===============================================================================
*/

#ifndef __NIOBJECT_H__
#define __NIOBJECT_H__

#include "NiRTTI.h"
#include "NiTArray.h"
#include "NiStreamMacros.h"

class NiStream;
template <class T> class NiPointer;

// FUNCTION: STBC 0x007d87c0 ??_GNiObject@@UAEPAXI@Z
class NiObject {
public:
	// registers the class's loader once; every unit that includes this
	// header calls it from a static initializer (0x7D8650)
	static int				Register();
	static void				CreateObject( NiStream &stream, NiObject *&object );

	// FUNCTION: STBC 0x458770 ?GetRTTI@NiObject@@UBEPBVNiRTTI@@XZ
	virtual const NiRTTI *	GetRTTI() const { return &rtti; }

	// cloning
	// FUNCTION: STBC 0x458780 ?CreateClone@NiObject@@UAEPAV1@XZ
	virtual NiObject *		CreateClone() { return this; }
	// FUNCTION: STBC 0x438FF0 ?ProcessClone@NiObject@@UAEXPAV1@@Z
	virtual void			ProcessClone( NiObject *clone ) {}
	// FUNCTION: STBC 0x439000 ?VirtualSlot03@NiObject@@UAEXXZ
	virtual void			VirtualSlot03() {}		// clears what cloning left behind

	// streaming
	virtual void			LoadBinary( NiStream &stream );
	virtual void			LinkObject( NiStream &stream );
	virtual bool			RegisterStreamables( NiStream &stream );
	virtual void			SaveBinary( NiStream &stream );
	virtual bool			IsEqual( NiObject *object );
	virtual void			GetViewerStrings( NiTArray<char *> *strings );

	virtual					~NiObject();

	// FUNCTION: STBC 0x40DA50 ?VirtualSlot11@NiObject@@UAEXXZ
	virtual void			VirtualSlot11() {}

							NiObject();

	void					IncRefCount() { refCount++; }
	int						DecRefCount();
	int						GetRefCount() const { return refCount; }

	NiObject *				Clone();
	// a copy made by streaming the object out and back in (Gamebryo lineage name)
	NiPointer<NiObject>		CreateDeepCopy();

	// run-time type tests (Gamebryo lineage names): true when the object's
	// class is rtti's or derives from it; the object, or NULL when not
	bool					IsKindOf( const NiRTTI *rtti_ ) const;
	bool					IsExactKindOf( const NiRTTI *rtti_ ) const { return GetRTTI() == rtti_; }	// Gamebryo lineage
	NiObject *				DynamicCast( const NiRTTI *rtti_ ) const;

	// how a clone's name is derived from the original's
	enum CopyType {
		COPY_NONE,
		COPY_EXACT,
		COPY_UNIQUE				// the name with the append character added
	};

	static void				SetDefaultCopyType( CopyType copyType );
	static CopyType			GetDefaultCopyType();
	static void				SetDefaultAppendCharacter( char appendCharacter );
	static char				GetDefaultAppendCharacter();
	// the number of live objects, for WinMain's leak report (Gamebryo
	// lineage name: NiRefObject::GetTotalObjectCount)
	static unsigned int		GetTotalObjectCount() { return objects; }

	static const NiRTTI		rtti;

protected:
	// each class's CopyMembers starts with its base's; the root copies
	// nothing (AnimOnly.exe 0x40A970)
	void					CopyMembers( NiObject *dest ) {}

	int						refCount;

	static unsigned int		objects;			// live NiObjects
	static CopyType			defaultCopyType;
	static char				defaultAppendCharacter;
};

NiRegisterStream( NiObject );

/*
================
NiObject::DecRefCount

Releases a reference; the last one deletes the object. Returns the count
left (the out-of-line copy 0x40CFE0 returns it).
================
*/
// FUNCTION: STBC 0x40CFE0 ?DecRefCount@NiObject@@QAEHXZ @game/SetClass
inline int NiObject::DecRefCount() {
	int count = --refCount;
	if ( count == 0 ) {
		delete this;
	}
	// the debug copy tests the local after the delete (AnimOnly.exe 0x403780); retail keeps it (user ruling 2026-11-20)
	assert( count >= 0 );
	return count;
}

// viewer strings: heap strings describing a member, "prefix = value"
char *						NiGetViewerString( const char *text );		// "--- text ---"
char *						NiGetViewerString( const char *prefix, bool value );
char *						NiGetViewerString( const char *prefix, char value );
char *						NiGetViewerString( const char *prefix, unsigned char value );
char *						NiGetViewerString( const char *prefix, short value );
char *						NiGetViewerString( const char *prefix, unsigned short value );
char *						NiGetViewerString( const char *prefix, int value );
char *						NiGetViewerString( const char *prefix, unsigned int value );
char *						NiGetViewerString( const char *prefix, long value );
char *						NiGetViewerString( const char *prefix, unsigned long value );
char *						NiGetViewerString( const char *prefix, float value );
char *						NiGetViewerString( const char *prefix, double value );
char *						NiGetViewerString( const char *prefix, const void *value );
char *						NiGetViewerString( const char *prefix, const char *value );	// "%s = NULL" for NULL

/*
================
NiIsKindOf, NiIsExactKindOf

NetImmerse 3.1's run-time type tests on a pointer that may be NULL: the
object's own test behind a null test. Its debug copies expand them to the
conditional (TargetIsRequiredType, AnimOnly.exe 0x47AFE0; the assert
"NiIsKindOf(NiTimeController, pClone)", 0x41237F).
================
*/
#define NiIsKindOf( classname, object )			( ( object ) ? ( object )->IsKindOf( &classname::rtti ) : false )
#define NiIsExactKindOf( classname, object )	( ( object ) ? ( object )->IsExactKindOf( &classname::rtti ) : false )

/*
================
NiObject::IsKindOf

The out-of-line copy (0x450530) comes from a game unit. The walk is a while
loop over a local, as its debug copy is (AnimOnly.exe 0x403620).
================
*/
// FUNCTION: STBC 0x450530 ?IsKindOf@NiObject@@QBE_NPBVNiRTTI@@@Z
inline bool NiObject::IsKindOf( const NiRTTI *rtti_ ) const {
	const NiRTTI *objectRTTI = GetRTTI();
	while ( objectRTTI ) {
		if ( objectRTTI == rtti_ ) {
			return true;
		}
		objectRTTI = objectRTTI->GetBaseRTTI();
	}
	return false;
}

/*
================
NiObject::DynamicCast
================
*/
inline NiObject *NiObject::DynamicCast( const NiRTTI *rtti_ ) const {
	return IsKindOf( rtti_ ) ? (NiObject *)this : 0;
}

/*
================
NiDynamicCast

The object as a classname, or NULL when it is NULL or not a classname. The
object is evaluated twice: once for the NULL test, once for the cast.

Evidence: the same NiD3DRender slots skip the chain walk for a NULL object
and turn the walk's result into the object or NULL without a branch;
NiBezierMesh's CreateClone (0x830F50) fetches the child once for the NULL
test and once for an out-of-line IsKindOf (0x450530) whose result selects
it.
================
*/
#define NiDynamicCast( classname, object ) 	( ( object ) ? (classname *)( object )->DynamicCast( &classname::rtti ) : 0 )

#endif /* !__NIOBJECT_H__ */
