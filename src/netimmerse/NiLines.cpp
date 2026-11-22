// NiLines.cpp -- line geometry objects

#include "NiRTLib.h"
#include <assert.h>
#include "NiCamera.h"
#include "NiStream.h"
#include "NiLines.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007fef80 _$E30
// FUNCTION: STBC 0x007fef90 _$E45
// FUNCTION: STBC 0x007fefa0 _$E49
// FUNCTION: STBC 0x007fefb0 _$E53
// FUNCTION: STBC 0x007fefc0 _$E57
// FUNCTION: STBC 0x007fefd0 _$E181
// FUNCTION: STBC 0x007fefe0 _$E34
// FUNCTION: STBC 0x007feff0 _$E123
// FUNCTION: STBC 0x007ff000 _$E136
// FUNCTION: STBC 0x007ff010 _$E186
// FUNCTION: STBC 0x007ff020 _$E191
// FUNCTION: STBC 0x007ff030 _$E195
// FUNCTION: STBC 0x007ff040 _$E199
// FUNCTION: STBC 0x007ff050 _$E206
// FUNCTION: STBC 0x007ff060 _$E214
// FUNCTION: STBC 0x007ff070 _$E221
// FUNCTION: STBC 0x007ff080 _$E225
// FUNCTION: STBC 0x007ff0d0 _$E229

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007fef60 _$E242
// FUNCTION: STBC 0x007fef00 _$E17
// FUNCTION: STBC 0x007fef20 _$E15
// FUNCTION: STBC 0x007fef30 _$E23
// FUNCTION: STBC 0x007fef50 _$E21

// FUNCTION: STBC 0x007fe950 _$E235
// FUNCTION: STBC 0x007fe970 _$E232
// DATA: STBC 0x9A2130 ?rtti@NiLines@@2VNiRTTI@@B
const NiRTTI NiLines::rtti( "NiLines", &NiGeometry::rtti );

/*
================
NiLines::Register
================
*/
// FUNCTION: STBC 0x7FE990 ?Register@NiLines@@SAHXZ
NiImplementRegister( NiLines )

/*
================
NiLines::NiLines

Creates the data, which takes ownership of the arrays.
================
*/
// FUNCTION: STBC 0x7FEAE0 ??0NiLines@@QAE@GPAVNiPoint3@@PAVNiColorA@@PAVNiPoint2@@PA_N@Z
NiLines::NiLines( unsigned short vertexCount_, NiPoint3 *vertices_, NiColorA *colors_, NiPoint2 *textures_, bool *flags_ )
	: NiGeometry( new NiLinesData( vertexCount_, vertices_, colors_, textures_, flags_ ) ) {
}

/*
================
NiLines::NiLines
================
*/
// FUNCTION: STBC 0x7FEB70 ??0NiLines@@QAE@PAVNiLinesData@@@Z
NiLines::NiLines( NiLinesData *data ) : NiGeometry( data ) {
}

/*
================
NiLines::NiLines
================
*/
// FUNCTION: STBC 0x7FEB90 ??0NiLines@@QAE@XZ
NiLines::NiLines() {
}

/*
================
NiLines::Display

Draws the lines unless the camera's sorter takes them.
================
*/
// FUNCTION: STBC 0x7FEBB0
void NiLines::Display( NiCamera *camera ) {
	NiRender *renderer = camera->GetRenderer();
	assert( renderer );
	if ( !renderer->RegisterObject( this ) ) {
		camera->UpdateActivePlanes();
		renderer->VirtualSlot42( worldTranslate, worldRotate, worldScale );
		renderer->VirtualSlot50( NiSmartPointerCast( NiLinesData, modelData ), GetWorldBound() );
	}
}

/*
================
NiLines::CreateClone
================
*/
// FUNCTION: STBC 0x7FEC10
NiObject *NiLines::CreateClone() {
	NiLines *clone = new NiLines;
	CopyMembers( clone );
	return clone;
}

/*
================
NiLines::CopyMembers
================
*/
// FUNCTION: STBC 0x7FEC80
void NiLines::CopyMembers( NiLines *dest ) {
	NiGeometry::CopyMembers( dest );
}

/*
================
NiLines::CreateObject

Creates the lines and loads them.
================
*/
// FUNCTION: STBC 0x7FEC90
void NiLines::CreateObject( NiStream &stream, NiObject *&object ) {
	NiLines *lines = new NiLines;
	stream.SetLoadRecord( new NiLinesLoadRecord );
	lines->LoadBinary( stream );
	object = lines;
}

/*
================
NiLines::LoadBinary
================
*/
// FUNCTION: STBC 0x7FEDB0
void NiLines::LoadBinary( NiStream &stream ) {
	NiGeometry::LoadBinary( stream );
}

/*
================
NiLines::LinkObject
================
*/
// FUNCTION: STBC 0x7FEDC0
void NiLines::LinkObject( NiStream &stream ) {
	NiGeometry::LinkObject( stream );
}

/*
================
NiLines::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7FEDD0
bool NiLines::RegisterStreamables( NiStream &stream ) {
	return NiGeometry::RegisterStreamables( stream );
}

/*
================
NiLines::SaveBinary
================
*/
// FUNCTION: STBC 0x7FEDE0
void NiLines::SaveBinary( NiStream &stream ) {
	NiGeometry::SaveBinary( stream );
}

/*
================
NiLines::IsEqual
================
*/
// FUNCTION: STBC 0x7FEDF0
bool NiLines::IsEqual( NiObject *object ) {
	return NiGeometry::IsEqual( object );
}

/*
================
NiLines::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7FEE00
void NiLines::GetViewerStrings( NiTArray<char *> *strings ) {
	NiGeometry::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}
