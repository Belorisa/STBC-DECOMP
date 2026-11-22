/*
===============================================================================

	TextureAnimManager.h

	Texture animations: named sets of tracks over one image, each track a
	list of frames, each frame a rectangle of the image. The scripts build
	them through the SWIG interface (TextureAnimManager_AddContainer,
	TextureAnimContainer_AddTextureTrack, TextureAnimTrack_SetFrame), and the
	effect controllers that draw animated quads (FlareController,
	AnimTSParticleController) look them up by name in g_kTextureAnimManager.

	TextureAnimManager holds the string map of the containers. Its
	constructor (0x4F40D0, called with no argument from the static
	initialiser) stores only the NiTMap vtable 0x88DC48 and then the
	string-map vtable 0x88DC5C, five slots each: the class has no vtable of
	its own, so the map is a member, not a base. The lookups call the map's
	GetAt (0x4C7F60), and AddContainer's copy in the wrapper unit (0x620740)
	inlines its SetAt.

	TextureAnimContainer is 0x20 bytes: AddContainer allocates it and its
	constructor (0x4F3DA0) copies the name, takes the image of that name from
	g_kImageManager and builds an NiTArray of tracks (vtable 0x88DC44).

	TextureAnimTrack is 8 bytes (AddTextureTrack's allocation, 0x6204EB): the
	frame count and a block of 16-byte frames, each two NiPoint2 built by
	the vector constructor iterator in its constructor (0x6205F0, which
	AddTextureTrack calls out of line). SetFrame's wrapper writes the
	four floats of a frame; the corner copy (0x4DA060) reads them as the
	left, top, right and bottom of the frame's rectangle.

	The member names other than the SWIG ones are invented, after their use.

===============================================================================
*/

#ifndef __TEXTUREANIMMANAGER_H__
#define __TEXTUREANIMMANAGER_H__

#include "NiTStringMap.h"
#include "NiTArray.h"
#include "NiPointer.h"
#include "NiPoint2.h"
#include "renderer/NiImage.h"

/*
================
TextureAnimTrack

One animation: the frames a quad shows one after another.
================
*/
class TextureAnimTrack {
public:
	// a frame: the top left and bottom right corners of its rectangle of
	// the image, in texture coordinates
	struct Frame {
		NiPoint2			corners[2];
	};

	// the frames allocated in the initialiser list: AddTextureTrack expands
	// this constructor when the allocation is an assignment in the body,
	// where retail calls it (0x6205F0)
	// FUNCTION: STBC 0x006205f0 ??0TextureAnimTrack@@QAE@H@Z @game/AppWrap
							TextureAnimTrack( int numFrames_ ) : numFrames( numFrames_ ), frames( new Frame[numFrames_] ) {}
							~TextureAnimTrack() { delete [] frames; }

	int						GetNumFrames() { return numFrames; }

	void					SetFrame( int index, float left, float top, float right, float bottom ) {
								frames[index].corners[0].x = left;
								frames[index].corners[0].y = top;
								frames[index].corners[1].x = right;
								frames[index].corners[1].y = bottom;
							}

	// the texture coordinates of a frame's four corners, clockwise from the
	// top left
	// FUNCTION: STBC 0x4DA060 ?GetFrameCorners@TextureAnimTrack@@QAEXHPAVNiPoint2@@@Z @game/AnimatedTriShapeParticles
	void					GetFrameCorners( int index, NiPoint2 *uvs ) {
								uvs[0] = frames[index].corners[0];
								uvs[1].x = frames[index].corners[1].x;
								uvs[1].y = frames[index].corners[0].y;
								uvs[2] = frames[index].corners[1];
								uvs[3].x = frames[index].corners[0].x;
								uvs[3].y = frames[index].corners[1].y;
							}

private:
	int						numFrames;		// 0x00
	Frame *					frames;			// 0x04 numFrames of them
};

/*
================
TextureAnimContainer

The tracks of one named texture animation, over one image.
================
*/
class TextureAnimContainer {
public:
							TextureAnimContainer( char *name_ );
							~TextureAnimContainer();

	char *					GetName() { return name; }
	NiImage *				GetImage() { return image; }

	int						GetNumTracks() { return tracks.GetSize(); }
	TextureAnimTrack *		GetTrack( int index ) { return tracks.GetAt( index ); }

	// FUNCTION: STBC 0x006204d0 ?AddTextureTrack@TextureAnimContainer@@QAEPAVTextureAnimTrack@@H@Z @game/AppWrap
	TextureAnimTrack *		AddTextureTrack( int numFrames ) {
								TextureAnimTrack *track = new TextureAnimTrack( numFrames );
								tracks.Add( track );
								return track;
							}

private:
	char *					name;			// 0x00 a new[] copy
	NiPointer<NiImage>		image;			// 0x04 g_kImageManager's image of the name
	NiTArray<TextureAnimTrack *>	tracks;	// 0x08 owned
};

/*
================
TextureAnimManager

The texture animations, by name.
================
*/
// FUNCTION: STBC 0x4F40D0 ??0TextureAnimManager@@QAE@XZ
// FUNCTION: STBC 0x4F4030 ??1TextureAnimManager@@QAE@XZ
class TextureAnimManager {
public:
	// FUNCTION: STBC 0x00620740 ?AddContainer@TextureAnimManager@@QAEPAVTextureAnimContainer@@PAD@Z @game/AppWrap
	TextureAnimContainer *	AddContainer( char *name ) {
								TextureAnimContainer *container = new TextureAnimContainer( name );
								containers.SetAt( container->GetName(), container );
								return container;
							}

	// the container of that name, or false
	bool					GetContainer( const char *name, TextureAnimContainer *&container ) { return containers.GetAt( name, container ); }

	// deletes every container of g_kTextureAnimManager, at shutdown
	static void				DeleteAllContainers();

private:
	NiTStringMap<TextureAnimContainer *>	containers;	// 0x00 owned, by name
};

// The texture animations (the SWIG variable g_kTextureAnimManager).
extern TextureAnimManager	g_kTextureAnimManager;

#endif /* !__TEXTUREANIMMANAGER_H__ */
