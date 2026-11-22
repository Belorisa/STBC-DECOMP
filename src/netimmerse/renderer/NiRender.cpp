// NiRender.cpp -- the abstract renderer

#include "NiRTLib.h"
#include "NiRender.h"
#include "NiStream.h"
#include "NiImage.h"
#include "NiAVObject.h"
#include "NiSortedObject.h"

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x007e3480 _$E30
// FUNCTION: STBC 0x007e3490 _$E34
// FUNCTION: STBC 0x007e34a0 _$E65
// FUNCTION: STBC 0x007e34b0 _$E69
// FUNCTION: STBC 0x007e34c0 _$E73
// FUNCTION: STBC 0x007e34d0 _$E77
// FUNCTION: STBC 0x007e34e0 _$E181
// FUNCTION: STBC 0x007e34f0 _$E48
// FUNCTION: STBC 0x007e3500 _$E61
// FUNCTION: STBC 0x007e3510 _$E186
// FUNCTION: STBC 0x007e35d0 _$E191
// FUNCTION: STBC 0x007e3620 _$E210

// The <iostream> quads (ios_base::Init, _Winit) and the locale-id guard.
// FUNCTION: STBC 0x007e3460 _$E243
// FUNCTION: STBC 0x007e3340 _$E17
// FUNCTION: STBC 0x007e3360 _$E15
// FUNCTION: STBC 0x007e3370 _$E23
// FUNCTION: STBC 0x007e3390 _$E21

// FUNCTION: STBC 0x007e2a00 _$E230
// FUNCTION: STBC 0x007e2a20 _$E227
// DATA: STBC 0x9A1720 ?rtti@NiRender@@2VNiRTTI@@B
const NiRTTI NiRender::rtti( "NiRender", &NiObject::rtti );

/*
================
NiRender::Register
================
*/
// FUNCTION: STBC 0x7E2A40 ?Register@NiRender@@SAHXZ
NiImplementRegister( NiRender )

// Defined after Register: its initialisers follow Register in retail (0x7E2B90),
// and its exit guard is bit 1 of a byte of its own (0x9A16ED).
// FUNCTION: STBC 0x007e2b90 _$E236
// FUNCTION: STBC 0x007e2bd0 _$E233
// DATA: STBC 0x9A16F0 ?renderers@NiRender@@2V?$NiTList@PAVNiRender@@@@A
NiTList<NiRender *> NiRender::renderers( 2 );

/*
================
NiRender::NiRender

Adds the renderer to the list of live ones.
================
*/
// FUNCTION: STBC 0x7E2C40
// PARKED: instructions match but for the operand order of the address that clears the last list item's next link in the inlined NiTListBlock constructor ([ebx+ecx-8] in retail, [ecx+ebx-8] here); no spelling of the store changed it
NiRender::NiRender() {
	sorter = NULL;
	renderers.AddTail( this );
}

/*
================
NiRender::~NiRender

Drops the renderer from the images and from the list of live ones.
================
*/
// FUNCTION: STBC 0x7E2DA0
NiRender::~NiRender() {
	NiImage::RemoveRenderer( this );
	renderers.Remove( this );
}

/*
================
NiRender::RegisterObject
================
*/
// FUNCTION: STBC 0x7E2EB0
bool NiRender::RegisterObject( NiAVObject *geometry ) {
	if ( sorter && sorter->RegisterObject( geometry ) ) {
		return true;
	}
	return false;
}

/*
================
NiRender::VirtualSlot38
================
*/
// FUNCTION: STBC 0x7E2ED0
void NiRender::VirtualSlot38( void *unknown ) {
}

/*
================
NiRender::VirtualSlot39
================
*/
// FUNCTION: STBC 0x7E2EE0
void NiRender::VirtualSlot39() {
}

/*
================
NiRender::RemoveEffect
================
*/
// FUNCTION: STBC 0x7E2EF0
void NiRender::RemoveEffect( NiDynamicEffect *effect ) {
	NiTListIterator pos = renderers.GetHeadPos();
	NiRender *render;
	while ( ( render = renderers.GetNext( pos ) ) != 0 ) {
		render->PurgeEffect( effect );
	}
}

/*
================
NiRender::RemoveGeometryData
================
*/
// FUNCTION: STBC 0x7E2F20
void NiRender::RemoveGeometryData( NiGeometryData *data ) {
	NiTListIterator pos = renderers.GetHeadPos();
	NiRender *render;
	while ( ( render = renderers.GetNext( pos ) ) != 0 ) {
		render->PurgeGeometryData( data );
	}
}

/*
================
NiRender::MapWindowPointToBufferPoint
================
*/
// FUNCTION: STBC 0x7E2F50
bool NiRender::MapWindowPointToBufferPoint( unsigned int x, unsigned int y, float &bufferX, float &bufferY ) {
	unsigned int width = VirtualSlot19();
	unsigned int height = VirtualSlot20();
	if ( width && height ) {
		bufferX = (float)x / (float)width;
		bufferY = 1.0f - (float)y / (float)height;
		return true;
	}
	return false;
}

/*
================
NiRender::Unknown7E2FD0

Draws a sorted object with its geometry's own properties and lights and no
sorter, then puts back the renderer's.
================
*/
// FUNCTION: STBC 0x7E2FD0
void NiRender::Unknown7E2FD0( NiCamera *camera, NiSortedObject *object ) {
	NiPointer<NiAccumulator> savedSorter = sorter;
	sorter = 0;
	NiPointer<NiDynamicEffectState> savedEffects = GetEffectState();
	SetEffectState( object->geometry->GetEffectState() );
	NiPointer<NiPropertyState> savedProperties = GetPropertyState();
	SetPropertyState( object->geometry->GetPropertyState() );

	object->Display( camera );

	SetPropertyState( savedProperties );
	SetEffectState( savedEffects );
	sorter = savedSorter;
}

/*
================
NiRender::CreateObject
================
*/
// FUNCTION: STBC 0x7E31B0
void NiRender::CreateObject( NiStream &stream, NiObject *&object ) {
}

/*
================
NiRender::LoadBinary
================
*/
// FUNCTION: STBC 0x7E31C0
void NiRender::LoadBinary( NiStream &stream ) {
	NiObject::LoadBinary( stream );
}

/*
================
NiRender::LinkObject
================
*/
// FUNCTION: STBC 0x7E31D0
void NiRender::LinkObject( NiStream &stream ) {
	NiObject::LinkObject( stream );
}

/*
================
NiRender::RegisterStreamables
================
*/
// FUNCTION: STBC 0x7E31E0
bool NiRender::RegisterStreamables( NiStream &stream ) {
	return NiObject::RegisterStreamables( stream );
}

/*
================
NiRender::SaveBinary
================
*/
// FUNCTION: STBC 0x7E31F0
void NiRender::SaveBinary( NiStream &stream ) {
	NiObject::SaveBinary( stream );
}

/*
================
NiRender::IsEqual
================
*/
// FUNCTION: STBC 0x7E3200
bool NiRender::IsEqual( NiObject *object ) {
	return NiObject::IsEqual( object );
}

/*
================
NiRender::GetViewerStrings
================
*/
// FUNCTION: STBC 0x7E3210
void NiRender::GetViewerStrings( NiTArray<char *> *strings ) {
	NiObject::GetViewerStrings( strings );
	strings->Add( NiGetViewerString( rtti.GetName() ) );
}

/*
================
NiRender::VirtualSlot43

Releases the lights of the object drawn last.
================
*/
// FUNCTION: STBC 0x7E3310
void NiRender::VirtualSlot43( bool unknown ) {
	effectState = 0;
}
