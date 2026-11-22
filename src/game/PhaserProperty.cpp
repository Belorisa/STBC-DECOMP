/*
===============================================================================

	PhaserProperty.cpp

===============================================================================
*/

#include <iostream>

#include <stdio.h>
#include <string.h>

#include "PhaserProperty.h"
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
#include <math.h>

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00684eb0 _$E69
// FUNCTION: STBC 0x00684ec0 _$E68
// FUNCTION: STBC 0x00684ed0 _$E221
// FUNCTION: STBC 0x00684ee0 _$E220
// FUNCTION: STBC 0x00684ef0 _$E80
// FUNCTION: STBC 0x00684f00 _$E79
// FUNCTION: STBC 0x00684f10 _$E84
// FUNCTION: STBC 0x00684f20 _$E83
// FUNCTION: STBC 0x00684f30 _$E88
// FUNCTION: STBC 0x00684f40 _$E87
// FUNCTION: STBC 0x00684f50 _$E92
// FUNCTION: STBC 0x00684f60 _$E91
// FUNCTION: STBC 0x00684f70 _$E207
// FUNCTION: STBC 0x00684f80 _$E206
// FUNCTION: STBC 0x00684f90 _$E149
// FUNCTION: STBC 0x00684fa0 _$E148
// FUNCTION: STBC 0x00684fb0 _$E162
// FUNCTION: STBC 0x00684fc0 _$E161
// FUNCTION: STBC 0x00684fd0 _$E212
// FUNCTION: STBC 0x00684fe0 _$E211
// FUNCTION: STBC 0x00684ff0 _$E226
// FUNCTION: STBC 0x00685000 _$E225
// FUNCTION: STBC 0x00685010 _$E216
// FUNCTION: STBC 0x00685020 _$E215
// FUNCTION: STBC 0x00685030 _$E230
// FUNCTION: STBC 0x00685040 _$E229
// FUNCTION: STBC 0x00685050 _$E237
// FUNCTION: STBC 0x00685060 _$E236
// FUNCTION: STBC 0x00685070 _$E259
// FUNCTION: STBC 0x00685080 _$E258
// FUNCTION: STBC 0x00685090 _$E263
// FUNCTION: STBC 0x006850a0 _$E262
// FUNCTION: STBC 0x006850b0 _$E111
// FUNCTION: STBC 0x006850c0 _$E110
// FUNCTION: STBC 0x006850d0 _$E245
// FUNCTION: STBC 0x006850e0 _$E244
// FUNCTION: STBC 0x006850f0 _$E252
// FUNCTION: STBC 0x00685100 _$E251
// FUNCTION: STBC 0x00685110 _$E275
// FUNCTION: STBC 0x00685120 _$E274
// FUNCTION: STBC 0x00685130 _$E279
// FUNCTION: STBC 0x00685140 _$E278
// FUNCTION: STBC 0x00685150 _$E319
// FUNCTION: STBC 0x00685160 _$E318
// FUNCTION: STBC 0x00685170 _$E325
// FUNCTION: STBC 0x00685180 _$E324
// FUNCTION: STBC 0x00685190 _$E267
// FUNCTION: STBC 0x006851a0 _$E266
// FUNCTION: STBC 0x006851b0 _$E271
// FUNCTION: STBC 0x006851c0 _$E270
// FUNCTION: STBC 0x006851d0 _$E283
// FUNCTION: STBC 0x006851e0 _$E282
// FUNCTION: STBC 0x006851f0 _$E287
// FUNCTION: STBC 0x00685200 _$E286
// FUNCTION: STBC 0x00685210 _$E291
// FUNCTION: STBC 0x00685220 _$E290
// FUNCTION: STBC 0x00685230 _$E295
// FUNCTION: STBC 0x00685240 _$E294
// FUNCTION: STBC 0x00685250 _$E299
// FUNCTION: STBC 0x00685260 _$E298
// FUNCTION: STBC 0x006852a0 _$E303
// FUNCTION: STBC 0x006852b0 _$E302
// FUNCTION: STBC 0x006852c0 _$E307
// FUNCTION: STBC 0x006852d0 _$E306
// FUNCTION: STBC 0x006852e0 _$E311
// FUNCTION: STBC 0x006852f0 _$E310
// FUNCTION: STBC 0x00685300 _$E116
// FUNCTION: STBC 0x00685310 _$E115
// FUNCTION: STBC 0x00685320 _$E315
// FUNCTION: STBC 0x00685330 _$E314
// FUNCTION: STBC 0x00685340 _$E121
// FUNCTION: STBC 0x00685350 _$E120
// FUNCTION: STBC 0x00685360 _$E329
// FUNCTION: STBC 0x00685370 _$E328
// FUNCTION: STBC 0x00685380 _$E333
// FUNCTION: STBC 0x00685390 _$E332
// FUNCTION: STBC 0x006853a0 _$E337
// FUNCTION: STBC 0x006853b0 _$E336
// FUNCTION: STBC 0x006853c0 _$E341
// FUNCTION: STBC 0x006853d0 _$E340
// FUNCTION: STBC 0x006853e0 _$E126
// FUNCTION: STBC 0x006853f0 _$E125
// FUNCTION: STBC 0x00685400 _$E345
// FUNCTION: STBC 0x00685410 _$E344
// FUNCTION: STBC 0x00685420 _$E349
// FUNCTION: STBC 0x00685430 _$E348
// FUNCTION: STBC 0x00685440 _$E353
// FUNCTION: STBC 0x00685450 _$E352
// FUNCTION: STBC 0x00685460 _$E132
// FUNCTION: STBC 0x00685470 _$E131
// FUNCTION: STBC 0x00685480 _$E137
// FUNCTION: STBC 0x00685490 _$E136
// FUNCTION: STBC 0x006854a0 _$E167
// FUNCTION: STBC 0x006854b0 _$E166
// FUNCTION: STBC 0x006854c0 _$E357
// FUNCTION: STBC 0x006854d0 _$E356
// FUNCTION: STBC 0x006854e0 _$E172
// FUNCTION: STBC 0x006854f0 _$E171
// FUNCTION: STBC 0x00685500 _$E361
// FUNCTION: STBC 0x00685510 _$E360
// FUNCTION: STBC 0x00685520 _$E365
// FUNCTION: STBC 0x00685530 _$E364
// FUNCTION: STBC 0x00685540 _$E177
// FUNCTION: STBC 0x00685550 _$E176
// FUNCTION: STBC 0x00685560 _$E369
// FUNCTION: STBC 0x00685570 _$E368
// FUNCTION: STBC 0x00685580 _$E142
// FUNCTION: STBC 0x00685590 _$E141
// FUNCTION: STBC 0x006855a0 _$E373
// FUNCTION: STBC 0x006855b0 _$E372
// FUNCTION: STBC 0x006855c0 _$E377
// FUNCTION: STBC 0x006855d0 _$E376
// FUNCTION: STBC 0x006855e0 _$E182
// FUNCTION: STBC 0x006855f0 _$E181
// FUNCTION: STBC 0x00685600 _$E187
// FUNCTION: STBC 0x00685610 _$E186
// FUNCTION: STBC 0x00685620 _$E381
// FUNCTION: STBC 0x00685630 _$E380
// FUNCTION: STBC 0x00685640 _$E385
// FUNCTION: STBC 0x00685650 _$E384
// FUNCTION: STBC 0x00685660 _$E389
// FUNCTION: STBC 0x00685670 _$E388
// FUNCTION: STBC 0x00685680 _$E393
// FUNCTION: STBC 0x00685690 _$E392
// FUNCTION: STBC 0x006856a0 _$E397
// FUNCTION: STBC 0x006856b0 _$E396
// FUNCTION: STBC 0x006856c0 _$E401
// FUNCTION: STBC 0x006856d0 _$E400
// FUNCTION: STBC 0x006856e0 _$E405
// FUNCTION: STBC 0x006856f0 _$E404
// FUNCTION: STBC 0x00685700 _$E409
// FUNCTION: STBC 0x00685710 _$E408
// FUNCTION: STBC 0x00685720 _$E192
// FUNCTION: STBC 0x00685730 _$E191
// FUNCTION: STBC 0x00685740 _$E413
// FUNCTION: STBC 0x00685750 _$E412
// FUNCTION: STBC 0x00685760 _$E197
// FUNCTION: STBC 0x00685770 _$E196
// FUNCTION: STBC 0x00685780 _$E202
// FUNCTION: STBC 0x00685790 _$E201
// FUNCTION: STBC 0x006857a0 _$E417
// FUNCTION: STBC 0x006857b0 _$E416
// FUNCTION: STBC 0x006857c0 _$E422
// FUNCTION: STBC 0x006857d0 _$E421
// FUNCTION: STBC 0x006857e0 _$E426
// FUNCTION: STBC 0x006857f0 _$E425

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00684e30 _$E32
// FUNCTION: STBC 0x00684e40 _$E29
// FUNCTION: STBC 0x00684e50 _$E31
// FUNCTION: STBC 0x00684e60 _$E30
// FUNCTION: STBC 0x00684e70 _$E38
// FUNCTION: STBC 0x00684e80 _$E35
// FUNCTION: STBC 0x00684e90 _$E37
// FUNCTION: STBC 0x00684ea0 _$E36
// FUNCTION: STBC 0x00685270 _$E434
// FUNCTION: STBC 0x00685290 _$E433

