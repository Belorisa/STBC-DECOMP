// NiImage.cpp -- the texture image

#include "NiRTLib.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "NiImage.h"
#include "NiRender.h"
#include "NiRawImageData.h"
#include "NiStream.h"
#include "NiFile.h"
#include "NiImageFile.h"
#include "NiFilename.h"
#include "NiSystem.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007e1e70 _$E30
// FUNCTION: STBC 0x007e1f60 _$E44
// FUNCTION: STBC 0x007e1fd0 _$E57
// FUNCTION: STBC 0x007e1ff0 _$E61
// FUNCTION: STBC 0x007e2000 _$E65
// FUNCTION: STBC 0x007e2010 _$E69
// FUNCTION: STBC 0x007e2020 _$E73
// FUNCTION: STBC 0x007e2030 _$E77
// FUNCTION: STBC 0x007e2040 _$E181
// FUNCTION: STBC 0x007e2050 _$E186
// FUNCTION: STBC 0x007e2060 _$E191

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007e1f40 _$E218
// FUNCTION: STBC 0x007e1e10 _$E17
// FUNCTION: STBC 0x007e1e30 _$E15
// FUNCTION: STBC 0x007e1e40 _$E23
// FUNCTION: STBC 0x007e1e60 _$E21

// FUNCTION: STBC 0x007e0950 _$E200
// FUNCTION: STBC 0x007e0970 _$E197
// DATA: STBC 0x9A1648 ?rtti@NiImage@@2VNiRTTI@@B
const NiRTTI NiImage::rtti( "NiImage", &NiObject::rtti );

// DATA: STBC 0x9A168C ?error@NiImage@@2IA
unsigned int NiImage::error;

// DATA: STBC 0x9A1694 ?createFunction@NiImage@@2P6APAV1@XZA
NiImage *(*NiImage::createFunction)();

// DATA: STBC 0x9A1690
NiTListIterator NiImage::rendererPos;

// DATA: STBC 0x9A1689
bool NiImage::defaultMipmap;

// DATA: STBC 0x9A168A
bool NiImage::defaultUnknown3;

// DATA: STBC 0x9783D0
unsigned int NiImage::nextSerial = 1;

// DATA: STBC 0x9783D8
bool NiImage::saveFileNames = true;

/*
================
NiImage::Register
================
*/
// FUNCTION: STBC 0x7e0990 ?Register@NiImage@@SAHXZ
NiImplementRegister( NiImage )

// FUNCTION: STBC 0x007e0ae0 _$E206
// FUNCTION: STBC 0x007e0b00 _$E202
// FUNCTION: STBC 0x007e0b60 _$E203
// DATA: STBC 0x9A165C ?fileImage@NiImage@@2V?$NiPointer@VNiRawImageData@@@@A
NiPointer<NiRawImageData> NiImage::fileImage = new NiRawImageData;

// FUNCTION: STBC 0x007e0ba0 _$E211
// FUNCTION: STBC 0x007e0be0 _$E209
// DATA: STBC 0x9A1660 ?renderers@NiImage@@2V?$NiTList@PAVNiRender@@@@A
NiTList<NiRender *> NiImage::renderers( 2 );

/*
================
NiImage::Create

Makes the renderer's kind of image for an image file, keeping the file
name standardized; the arguments as for raw pixels.
================
*/
// FUNCTION: STBC 0x7E0C50 ?Create@NiImage@@SAPAV1@PBDHHH@Z
NiImage *NiImage::Create( const char *file, int mipmap, int format, int unknown3 ) {
	if ( !file ) {
		error = 5;
		return 0;
	}
	if ( !createFunction ) {
		return 0;
	}
	NiImage *image = createFunction();
	if ( !image ) {
		error = 4;
		return 0;
	}

	if ( image->url ) {
		delete [] image->url;
	}
	image->url = new char[strlen( file ) + 1];
	strcpy( image->url, file );
	NiFile::StandardizeFilePath( &image->url );
	if ( mipmap == 1 || ( mipmap == 2 && defaultMipmap ) ) {
		image->AddFlags( 2 );
	}
	if ( unknown3 == 1 || ( unknown3 == 2 && defaultUnknown3 ) ) {
		image->AddFlags( 4 );
	}
	image->prefTexFormat = (texFormat_t)format;
	if ( !image->Unknown7E0EF0() ) {
		delete image;
		return 0;
	}
	return image;
}

