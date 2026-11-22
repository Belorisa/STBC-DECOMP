/*
===============================================================================

	TGConfigMapping.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TGConfigMapping.h"
#include "TGStream.h"
#include "NiMain.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0071be80 _$E414
// FUNCTION: STBC 0x0071bea0 _$E413

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0071bd20 _$E32
// FUNCTION: STBC 0x0071bd30 _$E29
// FUNCTION: STBC 0x0071bd40 _$E31
// FUNCTION: STBC 0x0071bd50 _$E30
// FUNCTION: STBC 0x0071bd60 _$E38
// FUNCTION: STBC 0x0071bd70 _$E35
// FUNCTION: STBC 0x0071bd80 _$E37
// FUNCTION: STBC 0x0071bd90 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0071bda0 _$E46
// FUNCTION: STBC 0x0071bdb0 _$E45
// FUNCTION: STBC 0x0071bdc0 _$E50
// FUNCTION: STBC 0x0071bdd0 _$E49
// FUNCTION: STBC 0x0071bde0 _$E61
// FUNCTION: STBC 0x0071bdf0 _$E60
// FUNCTION: STBC 0x0071be00 _$E65
// FUNCTION: STBC 0x0071be10 _$E64
// FUNCTION: STBC 0x0071be20 _$E69
// FUNCTION: STBC 0x0071be30 _$E68
// FUNCTION: STBC 0x0071be40 _$E73
// FUNCTION: STBC 0x0071be50 _$E72
// FUNCTION: STBC 0x0071be60 _$E197
// FUNCTION: STBC 0x0071be70 _$E196
// FUNCTION: STBC 0x0071beb0 _$E139
// FUNCTION: STBC 0x0071bec0 _$E138
// FUNCTION: STBC 0x0071bed0 _$E152
// FUNCTION: STBC 0x0071bee0 _$E151
// FUNCTION: STBC 0x0071bef0 _$E202
// FUNCTION: STBC 0x0071bf00 _$E201
// FUNCTION: STBC 0x0071bf10 _$E207
// FUNCTION: STBC 0x0071bf20 _$E206
// FUNCTION: STBC 0x0071bf30 _$E211
// FUNCTION: STBC 0x0071bf40 _$E210
// FUNCTION: STBC 0x0071bf50 _$E215
// FUNCTION: STBC 0x0071bf60 _$E214
// FUNCTION: STBC 0x0071bf70 _$E222
// FUNCTION: STBC 0x0071bf80 _$E221
// FUNCTION: STBC 0x0071bf90 _$E247
// FUNCTION: STBC 0x0071bfa0 _$E246
// FUNCTION: STBC 0x0071bfb0 _$E251
// FUNCTION: STBC 0x0071bfc0 _$E250
// FUNCTION: STBC 0x0071bfd0 _$E97
// FUNCTION: STBC 0x0071bfe0 _$E96
// FUNCTION: STBC 0x0071bff0 _$E233
// FUNCTION: STBC 0x0071c000 _$E232
// FUNCTION: STBC 0x0071c010 _$E240
// FUNCTION: STBC 0x0071c020 _$E239
// FUNCTION: STBC 0x0071c030 _$E263
// FUNCTION: STBC 0x0071c040 _$E262
// FUNCTION: STBC 0x0071c050 _$E267
// FUNCTION: STBC 0x0071c060 _$E266
// FUNCTION: STBC 0x0071c070 _$E307
// FUNCTION: STBC 0x0071c080 _$E306
// FUNCTION: STBC 0x0071c090 _$E313
// FUNCTION: STBC 0x0071c0a0 _$E312
// FUNCTION: STBC 0x0071c0b0 _$E255
// FUNCTION: STBC 0x0071c0c0 _$E254
// FUNCTION: STBC 0x0071c0d0 _$E259
// FUNCTION: STBC 0x0071c0e0 _$E258
// FUNCTION: STBC 0x0071c0f0 _$E271
// FUNCTION: STBC 0x0071c100 _$E270
// FUNCTION: STBC 0x0071c110 _$E275
// FUNCTION: STBC 0x0071c120 _$E274
// FUNCTION: STBC 0x0071c130 _$E279
// FUNCTION: STBC 0x0071c140 _$E278
// FUNCTION: STBC 0x0071c150 _$E283
// FUNCTION: STBC 0x0071c160 _$E282
// FUNCTION: STBC 0x0071c170 _$E287
// FUNCTION: STBC 0x0071c180 _$E286
// FUNCTION: STBC 0x0071c190 _$E291
// FUNCTION: STBC 0x0071c1a0 _$E290
// FUNCTION: STBC 0x0071c1b0 _$E295
// FUNCTION: STBC 0x0071c1c0 _$E294
// FUNCTION: STBC 0x0071c1d0 _$E299
// FUNCTION: STBC 0x0071c1e0 _$E298
// FUNCTION: STBC 0x0071c1f0 _$E102
// FUNCTION: STBC 0x0071c200 _$E101
// FUNCTION: STBC 0x0071c210 _$E303
// FUNCTION: STBC 0x0071c220 _$E302
// FUNCTION: STBC 0x0071c230 _$E107
// FUNCTION: STBC 0x0071c240 _$E106
// FUNCTION: STBC 0x0071c250 _$E317
// FUNCTION: STBC 0x0071c260 _$E316
// FUNCTION: STBC 0x0071c270 _$E321
// FUNCTION: STBC 0x0071c280 _$E320
// FUNCTION: STBC 0x0071c290 _$E325
// FUNCTION: STBC 0x0071c2a0 _$E324
// FUNCTION: STBC 0x0071c2b0 _$E329
// FUNCTION: STBC 0x0071c2c0 _$E328
// FUNCTION: STBC 0x0071c2d0 _$E116
// FUNCTION: STBC 0x0071c2e0 _$E115
// FUNCTION: STBC 0x0071c2f0 _$E333
// FUNCTION: STBC 0x0071c300 _$E332
// FUNCTION: STBC 0x0071c310 _$E337
// FUNCTION: STBC 0x0071c320 _$E336
// FUNCTION: STBC 0x0071c330 _$E341
// FUNCTION: STBC 0x0071c340 _$E340
// FUNCTION: STBC 0x0071c350 _$E122
// FUNCTION: STBC 0x0071c360 _$E121
// FUNCTION: STBC 0x0071c370 _$E127
// FUNCTION: STBC 0x0071c380 _$E126
// FUNCTION: STBC 0x0071c390 _$E157
// FUNCTION: STBC 0x0071c3a0 _$E156
// FUNCTION: STBC 0x0071c3b0 _$E345
// FUNCTION: STBC 0x0071c3c0 _$E344
// FUNCTION: STBC 0x0071c3d0 _$E162
// FUNCTION: STBC 0x0071c3e0 _$E161
// FUNCTION: STBC 0x0071c3f0 _$E349
// FUNCTION: STBC 0x0071c400 _$E348
// FUNCTION: STBC 0x0071c410 _$E353
// FUNCTION: STBC 0x0071c420 _$E352
// FUNCTION: STBC 0x0071c430 _$E167
// FUNCTION: STBC 0x0071c440 _$E166
// FUNCTION: STBC 0x0071c450 _$E357
// FUNCTION: STBC 0x0071c460 _$E356
// FUNCTION: STBC 0x0071c470 _$E132
// FUNCTION: STBC 0x0071c480 _$E131
// FUNCTION: STBC 0x0071c490 _$E361
// FUNCTION: STBC 0x0071c4a0 _$E360
// FUNCTION: STBC 0x0071c4b0 _$E365
// FUNCTION: STBC 0x0071c4c0 _$E364
// FUNCTION: STBC 0x0071c4d0 _$E172
// FUNCTION: STBC 0x0071c4e0 _$E171
// FUNCTION: STBC 0x0071c4f0 _$E177
// FUNCTION: STBC 0x0071c500 _$E176
// FUNCTION: STBC 0x0071c510 _$E369
// FUNCTION: STBC 0x0071c520 _$E368
// FUNCTION: STBC 0x0071c530 _$E373
// FUNCTION: STBC 0x0071c540 _$E372
// FUNCTION: STBC 0x0071c550 _$E377
// FUNCTION: STBC 0x0071c560 _$E376
// FUNCTION: STBC 0x0071c570 _$E381
// FUNCTION: STBC 0x0071c580 _$E380
// FUNCTION: STBC 0x0071c590 _$E385
// FUNCTION: STBC 0x0071c5a0 _$E384
// FUNCTION: STBC 0x0071c5b0 _$E389
// FUNCTION: STBC 0x0071c5c0 _$E388
// FUNCTION: STBC 0x0071c5d0 _$E393
// FUNCTION: STBC 0x0071c5e0 _$E392
// FUNCTION: STBC 0x0071c5f0 _$E397
// FUNCTION: STBC 0x0071c600 _$E396
// FUNCTION: STBC 0x0071c610 _$E182
// FUNCTION: STBC 0x0071c620 _$E181
// FUNCTION: STBC 0x0071c630 _$E401
// FUNCTION: STBC 0x0071c640 _$E400
// FUNCTION: STBC 0x0071c650 _$E187
// FUNCTION: STBC 0x0071c660 _$E186
// FUNCTION: STBC 0x0071c670 _$E192
// FUNCTION: STBC 0x0071c680 _$E191

// The options file's settings: g_kConfigMapping's initialiser and its
// exit-time destructor.
// FUNCTION: STBC 0x0071c690 _$E406
// FUNCTION: STBC 0x0071c6a0 _$E403
// FUNCTION: STBC 0x0071c6b0 _$E405
// FUNCTION: STBC 0x0071c6c0 _$E404

// The map instances this unit's sections and settings make, with their
// hooks: the NiTMap and NiTStringMap of the sections (NiTMap.h,
// NiTStringMap.h hold the definitions).
// FUNCTION: STBC 0x0071df50 ?KeyToHashIndex@?$NiTMap@PBDPAVTGConfigSection@@@@MBEIPBD@Z
// FUNCTION: STBC 0x0071df60 ?IsKeysEqual@?$NiTMap@PBDPAVTGConfigSection@@@@MBE_NPBD0@Z
// FUNCTION: STBC 0x0071df80 ?SetValue@?$NiTMap@PBDPAVTGConfigSection@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigSection@@@@PBDPAVTGConfigSection@@@Z
// FUNCTION: STBC 0x0071dfa0 ?ClearValue@?$NiTMap@PBDPAVTGConfigSection@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigSection@@@@@Z
// FUNCTION: STBC 0x0071dfb0 ??1?$NiTStringMap@PAVTGConfigSection@@@@UAE@XZ
// FUNCTION: STBC 0x0071e050 ?KeyToHashIndex@?$NiTStringMap@PAVTGConfigSection@@@@MBEIPBD@Z
// FUNCTION: STBC 0x0071e080 ?IsKeysEqual@?$NiTStringMap@PAVTGConfigSection@@@@MBE_NPBD0@Z
// FUNCTION: STBC 0x0071e0e0 ?SetValue@?$NiTStringMap@PAVTGConfigSection@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigSection@@@@PBDPAVTGConfigSection@@@Z
// FUNCTION: STBC 0x0071e130 ?ClearValue@?$NiTStringMap@PAVTGConfigSection@@@@MAEXPAV?$NiTMapItem@PBDPAVTGConfigSection@@@@@Z
// FUNCTION: STBC 0x0071e1e0 ??_G?$NiTMap@PBDPAVTGConfigSection@@@@UAEPAXI@Z
// FUNCTION: STBC 0x0071e260 ??_G?$NiTStringMap@PAVTGConfigSection@@@@UAEPAXI@Z

// DATA: STBC 0x0099c860
TGConfigMapping g_kConfigMapping;

/*
================
TGConfigMapping::TGConfigMapping
================
*/
// FUNCTION: STBC 0x0071c6d0
TGConfigMapping::TGConfigMapping( void ) {
}

