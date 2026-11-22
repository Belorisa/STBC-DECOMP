/*
===============================================================================

	TGModelPropertyInstance.h

	The members of TGModelPropertyInstance that the SWIG wrappers TGModelPropertyInstance_* call. Only what the
	wrappers prove is declared.

	A property placed on a model. The SWIG wrappers read the original
	template (0x04), the hardpoint name (0x08) and the property (0x0C)
	inline; the SWIG interface deletes it through slot 0 (a virtual
	destructor), and TGModelPropertySet's destructor (0x6EB0D0) deletes the
	instances of its lists through it.

===============================================================================
*/

#ifndef __TGMODELPROPERTYINSTANCE_H__
#define __TGMODELPROPERTYINSTANCE_H__

class TGModelProperty;
class TGStream;

// FUNCTION: STBC 0x006ed400 ??_GTGModelPropertyInstance@@UAEPAXI@Z
class TGModelPropertyInstance {
public:
	// The load's (TGNamedModelPropertyList::ReadFromStream) and the set's
	// (TGModelPropertySet::AddToSet: a copy of the template at the
	// hardpoint, the name copied).
					TGModelPropertyInstance( void );
					TGModelPropertyInstance( TGModelProperty *template_, char *hardpointName_ );
	virtual			~TGModelPropertyInstance( void );

	// Slots 1 to 4 of the vtable 0x8961F0. The set's stream functions call them
	// for each instance (WriteToStream 0x6ED500, ReadFromStream 0x6ED5C0,
	// FixUpPointers 0x6ED630, PostFixUp 0x6ED650).
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );

	void	UseOriginalTemplate( void );
	TGModelProperty *	GetOriginalTemplate( void ) { return originalTemplate; }
	char *			GetHardpointName( void ) { return hardpointName; }
	TGModelProperty *	GetProperty( void ) { return property; }

	// Each lets go of the reference on the old property and takes one on
	// the new (invented names; AddToSet calls them out of line).
	void	SetProperty( TGModelProperty *property_ );
	void	SetOriginalTemplate( TGModelProperty *template_ );

protected:
	TGModelProperty *	originalTemplate;	// 0x04 holds a reference; its ID between the load and PostFixUp
	char *			hardpointName;			// 0x08 owned
	TGModelProperty *	property;			// 0x0C holds a reference; the template itself or a copy
};

#endif /* !__TGMODELPROPERTYINSTANCE_H__ */
