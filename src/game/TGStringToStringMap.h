/*
===============================================================================

	TGStringToStringMap.h

	The members of TGStringToStringMap that the SWIG wrappers TGStringToStringMap_* call.

	0x18 bytes, no vtable: CharacterClass's constructor (0x666F10) allocates
	0x18 bytes for one and runs the constructor 0x45F640, which clears six
	members. AddMapping (0x45F670) grows the block at 0x14 to eight bytes
	per mapping beyond the count at 0x10.

===============================================================================
*/

#ifndef __TGSTRINGTOSTRINGMAP_H__
#define __TGSTRINGTOSTRINGMAP_H__

class TGStream;

class TGStringToStringMap {
public:
					TGStringToStringMap( void );
					~TGStringToStringMap( void );

	void	AddMapping( char *, char * );
	char *	GetDest( char * );
	void	DeleteMappings( void );

	// The mappings, saved with their owner (CharacterClass::WriteToStream
	// 0x66DC60 calls it; invented name).
	bool	WriteToStream( TGStream *stream );
	// Reads what WriteToStream saved (CharacterClass::ReadFromStream
	// 0x66E220 calls it; invented name).
	bool	ReadFromStream( TGStream *stream );

protected:
	// A mapping: where in the two string pools its strings begin (invented name).
	struct Mapping {
		unsigned int	nameOffset;			// in names
		unsigned int	destOffset;			// in dests
	};

	unsigned int	namesSize;			// 0x00 the bytes used in names (invented name)
	char *			names;				// 0x04 the mapped strings, one after the other, end marks included (invented name)
	unsigned int	destsSize;			// 0x08 the bytes used in dests (invented name)
	char *			dests;				// 0x0C what they map to (invented name)
	unsigned int	numMappings;		// 0x10
	Mapping *		mappings;			// 0x14 sorted by name (invented name)
};

#endif /* !__TGSTRINGTOSTRINGMAP_H__ */