// The default firing arcs are fractions of a half turn.
static const float PI = 3.1415927f;

/*
================
PhaserProperty::Cast

The property as a PhaserProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00685800 ?Cast@PhaserProperty@@SAPAV1@PAVTGModelProperty@@@Z
PhaserProperty *PhaserProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( PHASERPROPERTY_TYPE ) ) {
		return (PhaserProperty *)property;
	}
	return NULL;
}

/*
================
PhaserProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00685830
PhaserProperty *PhaserProperty::Create( char *name ) {
	PhaserProperty *property = new PhaserProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
PhaserProperty::PhaserProperty
================
*/
// FUNCTION: STBC 0x00685890
PhaserProperty::PhaserProperty( long id ) : EnergyWeaponProperty( id ) {
	arcWidthAngleMin = PI * -0.625f;
	arcWidthAngleMax = PI * 0.625f;
	arcHeightAngleMin = PI * -0.03125f;
	arcHeightAngleMax = PI * 0.75f;
	width = 1.8f;
	length = 1.4f;
	phaserWidth = 0.3f;
	phaserTextureStart = 0;
	phaserTextureEnd = 0;
	orientationForward = NiPoint3::UNIT_Y;
	orientationRight = NiPoint3::UNIT_X;
	orientationUp = NiPoint3::UNIT_Z;
	debugNode = NULL;
	damageRadiusFactor = 0.25f;
	outerShellColor = NiColorA( 1.0f, 44.0f / 255.0f, 2.0f / 255.0f, 1.0f );
	innerShellColor = NiColorA( 1.0f, 44.0f / 255.0f, 2.0f / 255.0f, 1.0f );
	outerCoreColor = NiColorA( 253.0f / 255.0f, 213.0f / 255.0f, 163.0f / 255.0f, 1.0f );
	innerCoreColor = NiColorA( 253.0f / 255.0f, 230.0f / 255.0f, 220.0f / 255.0f, 1.0f );
	numSides = 6;
	mainRadius = 0.15f;
	taperRadius = 0.01f;
	coreScale = 0.5f;
	taperRatio = 0.25f;
	taperMinLength = 5.0f;
	taperMaxLength = 30.0f;
	lengthTextureTilePerUnit = 0.5f;
	perimeterTile = 1.0f;
	textureSpeed = 2.5f;
	textureName = NULL;
	SetTextureName( "data/phaser.tga" );
}