/*
================
TGConfigMapping::~TGConfigMapping

Frees the strings the string values hold and deletes the sections, which
delete their values.
================
*/
// FUNCTION: STBC 0x0071c720
TGConfigMapping::~TGConfigMapping( void ) {
	NiTMapIterator pos = sections.GetFirstPos();
	while ( pos != NULL ) {
		const char *sectionName;
		TGConfigSection *section;
		sections.GetNext( pos, sectionName, section );
		if ( section != NULL ) {
			NiTMapIterator valuePos = section->values.GetFirstPos();
			while ( valuePos != NULL ) {
				const char *name;
				TGConfigValue *value;
				section->values.GetNext( valuePos, name, value );
				if ( value != NULL && value->type == CONFIG_STRING ) {
					delete value->stringValue;
				}
			}
			delete section;
		}
	}
	sections.RemoveAll();
}

/*
================
TGConfigSection::~TGConfigSection

Deletes the values.
================
*/
// FUNCTION: STBC 0x0071c950
TGConfigSection::~TGConfigSection( void ) {
	NiTMapIterator pos = values.GetFirstPos();
	while ( pos != NULL ) {
		const char *name;
		TGConfigValue *value;
		values.GetNext( pos, name, value );
		delete value;
	}
	values.RemoveAll();
}

