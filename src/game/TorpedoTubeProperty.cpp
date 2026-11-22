/*
===============================================================================

	TorpedoTubeProperty.cpp

===============================================================================
*/

#include <iostream>

#include <stdio.h>

#include "TorpedoTubeProperty.h"
#include "DisplayModelExtraData.h"
#include "ModelPropertyViewer.h"
#include "NiNode.h"
#include "TGStream.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00694610 _$E32
// FUNCTION: STBC 0x00694620 _$E29
// FUNCTION: STBC 0x00694630 _$E31
// FUNCTION: STBC 0x00694640 _$E30
// FUNCTION: STBC 0x00694650 _$E38
// FUNCTION: STBC 0x00694660 _$E35
// FUNCTION: STBC 0x00694670 _$E37
// FUNCTION: STBC 0x00694680 _$E36
// FUNCTION: STBC 0x00694b10 _$E434
// FUNCTION: STBC 0x00694b30 _$E433

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00694690 _$E65
// FUNCTION: STBC 0x006946a0 _$E64
// FUNCTION: STBC 0x006946d0 _$E76
// FUNCTION: STBC 0x006946e0 _$E75
// FUNCTION: STBC 0x006946f0 _$E81
// FUNCTION: STBC 0x00694700 _$E80
// FUNCTION: STBC 0x00694710 _$E85
// FUNCTION: STBC 0x00694720 _$E84
// FUNCTION: STBC 0x00694730 _$E89
// FUNCTION: STBC 0x00694740 _$E88
// FUNCTION: STBC 0x00694750 _$E208
// FUNCTION: STBC 0x00694760 _$E207
// FUNCTION: STBC 0x00694770 _$E150
// FUNCTION: STBC 0x00694780 _$E149
// FUNCTION: STBC 0x00694790 _$E163
// FUNCTION: STBC 0x006947a0 _$E162
// FUNCTION: STBC 0x006947f0 _$E217
// FUNCTION: STBC 0x00694800 _$E216
// FUNCTION: STBC 0x00694890 _$E108
// FUNCTION: STBC 0x006948a0 _$E107
// FUNCTION: STBC 0x00694ab0 _$E113
// FUNCTION: STBC 0x00694ac0 _$E112
// FUNCTION: STBC 0x00694af0 _$E118
// FUNCTION: STBC 0x00694b00 _$E117
// FUNCTION: STBC 0x00694bc0 _$E127
// FUNCTION: STBC 0x00694bd0 _$E126
// FUNCTION: STBC 0x00694c40 _$E133
// FUNCTION: STBC 0x00694c50 _$E132
// FUNCTION: STBC 0x00694c60 _$E138
// FUNCTION: STBC 0x00694c70 _$E137
// FUNCTION: STBC 0x00694c80 _$E168
// FUNCTION: STBC 0x00694c90 _$E167
// FUNCTION: STBC 0x00694cc0 _$E173
// FUNCTION: STBC 0x00694cd0 _$E172
// FUNCTION: STBC 0x00694d20 _$E178
// FUNCTION: STBC 0x00694d30 _$E177
// FUNCTION: STBC 0x00694d60 _$E143
// FUNCTION: STBC 0x00694d70 _$E142
// FUNCTION: STBC 0x00694dc0 _$E183
// FUNCTION: STBC 0x00694dd0 _$E182
// FUNCTION: STBC 0x00694de0 _$E188
// FUNCTION: STBC 0x00694df0 _$E187
// FUNCTION: STBC 0x00694f00 _$E193
// FUNCTION: STBC 0x00694f10 _$E192
// FUNCTION: STBC 0x00694f40 _$E198
// FUNCTION: STBC 0x00694f50 _$E197
// FUNCTION: STBC 0x00694f60 _$E203
// FUNCTION: STBC 0x00694f70 _$E202
// FUNCTION: STBC 0x00694f80 _$E417
// FUNCTION: STBC 0x00694f90 _$E416
// FUNCTION: STBC 0x00694fa0 _$E422
// FUNCTION: STBC 0x00694fb0 _$E421
// FUNCTION: STBC 0x00694fc0 _$E426
// FUNCTION: STBC 0x00694fd0 _$E425
// FUNCTION: STBC 0x006946b0 _$E221
// FUNCTION: STBC 0x006946c0 _$E220
// FUNCTION: STBC 0x006947b0 _$E213
// FUNCTION: STBC 0x006947c0 _$E212
// FUNCTION: STBC 0x006947d0 _$E226
// FUNCTION: STBC 0x006947e0 _$E225
// FUNCTION: STBC 0x00694810 _$E230
// FUNCTION: STBC 0x00694820 _$E229
// FUNCTION: STBC 0x00694830 _$E237
// FUNCTION: STBC 0x00694840 _$E236
// FUNCTION: STBC 0x00694850 _$E259
// FUNCTION: STBC 0x00694860 _$E258
// FUNCTION: STBC 0x00694870 _$E263
// FUNCTION: STBC 0x00694880 _$E262
// FUNCTION: STBC 0x006948b0 _$E245
// FUNCTION: STBC 0x006948c0 _$E244
// FUNCTION: STBC 0x006948d0 _$E252
// FUNCTION: STBC 0x006948e0 _$E251
// FUNCTION: STBC 0x006948f0 _$E275
// FUNCTION: STBC 0x00694900 _$E274
// FUNCTION: STBC 0x00694910 _$E279
// FUNCTION: STBC 0x00694920 _$E278
// FUNCTION: STBC 0x00694930 _$E319
// FUNCTION: STBC 0x00694940 _$E318
// FUNCTION: STBC 0x00694950 _$E325
// FUNCTION: STBC 0x00694960 _$E324
// FUNCTION: STBC 0x00694970 _$E267
// FUNCTION: STBC 0x00694980 _$E266
// FUNCTION: STBC 0x00694990 _$E271
// FUNCTION: STBC 0x006949a0 _$E270
// FUNCTION: STBC 0x006949b0 _$E283
// FUNCTION: STBC 0x006949c0 _$E282
// FUNCTION: STBC 0x006949d0 _$E287
// FUNCTION: STBC 0x006949e0 _$E286
// FUNCTION: STBC 0x006949f0 _$E291
// FUNCTION: STBC 0x00694a00 _$E290
// FUNCTION: STBC 0x00694a10 _$E295
// FUNCTION: STBC 0x00694a20 _$E294
// FUNCTION: STBC 0x00694a30 _$E299
// FUNCTION: STBC 0x00694a40 _$E298
// FUNCTION: STBC 0x00694a50 _$E303
// FUNCTION: STBC 0x00694a60 _$E302
// FUNCTION: STBC 0x00694a70 _$E307
// FUNCTION: STBC 0x00694a80 _$E306
// FUNCTION: STBC 0x00694a90 _$E311
// FUNCTION: STBC 0x00694aa0 _$E310
// FUNCTION: STBC 0x00694ad0 _$E315
// FUNCTION: STBC 0x00694ae0 _$E314
// FUNCTION: STBC 0x00694b40 _$E329
// FUNCTION: STBC 0x00694b50 _$E328
// FUNCTION: STBC 0x00694b60 _$E333
// FUNCTION: STBC 0x00694b70 _$E332
// FUNCTION: STBC 0x00694b80 _$E337
// FUNCTION: STBC 0x00694b90 _$E336
// FUNCTION: STBC 0x00694ba0 _$E341
// FUNCTION: STBC 0x00694bb0 _$E340
// FUNCTION: STBC 0x00694be0 _$E345
// FUNCTION: STBC 0x00694bf0 _$E344
// FUNCTION: STBC 0x00694c00 _$E349
// FUNCTION: STBC 0x00694c10 _$E348
// FUNCTION: STBC 0x00694c20 _$E353
// FUNCTION: STBC 0x00694c30 _$E352
// FUNCTION: STBC 0x00694ca0 _$E357
// FUNCTION: STBC 0x00694cb0 _$E356
// FUNCTION: STBC 0x00694ce0 _$E361
// FUNCTION: STBC 0x00694cf0 _$E360
// FUNCTION: STBC 0x00694d00 _$E365
// FUNCTION: STBC 0x00694d10 _$E364
// FUNCTION: STBC 0x00694d40 _$E369
// FUNCTION: STBC 0x00694d50 _$E368
// FUNCTION: STBC 0x00694d80 _$E373
// FUNCTION: STBC 0x00694d90 _$E372
// FUNCTION: STBC 0x00694da0 _$E377
// FUNCTION: STBC 0x00694db0 _$E376
// FUNCTION: STBC 0x00694e00 _$E381
// FUNCTION: STBC 0x00694e10 _$E380
// FUNCTION: STBC 0x00694e20 _$E385
// FUNCTION: STBC 0x00694e30 _$E384
// FUNCTION: STBC 0x00694e40 _$E389
// FUNCTION: STBC 0x00694e50 _$E388
// FUNCTION: STBC 0x00694e60 _$E393
// FUNCTION: STBC 0x00694e70 _$E392
// FUNCTION: STBC 0x00694e80 _$E397
// FUNCTION: STBC 0x00694e90 _$E396
// FUNCTION: STBC 0x00694ea0 _$E401
// FUNCTION: STBC 0x00694eb0 _$E400
// FUNCTION: STBC 0x00694ec0 _$E405
// FUNCTION: STBC 0x00694ed0 _$E404
// FUNCTION: STBC 0x00694ee0 _$E409
// FUNCTION: STBC 0x00694ef0 _$E408
// FUNCTION: STBC 0x00694f20 _$E413
// FUNCTION: STBC 0x00694f30 _$E412