/*
================
PhaserProperty::WriteScript

Writes "<variable> = App.PhaserProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00685bb0
void PhaserProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.PhaserProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
PhaserProperty::~PhaserProperty
================
*/
// FUNCTION: STBC 0x00685ce0
PhaserProperty::~PhaserProperty( void ) {
	if ( debugNode != NULL ) {
		debugNode->GetParent()->DetachChild( debugNode );
		debugNode = NULL;
	}
	delete textureName;
}

/*
================
PhaserProperty::SetTextureName

Keeps a copy of the name of the beam's texture.
================
*/
// FUNCTION: STBC 0x00685d90 ?SetTextureName@PhaserProperty@@QAEXPAD@Z
void PhaserProperty::SetTextureName( char *name ) {
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
PhaserProperty::WriteInit
================
*/
// FUNCTION: STBC 0x00685e00
void PhaserProperty::WriteInit( TGStream *stream, char *variableName ) {
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
	sprintf( line, "%s.SetWidth(%f)\n", variableName, GetWidth() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetLength(%f)\n", variableName, GetLength() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetArcWidthAngles(%f, %f)\n", variableName, GetArcWidthAngleMin(), GetArcWidthAngleMax() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetArcHeightAngles(%f, %f)\n", variableName, GetArcHeightAngleMin(), GetArcHeightAngleMax() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPhaserTextureStart(%d)\n", variableName, GetPhaserTextureStart() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPhaserTextureEnd(%d)\n", variableName, GetPhaserTextureEnd() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPhaserWidth(%f)\n", variableName, GetPhaserWidth() );
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
PhaserProperty::VirtualSlot15

Shows the bank's display model under the viewer's root node, building it the
first time, or hides it.
================
*/
// FUNCTION: STBC 0x00686360
void PhaserProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
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
	Unknown6866F0( viewer->GetRootNode() );
}

/*
================
PhaserProperty::Copy
================
*/
// FUNCTION: STBC 0x00686500
// PARKED: inline budget: retail calls NiPoint3::operator= (0x414560) out of line for the axis copies into the locals, which this build expands, and keeps this in ESI
bool PhaserProperty::Copy( TGModelProperty *property ) {
	PhaserProperty *phaser = Cast( property );
	bool copied = EnergyWeaponProperty::Copy( property );
	if ( copied == true ) {
		SetLength( phaser->GetLength() );
		SetWidth( phaser->GetWidth() );
		NiPoint3 forward;
		NiPoint3 up;
		forward = phaser->orientationForward;
		up = phaser->orientationUp;
		SetOrientation( forward, up );
		SetArcHeightAngles( phaser->GetArcHeightAngleMin(), phaser->GetArcHeightAngleMax() );
		SetArcWidthAngles( phaser->GetArcWidthAngleMin(), phaser->GetArcWidthAngleMax() );
		SetPhaserTextureStart( phaser->GetPhaserTextureStart() );
		SetPhaserTextureEnd( phaser->GetPhaserTextureEnd() );
		SetPhaserWidth( phaser->GetPhaserWidth() );
		SetOuterShellColor( phaser->GetOuterShellColor() );
		SetInnerShellColor( phaser->GetInnerShellColor() );
		SetOuterCoreColor( phaser->GetOuterCoreColor() );
		SetInnerCoreColor( phaser->GetInnerCoreColor() );
		SetNumSides( phaser->GetNumSides() );
		SetMainRadius( phaser->GetMainRadius() );
		SetTaperRadius( phaser->GetTaperRadius() );
		SetCoreScale( phaser->GetCoreScale() );
		SetTaperRatio( phaser->GetTaperRatio() );
		SetTaperMinLength( phaser->GetTaperMinLength() );
		SetTaperMaxLength( phaser->GetTaperMaxLength() );
		SetLengthTextureTilePerUnit( phaser->GetLengthTextureTilePerUnit() );
		SetPerimeterTile( phaser->GetPerimeterTile() );
		SetTextureSpeed( phaser->GetTextureSpeed() );
		SetTextureName( phaser->GetTextureName() );
	}
	return copied;
}

/*
================
PhaserProperty::Unknown6866F0

Builds the display model the first time, an unlit, opaque set of three
pieces: one from Unknown686960 for the radius and two sized to a third of
the root node's bound (Unknown686F00, Unknown687730); later calls update
them. Recomputes the right axis first.
================
*/
// FUNCTION: STBC 0x006866f0
// PARKED: retail passes the radius to the piece builders through the FPU, this build with integer moves (the open subsystem-group FPU-copy wall)
void PhaserProperty::Unknown6866F0( NiNode *root ) {
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

		debugNode->AttachChild( Unknown686960( GetRadius() ), false );
		debugNode->AttachChild( Unknown686F00( size ), false );
		debugNode->AttachChild( Unknown687730( size ), false );
		debugNode->Update( 0.0f );
		debugNode->UpdateProperties();
	} else {
		Unknown686C80( GetRadius() );
		Unknown687350( size );
		Unknown687A60( size );
		debugNode->Update( 0.0f );
	}
}