/*
================
NiImage::Create

Makes the renderer's kind of image for raw pixels: mipmapped for a
mipmap of 1, or of 2 when images are by default; told to every renderer,
and deleted when one refuses it.
================
*/
// FUNCTION: STBC 0x7E0D60 ?Create@NiImage@@SAPAV1@V?$NiPointer@VNiRawImageData@@@@HHH@Z
NiImage *NiImage::Create( NiPointer<NiRawImageData> raw, int mipmap, int format, int unknown3 ) {
	if ( !raw ) {
		error = 6;
		return 0;
	}
	if ( !createFunction ) {
		return 0;
	}
	NiImage *image = createFunction();
	if ( !image ) {
		error = 4;
		return 0;
	}

	image->SetRawImageData( raw );
	if ( mipmap == 1 || ( mipmap == 2 && defaultMipmap ) ) {
		image->AddFlags( 2 );
	}
	if ( unknown3 == 1 || ( unknown3 == 2 && defaultUnknown3 ) ) {
		image->AddFlags( 4 );
	}
	image->prefTexFormat = (texFormat_t)format;
	if ( !image->Unknown7E0EF0() ) {
		delete image;
		return 0;
	}
	return image;
}

/*
================
NiImage::Unknown7E0EF0
================
*/
// FUNCTION: STBC 0x7E0EF0
bool NiImage::Unknown7E0EF0() {
	NiTListIterator pos = renderers.GetHeadPos();
	while ( pos ) {
		NiRender *renderer = renderers.GetNext( pos );
		if ( !renderer->VirtualSlot53( this ) ) {
			return false;
		}
	}
	return true;
}

/*
================
NiImage::Unknown7E0F30

Reads the image's file and saves its pixels as a NIF file of the same
name, which the image then names.
================
*/
// FUNCTION: STBC 0x7E0F30
bool NiImage::Unknown7E0F30() {
	if ( !url ) {
		return false;
	}

	bool result = true;
	NiFilename filename( url );
	if ( NiStricmp( filename.GetExt(), ".nif" ) ) {
		filename.SetExt( ".nif" );
	}
	char *nifPath = new char[NiFilename::GetMaxPathLength()];
	filename.GetFullPath( nifPath );

	NiPointer<NiRawImageData> data = new NiRawImageData;
	if ( NiReadImageFile( url, data ) ) {
		NiStream stream;
		stream.InsertObject( data );
		stream.Save( nifPath );
		delete [] url;
		url = nifPath;
	} else {
		delete [] nifPath;
		result = false;
	}
	return result;
}

/*
================
NiImage::AddRenderer
================
*/
// FUNCTION: STBC 0x7E10E0
void NiImage::AddRenderer( NiRender *renderer ) {
	if ( renderer ) {
		renderers.AddTail( renderer );
	}
}

/*
================
NiImage::RemoveRenderer
================
*/
// FUNCTION: STBC 0x7E1150
void NiImage::RemoveRenderer( NiRender *renderer ) {
	NiTListIterator pos = renderers.FindPos( renderer );
	if ( pos ) {
		if ( rendererPos == pos ) {
			rendererPos = renderers.GetNextPos( rendererPos );
		}
		renderers.RemovePos( pos );
	}
}

/*
================
NiImage::Unknown7E1260

Drops every renderer.
================
*/
// FUNCTION: STBC 0x7E1260
void NiImage::Unknown7E1260() {
	while ( renderers.GetSize() ) {
		renderers.RemoveHead();
	}
	rendererPos = 0;
}

/*
================
NiImage::Unknown7E12C0

Starts a walk of the renderers.
================
*/
// FUNCTION: STBC 0x7E12C0
void NiImage::Unknown7E12C0() {
	rendererPos = renderers.GetHeadPos();
}

/*
================
NiImage::Unknown7E12D0

The next renderer of the walk, or NULL at its end.
================
*/
// FUNCTION: STBC 0x7E12D0
NiRender *NiImage::Unknown7E12D0() {
	if ( rendererPos ) {
		return renderers.GetNext( rendererPos );
	}
	return 0;
}

/*
================
NiImage::NiImage

An empty image with no file or pixels, at the default scale, and the
next serial number.
================
*/
// FUNCTION: STBC 0x7E12F0
NiImage::NiImage() : url( 0 ), raw( 0 ), width( 0 ), height( 0 ), mipLevels( 0 ), unknown20( 0 ), flags( 0 ),
	imageScale( 128.5f ) {
	unknown1C = nextSerial++;
	flags = 0;		// cleared a second time; the preferred format is left unset (both as retail)
}

/*
================
NiImage::~NiImage
================
*/
// FUNCTION: STBC 0x7E1340
// FUNCTION: STBC 0x7E1F80 ??_GNiImage@@UAEPAXI@Z
NiImage::~NiImage() {
	if ( url ) {
		delete [] url;
	}
}

