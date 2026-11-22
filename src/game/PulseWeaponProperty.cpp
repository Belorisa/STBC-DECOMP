/*
===============================================================================

	PulseWeaponProperty.cpp

===============================================================================
*/

#include <iostream>

#include <stdio.h>
#include <string.h>

#include "PulseWeaponProperty.h"
#include "NiNode.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "TGPointerMap.h"
#include "DisplayModelExtraData.h"
#include "ModelPropertyViewer.h"
#include "NiVertexColorProperty.h"
#include "NiAlphaProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "NiLines.h"
#include "NiLinesData.h"
#include "NiMatrix3.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0068b720 _$E32
// FUNCTION: STBC 0x0068b730 _$E29
// FUNCTION: STBC 0x0068b740 _$E31
// FUNCTION: STBC 0x0068b750 _$E30
// FUNCTION: STBC 0x0068b760 _$E38
// FUNCTION: STBC 0x0068b770 _$E35
// FUNCTION: STBC 0x0068b780 _$E37
// FUNCTION: STBC 0x0068b790 _$E36
// FUNCTION: STBC 0x0068bba0 _$E434
// FUNCTION: STBC 0x0068bbc0 _$E433

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0068b7a0 _$E65
// FUNCTION: STBC 0x0068b7b0 _$E64
// FUNCTION: STBC 0x0068b7e0 _$E76
// FUNCTION: STBC 0x0068b7f0 _$E75
// FUNCTION: STBC 0x0068b800 _$E80
// FUNCTION: STBC 0x0068b810 _$E79
// FUNCTION: STBC 0x0068b820 _$E84
// FUNCTION: STBC 0x0068b830 _$E83
// FUNCTION: STBC 0x0068b840 _$E88
// FUNCTION: STBC 0x0068b850 _$E87
// FUNCTION: STBC 0x0068b860 _$E207
// FUNCTION: STBC 0x0068b870 _$E206
// FUNCTION: STBC 0x0068b880 _$E149
// FUNCTION: STBC 0x0068b890 _$E148
// FUNCTION: STBC 0x0068b8a0 _$E162
// FUNCTION: STBC 0x0068b8b0 _$E161
// FUNCTION: STBC 0x0068b900 _$E216
// FUNCTION: STBC 0x0068b910 _$E215
// FUNCTION: STBC 0x0068b9a0 _$E107
// FUNCTION: STBC 0x0068b9b0 _$E106
// FUNCTION: STBC 0x0068bbf0 _$E112
// FUNCTION: STBC 0x0068bc00 _$E111
// FUNCTION: STBC 0x0068bc30 _$E117
// FUNCTION: STBC 0x0068bc40 _$E116
// FUNCTION: STBC 0x0068bcd0 _$E126
// FUNCTION: STBC 0x0068bce0 _$E125
// FUNCTION: STBC 0x0068bd50 _$E132
// FUNCTION: STBC 0x0068bd60 _$E131
// FUNCTION: STBC 0x0068bd70 _$E137
// FUNCTION: STBC 0x0068bd80 _$E136
// FUNCTION: STBC 0x0068bd90 _$E167
// FUNCTION: STBC 0x0068bda0 _$E166
// FUNCTION: STBC 0x0068bdd0 _$E172
// FUNCTION: STBC 0x0068bde0 _$E171
// FUNCTION: STBC 0x0068be30 _$E177
// FUNCTION: STBC 0x0068be40 _$E176
// FUNCTION: STBC 0x0068be70 _$E142
// FUNCTION: STBC 0x0068be80 _$E141
// FUNCTION: STBC 0x0068bed0 _$E182
// FUNCTION: STBC 0x0068bee0 _$E181
// FUNCTION: STBC 0x0068bef0 _$E187
// FUNCTION: STBC 0x0068bf00 _$E186
// FUNCTION: STBC 0x0068c010 _$E192
// FUNCTION: STBC 0x0068c020 _$E191
// FUNCTION: STBC 0x0068c050 _$E197
// FUNCTION: STBC 0x0068c060 _$E196
// FUNCTION: STBC 0x0068c070 _$E202
// FUNCTION: STBC 0x0068c080 _$E201
// FUNCTION: STBC 0x0068c090 _$E417
// FUNCTION: STBC 0x0068c0a0 _$E416
// FUNCTION: STBC 0x0068c0b0 _$E422
// FUNCTION: STBC 0x0068c0c0 _$E421
// FUNCTION: STBC 0x0068c0d0 _$E426
// FUNCTION: STBC 0x0068c0e0 _$E425
// FUNCTION: STBC 0x0068b7c0 _$E221
// FUNCTION: STBC 0x0068b7d0 _$E220
// FUNCTION: STBC 0x0068b8c0 _$E212
// FUNCTION: STBC 0x0068b8d0 _$E211
// FUNCTION: STBC 0x0068b8e0 _$E226
// FUNCTION: STBC 0x0068b8f0 _$E225
// FUNCTION: STBC 0x0068b920 _$E230
// FUNCTION: STBC 0x0068b930 _$E229
// FUNCTION: STBC 0x0068b940 _$E237
// FUNCTION: STBC 0x0068b950 _$E236
// FUNCTION: STBC 0x0068b960 _$E259
// FUNCTION: STBC 0x0068b970 _$E258
// FUNCTION: STBC 0x0068b980 _$E263
// FUNCTION: STBC 0x0068b990 _$E262
// FUNCTION: STBC 0x0068b9c0 _$E245
// FUNCTION: STBC 0x0068b9d0 _$E244
// FUNCTION: STBC 0x0068b9e0 _$E252
// FUNCTION: STBC 0x0068b9f0 _$E251
// FUNCTION: STBC 0x0068ba00 _$E275
// FUNCTION: STBC 0x0068ba10 _$E274
// FUNCTION: STBC 0x0068ba20 _$E279
// FUNCTION: STBC 0x0068ba30 _$E278
// FUNCTION: STBC 0x0068ba40 _$E319
// FUNCTION: STBC 0x0068ba50 _$E318
// FUNCTION: STBC 0x0068ba60 _$E325
// FUNCTION: STBC 0x0068ba70 _$E324
// FUNCTION: STBC 0x0068ba80 _$E267
// FUNCTION: STBC 0x0068ba90 _$E266
// FUNCTION: STBC 0x0068baa0 _$E271
// FUNCTION: STBC 0x0068bab0 _$E270
// FUNCTION: STBC 0x0068bac0 _$E283
// FUNCTION: STBC 0x0068bad0 _$E282
// FUNCTION: STBC 0x0068bae0 _$E287
// FUNCTION: STBC 0x0068baf0 _$E286
// FUNCTION: STBC 0x0068bb00 _$E291
// FUNCTION: STBC 0x0068bb10 _$E290
// FUNCTION: STBC 0x0068bb20 _$E295
// FUNCTION: STBC 0x0068bb30 _$E294
// FUNCTION: STBC 0x0068bb40 _$E299
// FUNCTION: STBC 0x0068bb50 _$E298
// FUNCTION: STBC 0x0068bb60 _$E303
// FUNCTION: STBC 0x0068bb70 _$E302
// FUNCTION: STBC 0x0068bb80 _$E307
// FUNCTION: STBC 0x0068bb90 _$E306
// FUNCTION: STBC 0x0068bbd0 _$E311
// FUNCTION: STBC 0x0068bbe0 _$E310
// FUNCTION: STBC 0x0068bc10 _$E315
// FUNCTION: STBC 0x0068bc20 _$E314
// FUNCTION: STBC 0x0068bc50 _$E329
// FUNCTION: STBC 0x0068bc60 _$E328
// FUNCTION: STBC 0x0068bc70 _$E333
// FUNCTION: STBC 0x0068bc80 _$E332
// FUNCTION: STBC 0x0068bc90 _$E337
// FUNCTION: STBC 0x0068bca0 _$E336
// FUNCTION: STBC 0x0068bcb0 _$E341
// FUNCTION: STBC 0x0068bcc0 _$E340
// FUNCTION: STBC 0x0068bcf0 _$E345
// FUNCTION: STBC 0x0068bd00 _$E344
// FUNCTION: STBC 0x0068bd10 _$E349
// FUNCTION: STBC 0x0068bd20 _$E348
// FUNCTION: STBC 0x0068bd30 _$E353
// FUNCTION: STBC 0x0068bd40 _$E352
// FUNCTION: STBC 0x0068bdb0 _$E357
// FUNCTION: STBC 0x0068bdc0 _$E356
// FUNCTION: STBC 0x0068bdf0 _$E361
// FUNCTION: STBC 0x0068be00 _$E360
// FUNCTION: STBC 0x0068be10 _$E365
// FUNCTION: STBC 0x0068be20 _$E364
// FUNCTION: STBC 0x0068be50 _$E369
// FUNCTION: STBC 0x0068be60 _$E368
// FUNCTION: STBC 0x0068be90 _$E373
// FUNCTION: STBC 0x0068bea0 _$E372
// FUNCTION: STBC 0x0068beb0 _$E377
// FUNCTION: STBC 0x0068bec0 _$E376
// FUNCTION: STBC 0x0068bf10 _$E381
// FUNCTION: STBC 0x0068bf20 _$E380
// FUNCTION: STBC 0x0068bf30 _$E385
// FUNCTION: STBC 0x0068bf40 _$E384
// FUNCTION: STBC 0x0068bf50 _$E389
// FUNCTION: STBC 0x0068bf60 _$E388
// FUNCTION: STBC 0x0068bf70 _$E393
// FUNCTION: STBC 0x0068bf80 _$E392
// FUNCTION: STBC 0x0068bf90 _$E397
// FUNCTION: STBC 0x0068bfa0 _$E396
// FUNCTION: STBC 0x0068bfb0 _$E401
// FUNCTION: STBC 0x0068bfc0 _$E400
// FUNCTION: STBC 0x0068bfd0 _$E405
// FUNCTION: STBC 0x0068bfe0 _$E404
// FUNCTION: STBC 0x0068bff0 _$E409
// FUNCTION: STBC 0x0068c000 _$E408
// FUNCTION: STBC 0x0068c030 _$E413
// FUNCTION: STBC 0x0068c040 _$E412

