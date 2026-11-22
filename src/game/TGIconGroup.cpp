/*
===============================================================================

	TGIconGroup.cpp

===============================================================================
*/

#include "TGIconGroup.h"
#include "TGIconManager.h"
#include "TGStream.h"
#include "PyEmbed.h"
#include "NiMain.h"
#include "TGApplication.h"
#include <iostream>

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0072b1e0 _$E420
// FUNCTION: STBC 0x0072b1f0 _$E417
// FUNCTION: STBC 0x0072b200 _$E419
// FUNCTION: STBC 0x0072b210 _$E418
// FUNCTION: STBC 0x0072b220 _$E426
// FUNCTION: STBC 0x0072b230 _$E423
// FUNCTION: STBC 0x0072b240 _$E425
// FUNCTION: STBC 0x0072b250 _$E424

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0072b260 _$E21
// FUNCTION: STBC 0x0072b270 _$E20
// FUNCTION: STBC 0x0072b280 _$E77
// FUNCTION: STBC 0x0072b290 _$E76
// FUNCTION: STBC 0x0072b2a0 _$E32
// FUNCTION: STBC 0x0072b2b0 _$E31
// FUNCTION: STBC 0x0072b2c0 _$E36
// FUNCTION: STBC 0x0072b2d0 _$E35
// FUNCTION: STBC 0x0072b2e0 _$E40
// FUNCTION: STBC 0x0072b2f0 _$E39
// FUNCTION: STBC 0x0072b300 _$E44
// FUNCTION: STBC 0x0072b310 _$E43
// FUNCTION: STBC 0x0072b320 _$E176
// FUNCTION: STBC 0x0072b330 _$E175
// FUNCTION: STBC 0x0072b340 _$E51
// FUNCTION: STBC 0x0072b350 _$E50
// FUNCTION: STBC 0x0072b360 _$E64
// FUNCTION: STBC 0x0072b370 _$E63
// FUNCTION: STBC 0x0072b3a0 _$E186
// FUNCTION: STBC 0x0072b3b0 _$E185
// FUNCTION: STBC 0x0072b3e0 _$E194
// FUNCTION: STBC 0x0072b3f0 _$E193
// FUNCTION: STBC 0x0072b400 _$E201
// FUNCTION: STBC 0x0072b410 _$E200
// FUNCTION: STBC 0x0072b460 _$E101
// FUNCTION: STBC 0x0072b470 _$E100
// FUNCTION: STBC 0x0072b680 _$E106
// FUNCTION: STBC 0x0072b690 _$E105
// FUNCTION: STBC 0x0072b6c0 _$E111
// FUNCTION: STBC 0x0072b6d0 _$E110
// FUNCTION: STBC 0x0072b760 _$E120
// FUNCTION: STBC 0x0072b770 _$E119
// FUNCTION: STBC 0x0072b7e0 _$E126
// FUNCTION: STBC 0x0072b7f0 _$E125
// FUNCTION: STBC 0x0072b800 _$E131
// FUNCTION: STBC 0x0072b810 _$E130
// FUNCTION: STBC 0x0072b820 _$E141
// FUNCTION: STBC 0x0072b830 _$E140
// FUNCTION: STBC 0x0072b860 _$E146
// FUNCTION: STBC 0x0072b870 _$E145
// FUNCTION: STBC 0x0072b8c0 _$E151
// FUNCTION: STBC 0x0072b8d0 _$E150
// FUNCTION: STBC 0x0072b900 _$E136
// FUNCTION: STBC 0x0072b910 _$E135
// FUNCTION: STBC 0x0072b960 _$E69
// FUNCTION: STBC 0x0072b970 _$E68
// FUNCTION: STBC 0x0072b980 _$E156
// FUNCTION: STBC 0x0072b990 _$E155
// FUNCTION: STBC 0x0072baa0 _$E161
// FUNCTION: STBC 0x0072bab0 _$E160
// FUNCTION: STBC 0x0072bae0 _$E166
// FUNCTION: STBC 0x0072baf0 _$E165
// FUNCTION: STBC 0x0072bb00 _$E171
// FUNCTION: STBC 0x0072bb10 _$E170
// FUNCTION: STBC 0x0072b380 _$E181
// FUNCTION: STBC 0x0072b390 _$E180
// FUNCTION: STBC 0x0072b3c0 _$E190
// FUNCTION: STBC 0x0072b3d0 _$E189
// FUNCTION: STBC 0x0072b420 _$E243
// FUNCTION: STBC 0x0072b430 _$E242
// FUNCTION: STBC 0x0072b440 _$E247
// FUNCTION: STBC 0x0072b450 _$E246
// FUNCTION: STBC 0x0072b480 _$E229
// FUNCTION: STBC 0x0072b490 _$E228
// FUNCTION: STBC 0x0072b4a0 _$E236
// FUNCTION: STBC 0x0072b4b0 _$E235
// FUNCTION: STBC 0x0072b4c0 _$E259
// FUNCTION: STBC 0x0072b4d0 _$E258
// FUNCTION: STBC 0x0072b4e0 _$E263
// FUNCTION: STBC 0x0072b4f0 _$E262
// FUNCTION: STBC 0x0072b500 _$E303
// FUNCTION: STBC 0x0072b510 _$E302
// FUNCTION: STBC 0x0072b520 _$E309
// FUNCTION: STBC 0x0072b530 _$E308
// FUNCTION: STBC 0x0072b540 _$E251
// FUNCTION: STBC 0x0072b550 _$E250
// FUNCTION: STBC 0x0072b560 _$E255
// FUNCTION: STBC 0x0072b570 _$E254
// FUNCTION: STBC 0x0072b580 _$E267
// FUNCTION: STBC 0x0072b590 _$E266
// FUNCTION: STBC 0x0072b5a0 _$E271
// FUNCTION: STBC 0x0072b5b0 _$E270
// FUNCTION: STBC 0x0072b5c0 _$E275
// FUNCTION: STBC 0x0072b5d0 _$E274
// FUNCTION: STBC 0x0072b5e0 _$E279
// FUNCTION: STBC 0x0072b5f0 _$E278
// FUNCTION: STBC 0x0072b600 _$E283
// FUNCTION: STBC 0x0072b610 _$E282
// FUNCTION: STBC 0x0072b620 _$E287
// FUNCTION: STBC 0x0072b630 _$E286
// FUNCTION: STBC 0x0072b640 _$E291
// FUNCTION: STBC 0x0072b650 _$E290
// FUNCTION: STBC 0x0072b660 _$E295
// FUNCTION: STBC 0x0072b670 _$E294
// FUNCTION: STBC 0x0072b6a0 _$E299
// FUNCTION: STBC 0x0072b6b0 _$E298
// FUNCTION: STBC 0x0072b6e0 _$E313
// FUNCTION: STBC 0x0072b6f0 _$E312
// FUNCTION: STBC 0x0072b700 _$E317
// FUNCTION: STBC 0x0072b710 _$E316
// FUNCTION: STBC 0x0072b720 _$E321
// FUNCTION: STBC 0x0072b730 _$E320
// FUNCTION: STBC 0x0072b740 _$E325
// FUNCTION: STBC 0x0072b750 _$E324
// FUNCTION: STBC 0x0072b780 _$E329
// FUNCTION: STBC 0x0072b790 _$E328
// FUNCTION: STBC 0x0072b7a0 _$E333
// FUNCTION: STBC 0x0072b7b0 _$E332
// FUNCTION: STBC 0x0072b7c0 _$E337
// FUNCTION: STBC 0x0072b7d0 _$E336
// FUNCTION: STBC 0x0072b840 _$E341
// FUNCTION: STBC 0x0072b850 _$E340
// FUNCTION: STBC 0x0072b880 _$E345
// FUNCTION: STBC 0x0072b890 _$E344
// FUNCTION: STBC 0x0072b8a0 _$E349
// FUNCTION: STBC 0x0072b8b0 _$E348
// FUNCTION: STBC 0x0072b8e0 _$E353
// FUNCTION: STBC 0x0072b8f0 _$E352
// FUNCTION: STBC 0x0072b920 _$E357
// FUNCTION: STBC 0x0072b930 _$E356
// FUNCTION: STBC 0x0072b940 _$E361
// FUNCTION: STBC 0x0072b950 _$E360
// FUNCTION: STBC 0x0072b9a0 _$E365
// FUNCTION: STBC 0x0072b9b0 _$E364
// FUNCTION: STBC 0x0072b9c0 _$E369
// FUNCTION: STBC 0x0072b9d0 _$E368
// FUNCTION: STBC 0x0072b9e0 _$E373
// FUNCTION: STBC 0x0072b9f0 _$E372
// FUNCTION: STBC 0x0072ba00 _$E377
// FUNCTION: STBC 0x0072ba10 _$E376
// FUNCTION: STBC 0x0072ba20 _$E381
// FUNCTION: STBC 0x0072ba30 _$E380
// FUNCTION: STBC 0x0072ba40 _$E385
// FUNCTION: STBC 0x0072ba50 _$E384
// FUNCTION: STBC 0x0072ba60 _$E389
// FUNCTION: STBC 0x0072ba70 _$E388
// FUNCTION: STBC 0x0072ba80 _$E393
// FUNCTION: STBC 0x0072ba90 _$E392
// FUNCTION: STBC 0x0072bac0 _$E397
// FUNCTION: STBC 0x0072bad0 _$E396