/*
================
TorpedoTubeProperty::Cast

The property as a TorpedoTubeProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00694fe0 ?Cast@TorpedoTubeProperty@@SAPAV1@PAVTGModelProperty@@@Z
TorpedoTubeProperty *TorpedoTubeProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( TORPEDOTUBEPROPERTY_TYPE ) ) {
		return (TorpedoTubeProperty *)property;
	}
	return NULL;
}

/*
================
TorpedoTubeProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00695010
TorpedoTubeProperty *TorpedoTubeProperty::Create( char *name ) {
	TorpedoTubeProperty *property = new TorpedoTubeProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
TorpedoTubeProperty::TorpedoTubeProperty
================
*/
// FUNCTION: STBC 0x00695070
TorpedoTubeProperty::TorpedoTubeProperty( long id ) : WeaponProperty( id ) {
	direction = NiPoint3::UNIT_Y;
	right = NiPoint3::UNIT_X;
	immediateDelay = 0.25f;
	reloadDelay = 5.0f;
	maxReady = 3;
	debugNode = NULL;
	damageRadiusFactor = 0.6f;
	dumbfire = true;
}

/*
================
TorpedoTubeProperty::WriteScript

Writes "<variable> = App.TorpedoTubeProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00695190
void TorpedoTubeProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.TorpedoTubeProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
TorpedoTubeProperty::~TorpedoTubeProperty
================
*/
// FUNCTION: STBC 0x006952c0
TorpedoTubeProperty::~TorpedoTubeProperty( void ) {
	if ( debugNode != NULL ) {
		debugNode->GetParent()->DetachChild( debugNode );
		debugNode = NULL;
	}
}