// The default firing arcs are fractions of a half turn.
static const float PI = 3.1415927f;

/*
================
PulseWeaponProperty::Cast

The property as a PulseWeaponProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0068c0f0 ?Cast@PulseWeaponProperty@@SAPAV1@PAVTGModelProperty@@@Z
PulseWeaponProperty *PulseWeaponProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( PULSEWEAPONPROPERTY_TYPE ) ) {
		return (PulseWeaponProperty *)property;
	}
	return NULL;
}

/*
================
PulseWeaponProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x0068c120
PulseWeaponProperty *PulseWeaponProperty::Create( char *name ) {
	PulseWeaponProperty *property = new PulseWeaponProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
PulseWeaponProperty::PulseWeaponProperty
================
*/
// FUNCTION: STBC 0x0068c180
PulseWeaponProperty::PulseWeaponProperty( long id ) : EnergyWeaponProperty( id ) {
	arcWidthAngleMin = PI * -0.625f;
	arcWidthAngleMax = PI * 0.625f;
	arcHeightAngleMin = PI * -0.03125f;
	arcHeightAngleMax = PI * 0.75f;
	cooldownTime = 0.3f;
	orientationForward = NiPoint3::UNIT_Y;
	orientationRight = NiPoint3::UNIT_X;
	orientationUp = NiPoint3::UNIT_Z;
	debugNode = NULL;
	moduleName = new char[1];
	strcpy( moduleName, "" );
	dumbfire = true;
	damageRadiusFactor = 0.3f;
}