/*
================
TGIconGroup::TGIconGroup

An empty group of the name, with room for fifteen textures.
================
*/
// FUNCTION: STBC 0x0072bb20
// DRAFT: the inlined SetSize( 15 ) keeps the size in edx here, ebx in retail (one dec through memory there)
TGIconGroup::TGIconGroup( char *name_ ) {
	name = NULL;
	refCount = 0;
	unknown1C.SetSize( 15 );
	unknown1C.SetGrowBy( 10 );
	SetName( name_ );
}

/*
================
TGIconGroup::~TGIconGroup

Deletes the name and empties the group.
================
*/
// FUNCTION: STBC 0x0072bce0
TGIconGroup::~TGIconGroup( void ) {
	refCount = 0;
	if ( name != NULL ) {
		delete [] name;
	}
	Reset();
}

/*
================
TGIconGroup::Reset

Drops the textures and deletes the icons.
================
*/
// FUNCTION: STBC 0x0072bde0
void TGIconGroup::Reset( void ) {
	unsigned int count = unknown1C.GetSize();
	for ( unsigned int i = 0; i < count; i++ ) {
		unknown1C.RemoveAt( i );
	}
	unknown1C.RemoveAll();

	NiTMapIterator pos = icons.GetFirstPos();
	while ( pos ) {
		int iconNum;
		TGIconLocation *location;
		icons.GetNext( pos, iconNum, location );
		delete location;
	}
	icons.RemoveAll();
}

