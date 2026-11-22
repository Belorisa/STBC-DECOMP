/*
===============================================================================

	TractorBeamProperty.cpp

===============================================================================
*/

#include <iostream>

#include <stdio.h>
#include <string.h>

#include "TractorBeamProperty.h"
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
// FUNCTION: STBC 0x00695ce0 _$E32
// FUNCTION: STBC 0x00695cf0 _$E29
// FUNCTION: STBC 0x00695d00 _$E31
// FUNCTION: STBC 0x00695d10 _$E30
// FUNCTION: STBC 0x00695d20 _$E38
// FUNCTION: STBC 0x00695d30 _$E35
// FUNCTION: STBC 0x00695d40 _$E37
// FUNCTION: STBC 0x00695d50 _$E36
// FUNCTION: STBC 0x00696220 _$E434
// FUNCTION: STBC 0x00696240 _$E433

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00695d60 _$E69
// FUNCTION: STBC 0x00695d70 _$E68
// FUNCTION: STBC 0x00695da0 _$E80
// FUNCTION: STBC 0x00695db0 _$E79
// FUNCTION: STBC 0x00695dc0 _$E84
// FUNCTION: STBC 0x00695dd0 _$E83
// FUNCTION: STBC 0x00695de0 _$E88
// FUNCTION: STBC 0x00695df0 _$E87
// FUNCTION: STBC 0x00695e00 _$E92
// FUNCTION: STBC 0x00695e10 _$E91
// FUNCTION: STBC 0x00695e20 _$E207
// FUNCTION: STBC 0x00695e30 _$E206
// FUNCTION: STBC 0x00695e40 _$E149
// FUNCTION: STBC 0x00695e50 _$E148
// FUNCTION: STBC 0x00695e60 _$E162
// FUNCTION: STBC 0x00695e70 _$E161
// FUNCTION: STBC 0x00695ec0 _$E216
// FUNCTION: STBC 0x00695ed0 _$E215
// FUNCTION: STBC 0x00695f60 _$E111
// FUNCTION: STBC 0x00695f70 _$E110
// FUNCTION: STBC 0x00696180 _$E116
// FUNCTION: STBC 0x00696190 _$E115
// FUNCTION: STBC 0x006961c0 _$E121
// FUNCTION: STBC 0x006961d0 _$E120
// FUNCTION: STBC 0x00696290 _$E126
// FUNCTION: STBC 0x006962a0 _$E125
// FUNCTION: STBC 0x00696310 _$E132
// FUNCTION: STBC 0x00696320 _$E131
// FUNCTION: STBC 0x00696330 _$E137
// FUNCTION: STBC 0x00696340 _$E136
// FUNCTION: STBC 0x00696350 _$E167
// FUNCTION: STBC 0x00696360 _$E166
// FUNCTION: STBC 0x00696390 _$E172
// FUNCTION: STBC 0x006963a0 _$E171
// FUNCTION: STBC 0x006963f0 _$E177
// FUNCTION: STBC 0x00696400 _$E176
// FUNCTION: STBC 0x00696430 _$E142
// FUNCTION: STBC 0x00696440 _$E141
// FUNCTION: STBC 0x00696490 _$E182
// FUNCTION: STBC 0x006964a0 _$E181
// FUNCTION: STBC 0x006964b0 _$E187
// FUNCTION: STBC 0x006964c0 _$E186
// FUNCTION: STBC 0x006965d0 _$E192
// FUNCTION: STBC 0x006965e0 _$E191
// FUNCTION: STBC 0x00696610 _$E197
// FUNCTION: STBC 0x00696620 _$E196
// FUNCTION: STBC 0x00696630 _$E202
// FUNCTION: STBC 0x00696640 _$E201
// FUNCTION: STBC 0x00696650 _$E417
// FUNCTION: STBC 0x00696660 _$E416
// FUNCTION: STBC 0x00696670 _$E422
// FUNCTION: STBC 0x00696680 _$E421
// FUNCTION: STBC 0x00696690 _$E426
// FUNCTION: STBC 0x006966a0 _$E425
// FUNCTION: STBC 0x00695d80 _$E221
// FUNCTION: STBC 0x00695d90 _$E220
// FUNCTION: STBC 0x00695e80 _$E212
// FUNCTION: STBC 0x00695e90 _$E211
// FUNCTION: STBC 0x00695ea0 _$E226
// FUNCTION: STBC 0x00695eb0 _$E225
// FUNCTION: STBC 0x00695ee0 _$E230
// FUNCTION: STBC 0x00695ef0 _$E229
// FUNCTION: STBC 0x00695f00 _$E237
// FUNCTION: STBC 0x00695f10 _$E236
// FUNCTION: STBC 0x00695f20 _$E259
// FUNCTION: STBC 0x00695f30 _$E258
// FUNCTION: STBC 0x00695f40 _$E263
// FUNCTION: STBC 0x00695f50 _$E262
// FUNCTION: STBC 0x00695f80 _$E245
// FUNCTION: STBC 0x00695f90 _$E244
// FUNCTION: STBC 0x00695fa0 _$E252
// FUNCTION: STBC 0x00695fb0 _$E251
// FUNCTION: STBC 0x00695fc0 _$E275
// FUNCTION: STBC 0x00695fd0 _$E274
// FUNCTION: STBC 0x00695fe0 _$E279
// FUNCTION: STBC 0x00695ff0 _$E278
// FUNCTION: STBC 0x00696000 _$E319
// FUNCTION: STBC 0x00696010 _$E318
// FUNCTION: STBC 0x00696020 _$E325
// FUNCTION: STBC 0x00696030 _$E324
// FUNCTION: STBC 0x00696040 _$E267
// FUNCTION: STBC 0x00696050 _$E266
// FUNCTION: STBC 0x00696060 _$E271
// FUNCTION: STBC 0x00696070 _$E270
// FUNCTION: STBC 0x00696080 _$E283
// FUNCTION: STBC 0x00696090 _$E282
// FUNCTION: STBC 0x006960a0 _$E287
// FUNCTION: STBC 0x006960b0 _$E286
// FUNCTION: STBC 0x006960c0 _$E291
// FUNCTION: STBC 0x006960d0 _$E290
// FUNCTION: STBC 0x006960e0 _$E295
// FUNCTION: STBC 0x006960f0 _$E294
// FUNCTION: STBC 0x00696100 _$E299
// FUNCTION: STBC 0x00696110 _$E298
// FUNCTION: STBC 0x00696120 _$E303
// FUNCTION: STBC 0x00696130 _$E302
// FUNCTION: STBC 0x00696140 _$E307
// FUNCTION: STBC 0x00696150 _$E306
// FUNCTION: STBC 0x00696160 _$E311
// FUNCTION: STBC 0x00696170 _$E310
// FUNCTION: STBC 0x006961a0 _$E315
// FUNCTION: STBC 0x006961b0 _$E314
// FUNCTION: STBC 0x006961e0 _$E329
// FUNCTION: STBC 0x006961f0 _$E328
// FUNCTION: STBC 0x00696200 _$E333
// FUNCTION: STBC 0x00696210 _$E332
// FUNCTION: STBC 0x00696250 _$E337
// FUNCTION: STBC 0x00696260 _$E336
// FUNCTION: STBC 0x00696270 _$E341
// FUNCTION: STBC 0x00696280 _$E340
// FUNCTION: STBC 0x006962b0 _$E345
// FUNCTION: STBC 0x006962c0 _$E344
// FUNCTION: STBC 0x006962d0 _$E349
// FUNCTION: STBC 0x006962e0 _$E348
// FUNCTION: STBC 0x006962f0 _$E353
// FUNCTION: STBC 0x00696300 _$E352
// FUNCTION: STBC 0x00696370 _$E357
// FUNCTION: STBC 0x00696380 _$E356
// FUNCTION: STBC 0x006963b0 _$E361
// FUNCTION: STBC 0x006963c0 _$E360
// FUNCTION: STBC 0x006963d0 _$E365
// FUNCTION: STBC 0x006963e0 _$E364
// FUNCTION: STBC 0x00696410 _$E369
// FUNCTION: STBC 0x00696420 _$E368
// FUNCTION: STBC 0x00696450 _$E373
// FUNCTION: STBC 0x00696460 _$E372
// FUNCTION: STBC 0x00696470 _$E377
// FUNCTION: STBC 0x00696480 _$E376
// FUNCTION: STBC 0x006964d0 _$E381
// FUNCTION: STBC 0x006964e0 _$E380
// FUNCTION: STBC 0x006964f0 _$E385
// FUNCTION: STBC 0x00696500 _$E384
// FUNCTION: STBC 0x00696510 _$E389
// FUNCTION: STBC 0x00696520 _$E388
// FUNCTION: STBC 0x00696530 _$E393
// FUNCTION: STBC 0x00696540 _$E392
// FUNCTION: STBC 0x00696550 _$E397
// FUNCTION: STBC 0x00696560 _$E396
// FUNCTION: STBC 0x00696570 _$E401
// FUNCTION: STBC 0x00696580 _$E400
// FUNCTION: STBC 0x00696590 _$E405
// FUNCTION: STBC 0x006965a0 _$E404
// FUNCTION: STBC 0x006965b0 _$E409
// FUNCTION: STBC 0x006965c0 _$E408
// FUNCTION: STBC 0x006965f0 _$E413
// FUNCTION: STBC 0x00696600 _$E412