/*
================
NiImage::SetURL
================
*/
// FUNCTION: STBC 0x7E13C0
void NiImage::SetURL( const char *file ) {
	if ( url ) {
		delete [] url;
	}
	url = new char[strlen( file ) + 1];
	strcpy( url, file );
	NiFile::StandardizeFilePath( &url );
}

/*
================
NiImage::SetRawImageData
================
*/
// FUNCTION: STBC 0x7E1420
void NiImage::SetRawImageData( NiRawImageData *raw_ ) {
	raw = raw_;
}

/*
================
NiImage::CopyRawImageData
================
*/
// FUNCTION: STBC 0x7E1460
NiRawImageData *NiImage::CopyRawImageData() {
	if ( raw == 0 ) {
		return 0;
	}
	NiRawImageData *copy = new NiRawImageData;
	if ( copy && !copy->Copy( raw ) ) {
		delete copy;
		copy = 0;
	}
	return copy;
}

/*
================
NiImage::Unknown7E1500
================
*/
// FUNCTION: STBC 0x7E1500
NiImage *NiImage::Unknown7E1500() {
	return new NiImage;
}

/*
================
NiImage::Unknown7E15A0
================
*/
// FUNCTION: STBC 0x7E15A0
NiRawImageData *NiImage::Unknown7E15A0( const char *file ) {
	return NiReadImageFile( file, fileImage ) ? (NiRawImageData *)fileImage : 0;
}

/*
================
NiImage::ComputeMipmapLevels

Counts the halvings of each side down to 1: 0 when a side is 0 or not a
power of two, else the smaller count.
================
*/
// FUNCTION: STBC 0x7E15C0
unsigned int NiImage::ComputeMipmapLevels( unsigned int width, unsigned int height ) {
	if ( !width || !height ) {
		return 0;
	}

	unsigned int widthLog = 0;
	while ( !( width & 1 ) ) {
		width >>= 1;
		widthLog++;
	}
	if ( width & ~1 ) {
		return 0;
	}

	unsigned int heightLog = 0;
	while ( !( height & 1 ) ) {
		height >>= 1;
		heightLog++;
	}
	if ( height & ~1 ) {
		return 0;
	}

	return heightLog < widthLog ? heightLog : widthLog;
}

/*
================
NiImage::CreateClone

Images are not cloned.
================
*/
// FUNCTION: STBC 0x7E1610
NiObject *NiImage::CreateClone() {
	assert( false );
	return 0;
}

/*
================
NiImage::CreateObject

Makes the renderer's kind of image and loads it; when the stream already
holds an image of the same file, the new one is deleted and the stream's
image takes its link ID.
================
*/
// FUNCTION: STBC 0x7E1630
void NiImage::CreateObject( NiStream &stream, NiObject *&object ) {
	if ( !createFunction ) {
		assert( createFunction );
		return;
	}
	NiImage *image = createFunction();
	assert( image );
	NiImageLoadRecord *record = new NiImageLoadRecord;
	assert( record );
	stream.SetLoadRecord( record );
	image->LoadBinary( stream );
	if ( record->sharedImage ) {
		delete image;
		image = record->sharedImage;
		stream.SetObjectFromLinkID( record->linkID, image );
	}
	object = image;
}

/*
================
NiImage::LoadBinary

Reads the file name, standardized, or the link ID of the raw pixels, then
the preferred format and, from version 3.04, the scale. The first image
of a file in the stream names itself after it; a later one only records
the first (CreateObject then uses that one).
================
*/
// FUNCTION: STBC 0x7E16B0
void NiImage::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );

	NiImageLoadRecord *record = (NiImageLoadRecord *)stream.GetLoadRecord();
	NiStreamLoadBinary( stream, record->external );
	if ( record->external ) {
		stream.LoadCString( url );
		stream.Unknown818680( url );
		record->sharedImage = stream.GetImage( url );
		if ( !record->sharedImage ) {
			stream.SetImage( url, this );
		}
	} else {
		NiStreamLoadBinary( stream, record->rawLinkID );
	}

	if ( defaultMipmap ) {
		AddFlags( 2 );		// the mipmap flag TestFlags( 2 ) reads
	}

	NiStreamLoadEnum( stream, prefTexFormat );
	if ( strcmp( stream.GetFileVersion(), "3.04" ) >= 0 ) {
		NiStreamLoadBinary( stream, imageScale );
	}
}

/*
================
NiImage::LinkObject

Takes the raw pixels of an image that names no file, then tells every
renderer of the image.
================
*/
// FUNCTION: STBC 0x7E1790
void NiImage::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );

	NiImageLoadRecord *record = (NiImageLoadRecord *)stream.GetLoadRecord();
	if ( !record->external ) {
		NiRawImageData *rawImage = (NiRawImageData *)stream.GetObjectFromLinkID( record->rawLinkID );
		SetRawImageData( rawImage );
	}
	bool bResult = Unknown7E0EF0();
	assert( bResult );
}