/*
================
TGIconGroup::DecRefCount
================
*/
// FUNCTION: STBC 0x0072bf60
void TGIconGroup::DecRefCount( void ) {
	refCount--;
}

/*
================
TGIconGroup::SetName

The group keeps a copy of the name.
================
*/
// FUNCTION: STBC 0x0072bf70
void TGIconGroup::SetName( char *name_ ) {
	if ( name != NULL ) {
		delete [] name;
		name = NULL;
	}
	if ( name_ != NULL ) {
		name = new char[strlen( name_ ) + 1];
		strcpy( name, name_ );
	}
}

/*
================
TGIconGroup::LoadIconTexture

Adds a texture of the image file (under the working directory unless the
path has a drive), loading the image once for every group; the texture's
number, or -1 when the image is missing.
================
*/
// FUNCTION: STBC 0x0072bfe0
int TGIconGroup::LoadIconTexture( char *fileName, bool mipmap ) {
	NiPointer<NiTextureProperty> texture;
	texture = new NiTextureProperty;
	int textureNum = -1;
	char path[_MAX_PATH];
	if ( fileName[1] != ':' ) {
		strcpy( path, workingDirectory );
		strcat( path, fileName );
	} else {
		strcpy( path, fileName );
	}
	NiPointer<NiImage> image;
	if ( !g_kIconManager.GetImages()->GetAt( path, image ) ) {
		image = NiImage::Create( path, mipmap, 7, 2 );
		g_kIconManager.GetImages()->SetAt( path, image );
	}
	texture->SetImage( image );
	if ( texture->GetImage() != NULL ) {
		unknown1C.Add( texture );
		textureNum = unknown1C.GetSize() - 1;
	}
	return textureNum;
}