// The default firing arcs are fractions of a half turn.
static const float PI = 3.1415927f;

/*
================
TractorBeamProperty::Cast

The property as a TractorBeamProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006966b0 ?Cast@TractorBeamProperty@@SAPAV1@PAVTGModelProperty@@@Z
TractorBeamProperty *TractorBeamProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( TRACTORBEAMPROPERTY_TYPE ) ) {
		return (TractorBeamProperty *)property;
	}
	return NULL;
}

/*
================
TractorBeamProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x006966e0
TractorBeamProperty *TractorBeamProperty::Create( char *name ) {
	TractorBeamProperty *property = new TractorBeamProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
TractorBeamProperty::TractorBeamProperty
================
*/
// FUNCTION: STBC 0x00696740
TractorBeamProperty::TractorBeamProperty( long id ) : EnergyWeaponProperty( id ) {
	arcWidthAngleMin = PI * -0.625f;
	arcWidthAngleMax = PI * 0.625f;
	arcHeightAngleMin = PI * -0.03125f;
	arcHeightAngleMax = PI * 0.75f;
	orientationForward = NiPoint3::UNIT_Y;
	orientationRight = NiPoint3::UNIT_X;
	orientationUp = NiPoint3::UNIT_Z;
	debugNode = NULL;
	textureStart = 0;
	textureEnd = 0;
	tractorBeamWidth = 0.3f;
	outerShellColor = NiColorA( 0.4f, 0.4f, 1.0f, 1.0f );
	innerShellColor = NiColorA( 0.4f, 0.4f, 1.0f, 1.0f );
	outerCoreColor = NiColorA( 0.4f, 0.4f, 1.0f, 1.0f );
	innerCoreColor = NiColorA( 0.4f, 0.4f, 1.0f, 1.0f );
	numSides = 12;
	mainRadius = tractorBeamWidth * 0.25f;
	taperRadius = 0.0f;
	coreScale = 0.45f;
	taperRatio = 0.2f;
	taperMinLength = 1.0f;
	taperMaxLength = 5.0f;
	lengthTextureTilePerUnit = 0.25f;
	perimeterTile = 1.0f;
	textureSpeed = 0.2f;
	textureName = NULL;
	SetTextureName( "data/Textures/Tactical/TractorBeam.tga" );
}