/*
================
PhaserProperty::Unknown686960

The display geometry: three lines from the position along the forward, up and
right axes, in red, green and blue. The forward axis is as long as the length,
the up axis as the radius and the right axis as the width.
================
*/
// FUNCTION: STBC 0x00686960
// RULE-EXCEPTION: R35, the flag loop and the radius clamp are written without braces: each braced statement adds
// two tuples of inline cost, and with any of the four the budget (2 * own cost) leaves enough to expand the blue
// colour's NiColorA constructor, which retail calls (0x46C280); c2trace: own cost 629 flips it, 631 doesn't
NiAVObject *PhaserProperty::Unknown686960( float radius ) {
	NiPoint3 *vertices = new NiPoint3[6];
	NiPoint3 upAxis = radius * orientationUp;
	NiPoint3 rightAxis = width * orientationRight;
	NiPoint3 forwardAxis = length * orientationForward;
	vertices[0] = position;
	vertices[1] = position + forwardAxis;
	vertices[2] = position;
	vertices[3] = position + upAxis;
	vertices[4] = position;
	vertices[5] = position + rightAxis;

	NiColorA *colors = new NiColorA[6];
	colors[0] = NiColorA( 1.0f, 0.0f, 0.0f, 1.0f );
	colors[1] = colors[0];
	colors[2] = NiColorA( 0.0f, 1.0f, 0.0f, 1.0f );
	colors[3] = colors[2];
	colors[4] = NiColorA( 0.0f, 0.0f, 1.0f, 1.0f );
	colors[5] = colors[4];

	bool *flags = new bool[6];
	for ( int i = 0; i < 6; i++ )
		if ( i % 2 == 0 )
			flags[i] = true;
		else
			flags[i] = false;

	NiLines *lines = new NiLines( 6, vertices, colors, NULL, flags );
	NiBound &bound = lines->GetLinesData()->GetBound();
	float extent = ( GetLength() > GetWidth() ) ? GetLength() : GetWidth();
	if ( radius > extent )
		extent = radius;
	bound.radius = extent;
	bound.unknown10 = true;
	return lines;
}