/*
================
TGIconGroup::SetIconLocation

Files icon iconNum as the rectangle of texture textureNum (in pixels),
turned and mirrored; nothing for texture -1.
================
*/
// FUNCTION: STBC 0x0072c260 ?SetIconLocation@TGIconGroup@@UAEXHJMMMMHH@Z
void TGIconGroup::SetIconLocation( int iconNum, long textureNum, float x, float y, float width, float height, int rotation, int mirror ) {
	if ( textureNum == -1 ) {
		return;
	}
	TGIconLocation *location = new TGIconLocation;
	location->texture = unknown1C.GetAt( textureNum );
	location->pixelWidth = width;
	location->pixelHeight = height;

	NiImage *image = location->texture->GetImage();
	float imageWidth = (float)image->GetWidth();
	float imageHeight = (float)image->GetHeight();
	float bottom = y + height;
	float right = x + width;
	location->corners[0][0] = x / imageWidth;
	location->corners[0][1] = y / imageHeight;
	location->corners[1][0] = x / imageWidth;
	location->corners[1][1] = bottom / imageHeight;
	location->corners[2][0] = right / imageWidth;
	location->corners[2][1] = bottom / imageHeight;
	location->corners[3][0] = right / imageWidth;
	location->corners[3][1] = y / imageHeight;
	icons.SetAt( iconNum, location );

	if ( rotation != 0 ) {
		Unknown72C530( iconNum, rotation );
	}
	if ( mirror != 0 ) {
		Unknown72C690( iconNum, mirror );
	}
}

/*
================
TGIconLocation::WriteToStream

The coordinates of the four corners, then the pixel size.
================
*/
// FUNCTION: STBC 0x0072c480 ?WriteToStream@TGIconLocation@@UAE_NPAVTGStream@@@Z
bool TGIconLocation::WriteToStream( TGStream *stream ) {
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 2; j++ ) {
			stream->WriteFloat( corners[i][j] );
		}
	}
	stream->WriteFloat( pixelWidth );
	stream->WriteFloat( pixelHeight );
	return true;
}

/*
================
TGIconLocation::ReadFromStream

Reads what WriteToStream wrote.
================
*/
// FUNCTION: STBC 0x0072c4e0 ?ReadFromStream@TGIconLocation@@UAE_NPAVTGStream@@@Z
bool TGIconLocation::ReadFromStream( TGStream *stream ) {
	for ( int i = 0; i < 4; i++ ) {
		for ( int j = 0; j < 2; j++ ) {
			corners[i][j] = stream->ReadFloat();
		}
	}
	pixelWidth = stream->ReadFloat();
	pixelHeight = stream->ReadFloat();
	return true;
}

/*
================
TGIconGroup::Unknown72C530

The corners run top left, bottom left, bottom right, top right; each step
moves every corner on by one.
================
*/
// FUNCTION: STBC 0x0072c530
void TGIconGroup::Unknown72C530( int iconNum, int rotation ) {
	TGIconLocation *location = NULL;
	icons.GetAt( iconNum, location );
	float corners[4][2];
	for ( int i = 0; i < 4; i++ ) {
		corners[i][0] = location->corners[i][0];
		corners[i][1] = location->corners[i][1];
	}
	float pixelWidth = location->pixelWidth;
	float pixelHeight = location->pixelHeight;
	switch ( rotation ) {
		case 1:
			location->corners[0][0] = corners[1][0];
			location->corners[0][1] = corners[1][1];
			location->corners[1][0] = corners[2][0];
			location->corners[1][1] = corners[2][1];
			location->corners[2][0] = corners[3][0];
			location->corners[2][1] = corners[3][1];
			location->corners[3][0] = corners[0][0];
			location->corners[3][1] = corners[0][1];
			location->pixelWidth = pixelHeight;
			location->pixelHeight = pixelWidth;
			break;
		case 2:
			location->corners[0][0] = corners[2][0];
			location->corners[0][1] = corners[2][1];
			location->corners[1][0] = corners[3][0];
			location->corners[1][1] = corners[3][1];
			location->corners[2][0] = corners[0][0];
			location->corners[2][1] = corners[0][1];
			location->corners[3][0] = corners[1][0];
			location->corners[3][1] = corners[1][1];
			break;
		case 3:
			location->corners[0][0] = corners[3][0];
			location->corners[0][1] = corners[3][1];
			location->corners[1][0] = corners[0][0];
			location->corners[1][1] = corners[0][1];
			location->corners[2][0] = corners[1][0];
			location->corners[2][1] = corners[1][1];
			location->corners[3][0] = corners[2][0];
			location->corners[3][1] = corners[2][1];
			location->pixelWidth = pixelHeight;
			location->pixelHeight = pixelWidth;
			break;
	}
}