/*
================
TGConfigMapping::GetFloatValue

The float setting of the section; 0 when there is none or it is of another
type.
================
*/
// FUNCTION: STBC 0x0071cae0 ?GetFloatValue@TGConfigMapping@@QAEMPAD0@Z
float TGConfigMapping::GetFloatValue( char *section, char *name ) {
	TGConfigSection *found = Unknown71DB60( section );
	if ( found == NULL ) {
		return 0.0f;
	}
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		return 0.0f;
	}
	if ( value->type != CONFIG_FLOAT ) {
		return 0.0f;
	}
	return value->floatValue;
}

/*
================
TGConfigMapping::GetIntValue

The integer setting of the section; 0 when there is none or it is of
another type.
================
*/
// FUNCTION: STBC 0x0071cb60 ?GetIntValue@TGConfigMapping@@QAEHPAD0@Z
int TGConfigMapping::GetIntValue( char *section, char *name ) {
	TGConfigSection *found = Unknown71DB60( section );
	if ( found == NULL ) {
		return 0;
	}
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		return 0;
	}
	if ( value->type != CONFIG_INT ) {
		return 0;
	}
	return value->intValue;
}

/*
================
TGConfigMapping::GetStringValue

The string setting of the section; NULL when there is none or it is of
another type.
================
*/
// FUNCTION: STBC 0x0071cbd0 ?GetStringValue@TGConfigMapping@@QAEPADPAD0@Z
char *TGConfigMapping::GetStringValue( char *section, char *name ) {
	TGConfigSection *found = Unknown71DB60( section );
	if ( found == NULL ) {
		return NULL;
	}
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		return NULL;
	}
	if ( value->type != CONFIG_STRING ) {
		return NULL;
	}
	return value->stringValue;
}

