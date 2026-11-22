/*
===============================================================================

	TGConfigMapping.h

	The options file's settings: sections by name, each a map of values by
	name. A value is an integer, a float, a string of chars or a TGString,
	and keeps its type with it; a getter of one type answers 0 for a value
	of another. There is one, g_kConfigMapping, and the SWIG wrappers
	TGConfigMapping_* call its members.

	0x10 bytes, all of it the map of sections by name, a member: the class
	has no vtable of its own. Evidence: the dynamic initialiser (0x71C6A0)
	runs the constructor (0x71C6D0), which builds the map of 37 buckets with
	NiTMap's vtable (0x896B30) and then NiTStringMap's (0x896B1C); the
	destructor (0x71C720) stores no vtable on entry, and its first store is
	NiTStringMap's, after its body, as the member's destructor starts. The
	sections (0x10 bytes, from 0x71DA20) are the same: a map of values by
	name, NiTMap's vtable (0x896B44) then NiTStringMap's (0x896B58), and a
	destructor of their own (0x71C950) that deletes the values before the
	map's runs. The values are 0x18 bytes: the type at 0x00, an integer, a
	float or a char pointer at 0x04, and a TGString at 0x08. The class name
	is Totally Games' (the SWIG wrapper); the section's and value's names,
	the members' and the value types' are invented (no string names them).

===============================================================================
*/

#ifndef __TGCONFIGMAPPING_H__
#define __TGCONFIGMAPPING_H__

#include "NiTMap.h"
#include "NiTStringMap.h"
#include "TGString.h"

// Which member of a TGConfigValue holds the value (invented names).
enum configValueType_t {
	CONFIG_FLOAT,
	CONFIG_INT,
	CONFIG_STRING,
	CONFIG_TGSTRING
};

class TGConfigValue {
public:
	int				type;				// 0x00 configValueType_t
	union {
		int			intValue;			// 0x04
		float		floatValue;
		char *		stringValue;		// owned
	};
	TGString		tgString;			// 0x08

	// Made without a type and empty: the setter fills it in.
					TGConfigValue( void ) {}
};

// A section: its values by name, keeping copies of the names. It owns its
// values (invented name).
class TGConfigSection {
public:
	// Deletes the values, then the map.
						~TGConfigSection( void );

	NiTStringMap<TGConfigValue *>	values;			// 0x00
};

class TGConfigMapping {
public:
						TGConfigMapping( void );
	// Frees the string values and deletes the sections, then the map.
						~TGConfigMapping( void );

	float	GetFloatValue( char *section, char *name );
	int	GetIntValue( char *section, char *name );
	char *	GetStringValue( char *section, char *name );
	TGString *	GetTGStringValue( char *section, char *name );
	void	SetFloatValue( char *, char *, float );
	void	SetIntValue( char *, char *, int );
	void	SetStringValue( char *, char *, char * );
	void	SetTGStringValue( char *, char *, TGString * );
	bool	HasValue( char *section, char *name );
	bool	LoadConfigFile( char * );
	bool	SaveConfigFile( char * );

	// The section of the name; NULL when there is none.
	TGConfigSection *	Unknown71DB60( char *section );
	// The section of the name, made when there is none.
	TGConfigSection *	Unknown71DA20( char *section );

	// LoadConfigFile's helpers (invented names).
	char *	CopyTrimmed( char *text );
	bool	IsIntString( char *text );
	bool	IsFloatString( char *text );

	NiTStringMap<TGConfigSection *>	sections;		// 0x00 by name
};

// The options file's settings (the SWIG variable g_kConfigMapping).
extern TGConfigMapping	g_kConfigMapping;

#endif /* !__TGCONFIGMAPPING_H__ */