/*
================
TGIconGroup::Unknown72C690
================
*/
// FUNCTION: STBC 0x0072c690
void TGIconGroup::Unknown72C690( int iconNum, int mirror ) {
	TGIconLocation *location = NULL;
	icons.GetAt( iconNum, location );
	float corners[4][2];
	for ( int i = 0; i < 4; i++ ) {
		corners[i][0] = location->corners[i][0];
		corners[i][1] = location->corners[i][1];
	}
	if ( mirror == 0 ) {
		return;
	}
	if ( mirror == 1 ) {
		location->corners[0][0] = corners[3][0];
		location->corners[0][1] = corners[3][1];
		location->corners[1][0] = corners[2][0];
		location->corners[1][1] = corners[2][1];
		location->corners[2][0] = corners[1][0];
		location->corners[2][1] = corners[1][1];
		location->corners[3][0] = corners[0][0];
		location->corners[3][1] = corners[0][1];
	} else {
		location->corners[0][0] = corners[1][0];
		location->corners[0][1] = corners[1][1];
		location->corners[1][0] = corners[0][0];
		location->corners[1][1] = corners[0][1];
		location->corners[2][0] = corners[3][0];
		location->corners[2][1] = corners[3][1];
		location->corners[3][0] = corners[2][0];
		location->corners[3][1] = corners[2][1];
	}
}

/*
================
TGIconGroup::GetIconTexture

The texture the icon lies in; NULL for an icon the group lacks.
================
*/
// FUNCTION: STBC 0x0072c780
NiTextureProperty *TGIconGroup::GetIconTexture( int iconNum ) {
	TGIconLocation *location;
	if ( !icons.GetAt( iconNum, location ) ) {
		return NULL;
	}
	if ( location == NULL ) {
		return NULL;
	}
	return location->texture;
}

/*
================
TGIconGroup::GetIconPixelWidth

The icon's width in the texture's pixels; 0 for an icon the group lacks.
================
*/
// FUNCTION: STBC 0x0072c7e0 ?GetIconPixelWidth@TGIconGroup@@QAEMH@Z
float TGIconGroup::GetIconPixelWidth( int iconNum ) {
	TGIconLocation *location;
	if ( !icons.GetAt( iconNum, location ) ) {
		return 0.0f;
	}
	if ( location == NULL ) {
		return 0.0f;
	}
	return location->pixelWidth;
}

/*
================
TGIconGroup::GetIconPixelHeight
================
*/
// FUNCTION: STBC 0x0072c840 ?GetIconPixelHeight@TGIconGroup@@QAEMH@Z
float TGIconGroup::GetIconPixelHeight( int iconNum ) {
	TGIconLocation *location;
	if ( !icons.GetAt( iconNum, location ) ) {
		return 0.0f;
	}
	if ( location == NULL ) {
		return 0.0f;
	}
	return location->pixelHeight;
}

/*
================
TGIconGroup::GetIconScreenWidth

The icon's width as a fraction of the screen: its pixels at the display's
scale, over the screen's width.
================
*/
// FUNCTION: STBC 0x0072c8a0 ?GetIconScreenWidth@TGIconGroup@@QAEMH@Z
// PARKED: bytes match except the divide, which retail schedules before the last pop of the epilogue; operand order, a local for the size and a combined form tried
float TGIconGroup::GetIconScreenWidth( int iconNum ) {
	TGIconLocation *location;
	if ( !icons.GetAt( iconNum, location ) ) {
		return 0.0f;
	}
	if ( location == NULL ) {
		return 0.0f;
	}
	return g_kIconManager.GetScale() * location->pixelWidth / g_kIconManager.GetScreenWidth();
}

/*
================
TGIconGroup::GetIconScreenHeight
================
*/
// FUNCTION: STBC 0x0072c910 ?GetIconScreenHeight@TGIconGroup@@QAEMH@Z
// PARKED: bytes match except the divide, which retail schedules before the last pop of the epilogue; operand order, a local for the size and a combined form tried
float TGIconGroup::GetIconScreenHeight( int iconNum ) {
	TGIconLocation *location;
	if ( !icons.GetAt( iconNum, location ) ) {
		return 0.0f;
	}
	if ( location == NULL ) {
		return 0.0f;
	}
	return g_kIconManager.GetScale() * location->pixelHeight / g_kIconManager.GetScreenHeight();
}