/*
================
PulseWeaponProperty::WriteScript

Writes "<variable> = App.PulseWeaponProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x0068c360
void PulseWeaponProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.PulseWeaponProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
PulseWeaponProperty::~PulseWeaponProperty
================
*/
// FUNCTION: STBC 0x0068c490
PulseWeaponProperty::~PulseWeaponProperty( void ) {
	if ( debugNode != NULL ) {
		debugNode->GetParent()->DetachChild( debugNode );
		debugNode = NULL;
	}
	if ( moduleName != NULL ) {
		delete moduleName;
		moduleName = NULL;
	}
}

/*
================
PulseWeaponProperty::WriteInit
================
*/
// FUNCTION: STBC 0x0068c550
void PulseWeaponProperty::WriteInit( TGStream *stream, char *variableName ) {
	EnergyWeaponProperty::WriteInit( stream, variableName );

	char *line = new char[2048];
	sprintf( line, "%sForward = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sForward.SetXYZ(%f, %f, %f)\n", variableName, orientationForward.x, orientationForward.y, orientationForward.z );
	stream->WriteCLine( line );
	sprintf( line, "%sUp = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sUp.SetXYZ(%f, %f, %f)\n", variableName, orientationUp.x, orientationUp.y, orientationUp.z );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetOrientation(%sForward, %sUp)\n", variableName, variableName, variableName );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetArcWidthAngles(%f, %f)\n", variableName, arcWidthAngleMin, arcWidthAngleMax );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetArcHeightAngles(%f, %f)\n", variableName, arcHeightAngleMin, arcHeightAngleMax );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetCooldownTime(%f)\n", variableName, cooldownTime );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetModuleName(\"%s\")\n", variableName, moduleName );
	stream->WriteCLine( line );

	delete line;
}

