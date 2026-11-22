/*
===============================================================================

	TGAttrTable.h

	The attribute values of a TGAttrObject: floats keyed by an AttrType, in
	a map of 37 buckets, written to and read from a stream as a count and
	(key, value) pairs.

	0x14 bytes: a vtable (0x896340: the scalar deleting destructor, then
	WriteToStream and ReadFromStream) and the map (vtable 0x89634C).

	Evidence: TGAttrObject's SetAttrValue (0x6F3400) allocates 0x14 bytes
	and constructs this (0x6F65C0) on first use; LookupAttrValue,
	SetAttrValue and RemoveAttr forward to Lookup, Set and Remove. AttrType
	is SWIG's name for the key type ("_p_AttrType", a by-value type SWIG
	was not told about); the stream methods write it with WriteInt. The
	class name and the method names are invented.

===============================================================================
*/

#ifndef __TGATTRTABLE_H__
#define __TGATTRTABLE_H__

#include "NiTMap.h"

class TGStream;

// The key of an attribute.
typedef int AttrType;

// An attribute's value: a float, or the bits of an int (the Int accessors
// of TGAttrObject). The name is invented.
typedef union {
	int				i;
	float			f;
} attrValue_t;

// FUNCTION: STBC 0x006f6610 ??_GTGAttrTable@@UAEPAXI@Z @game/TGAttrTable
class TGAttrTable {
public:
					TGAttrTable( void );
	virtual			~TGAttrTable( void );

	virtual bool	WriteToStream( TGStream *stream );
	virtual bool	ReadFromStream( TGStream *stream );

	bool			CopyFrom( TGAttrTable *other );
	bool			Merge( TGAttrTable *other, bool overwrite );
	bool			Lookup( AttrType type, float *value );
	bool			Set( AttrType type, float value );
	bool			Has( AttrType type );
	bool			Remove( AttrType type );
	void			RemoveAll( void );
	int				GetCount( void );

protected:
	NiTMap<AttrType, float>	values;		// 0x04
};

#endif /* !__TGATTRTABLE_H__ */
