/*
===============================================================================

	TGAnimationManagerClass.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGAnimationManagerClass.h"
#include "NiFile.h"
#include "TGStream.h"

// The standard streams' initialisers and exit-time destructors (<iostream>)
// and the locale-id pair: retail lays them out after the unit's code, then
// the key registrations of TGAnimationEntry.h, whose stores follow the
// unit's <iostream> bytes (0x9979A4).
// FUNCTION: STBC 0x006c8d40 _$E32
// FUNCTION: STBC 0x006c8d50 _$E29
// FUNCTION: STBC 0x006c8d60 _$E31
// FUNCTION: STBC 0x006c8d70 _$E30
// FUNCTION: STBC 0x006c8d80 _$E38
// FUNCTION: STBC 0x006c8d90 _$E35
// FUNCTION: STBC 0x006c8da0 _$E37
// FUNCTION: STBC 0x006c8db0 _$E36
// FUNCTION: STBC 0x006c8dc0 _$E92
// FUNCTION: STBC 0x006c8de0 _$E91

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c8df0 _$E77
// FUNCTION: STBC 0x006c8e00 _$E76
// FUNCTION: STBC 0x006c8e10 _$E66
// FUNCTION: STBC 0x006c8e20 _$E65
// FUNCTION: STBC 0x006c8e30 _$E47
// FUNCTION: STBC 0x006c8e40 _$E46

// The class handler table's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006c7d90 _$E85
// FUNCTION: STBC 0x006c7da0 _$E82
// FUNCTION: STBC 0x006c7db0 _$E84
// FUNCTION: STBC 0x006c7dc0 _$E83
// DATA: STBC 0x997988 ?g_kAnimationManager@@3VTGAnimationManagerClass@@A
TGAnimationManagerClass g_kAnimationManager;

/*
================
TGAnimationManagerClass::ClearAnimations
================
*/
// FUNCTION: STBC 0x006c7dd0 ?ClearAnimations@TGAnimationManagerClass@@QAEXXZ
void TGAnimationManagerClass::ClearAnimations( void ) {
	animations.DeleteAll();
}

/*
================
TGAnimationManagerClass::~TGAnimationManagerClass

Frees the file names; the members free the rest.
================
*/
// FUNCTION: STBC 0x006c7ec0
TGAnimationManagerClass::~TGAnimationManagerClass( void ) {
	NiTMapIterator pos = files.GetFirstPos();
	while ( pos ) {
		const char *name;
		char *file;
		files.GetNext( pos, name, file );
		if ( file != NULL ) {
			delete [] file;
		}
	}
	files.RemoveAll();
}

/*
================
TGAnimationManagerClass::MapDefaultAnimation

Adds an animation of the name made from the node's own controllers; 0 when
the name is taken or there is no node.
================
*/
// FUNCTION: STBC 0x006c80a0 ?MapDefaultAnimation@TGAnimationManagerClass@@QAEHPAVNiNode@@PAD@Z
int TGAnimationManagerClass::MapDefaultAnimation( NiNode *node, char *name ) {
	if ( animations.Get( name ) != NULL ) {
		return 0;
	}
	if ( node == NULL ) {
		return 0;
	}
	TGAnimationEntry *entry = new TGAnimationEntry( name );
	entry->Unknown6CAAE0( node );
	animations.Insert( entry );
	return 1;
}

/*
================
TGAnimationManagerClass::LoadAnimation

Reads the animation of the name from the file and remembers the file; 0
when the name is taken, the file is missing or cannot be read.
================
*/
// FUNCTION: STBC 0x006c8290 ?LoadAnimation@TGAnimationManagerClass@@QAEHPAD0@Z
int TGAnimationManagerClass::LoadAnimation( char *file, char *name ) {
	if ( animations.Get( name ) != NULL ) {
		return 0;
	}
	if ( NiFile::Access( file, 0 ) ) {
		return 0;
	}
	TGAnimationEntry *entry = new TGAnimationEntry( name );
	if ( entry->Load( file ) ) {
		animations.Insert( entry );
		char *copy = new char[strlen( file ) + 1];
		strcpy( copy, file );
		files.SetAt( name, copy );
		return 1;
	}
	delete entry;
	return 0;
}

/*
================
TGAnimationManagerClass::FreeAnimation
================
*/
// FUNCTION: STBC 0x006c8560 ?FreeAnimation@TGAnimationManagerClass@@QAEXPAD@Z
void TGAnimationManagerClass::FreeAnimation( char *name ) {
	animations.Delete( name );
	char *file;
	if ( files.GetAt( name, file ) && file != NULL ) {
		delete [] file;
	}
	files.RemoveAt( name );
}