/*
================
PulseWeaponProperty::VirtualSlot15

Shows the weapon's display model under the viewer's root node, building it
the first time, or hides it.
================
*/
// FUNCTION: STBC 0x0068c710
void PulseWeaponProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
	if ( !show ) {
		if ( debugNode != NULL ) {
			debugNode->SetAppCulled( true );
		}
		return;
	}
	if ( debugNode == NULL ) {
		debugNode = new NiNode;
		debugNode->AddExtraData( new DisplayModelExtraData );
		viewer->GetRootNode()->AttachChild( debugNode, false );
	}
	debugNode->SetAppCulled( false );
	if ( debugNode->GetParent() != parent ) {
		debugNode->IncRefCount();
		debugNode->GetParent()->DetachChild( debugNode );
		viewer->GetRootNode()->AttachChild( debugNode, false );
		debugNode->DecRefCount();
	}
	Unknown68C980( viewer->GetRootNode() );
}

/*
================
PulseWeaponProperty::Copy
================
*/
// FUNCTION: STBC 0x0068c8b0
// PARKED: register schedule: this build loads this into ECX for SetModuleName between the arc copies, retail just before the call
bool PulseWeaponProperty::Copy( TGModelProperty *property ) {
	PulseWeaponProperty *pulse = Cast( property );
	bool copied = EnergyWeaponProperty::Copy( property );
	if ( copied == true ) {
		NiPoint3 forward;
		NiPoint3 up;
		forward = pulse->orientationForward;
		up = pulse->orientationUp;
		SetOrientation( forward, up );
		SetArcHeightAngles( pulse->GetArcHeightAngleMin(), pulse->GetArcHeightAngleMax() );
		SetArcWidthAngles( pulse->GetArcWidthAngleMin(), pulse->GetArcWidthAngleMax() );
		SetCooldownTime( pulse->GetCooldownTime() );
		SetModuleName( pulse->GetModuleName() );
	}
	return copied;
}