/*
================
NiImage::RegisterStreamables

An image that saves its pixels, or has no file, registers its raw pixels,
reading them from the file first when they are not loaded yet.
================
*/
// FUNCTION: STBC 0x7E1810
bool NiImage::RegisterStreamables( NiStream &stream ) {
	if ( !NiObject::RegisterStreamables( stream ) ) {
		return false;
	}

	if ( !saveFileNames && !raw ) {
		raw = new NiRawImageData;
		assert( raw );
		NiReadImageFile( url, raw );
	}
	if ( !saveFileNames || !url ) {
		assert( raw );
		raw->RegisterStreamables( stream );
	}
	return true;
}

/*
================
NiImage::SaveBinary

Saved under NiImage's own name whatever the renderer's image class, then
either the file name (flagged true) or the link to the raw pixels
(flagged false); an image with neither saves an empty file name.
================
*/
// FUNCTION: STBC 0x7E1900
void NiImage::SaveBinary( NiStream &stream ) {
	stream.SaveCString( "NiImage" );
	NiStreamSaveBinary( stream, this );	// the link ID: AnimOnly.exe 0x40F960 (NiImage.cpp)

	if ( saveFileNames && url ) {
		NiStreamSaveBinary( stream, true );
		stream.SaveCString( url );
	} else if ( raw ) {
		NiStreamSaveBinary( stream, false );
		NiStreamSaveBinary( stream, raw );
	} else if ( url ) {
		NiStreamSaveBinary( stream, true );
		stream.SaveCString( url );
	} else {
		assert( 0 );
		NiStreamSaveBinary( stream, true );
		stream.SaveCString( "" );
	}

	NiStreamSaveEnum( stream, prefTexFormat );
	NiStreamSaveBinary( stream, imageScale );
}

/*
================
NiImage::IsEqual

The same raw pixels, or without them the same file, and the same
preferred format and scale.
================
*/
// FUNCTION: STBC 0x7E19E0
bool NiImage::IsEqual( NiObject *object ) {
	if ( !NiObject::IsEqual( object ) ) {
		return false;
	}

	NiImage *image = (NiImage *)object;
	if ( ( raw && !image->raw ) || ( !raw && image->raw ) ) {
		return false;
	}
	if ( raw && !raw->IsEqual( image->raw ) ) {
		return false;
	}
	if ( !raw ) {
		if ( ( url && !image->url ) || ( !url && image->url ) ) {
			return false;
		}
		if ( url && strcmp( url, image->url ) ) {
			return false;
		}
	}
	if ( prefTexFormat != image->prefTexFormat ) {
		return false;
	}
	if ( imageScale != image->imageScale ) {
		return false;
	}
	return true;
}

/*
================
NiImage::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7E1AD0
void NiImage::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
	strings->Add( NiGetViewerString( "m_pURL", url ) );
	strings->Add( NiGetViewerString( "m_uiWidth", width ) );
	strings->Add( NiGetViewerString( "m_uiHeight", height ) );
	strings->Add( NiGetViewerString( "m_uiMipLevels", mipLevels ) );
	strings->Add( NiGetViewerString( "m_uiFlags", flags ) );
	strings->Add( NiGetViewerString( "m_fImageScale", imageScale ) );
	strings->Add( GetViewerString( "m_ePrefTexFormat", prefTexFormat ) );
	strings->Add( NiGetViewerString( "m_spRaw", raw ) );
}

/*
================
NiImage::GetViewerString

The texture format by name.
================
*/
// FUNCTION: STBC 0x7E1D10
char *NiImage::GetViewerString( const char *prefix, texFormat_t format ) {
	char *string = new char[strlen( prefix ) + 16];
	switch ( format ) {
	case TEX_RGB555:
		sprintf( string, "%s = TEX_RGB555", prefix );
		break;
	case TEX_RGB565:
		sprintf( string, "%s = TEX_RGB565", prefix );
		break;
	case TEX_RGB888:
		sprintf( string, "%s = TEX_RGB888", prefix );
		break;
	case TEX_RGBA5551:
		sprintf( string, "%s = TEX_RGBA5551", prefix );
		break;
	case TEX_RGBA4444:
		sprintf( string, "%s = TEX_RGBA4444", prefix );
		break;
	case TEX_RGBA8888:
		sprintf( string, "%s = TEX_RGBA8888", prefix );
		break;
	case TEX_PAL8:
		sprintf( string, "%s = TEX_PAL8", prefix );
		break;
	case TEX_DEFAULT:
		sprintf( string, "%s = TEX_DEFAULT", prefix );
		break;
	}
	return string;
}