/*
================
PhaserProperty::Unknown686C80

Moves the three lines of Unknown686960 to the properties' present values.
================
*/
// FUNCTION: STBC 0x00686c80
// PARKED: PCH boundary: exact only with TGPointerMap.h, then NiMain.h, inside the precompiled block (/YX); the game units build without a precompiled header (accepted loss, user's ruling 2026-11-20; docs/c2-backend.md, "A/B over the tree")
void PhaserProperty::Unknown686C80( float radius ) {
	NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 0 );
	if ( lines == NULL ) {
		return;
	}
	NiPoint3 *vertices = lines->GetLinesData()->GetVertices();
	NiPoint3 upAxis = radius * orientationUp;
	NiPoint3 rightAxis = width * orientationRight;
	NiPoint3 forwardAxis = length * orientationForward;
	vertices[0] = position;
	vertices[1] = position + forwardAxis;
	vertices[2] = position;
	vertices[3] = position + upAxis;
	vertices[4] = position;
	vertices[5] = position + rightAxis;
	NiBound &bound = lines->GetLinesData()->GetBound();
	float extent = ( width < length ) ? length : width;
	if ( radius > extent ) {
		extent = radius;
	}
	bound.radius = extent;
	bound.unknown10 = true;
	lines->GetLinesData()->IncRevisionID();
}

