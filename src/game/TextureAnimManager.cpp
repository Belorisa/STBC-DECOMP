/*
===============================================================================

	TextureAnimManager.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TextureAnimManager.h"
#include "ImageManagerClass.h"
#include "NiTextureProperty.h"

// The standard streams' initialisers and exit-time destructors (<iostream>),
// and the guard of std::ctype<unsigned short>::id with its exit-time thunk.
// FUNCTION: STBC 0x004f4400 _$E114
// FUNCTION: STBC 0x004f4420 _$E113
// FUNCTION: STBC 0x004f4430 _$E32
// FUNCTION: STBC 0x004f4440 _$E29
// FUNCTION: STBC 0x004f4450 _$E31
// FUNCTION: STBC 0x004f4460 _$E30
// FUNCTION: STBC 0x004f4470 _$E38
// FUNCTION: STBC 0x004f4480 _$E35
// FUNCTION: STBC 0x004f4490 _$E37
// FUNCTION: STBC 0x004f44a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004f44b0 _$E48
// FUNCTION: STBC 0x004f44c0 _$E47
// FUNCTION: STBC 0x004f44d0 _$E62
// FUNCTION: STBC 0x004f44e0 _$E61
// FUNCTION: STBC 0x004f44f0 _$E75
// FUNCTION: STBC 0x004f4500 _$E74
// FUNCTION: STBC 0x004f4510 _$E80
// FUNCTION: STBC 0x004f4520 _$E79
// FUNCTION: STBC 0x004f4530 _$E84
// FUNCTION: STBC 0x004f4540 _$E83
// FUNCTION: STBC 0x004f4550 _$E88
// FUNCTION: STBC 0x004f4560 _$E87
// FUNCTION: STBC 0x004f4570 _$E92
// FUNCTION: STBC 0x004f4580 _$E91
// FUNCTION: STBC 0x004f4590 _$E102
// FUNCTION: STBC 0x004f45a0 _$E101
// FUNCTION: STBC 0x004f45b0 _$E97
// FUNCTION: STBC 0x004f45c0 _$E96

// The global's constructor and exit-time destructor, with their thunks.
// FUNCTION: STBC 0x004f3ff0 _$E107
// FUNCTION: STBC 0x004f4000 _$E104
// FUNCTION: STBC 0x004f4010 _$E106
// FUNCTION: STBC 0x004f4020 _$E105
// DATA: STBC 0x986938 ?g_kTextureAnimManager@@3VTextureAnimManager@@A
TextureAnimManager g_kTextureAnimManager;

/*
================
TextureAnimContainer::TextureAnimContainer

Copies the name and takes the image of that name, with mipmaps.
================
*/
// FUNCTION: STBC 0x4F3DA0
TextureAnimContainer::TextureAnimContainer( char *name_ ) {
	if ( name_ ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
	image = g_kImageManager.GetImage( name, true, true );
}

/*
================
TextureAnimContainer::~TextureAnimContainer
================
*/
// FUNCTION: STBC 0x4F3E90
TextureAnimContainer::~TextureAnimContainer() {
	delete [] name;
	for ( unsigned int i = 0; i < tracks.GetSize(); i++ ) {
		delete tracks.GetAt( i );
	}
}

/*
================
TextureAnimManager::DeleteAllContainers
================
*/
// FUNCTION: STBC 0x4F3F00
void TextureAnimManager::DeleteAllContainers() {
	NiTMapIterator pos = g_kTextureAnimManager.containers.GetFirstPos();
	while ( pos ) {
		const char *name;
		TextureAnimContainer *container;
		g_kTextureAnimManager.containers.GetNext( pos, name, container );
		delete container;
	}
	g_kTextureAnimManager.containers.RemoveAll();
}