/*
================
PulseWeaponProperty::Unknown68C980

Builds the display model the first time, an unlit, opaque pair of pieces
from Unknown68CBE0 (for the radius) and Unknown68D040 (sized to a third of
the root node's bound); later calls update them. Recomputes the right axis
first.
================
*/
// FUNCTION: STBC 0x0068c980
// PARKED: retail passes the radius to the piece builders through the FPU, this build with integer moves (the open subsystem-group FPU-copy wall)
void PulseWeaponProperty::Unknown68C980( NiNode *root ) {
	float size = root->GetWorldBound().GetRadius() * 0.33f;
	orientationRight = orientationForward.Cross( orientationUp );
	if ( debugNode->GetChildCount() == 0 ) {
		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		debugNode->AttachProperty( vertexColor );

		NiAlphaProperty *alpha = new NiAlphaProperty;
		alpha->SetAlphaBlending( false );
		debugNode->AttachProperty( alpha );

		debugNode->AttachChild( Unknown68CBE0( GetRadius() ), false );
		debugNode->AttachChild( Unknown68D040( size ), false );
		debugNode->Update( 0.0f );
		debugNode->UpdateProperties();
	} else {
		Unknown68CDF0( GetRadius() );
		Unknown68D460( size );
		debugNode->Update( 0.0f );
	}
}

/*
================
PulseWeaponProperty::Unknown68CBE0

The display geometry: three lines of the given length from the position along
the forward, up and right axes, in red, green and blue.
================
*/
// FUNCTION: STBC 0x0068cbe0
NiAVObject *PulseWeaponProperty::Unknown68CBE0( float radius ) {
	NiPoint3 *vertices = new NiPoint3[6];
	vertices[0] = NiPoint3( position.x, position.y, position.z );
	vertices[1] = NiPoint3( radius * orientationForward.x + position.x, radius * orientationForward.y + position.y, radius * orientationForward.z + position.z );
	vertices[2] = NiPoint3( position.x, position.y, position.z );
	vertices[3] = NiPoint3( radius * orientationUp.x + position.x, radius * orientationUp.y + position.y, radius * orientationUp.z + position.z );
	vertices[4] = NiPoint3( position.x, position.y, position.z );
	vertices[5] = NiPoint3( radius * orientationRight.x + position.x, radius * orientationRight.y + position.y, radius * orientationRight.z + position.z );

	NiColorA *colors = new NiColorA[6];
	colors[0] = NiColorA( 1.0f, 0.0f, 0.0f, 1.0f );
	colors[1] = colors[0];
	colors[2] = NiColorA( 0.0f, 1.0f, 0.0f, 1.0f );
	colors[3] = colors[2];
	colors[4] = NiColorA( 0.0f, 0.0f, 1.0f, 1.0f );
	colors[5] = colors[4];

	bool *flags = new bool[6];
	for ( int i = 0; i < 6; i++ ) {
		if ( i % 2 == 0 ) {
			flags[i] = true;
		} else {
			flags[i] = false;
		}
	}

	NiLines *lines = new NiLines( 6, vertices, colors, NULL, flags );
	NiBound &bound = lines->GetLinesData()->GetBound();
	bound.radius = radius;
	bound.unknown10 = true;
	return lines;
}

/*
================
PulseWeaponProperty::Unknown68CDF0

Moves the three lines of Unknown68CBE0 to the properties' present values.
================
*/
// FUNCTION: STBC 0x0068cdf0
void PulseWeaponProperty::Unknown68CDF0( float radius ) {
	NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 0 );
	NiPoint3 *vertices = lines->GetLinesData()->GetVertices();
	vertices[0] = position;
	vertices[1] = radius * orientationForward + position;
	vertices[2] = position;
	vertices[3] = radius * orientationUp + position;
	vertices[4] = position;
	vertices[5] = radius * orientationRight + position;
	NiBound &bound = lines->GetModelBound();
	bound.radius = radius;
	bound.unknown10 = true;
	lines->GetLinesData()->IncRevisionID();
}