/*
================
TGAnimationManagerClass::UseAnimation
================
*/
// FUNCTION: STBC 0x006c8730 ?UseAnimation@TGAnimationManagerClass@@QAEHPAVTGAnimBlender@@PAD_NMM@Z
int TGAnimationManagerClass::UseAnimation( TGAnimBlender *blender, char *name, bool unknown, float start, float end ) {
	TGAnimationEntry *entry = animations.Get( name );
	if ( entry == NULL ) {
		return 0;
	}
	return entry->Unknown6CB200( blender, unknown, start, end );
}

/*
================
TGAnimationManagerClass::BlendAnimation
================
*/
// FUNCTION: STBC 0x006c87e0 ?BlendAnimation@TGAnimationManagerClass@@QAEHPAVTGAnimBlender@@MM_N11PAD@Z
int TGAnimationManagerClass::BlendAnimation( TGAnimBlender *blender, float unknown, float unknown2, bool flag, bool flag2, bool flag3, char *name ) {
	TGAnimationEntry *entry = animations.Get( name );
	if ( entry == NULL ) {
		return 0;
	}
	return entry->Unknown6CB450( blender, unknown, unknown2, flag, flag2, flag3, name );
}

/*
================
TGAnimationManagerClass::UseDefault
================
*/
// FUNCTION: STBC 0x006c88a0 ?UseDefault@TGAnimationManagerClass@@QAEHPAVTGAnimBlender@@PAD@Z
int TGAnimationManagerClass::UseDefault( TGAnimBlender *blender, char *name ) {
	TGAnimationEntry *entry = animations.Get( name );
	if ( entry == NULL ) {
		return 0;
	}
	return entry->Unknown6CBD80( blender );
}

/*
================
TGAnimationManagerClass::GetAnimationLength

The length of the animation in seconds; 0 when it is not loaded.
================
*/
// FUNCTION: STBC 0x006c8940 ?GetAnimationLength@TGAnimationManagerClass@@QAEMPAD@Z
float TGAnimationManagerClass::GetAnimationLength( char *name ) {
	TGAnimationEntry *entry = animations.Get( name );
	if ( entry == NULL ) {
		return 0.0f;
	}
	return entry->GetLength();
}

/*
================
TGAnimationManagerClass::IsLoaded
================
*/
// FUNCTION: STBC 0x006c89e0 ?IsLoaded@TGAnimationManagerClass@@QAE_NPAD@Z
bool TGAnimationManagerClass::IsLoaded( char *name ) {
	if ( animations.Get( name ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TGAnimationManagerClass::PositionUsingAnimation
================
*/
// FUNCTION: STBC 0x006c8a80 ?PositionUsingAnimation@TGAnimationManagerClass@@QAEHPAVTGAnimBlender@@PAD@Z
int TGAnimationManagerClass::PositionUsingAnimation( TGAnimBlender *blender, char *name ) {
	TGAnimationEntry *entry = animations.Get( name );
	if ( entry == NULL ) {
		return 0;
	}
	return entry->Unknown6CC400( blender );
}

/*
================
TGAnimationManagerClass::GetAnimationType

What the animation does to the root node; normal for an animation that is
not loaded.
================
*/
// FUNCTION: STBC 0x006c8b20
int TGAnimationManagerClass::GetAnimationType( char *name ) {
	TGAnimationEntry *entry = animations.Get( name );
	if ( entry == NULL ) {
		return SAT_NORMAL;
	}
	if ( entry->IsSpecial() == true ) {
		return SAT_SPECIAL;
	}
	return entry->MovesPelvis() == true ? SAT_MOVES_PELVIS : SAT_NORMAL;
}

/*
================
TGAnimationManagerClass::WriteToStream

Writes the number of animations, then each one's name and file. The name is
invented, after the other managers'.
================
*/
// FUNCTION: STBC 0x006c8bd0
bool TGAnimationManagerClass::WriteToStream( TGStream *stream ) {
	int count = animations.GetCount();
	TGAnimationEntry **entries = animations.GetItems();
	stream->WriteInt( count );
	for ( int i = 0; i < count; i++ ) {
		stream->VirtualSlot11( entries[i]->GetName() );
		char *file;
		files.GetAt( entries[i]->GetName(), file );
		stream->VirtualSlot11( file );
	}
	return true;
}

/*
================
TGAnimationManagerClass::ReadFromStream

Loads the animations WriteToStream wrote from their files again. The name
is invented, after the other managers'.
================
*/
// FUNCTION: STBC 0x006c8c70
bool TGAnimationManagerClass::ReadFromStream( TGStream *stream ) {
	for ( int count = stream->ReadInt(); count > 0; count-- ) {
		char *name = stream->VirtualSlot10( -1 );
		char *file = stream->VirtualSlot10( -1 );
		LoadAnimation( file, name );
		delete file;	// BUG: the name read is never freed
	}
	return true;
}