/*
================
TGConfigMapping::GetTGStringValue

The TGString setting of the section; NULL when there is none or it is of
another type.
================
*/
// FUNCTION: STBC 0x0071cc40 ?GetTGStringValue@TGConfigMapping@@QAEPAVTGString@@PAD0@Z
TGString *TGConfigMapping::GetTGStringValue( char *section, char *name ) {
	TGConfigSection *found = Unknown71DB60( section );
	if ( found == NULL ) {
		return NULL;
	}
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		return NULL;
	}
	if ( value->type != CONFIG_TGSTRING ) {
		return NULL;
	}
	return &value->tgString;
}

/*
================
TGConfigMapping::SetFloatValue

Sets the float setting of the section, making the section and the setting
when there are none.
================
*/
// FUNCTION: STBC 0x0071ccb0 ?SetFloatValue@TGConfigMapping@@QAEXPAD0M@Z
void TGConfigMapping::SetFloatValue( char *section, char *name, float floatValue ) {
	TGConfigSection *found = Unknown71DA20( section );
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		value = new TGConfigValue;
	}
	value->type = CONFIG_FLOAT;
	value->floatValue = floatValue;
	found->values.SetAt( name, value );
}

/*
================
TGConfigMapping::SetIntValue
================
*/
// FUNCTION: STBC 0x0071cdd0 ?SetIntValue@TGConfigMapping@@QAEXPAD0H@Z
void TGConfigMapping::SetIntValue( char *section, char *name, int intValue ) {
	TGConfigSection *found = Unknown71DA20( section );
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		value = new TGConfigValue;
	}
	value->type = CONFIG_INT;
	value->intValue = intValue;
	found->values.SetAt( name, value );
}