/*
================
TorpedoTubeProperty::Copy
================
*/
// FUNCTION: STBC 0x00695350
bool TorpedoTubeProperty::Copy( TGModelProperty *property ) {
	TorpedoTubeProperty *torpedoTube = Cast( property );
	bool copied = WeaponProperty::Copy( property );
	if ( copied == true ) {
		SetImmediateDelay( torpedoTube->GetImmediateDelay() );
		SetReloadDelay( torpedoTube->GetReloadDelay() );
		SetMaxReady( torpedoTube->GetMaxReady() );
		direction = torpedoTube->GetDirection();
		right = torpedoTube->GetRight();
	}
	return copied;
}

/*
================
TorpedoTubeProperty::VirtualSlot15

Shows the tube's display model under the viewer's root node, building it the
first time, or hides it.
================
*/
// FUNCTION: STBC 0x006953f0
void TorpedoTubeProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
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
	Unknown695560( viewer->GetRootNode() );
}

/*
================
TorpedoTubeProperty::Unknown695560

Builds the tube's display model the first time: a red line along the launch
direction, a third of the root node's bound long, and a green one along the
right axis, the subsystem's radius long, unlit and opaque. Later calls move
the two lines to the present values.
================
*/
// FUNCTION: STBC 0x00695560
// DRAFT: structure and calls match; register allocation (retail keeps 0 in ebx and the flags in a frame slot), and the update branch's vertex copies and smart-pointer release differ in the inline budget (retail: out-of-line operator= and DecRefCount, this build: inline copies and the out-of-line ~NiPointer)
void TorpedoTubeProperty::Unknown695560( NiNode *root ) {
	float length = root->GetWorldBound().GetRadius() * 0.33f;
	NiPoint3 origin = GetPosition();
	if ( debugNode->GetChildCount() == 0 ) {
		NiPoint3 *vertices = new NiPoint3[4];
		vertices[0] = origin;
		vertices[1] = length * direction + origin;
		vertices[2] = origin;
		vertices[3] = radius * right + origin;

		NiColorA *colors = new NiColorA[4];
		colors[0] = NiColorA( 1.0f, 0.0f, 0.0f, 1.0f );
		colors[1] = NiColorA( 1.0f, 0.0f, 0.0f, 1.0f );
		colors[2] = NiColorA( 0.0f, 1.0f, 0.0f, 1.0f );
		colors[3] = NiColorA( 0.0f, 1.0f, 0.0f, 1.0f );

		bool *flags = new bool[4];
		flags[0] = true;
		flags[1] = false;
		flags[2] = true;
		flags[3] = false;

		debugNode->AttachChild( new NiLines( 4, vertices, colors, NULL, flags ), false );

		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		debugNode->AttachProperty( vertexColor );

		NiAlphaProperty *alpha = new NiAlphaProperty;
		alpha->SetAlphaBlending( false );
		debugNode->AttachProperty( alpha );

		debugNode->Update( 0.0f );
		debugNode->UpdateProperties();
	} else {
		NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 0 );
		NiPoint3 *vertices = lines->GetLinesData()->GetVertices();
		vertices[0] = origin;
		vertices[1] = length * direction + origin;
		vertices[2] = origin;
		vertices[3] = right * radius + origin;
		NiBound &bound = lines->GetLinesData()->GetBound();
		bound.radius = length;
		bound.unknown10 = true;
		lines->GetLinesData()->IncRevisionID();
		debugNode->Update( 0.0f );
	}
}

