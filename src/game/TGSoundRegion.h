/*
===============================================================================

	TGSoundRegion.h

	A named place sounds belong to: each set has one, named after the set,
	placed where its camera is. A sound joins at most one region; the
	region's filter applies to the sounds it holds (filter 1 silences them).
	Every region is filed by name in a static map, which the saved games
	write and read whole.

	0x58 bytes, allocated from the pools. Evidence: the constructor
	(0x70DD90, called by SetManager::AddSet) calls TGObject's with 0, sets
	the position to NiPoint3::ZERO and the orientation to
	NiMatrix3::IDENTITY, builds a list of two items per block at 0x40,
	stores the vtable 0x896A40 (TGObject's 12 slots), deletes the region
	the map holds under the name, copies the name to 0x0C and files the
	region under it in the map at 0x99B988. GetObjType returns 0x303; the
	SWIG wrappers TGSoundRegion_GetRegion, _AddSound, _RemoveSound and
	_SetFilter name those members; TGSound keeps its region at 0x94. The
	other names are invented.

===============================================================================
*/

#ifndef __TGSOUNDREGION_H__
#define __TGSOUNDREGION_H__

#include "TGObject.h"
#include "NiMatrix3.h"
#include "NiPoint3.h"
#include "NiTList.h"
#include "NiTStringMap.h"

class TGSound;

// GetObjType's value for a TGSoundRegion.
#define TGSOUNDREGION_TYPE		0x303

// The filters a region applies to its sounds (invented names).
#define SOUND_FILTER_NONE		0
#define SOUND_FILTER_SILENCE	1		// the sounds are silenced while in the region
#define SOUND_FILTER_2			2		// accepted; does nothing to the sounds

// The bases are the ones the SWIG interface declares (the shadow classes in App.pyc).
// FUNCTION: STBC 0x0070dd60 ??_GTGSoundRegion@@UAEPAXI@Z
class TGSoundRegion : public TGObject {
public:
					TGSoundRegion( char *name_ );
					TGSoundRegion( long id );
	virtual			~TGSoundRegion( void );

	// TGObject
	// FUNCTION: STBC 0x0070ec30 ?GetObjType@TGSoundRegion@@UAEHXZ
	virtual int		GetObjType( void ) { return TGSOUNDREGION_TYPE; }
	virtual bool	IsTypeOf( int type );
	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );
	virtual bool	FixUpPointers( void );
	virtual void	PostFixUp( void );
	// FUNCTION: STBC 0x0070ec60 ?GetClassName@TGSoundRegion@@UAEPADXZ
	virtual char *	GetClassName( void ) { return "TGSoundRegion"; }
	// FUNCTION: STBC 0x0070ec70 ?GetSwigTypeName@TGSoundRegion@@UAEPADXZ
	virtual char *	GetSwigTypeName( void ) { return "_p_TGSoundRegion"; }
	// FUNCTION: STBC 0x0070ec80 ?GetPythonClassName@TGSoundRegion@@UAEPADXZ
	virtual char *	GetPythonClassName( void ) { return "TGSoundRegionPtr"; }

	// The region of the name; NULL for none.
	static TGSoundRegion *	GetRegion( char *name_ );

	bool			AddSound( TGSound *sound );
	bool			RemoveSound( TGSound *sound );
	bool			SetFilter( int filter_ );

	// Places the region; the orientation is built from the forward and up
	// directions.
	void			SetTransform( const NiPoint3 &position_, const NiPoint3 &forward_, const NiPoint3 &up_ );

	// Every region, for the saved games: each written after a true, the
	// list ended by a false.
	static bool		SaveRegions( TGStream *stream );
	static bool		LoadRegions( TGStream *stream );
	static void		DeleteAllRegions( void );

protected:
	// A sound places its source relative to the region it is in.
	friend class TGSound;

	// The filter's effect on a sound joining and leaving the region.
	bool			ApplyFilter( TGSound *sound );
	void			RemoveFilter( TGSound *sound );

	int				filter;					// 0x08 SOUND_FILTER_*
	char *			name;					// 0x0C owned copy: the key in the map
	NiPoint3		position;				// 0x10
	NiMatrix3		orientation;			// 0x1C
	NiTList<TGSound *>	sounds;				// 0x40 the sounds in the region

	// Every region, by name.
	static NiTStringMap<TGSoundRegion *>	regions;

	// Declared for the SWIG wrappers that call them.
public:
	static TGSoundRegion *	Create( char *name ) { return new TGSoundRegion( name ); }	// the SWIG wrapper TGSoundRegion_Create expands it
};

/*
================
TGSoundRegion::IsTypeOf
================
*/
// FUNCTION: STBC 0x0070ec40 ?IsTypeOf@TGSoundRegion@@UAE_NH@Z
inline bool TGSoundRegion::IsTypeOf( int type ) {
	if ( type == TGSOUNDREGION_TYPE ) {
		return true;
	}
	return TGObject::IsTypeOf( type );
}

#endif /* !__TGSOUNDREGION_H__ */