/*
================
TGConfigMapping::SetStringValue

Keeps a copy of the string.
================
*/
// FUNCTION: STBC 0x0071cef0 ?SetStringValue@TGConfigMapping@@QAEXPAD00@Z
void TGConfigMapping::SetStringValue( char *section, char *name, char *stringValue ) {
	TGConfigSection *found = Unknown71DA20( section );
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		value = new TGConfigValue;
	}
	value->type = CONFIG_STRING;
	// BUG: the string a value of this type already holds is not freed.
	value->stringValue = new char[strlen( stringValue ) + 1];
	strcpy( value->stringValue, stringValue );
	found->values.SetAt( name, value );
}

/*
================
TGConfigMapping::SetTGStringValue
================
*/
// FUNCTION: STBC 0x0071d050 ?SetTGStringValue@TGConfigMapping@@QAEXPAD0PAVTGString@@@Z
void TGConfigMapping::SetTGStringValue( char *section, char *name, TGString *tgStringValue ) {
	TGConfigSection *found = Unknown71DA20( section );
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		value = new TGConfigValue;
	}
	value->type = CONFIG_TGSTRING;
	value->tgString = *tgStringValue;
	found->values.SetAt( name, value );
}

/*
================
TGConfigMapping::HasValue
================
*/
// FUNCTION: STBC 0x0071d180 ?HasValue@TGConfigMapping@@QAE_NPAD0@Z
bool TGConfigMapping::HasValue( char *section, char *name ) {
	TGConfigSection *found = Unknown71DB60( section );
	if ( found == NULL ) {
		return false;
	}
	TGConfigValue *value;
	if ( !found->values.GetAt( name, value ) ) {
		return false;
	}
	return true;
}

/*
================
TGConfigMapping::LoadConfigFile

Reads a settings file in the format SaveConfigFile writes: "[name]" opens
a section, then each line until the next section is a value, "name|value"
a number (an integer or a float), "name=value" a string and "name-value"
a TGString. Values replace those of the same name. False when the file
can't be opened.
================
*/
// FUNCTION: STBC 0x0071d1e0
// DRAFT: calls and branches agree; the first line-terminating store after a section header indexes the buffer differently (retail sub eax, esi; mov [esp+eax+0x3c]) and the TGString default literal referent waits for the frame to align
bool TGConfigMapping::LoadConfigFile( char *fileName ) {
	TGStream stream;
	stream.VirtualSlot00( fileName, "rt" );
	if ( stream.VirtualSlot58() == -1 ) {
		return false;
	}

	char line[256];
	stream.ReadCLine( line, 255, '\n' );
	while ( !stream.Eof() ) {
		// BUG: a line before the first section is never stepped over, so the loop does not end
		char *mark = strchr( line, '[' );
		if ( mark != NULL && mark == line ) {
			mark = strchr( line, ']' );
			if ( mark != NULL ) {
				*mark = '\0';
				TGConfigSection *section = Unknown71DA20( line + 1 );
				*mark = ']';
				long start = stream.GetPos();
				stream.ReadCLine( line, 255, '\n' );
				line[stream.GetPos() - start] = '\0';
				while ( !stream.Eof() ) {
					char *open = strchr( line, '[' );
					if ( open != NULL && open == line ) {
						break;
					}
					bool number = false;
					bool wide = false;
					mark = strchr( line, '=' );
					if ( mark == NULL ) {
						mark = strchr( line, '|' );
						if ( mark != NULL ) {
							number = true;
						} else {
							mark = strchr( line, '-' );
							if ( mark != NULL ) {
								wide = true;
							}
						}
					}
					if ( mark != NULL ) {
						char *text = CopyTrimmed( mark + 1 );
						TGConfigValue *value = new TGConfigValue;
						if ( IsIntString( text ) && number ) {
							value->type = CONFIG_INT;
							value->intValue = strtol( text, &mark, 10 );
							delete text;
						} else if ( IsFloatString( text ) && number ) {
							value->type = CONFIG_FLOAT;
							value->floatValue = (float)strtod( text, &mark );
							delete text;
						} else if ( wide ) {
							value->type = CONFIG_TGSTRING;
							// BUG: the text is read as wide characters and never freed
							unsigned short *c = (unsigned short *)text;
							while ( *c != 0 ) {
								value->tgString.Unknown6F54C0( value->tgString.GetLength(), *c );
								c++;
							}
						} else {
							value->type = CONFIG_STRING;
							value->stringValue = text;
						}
						mark = strchr( line, '=' );
						if ( mark != NULL || ( mark = strchr( line, '|' ) ) != NULL || ( mark = strchr( line, '-' ) ) != NULL ) {
							*mark = '\0';
						}
						section->values.SetAt( line, value );
					}
					start = stream.GetPos();
					stream.ReadCLine( line, 255, '\n' );
					line[stream.GetPos() - start] = '\0';
				}
			} else {
				long start = stream.GetPos();
				stream.ReadCLine( line, 255, '\n' );
				line[stream.GetPos() - start] = '\0';
			}
		}
	}
	stream.Close();
	return true;
}