/*
================
TractorBeamProperty::WriteScript

Writes "<variable> = App.TractorBeamProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00696a20
void TractorBeamProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.TractorBeamProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
TractorBeamProperty::~TractorBeamProperty
================
*/
// FUNCTION: STBC 0x00696b50
TractorBeamProperty::~TractorBeamProperty( void ) {
	if ( debugNode != NULL ) {
		debugNode->GetParent()->DetachChild( debugNode );
		debugNode = NULL;
	}
	if ( textureName != NULL ) {
		delete textureName;
		textureName = NULL;
	}
}

/*
================
TractorBeamProperty::SetTextureName

Keeps a copy of the name of the beam's texture.
================
*/
// FUNCTION: STBC 0x00696c10 ?SetTextureName@TractorBeamProperty@@QAEXPAD@Z
void TractorBeamProperty::SetTextureName( char *name ) {
	if ( textureName != NULL ) {
		delete [] textureName;
		textureName = NULL;
	}
	if ( name != NULL ) {
		textureName = new char[strlen( name ) + 1];
		strcpy( textureName, name );
	}
}

/*
================
TractorBeamProperty::WriteInit

The texture name is written twice, once after the texture range and again
at the end.
================
*/
// FUNCTION: STBC 0x00696c80
void TractorBeamProperty::WriteInit( TGStream *stream, char *variableName ) {
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
	sprintf( line, "%s.SetArcWidthAngles(%f, %f)\n", variableName, GetArcWidthAngleMin(), GetArcWidthAngleMax() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetArcHeightAngles(%f, %f)\n", variableName, GetArcHeightAngleMin(), GetArcHeightAngleMax() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTractorBeamWidth(%f)\n", variableName, GetTractorBeamWidth() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTextureStart(%d)\n", variableName, GetTextureStart() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTextureEnd(%d)\n", variableName, GetTextureEnd() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTextureName(\"%s\")\n", variableName, GetTextureName() );
	stream->WriteCLine( line );
	sprintf( line, "kColor = App.TGColorA()\n" );
	stream->WriteCLine( line );
	sprintf( line, "kColor.SetRGBA(%f, %f, %f, %f)\n", outerShellColor.r, outerShellColor.g, outerShellColor.b, outerShellColor.a );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetOuterShellColor(kColor)\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "kColor.SetRGBA(%f, %f, %f, %f)\n", innerShellColor.r, innerShellColor.g, innerShellColor.b, innerShellColor.a );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetInnerShellColor(kColor)\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "kColor.SetRGBA(%f, %f, %f, %f)\n", outerCoreColor.r, outerCoreColor.g, outerCoreColor.b, outerCoreColor.a );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetOuterCoreColor(kColor)\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "kColor.SetRGBA(%f, %f, %f, %f)\n", innerCoreColor.r, innerCoreColor.g, innerCoreColor.b, innerCoreColor.a );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetInnerCoreColor(kColor)\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetNumSides(%d)\n", variableName, GetNumSides() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMainRadius(%f)\n", variableName, GetMainRadius() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTaperRadius(%f)\n", variableName, GetTaperRadius() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetCoreScale(%f)\n", variableName, GetCoreScale() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTaperRatio(%f)\n", variableName, GetTaperRatio() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTaperMinLength(%f)\n", variableName, GetTaperMinLength() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTaperMaxLength(%f)\n", variableName, GetTaperMaxLength() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetLengthTextureTilePerUnit(%f)\n", variableName, GetLengthTextureTilePerUnit() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPerimeterTile(%f)\n", variableName, GetPerimeterTile() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTextureSpeed(%f)\n", variableName, GetTextureSpeed() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTextureName(\"%s\")\n", variableName, GetTextureName() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
TractorBeamProperty::VirtualSlot15

Shows the projector's display model under the viewer's root node, building
it the first time, or hides it.
================
*/
// FUNCTION: STBC 0x006971c0
void TractorBeamProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
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
	Unknown697500( viewer->GetRootNode() );
}

