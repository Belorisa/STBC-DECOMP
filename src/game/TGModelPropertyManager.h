/*
===============================================================================

	TGModelPropertyManager.h

	The model properties the scripts have built, kept as templates that a
	ship's property set is made from: the global ones (the ship classes'),
	and the local ones of the set being built. Filters decide which of them a
	model takes. The members of TGModelPropertyManager that the SWIG wrappers
	TGModelPropertyManager_* call, and the layout the clears prove.

	0x64 bytes. The constructor (0x6E9650) builds the four lists and stores
	the vtable 0x8961A0: two slots, the save (0x6EA2A0) and the load
	(0x6EA320) of the templates. Evidence: the clears (0x6E9BA0, 0x6E9C00,
	0x6E9D90, 0x6EA1A0) work on four NiTLists of two items per block, at
	0x04, 0x1C, 0x34 and 0x4C. The template clears let go of the reference
	each list item holds on its property (TGModelProperty::DecRefCount, 0x6EC790)
	before the list is emptied; the registered filters' clear deletes each
	record and the string the record holds at 0x04; the clear of the current
	filters only empties the list, since its items are functions that
	ApplyFilters (0x6E9F40) calls with the property until one refuses. The
	names of the lists and of the record are invented.

===============================================================================
*/

#ifndef __TGMODELPROPERTYMANAGER_H__
#define __TGMODELPROPERTYMANAGER_H__

#include "NiTList.h"

class TGModelProperty;

// A filter that decides whether a property is taken; false refuses it.
typedef bool ( *modelPropertyFilter_t )( TGModelProperty *property );

// A registered filter (invented name): what it holds besides the string at
// 0x04 is not known.
struct TGModelPropertyFilterRecord {
	modelPropertyFilter_t	filter;		// 0x00 (invented names, from RegisterFilter's arguments)
	char *			name;				// 0x04 owned copy
};

class TGStream;

// FUNCTION: STBC 0x006e9570 ??1TGModelPropertyManager@@QAE@XZ @game/TGModelPropertyModule
class TGModelPropertyManager {
public:
					TGModelPropertyManager( void );

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	bool	IsLocalTemplate( TGModelProperty *property );
	bool	IsGlobalTemplate( TGModelProperty *property );
	bool	RegisterLocalTemplate( TGModelProperty * );
	bool	RegisterGlobalTemplate( TGModelProperty * );
	void	ClearGlobalTemplates( void );
	void	ClearLocalTemplates( void );
	void	ClearRegisteredFilters( void );
	void	AddFilter( modelPropertyFilter_t filter );
	void	RegisterFilter( modelPropertyFilter_t filter, char *name );
	bool	RemoveFilter( modelPropertyFilter_t filter );
	bool	ApplyFilters( TGModelProperty *property );
	// a new list of the templates the filters take (unreferenced in retail)
	NiTList<TGModelProperty *> *	Unknown6E9F80( int local );
	void	ClearCurrentFilters( void );
	bool	RemoveTemplate( int local, TGModelProperty *property, bool deleteIt );
	TGModelProperty *	FindByNameAndType( char *name, int type, int local );
	TGModelProperty *	FindByName( char *name, int local );

	// the lists the registrations replace in (names invented)
	NiTList<TGModelProperty *> *	GetGlobalTemplates( void );
	NiTList<TGModelProperty *> *	GetLocalTemplates( void );

	NiTList<TGModelPropertyFilterRecord *>	registeredFilters;	// 0x04
	NiTList<modelPropertyFilter_t>	currentFilters;				// 0x1C
	NiTList<TGModelProperty *>		globalTemplates;			// 0x34
	NiTList<TGModelProperty *>		localTemplates;				// 0x4C
};

// The model property manager (the SWIG variable g_kModelPropertyManager).
extern TGModelPropertyManager	g_kModelPropertyManager;

#endif /* !__TGMODELPROPERTYMANAGER_H__ */