/*
================
TGConfigMapping::SaveConfigFile

Writes the settings to a text file, which must be named *.cfg: each section
as "[name]", then a line for each of its values, "name|value" for a number,
"name=value" for a string and "name-" and the characters for a TGString,
then an empty line. False when the name is not a .cfg file or a write fails.
================
*/
// FUNCTION: STBC 0x0071d600 ?SaveConfigFile@TGConfigMapping@@QAE_NPAD@Z
// DRAFT: retail keeps GetFirstPos's result in a frame slot and copies it to a second one for the section walk (a frame 8 bytes larger: NiTMap's inline, off-limits to this lane); the section and value loops' registers follow from it
bool TGConfigMapping::SaveConfigFile( char *fileName ) {
	char text[256];

	NiTMapIterator pos = sections.GetFirstPos();
	if ( strlen( fileName ) < 4 || strcmp( &fileName[strlen( fileName ) - 4], ".cfg" ) != 0 ) {
		return false;
	}

	TGStream file;
	file.VirtualSlot00( fileName, "wt" );
	if ( file.VirtualSlot58() ) {
		return false;
	}

	while ( pos != NULL ) {
		const char *sectionName;
		TGConfigSection *section;
		sections.GetNext( pos, sectionName, section );
		if ( sectionName != NULL ) {
			char *header = new char[strlen( sectionName ) + 3];
			sprintf( header, "[%s]\n", sectionName );
			file.Write( header, strlen( header ) );
			delete header;
		}

		NiTMapIterator valuePos = section->values.GetFirstPos();
		while ( valuePos != NULL ) {
			const char *name;
			TGConfigValue *value;
			section->values.GetNext( valuePos, name, value );
			if ( value == NULL ) {
				continue;
			}
			switch ( value->type ) {
				case CONFIG_FLOAT:
					sprintf( text, "%s|%f\n", name, value->floatValue );
					file.Write( text, strlen( text ) );
					if ( file.VirtualSlot58() ) {
						return false;
					}
					break;
				case CONFIG_INT:
					sprintf( text, "%s|%d\n", name, value->intValue );
					file.Write( text, strlen( text ) );
					if ( file.VirtualSlot58() ) {
						return false;
					}
					break;
				case CONFIG_STRING:
					sprintf( text, "%s=%s\n", name, value->stringValue );
					file.Write( text, strlen( text ) );
					if ( file.VirtualSlot58() ) {
						return false;
					}
					break;
				case CONFIG_TGSTRING:
					sprintf( text, "%s-", name );
					file.Write( text, strlen( text ) );
					if ( file.VirtualSlot58() ) {
						return false;
					}
					if ( value->tgString.GetLength() > 0 ) {
						for ( int i = 0; i < value->tgString.GetLength(); i++ ) {
							file.WriteWChar( value->tgString.Unknown6F4C20( i ) );
							if ( file.VirtualSlot58() ) {
								return false;
							}
						}
					} else {
						file.WriteWChar( 0 );
						if ( file.VirtualSlot58() ) {
							return false;
						}
					}
					file.WriteChar( '\n' );
					break;
			}
		}
		file.Write( "\n", 1 );
		if ( file.VirtualSlot58() ) {
			return false;
		}
	}
	file.Close();
	if ( file.VirtualSlot58() ) {
		return false;
	}
	return true;
}