/*
================
TractorBeamProperty::Copy
================
*/
// FUNCTION: STBC 0x00697360
// PARKED: inline budget: retail calls NiPoint3::operator= (0x414560) out of line for the two axis copies, which this build expands
bool TractorBeamProperty::Copy( TGModelProperty *property ) {
	TractorBeamProperty *tractorBeam = Cast( property );
	bool copied = EnergyWeaponProperty::Copy( property );
	if ( copied == true ) {
		NiPoint3 forward;
		NiPoint3 up;
		forward = tractorBeam->orientationForward;
		up = tractorBeam->orientationUp;
		SetOrientation( forward, up );
		SetArcHeightAngles( tractorBeam->GetArcHeightAngleMin(), tractorBeam->GetArcHeightAngleMax() );
		SetArcWidthAngles( tractorBeam->GetArcWidthAngleMin(), tractorBeam->GetArcWidthAngleMax() );
		SetTextureStart( tractorBeam->GetTextureStart() );
		SetTextureEnd( tractorBeam->GetTextureEnd() );
		SetOuterShellColor( tractorBeam->GetOuterShellColor() );
		SetInnerShellColor( tractorBeam->GetInnerShellColor() );
		SetOuterCoreColor( tractorBeam->GetOuterCoreColor() );
		SetInnerCoreColor( tractorBeam->GetInnerCoreColor() );
		SetNumSides( tractorBeam->GetNumSides() );
		SetMainRadius( tractorBeam->GetMainRadius() );
		SetTaperRadius( tractorBeam->GetTaperRadius() );
		SetCoreScale( tractorBeam->GetCoreScale() );
		SetTaperRatio( tractorBeam->GetTaperRatio() );
		SetTaperMinLength( tractorBeam->GetTaperMinLength() );
		SetTaperMaxLength( tractorBeam->GetTaperMaxLength() );
		SetLengthTextureTilePerUnit( tractorBeam->GetLengthTextureTilePerUnit() );
		SetPerimeterTile( tractorBeam->GetPerimeterTile() );
		SetTextureSpeed( tractorBeam->GetTextureSpeed() );
		SetTextureName( tractorBeam->GetTextureName() );
		SetTractorBeamWidth( tractorBeam->GetTractorBeamWidth() );
	}
	return copied;
}