/*
================
TGIconGroup::GetPyWrapper
================
*/
// FUNCTION: STBC 0x0072c980 ?GetPyWrapper@TGIconGroup@@UAEPAU_object@@XZ
_object *TGIconGroup::GetPyWrapper( void ) {
	return PyEmbed::Unknown6F9920( this, "TGIconGroup" );
}

/*
================
TGIconGroup::Unknown72C990
================
*/
// FUNCTION: STBC 0x0072c990
// MATCH-PENDING: R17, exact only through the invented TGIconLocation::operator= (block copy of the corners: retail's rep movsd rules out the implicit copy, but not which original form)
void TGIconGroup::Unknown72C990( TGIconGroup *group ) {
	group->SetName( name );
	group->refCount = 0;
	NiTMapIterator pos = icons.GetFirstPos();
	while ( pos ) {
		TGIconLocation *copy = new TGIconLocation;
		int iconNum;
		TGIconLocation *location;
		icons.GetNext( pos, iconNum, location );
		*copy = *location;
		group->icons.SetAt( iconNum, copy );
	}
	unsigned int count = unknown1C.GetSize();
	for ( unsigned int i = 0; i < count; i++ ) {
		group->unknown1C.Add( unknown1C.GetAt( i ) );
	}
}

/*
================
TGIconGroup::WriteToStream

The name, the reference count, the number of icons, then each icon behind a
flag with its number, and the size of the array.
================
*/
// FUNCTION: STBC 0x0072cb70 ?WriteToStream@TGIconGroup@@UAE_NPAVTGStream@@@Z
bool TGIconGroup::WriteToStream( TGStream *stream ) {
	stream->VirtualSlot11( name );
	stream->WriteLong( refCount );
	stream->WriteLong( icons.GetCount() );

	NiTMapIterator pos = icons.GetFirstPos();
	while ( pos ) {
		int iconNum;
		TGIconLocation *location;
		icons.GetNext( pos, iconNum, location );
		if ( location != NULL ) {
			stream->WriteBool( true );
			stream->WriteLong( iconNum );
			location->WriteToStream( stream );
		} else {
			stream->WriteBool( false );
		}
	}
	stream->WriteLong( unknown1C.GetSize() );
	return true;
}

/*
================
TGIconGroup::ReadFromStream

Reads what WriteToStream wrote; the size of the array is read and dropped.
================
*/
// FUNCTION: STBC 0x0072cc50 ?ReadFromStream@TGIconGroup@@UAE_NPAVTGStream@@@Z
bool TGIconGroup::ReadFromStream( TGStream *stream ) {
	name = stream->VirtualSlot10( -1 );
	refCount = stream->ReadLong();
	int count = stream->ReadLong();
	for ( int i = 0; i < count; i++ ) {
		if ( stream->ReadBool() == true ) {
			TGIconLocation *location = new TGIconLocation;
			int iconNum = stream->ReadLong();
			location->ReadFromStream( stream );
			icons.SetAt( iconNum, location );
		}
	}
	stream->ReadLong();
	return true;
}

/*
================
TGIconGroup::IsNumValid
================
*/
// FUNCTION: STBC 0x0072cd80 ?IsNumValid@TGIconGroup@@QAE_NH@Z
bool TGIconGroup::IsNumValid( int iconNum ) {
	TGIconLocation *location;
	return icons.GetAt( iconNum, location );
}

// DATA: STBC 0x0099d48c
NiTMapIterator TGIconGroup::iconPos;

/*
================
TGIconGroup::GoToFirstPos

Starts a walk over the icon numbers.
================
*/
// FUNCTION: STBC 0x0072cdd0
void TGIconGroup::GoToFirstPos( void ) {
	iconPos = icons.GetFirstPos();
}

/*
================
TGIconGroup::GetNext

The number of the icon at the walk's position, which moves on; the walk
must have been started on a group with an icon.
================
*/
// FUNCTION: STBC 0x0072ce00
int TGIconGroup::GetNext( void ) {
	int number;
	TGIconLocation *location;
	icons.GetNext( iconPos, number, location );
	return number;
}

// The standard library's locale facet id (<iostream>): a test-and-set initialiser and its exit-time destructor.
// FUNCTION: STBC 0x0072d1c0 _$E436
// FUNCTION: STBC 0x0072d1e0 _$E435