/*
================
TGConfigMapping::Unknown71DA20
================
*/
// FUNCTION: STBC 0x0071da20 ?Unknown71DA20@TGConfigMapping@@QAEPAVTGConfigSection@@PAD@Z
TGConfigSection *TGConfigMapping::Unknown71DA20( char *section ) {
	TGConfigSection *found;
	if ( !sections.GetAt( section, found ) ) {
		found = new TGConfigSection;
		sections.SetAt( section, found );
	}
	return found;
}

/*
================
TGConfigMapping::Unknown71DB60
================
*/
// FUNCTION: STBC 0x0071db60 ?Unknown71DB60@TGConfigMapping@@QAEPAVTGConfigSection@@PAD@Z
TGConfigSection *TGConfigMapping::Unknown71DB60( char *section ) {
	TGConfigSection *found;
	if ( sections.GetAt( section, found ) ) {
		return found;
	}
	return NULL;
}

/*
================
TGConfigMapping::CopyTrimmed

A copy of the text up to its double NUL, with line breaks turned into
spaces and the spaces at both ends left out, itself ending in a double NUL;
NULL for no text (invented name). The text is left as it was but for its
line breaks.
================
*/
// FUNCTION: STBC 0x0071dba0
// DRAFT: same instructions but for the register pair: retail keeps the text in ebx and the length in ebp
char *TGConfigMapping::CopyTrimmed( char *text ) {
	if ( text == NULL ) {
		return NULL;
	}

	unsigned int length = 0;
	while ( text[length] != '\0' || text[length + 1] != '\0' ) {
		length++;
	}
	for ( unsigned int i = 0; i < length; i++ ) {
		if ( text[i] == '\n' ) {
			text[i] = ' ';
		}
	}

	unsigned int start = 0;
	while ( start < length && text[start] == ' ' ) {
		start++;
	}
	unsigned int end = length;
	do {
		end--;
	} while ( text[end] == ' ' );
	end++;

	char *copy = new char[end - start + 2];
	text[end] = '\0';
	text[end + 1] = '\0';
	memcpy( copy, text + start, end - start + 2 );
	if ( end != length ) {
		text[end] = ' ';
	}
	return copy;
}

/*
================
TGConfigMapping::IsIntString

True when every character is a digit or a minus sign (invented name).
================
*/
// FUNCTION: STBC 0x0071dc40
bool TGConfigMapping::IsIntString( char *text ) {
	if ( text == NULL ) {
		return false;
	}
	for ( unsigned int i = 0; i < strlen( text ); i++ ) {
		char c = text[i];
		if ( ( c < '0' || c > '9' ) && c != '-' ) {
			return false;
		}
	}
	return true;
}

/*
================
TGConfigMapping::IsFloatString

True when every character is a digit, a point or a minus sign (invented
name).
================
*/
// FUNCTION: STBC 0x0071dc90
bool TGConfigMapping::IsFloatString( char *text ) {
	if ( text == NULL ) {
		return false;
	}
	for ( unsigned int i = 0; i < strlen( text ); i++ ) {
		char c = text[i];
		if ( ( c < '0' || c > '9' ) && c != '.' && c != '-' ) {
			return false;
		}
	}
	return true;
}