/*
================
TractorBeamProperty::Unknown697500

Builds the display model the first time, an unlit, opaque pair of pieces
from Unknown697760 (for the radius) and Unknown697BC0 (sized to a third of
the root node's bound); later calls update them. Recomputes the right axis
first.
================
*/
// FUNCTION: STBC 0x00697500
// PARKED: retail passes the radius to the piece builders through the FPU, this build with integer moves (the open subsystem-group FPU-copy wall)
void TractorBeamProperty::Unknown697500( NiNode *root ) {
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

		debugNode->AttachChild( Unknown697760( GetRadius() ), false );
		debugNode->AttachChild( Unknown697BC0( size ), false );
		debugNode->Update( 0.0f );
		debugNode->UpdateProperties();
	} else {
		Unknown697970( GetRadius() );
		Unknown697FE0( size );
		debugNode->Update( 0.0f );
	}
}

/*
================
TractorBeamProperty::Unknown697760

The display geometry: three lines of the given length from the position along
the forward, up and right axes, in red, green and blue.
================
*/
// FUNCTION: STBC 0x00697760
NiAVObject *TractorBeamProperty::Unknown697760( float radius ) {
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
TractorBeamProperty::Unknown697970

Moves the three lines of Unknown697760 to the properties' present values.
================
*/
// FUNCTION: STBC 0x00697970
void TractorBeamProperty::Unknown697970( float radius ) {
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
TractorBeamProperty::Unknown697BC0

The display geometry of the firing arc: lines from the position to the four
corners of the arc, a size away along the forward axis, and the corners joined.
================
*/
// FUNCTION: STBC 0x00697bc0
// PARKED: register: the inlined memset of flags 8-11 takes its address into edx (lea) before the dword store where retail stores through [edi+8]; the corner sums are exact and the jump offsets follow
NiAVObject *TractorBeamProperty::Unknown697BC0( float size ) {
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
TractorBeamProperty::Unknown697FE0

Moves the lines of Unknown697BC0 to the properties' present values.
================
*/
// FUNCTION: STBC 0x00697fe0
void TractorBeamProperty::Unknown697FE0( float size ) {
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
TractorBeamProperty::SetOrientation
================
*/
// FUNCTION: STBC 0x006983a0
void TractorBeamProperty::SetOrientation( const NiPoint3 &forward, const NiPoint3 &up ) {
	orientationForward = forward;
	orientationUp = up;
	orientationRight = orientationForward.Cross( orientationUp );
}

/*
================
TractorBeamProperty::WriteToStream

The display model is saved as its address.
================
*/
// FUNCTION: STBC 0x00698460
bool TractorBeamProperty::WriteToStream( TGStream *stream ) {
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
	stream->WriteLong( textureStart );
	stream->WriteLong( textureEnd );
	stream->WriteFloat( tractorBeamWidth );
	stream->WriteFloat( outerShellColor.r );
	stream->WriteFloat( outerShellColor.g );
	stream->WriteFloat( outerShellColor.b );
	stream->WriteFloat( outerShellColor.a );
	stream->WriteFloat( innerShellColor.r );
	stream->WriteFloat( innerShellColor.g );
	stream->WriteFloat( innerShellColor.b );
	stream->WriteFloat( innerShellColor.a );
	stream->WriteFloat( outerCoreColor.r );
	stream->WriteFloat( outerCoreColor.g );
	stream->WriteFloat( outerCoreColor.b );
	stream->WriteFloat( outerCoreColor.a );
	stream->WriteFloat( innerCoreColor.r );
	stream->WriteFloat( innerCoreColor.g );
	stream->WriteFloat( innerCoreColor.b );
	stream->WriteFloat( innerCoreColor.a );
	stream->WriteInt( numSides );
	stream->WriteFloat( mainRadius );
	stream->WriteFloat( taperRadius );
	stream->WriteFloat( coreScale );
	stream->WriteFloat( taperRatio );
	stream->WriteFloat( taperMinLength );
	stream->WriteFloat( taperMaxLength );
	stream->WriteFloat( lengthTextureTilePerUnit );
	stream->WriteFloat( perimeterTile );
	stream->WriteFloat( textureSpeed );
	stream->VirtualSlot11( textureName );
	return true;
}

/*
================
TractorBeamProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x006986f0
bool TractorBeamProperty::ReadFromStream( TGStream *stream ) {
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
	textureStart = stream->ReadLong();
	textureEnd = stream->ReadLong();
	tractorBeamWidth = stream->ReadFloat();
	outerShellColor.r = stream->ReadFloat();
	outerShellColor.g = stream->ReadFloat();
	outerShellColor.b = stream->ReadFloat();
	outerShellColor.a = stream->ReadFloat();
	innerShellColor.r = stream->ReadFloat();
	innerShellColor.g = stream->ReadFloat();
	innerShellColor.b = stream->ReadFloat();
	innerShellColor.a = stream->ReadFloat();
	outerCoreColor.r = stream->ReadFloat();
	outerCoreColor.g = stream->ReadFloat();
	outerCoreColor.b = stream->ReadFloat();
	outerCoreColor.a = stream->ReadFloat();
	innerCoreColor.r = stream->ReadFloat();
	innerCoreColor.g = stream->ReadFloat();
	innerCoreColor.b = stream->ReadFloat();
	innerCoreColor.a = stream->ReadFloat();
	numSides = stream->ReadInt();
	mainRadius = stream->ReadFloat();
	taperRadius = stream->ReadFloat();
	coreScale = stream->ReadFloat();
	taperRatio = stream->ReadFloat();
	taperMinLength = stream->ReadFloat();
	taperMaxLength = stream->ReadFloat();
	lengthTextureTilePerUnit = stream->ReadFloat();
	perimeterTile = stream->ReadFloat();
	textureSpeed = stream->ReadFloat();
	textureName = stream->VirtualSlot10( -1 );
	return true;
}

/*
================
TractorBeamProperty::FixUpPointers

False while the saved display model has not been loaded.
================
*/
// FUNCTION: STBC 0x00698950
bool TractorBeamProperty::FixUpPointers( void ) {
	if ( debugNode != NULL && pointerMap.Find( (long)debugNode ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
TractorBeamProperty::PostFixUp
================
*/
// FUNCTION: STBC 0x006989b0
void TractorBeamProperty::PostFixUp( void ) {
	debugNode = (NiNode *)pointerMap.Find( (long)debugNode );
}