/*
================
PulseWeaponProperty::Unknown68D040

The display geometry of the firing arc: lines from the position to the four
corners of the arc, a size away along the forward axis, and the corners joined.
================
*/
// FUNCTION: STBC 0x0068d040
// PARKED: register: the inlined memset of flags 8-11 takes its address into edx (lea) before the dword store where retail stores through [edi+8]; the jump offsets follow (same wall as TractorBeamProperty::Unknown697BC0)
NiAVObject *PulseWeaponProperty::Unknown68D040( float size ) {
	NiMatrix3 widthMin;
	NiMatrix3 widthMax;
	NiMatrix3 heightMin;
	NiMatrix3 heightMax;
	widthMin.MakeRotation( arcWidthAngleMin, orientationUp );
	widthMax.MakeRotation( arcWidthAngleMax, orientationUp );
	heightMin.MakeRotation( arcHeightAngleMin, orientationRight );
	heightMax.MakeRotation( arcHeightAngleMax, orientationRight );
	NiPoint3 reach = size * orientationForward;
	NiPoint3 minMin = reach * widthMin * heightMin + position;
	NiPoint3 minMax = reach * widthMin * heightMax + position;
	NiPoint3 maxMax = reach * widthMax * heightMax + position;
	NiPoint3 maxMin = reach * widthMax * heightMin + position;

	NiPoint3 *vertices = new NiPoint3[13];
	vertices[0] = position;
	vertices[1] = minMin;
	vertices[2] = position;
	vertices[3] = minMax;
	vertices[4] = position;
	vertices[5] = maxMin;
	vertices[6] = position;
	vertices[7] = maxMax;
	vertices[8] = minMin;
	vertices[9] = maxMin;
	vertices[10] = maxMax;
	vertices[11] = minMax;
	vertices[12] = minMin;

	bool *flags = new bool[13];
	int i;
	for ( i = 0; i < 8; i++ ) {
		if ( i % 2 == 0 ) {
			flags[i] = true;
		} else {
			flags[i] = false;
		}
	}
	memset( &flags[8], true, 4 );
	flags[12] = false;

	NiColorA *colors = new NiColorA[13];
	for ( i = 0; i < 13; i++ ) {
		colors[i] = NiColorA::WHITE;
	}

	NiLines *lines = new NiLines( 13, vertices, colors, NULL, flags );
	NiBound &bound = lines->GetLinesData()->GetBound();
	bound.radius = size;
	bound.unknown10 = true;
	return lines;
}

/*
================
PulseWeaponProperty::Unknown68D460

Moves the lines of Unknown68D040 to the properties' present values.
================
*/
// FUNCTION: STBC 0x0068d460
void PulseWeaponProperty::Unknown68D460( float size ) {
	NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 1 );
	NiLinesData *data = lines->GetLinesData();
	NiPoint3 *vertices = data->GetVertices();
	NiMatrix3 widthMin;
	NiMatrix3 widthMax;
	NiMatrix3 heightMin;
	NiMatrix3 heightMax;
	widthMin.MakeRotation( arcWidthAngleMin, orientationUp );
	widthMax.MakeRotation( arcWidthAngleMax, orientationUp );
	heightMin.MakeRotation( arcHeightAngleMin, orientationRight );
	heightMax.MakeRotation( arcHeightAngleMax, orientationRight );
	NiPoint3 reach = size * orientationForward;
	NiPoint3 minMin = reach * widthMin * heightMin + position;
	NiPoint3 minMax = reach * widthMin * heightMax + position;
	NiPoint3 maxMax = reach * widthMax * heightMax + position;
	NiPoint3 maxMin = reach * widthMax * heightMin + position;
	vertices[0] = position;
	vertices[1] = minMin;
	vertices[2] = position;
	vertices[3] = minMax;
	vertices[4] = position;
	vertices[5] = maxMin;
	vertices[6] = position;
	vertices[7] = maxMax;
	vertices[8] = minMin;
	vertices[9] = maxMin;
	vertices[10] = maxMax;
	vertices[11] = minMax;
	vertices[12] = minMin;
	NiBound &bound = lines->GetLinesData()->GetBound();
	bound.radius = size;
	bound.unknown10 = true;
	lines->GetLinesData()->IncRevisionID();
}