/*
================
PhaserProperty::Unknown686F00

The display geometry of the firing arc: lines from the position to the four
corners of the arc, a size away along the forward axis, and the corners joined.
================
*/
// FUNCTION: STBC 0x00686f00
// PARKED: register: the inlined memset of flags 8-11 takes its address into edx (lea) before the dword store where retail stores through [edi+8] (same wall as PulseWeaponProperty::Unknown68D040, TractorBeamProperty::Unknown697BC0)
NiAVObject *PhaserProperty::Unknown686F00( float size ) {
	NiPoint3 origin = position;
	NiMatrix3 widthMin;
	NiMatrix3 widthMax;
	NiMatrix3 heightMin;
	NiMatrix3 heightMax;
	widthMin.MakeRotation( arcWidthAngleMin, orientationUp );
	widthMax.MakeRotation( arcWidthAngleMax, orientationUp );
	heightMin.MakeRotation( arcHeightAngleMin, orientationRight );
	heightMax.MakeRotation( arcHeightAngleMax, orientationRight );
	NiPoint3 reach = size * orientationForward;
	NiPoint3 minMin = reach * widthMin * heightMin + origin;
	NiPoint3 minMax = reach * widthMin * heightMax + origin;
	NiPoint3 maxMax = reach * widthMax * heightMax + origin;
	NiPoint3 maxMin = reach * widthMax * heightMin + origin;

	NiPoint3 *vertices = new NiPoint3[13];
	vertices[0] = origin;
	vertices[1] = minMin;
	vertices[2] = origin;
	vertices[3] = minMax;
	vertices[4] = origin;
	vertices[5] = maxMin;
	vertices[6] = origin;
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
PhaserProperty::Unknown687350

Moves the lines of Unknown686F00 to the properties' present values.
================
*/
// FUNCTION: STBC 0x00687350
// RULE-EXCEPTION: R35, the early return is written without braces: a braced body adds two tuples of inline cost,
// and with them the budget (2 * own cost) leaves enough to expand IncRevisionID, which retail calls (0x4D9050);
// c2trace: own cost 589 keeps it out of line (budget 38 < 41)
void PhaserProperty::Unknown687350( float size ) {
	NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 1 );
	if ( !lines )
		return;
	NiPoint3 *vertices = lines->GetLinesData()->GetVertices();
	NiPoint3 origin = GetPosition();
	NiMatrix3 widthMin;
	NiMatrix3 widthMax;
	NiMatrix3 heightMin;
	NiMatrix3 heightMax;
	widthMin.MakeRotation( arcWidthAngleMin, orientationUp );
	widthMax.MakeRotation( arcWidthAngleMax, orientationUp );
	heightMin.MakeRotation( arcHeightAngleMin, orientationRight );
	heightMax.MakeRotation( arcHeightAngleMax, orientationRight );
	NiPoint3 reach = size * orientationForward;
	NiPoint3 minMin = reach * widthMin * heightMin + origin;
	NiPoint3 minMax = reach * widthMin * heightMax + origin;
	NiPoint3 maxMax = reach * widthMax * heightMax + origin;
	NiPoint3 maxMin = reach * widthMax * heightMin + origin;
	vertices[0] = origin;
	vertices[1] = minMin;
	vertices[2] = origin;
	vertices[3] = minMax;
	vertices[4] = origin;
	vertices[5] = maxMin;
	vertices[6] = origin;
	vertices[7] = maxMax;
	vertices[8] = minMin;
	vertices[9] = maxMin;
	vertices[10] = maxMax;
	vertices[11] = minMax;
	vertices[12] = minMin;
	NiBound &bound = lines->GetModelBound();
	bound.radius = size;
	bound.unknown10 = true;
	lines->GetLinesData()->IncRevisionID();
}