/*
================
TorpedoTubeProperty::WriteInit
================
*/
// FUNCTION: STBC 0x006959f0
void TorpedoTubeProperty::WriteInit( TGStream *stream, char *variableName ) {
	char line[2048];

	WeaponProperty::WriteInit( stream, variableName );

	sprintf( line, "%s.SetImmediateDelay(%f)\n", variableName, GetImmediateDelay() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetReloadDelay(%f)\n", variableName, GetReloadDelay() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetMaxReady(%d)\n", variableName, GetMaxReady() );
	stream->WriteCLine( line );
	sprintf( line, "%sDirection = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sDirection.SetXYZ(%f, %f, %f)\n", variableName, direction.x, direction.y, direction.z );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetDirection(%sDirection)\n", variableName, variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sRight = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sRight.SetXYZ(%f, %f, %f)\n", variableName, right.x, right.y, right.z );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetRight(%sRight)\n", variableName, variableName );
	stream->WriteCLine( line );
}

/*
================
TorpedoTubeProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x00695b90
bool TorpedoTubeProperty::WriteToStream( TGStream *stream ) {
	if ( !WeaponProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( direction.GetX() );
	stream->WriteFloat( direction.GetY() );
	stream->WriteFloat( direction.GetZ() );
	stream->WriteFloat( right.GetX() );
	stream->WriteFloat( right.GetY() );
	stream->WriteFloat( right.GetZ() );
	stream->WriteFloat( immediateDelay );
	stream->WriteFloat( reloadDelay );
	stream->WriteLong( maxReady );
	return true;
}

/*
================
TorpedoTubeProperty::ReadFromStream

The display model is not streamed.
================
*/
// FUNCTION: STBC 0x00695c30
bool TorpedoTubeProperty::ReadFromStream( TGStream *stream ) {
	if ( !WeaponProperty::ReadFromStream( stream ) ) {
		return false;
	}
	debugNode = NULL;
	direction.SetXYZ( stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat() );
	right.SetXYZ( stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat() );
	immediateDelay = stream->ReadFloat();
	reloadDelay = stream->ReadFloat();
	maxReady = stream->ReadLong();
	return true;
}