/*
================
PulseWeaponProperty::SetOrientation
================
*/
// FUNCTION: STBC 0x0068d820
void PulseWeaponProperty::SetOrientation( const NiPoint3 &forward, const NiPoint3 &up ) {
	orientationForward = forward;
	orientationUp = up;
	orientationRight = orientationForward.Cross( orientationUp );
}

/*
================
PulseWeaponProperty::SetModuleName

Keeps a copy of the name of the pulse's script module.
================
*/
// FUNCTION: STBC 0x0068d8e0 ?SetModuleName@PulseWeaponProperty@@QAEXPAD@Z
void PulseWeaponProperty::SetModuleName( char *name ) {
	if ( moduleName != NULL ) {
		delete [] moduleName;
		moduleName = NULL;
	}
	if ( name != NULL ) {
		moduleName = new char[strlen( name ) + 1];
		strcpy( moduleName, name );
	}
}

/*
================
PulseWeaponProperty::WriteToStream

The display model is saved as its address.
================
*/
// FUNCTION: STBC 0x0068d950
bool PulseWeaponProperty::WriteToStream( TGStream *stream ) {
	if ( !EnergyWeaponProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->VirtualSlot17( (long)debugNode );
	stream->WriteFloat( arcWidthAngleMin );
	stream->WriteFloat( arcWidthAngleMax );
	stream->WriteFloat( arcHeightAngleMin );
	stream->WriteFloat( arcHeightAngleMax );
	stream->WriteFloat( orientationForward.x );
	stream->WriteFloat( orientationForward.y );
	stream->WriteFloat( orientationForward.z );
	stream->WriteFloat( orientationUp.x );
	stream->WriteFloat( orientationUp.y );
	stream->WriteFloat( orientationUp.z );
	stream->WriteFloat( orientationRight.x );
	stream->WriteFloat( orientationRight.y );
	stream->WriteFloat( orientationRight.z );
	stream->WriteFloat( cooldownTime );
	stream->VirtualSlot11( moduleName );
	return true;
}

/*
================
PulseWeaponProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x0068da50
bool PulseWeaponProperty::ReadFromStream( TGStream *stream ) {
	if ( !EnergyWeaponProperty::ReadFromStream( stream ) ) {
		return false;
	}
	debugNode = (NiNode *)stream->VirtualSlot16();
	arcWidthAngleMin = stream->ReadFloat();
	arcWidthAngleMax = stream->ReadFloat();
	arcHeightAngleMin = stream->ReadFloat();
	arcHeightAngleMax = stream->ReadFloat();
	orientationForward.x = stream->ReadFloat();
	orientationForward.y = stream->ReadFloat();
	orientationForward.z = stream->ReadFloat();
	orientationUp.x = stream->ReadFloat();
	orientationUp.y = stream->ReadFloat();
	orientationUp.z = stream->ReadFloat();
	orientationRight.x = stream->ReadFloat();
	orientationRight.y = stream->ReadFloat();
	orientationRight.z = stream->ReadFloat();
	cooldownTime = stream->ReadFloat();
	moduleName = stream->VirtualSlot10( -1 );
	return true;
}

/*
================
PulseWeaponProperty::FixUpPointers

False while the saved display model has not been loaded.
================
*/
// FUNCTION: STBC 0x0068db40
bool PulseWeaponProperty::FixUpPointers( void ) {
	if ( debugNode != NULL && pointerMap.Find( (long)debugNode ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
PulseWeaponProperty::PostFixUp
================
*/
// FUNCTION: STBC 0x0068dba0
void PulseWeaponProperty::PostFixUp( void ) {
	debugNode = (NiNode *)pointerMap.Find( (long)debugNode );
}