/*
================
PhaserProperty::Unknown687730

The display geometry of the beam's section: an ellipse around the position,
one degree of arc per two points, with the width along the right axis and the
length along the forward axis.
================
*/
// FUNCTION: STBC 0x00687730
// PARKED: PCH boundary: exact with /YX (the call set closes with the GetWidth/GetLength reads and NiSinCos: free sites that drop the up axis's share to 46 and keep the right axis's at 47); without the precompiled header the NiColorA array's EH state and a 12-byte frame slot differ (accepted loss, user's ruling 2026-11-20)
NiAVObject *PhaserProperty::Unknown687730( float size ) {
	NiPoint3 *vertices = new NiPoint3[180];
	// BUG: redundant: the up axis is built and never used
	NiPoint3 upAxis = size * orientationUp;
	NiPoint3 rightAxis = GetWidth() * orientationRight;
	NiPoint3 forwardAxis = GetLength() * orientationForward;
	vertices[0] = position + rightAxis;
	int i;
	for ( i = 1; i < 180; i++ ) {
		float angle = ( ( i - 1 ) * ( 1.0f / 180.0f ) ) * PI;
		angle = angle + angle;
		float sine;
		float cosine;
		NiSinCos( angle, sine, cosine );
		vertices[i] = ( rightAxis * cosine + position ) + forwardAxis * sine;
	}

	NiColorA *colors = new NiColorA[180];
	for ( i = 0; i < 180; i++ ) {
		colors[i] = NiColorA::WHITE;
	}

	bool *flags = new bool[180];
	for ( i = 0; i < 180; i++ ) {
		flags[i] = true;
	}

	NiLines *lines = new NiLines( 180, vertices, colors, NULL, flags );
	NiBound &bound = lines->GetLinesData()->GetBound();
	float extent = ( GetLength() > GetWidth() ) ? GetLength() : GetWidth();
	if ( size > extent ) {
		extent = size;
	}
	bound.radius = extent;
	bound.unknown10 = true;
	return lines;
}

/*
================
PhaserProperty::Unknown687A60

Moves the points of Unknown687730 to the properties' present values.
================
*/
// FUNCTION: STBC 0x00687a60
// PARKED: PCH boundary: exact only with TGPointerMap.h, then NiMain.h, inside the precompiled block (/YX); the game units build without a precompiled header (accepted loss, user's ruling 2026-11-20; docs/c2-backend.md, "A/B over the tree")
void PhaserProperty::Unknown687A60( float size ) {
	NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 2 );
	if ( lines == NULL ) {
		return;
	}
	NiPoint3 *vertices = lines->GetLinesData()->GetVertices();
	// BUG: redundant: the up axis is built and never used
	NiPoint3 upAxis = size * orientationUp;
	NiPoint3 rightAxis = width * orientationRight;
	NiPoint3 forwardAxis = length * orientationForward;
	vertices[0] = position + rightAxis;
	for ( int i = 1; i < 180; i++ ) {
		float angle = ( ( i - 1 ) * ( 1.0f / 180.0f ) ) * PI;
		angle = angle + angle;
		float sine;
		float cosine;
		NiSinCos( angle, sine, cosine );
		vertices[i] = ( rightAxis * cosine + position ) + forwardAxis * sine;
	}
	NiBound &bound = lines->GetLinesData()->GetBound();
	float extent = ( width < length ) ? length : width;
	if ( size > extent ) {
		extent = size;
	}
	bound.radius = extent;
	bound.unknown10 = true;
	lines->GetLinesData()->IncRevisionID();
}

/*
================
PhaserProperty::WriteToStream

The display model is saved as its address. The record ends with the marker
45, which ReadFromStream reads back and drops.
================
*/
// FUNCTION: STBC 0x00687d40
bool PhaserProperty::WriteToStream( TGStream *stream ) {
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
	stream->WriteLong( phaserTextureStart );
	stream->WriteLong( phaserTextureEnd );
	stream->WriteFloat( phaserWidth );
	stream->WriteFloat( width );
	stream->WriteFloat( length );
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
	stream->WriteInt( PHASERPROPERTY_STREAM_END );
	return true;
}

/*
================
PhaserProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00687ff0
bool PhaserProperty::ReadFromStream( TGStream *stream ) {
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
	phaserTextureStart = stream->ReadLong();
	phaserTextureEnd = stream->ReadLong();
	phaserWidth = stream->ReadFloat();
	width = stream->ReadFloat();
	length = stream->ReadFloat();
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
	// The end marker; its value is not checked.
	stream->ReadInt();
	return true;
}

/*
================
PhaserProperty::FixUpPointers

False while the saved display model has not been loaded.
================
*/
// FUNCTION: STBC 0x00688270
bool PhaserProperty::FixUpPointers( void ) {
	if ( debugNode != NULL && pointerMap.Find( (long)debugNode ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
PhaserProperty::PostFixUp
================
*/
// FUNCTION: STBC 0x006882d0
void PhaserProperty::PostFixUp( void ) {
	debugNode = (NiNode *)pointerMap.Find( (long)debugNode );
}
