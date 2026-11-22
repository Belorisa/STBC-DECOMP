/*
===============================================================================

	PositionOrientationProperty.cpp

===============================================================================
*/

#include <iostream>

#include "PositionOrientationProperty.h"
#include "TGStream.h"

#include <stdio.h>
#include "NiNode.h"
#include "NiLines.h"
#include "NiColor.h"
#include "DisplayModelExtraData.h"
#include "ModelPropertyViewer.h"
#include "NiVertexColorProperty.h"
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00688330 _$E32
// FUNCTION: STBC 0x00688340 _$E29
// FUNCTION: STBC 0x00688350 _$E31
// FUNCTION: STBC 0x00688360 _$E30
// FUNCTION: STBC 0x00688370 _$E38
// FUNCTION: STBC 0x00688380 _$E35
// FUNCTION: STBC 0x00688390 _$E37
// FUNCTION: STBC 0x006883a0 _$E36
// FUNCTION: STBC 0x00688990 _$E434
// FUNCTION: STBC 0x006889b0 _$E433

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006883b0 _$E62
// FUNCTION: STBC 0x006883c0 _$E61
// FUNCTION: STBC 0x006883f0 _$E73
// FUNCTION: STBC 0x00688400 _$E72
// FUNCTION: STBC 0x00688410 _$E77
// FUNCTION: STBC 0x00688420 _$E76
// FUNCTION: STBC 0x00688430 _$E81
// FUNCTION: STBC 0x00688440 _$E80
// FUNCTION: STBC 0x00688450 _$E85
// FUNCTION: STBC 0x00688460 _$E84
// FUNCTION: STBC 0x00688470 _$E204
// FUNCTION: STBC 0x00688480 _$E203
// FUNCTION: STBC 0x00688490 _$E146
// FUNCTION: STBC 0x006884a0 _$E145
// FUNCTION: STBC 0x006884b0 _$E159
// FUNCTION: STBC 0x006884c0 _$E158
// FUNCTION: STBC 0x00688510 _$E213
// FUNCTION: STBC 0x00688520 _$E212
// FUNCTION: STBC 0x006885b0 _$E104
// FUNCTION: STBC 0x006885c0 _$E103
// FUNCTION: STBC 0x006885d0 _$E221
// FUNCTION: STBC 0x006885e0 _$E220
// FUNCTION: STBC 0x006885f0 _$E228
// FUNCTION: STBC 0x00688600 _$E227
// FUNCTION: STBC 0x006887d0 _$E109
// FUNCTION: STBC 0x006887e0 _$E108
// FUNCTION: STBC 0x00688810 _$E114
// FUNCTION: STBC 0x00688820 _$E113
// FUNCTION: STBC 0x006888b0 _$E123
// FUNCTION: STBC 0x006888c0 _$E122
// FUNCTION: STBC 0x006888d0 _$E232
// FUNCTION: STBC 0x006888e0 _$E231
// FUNCTION: STBC 0x006888f0 _$E236
// FUNCTION: STBC 0x00688900 _$E235
// FUNCTION: STBC 0x00688930 _$E129
// FUNCTION: STBC 0x00688940 _$E128
// FUNCTION: STBC 0x00688950 _$E134
// FUNCTION: STBC 0x00688960 _$E133
// FUNCTION: STBC 0x00688970 _$E164
// FUNCTION: STBC 0x00688980 _$E163
// FUNCTION: STBC 0x006889e0 _$E169
// FUNCTION: STBC 0x006889f0 _$E168
// FUNCTION: STBC 0x00688a40 _$E174
// FUNCTION: STBC 0x00688a50 _$E173
// FUNCTION: STBC 0x00688a80 _$E139
// FUNCTION: STBC 0x00688a90 _$E138
// FUNCTION: STBC 0x00688ae0 _$E179
// FUNCTION: STBC 0x00688af0 _$E178
// FUNCTION: STBC 0x00688b00 _$E184
// FUNCTION: STBC 0x00688b10 _$E183
// FUNCTION: STBC 0x00688c20 _$E189
// FUNCTION: STBC 0x00688c30 _$E188
// FUNCTION: STBC 0x00688c60 _$E194
// FUNCTION: STBC 0x00688c70 _$E193
// FUNCTION: STBC 0x00688c80 _$E199
// FUNCTION: STBC 0x00688c90 _$E198
// FUNCTION: STBC 0x00688ca0 _$E417
// FUNCTION: STBC 0x00688cb0 _$E416
// FUNCTION: STBC 0x00688cc0 _$E422
// FUNCTION: STBC 0x00688cd0 _$E421
// FUNCTION: STBC 0x00688ce0 _$E426
// FUNCTION: STBC 0x00688cf0 _$E425
// FUNCTION: STBC 0x006883d0 _$E241
// FUNCTION: STBC 0x006883e0 _$E240
// FUNCTION: STBC 0x006884d0 _$E209
// FUNCTION: STBC 0x006884e0 _$E208
// FUNCTION: STBC 0x006884f0 _$E246
// FUNCTION: STBC 0x00688500 _$E245
// FUNCTION: STBC 0x00688530 _$E250
// FUNCTION: STBC 0x00688540 _$E249
// FUNCTION: STBC 0x00688550 _$E257
// FUNCTION: STBC 0x00688560 _$E256
// FUNCTION: STBC 0x00688570 _$E267
// FUNCTION: STBC 0x00688580 _$E266
// FUNCTION: STBC 0x00688590 _$E271
// FUNCTION: STBC 0x006885a0 _$E270
// FUNCTION: STBC 0x00688610 _$E283
// FUNCTION: STBC 0x00688620 _$E282
// FUNCTION: STBC 0x00688630 _$E287
// FUNCTION: STBC 0x00688640 _$E286
// FUNCTION: STBC 0x00688650 _$E327
// FUNCTION: STBC 0x00688660 _$E326
// FUNCTION: STBC 0x00688670 _$E333
// FUNCTION: STBC 0x00688680 _$E332
// FUNCTION: STBC 0x00688690 _$E275
// FUNCTION: STBC 0x006886a0 _$E274
// FUNCTION: STBC 0x006886b0 _$E279
// FUNCTION: STBC 0x006886c0 _$E278
// FUNCTION: STBC 0x006886d0 _$E291
// FUNCTION: STBC 0x006886e0 _$E290
// FUNCTION: STBC 0x006886f0 _$E295
// FUNCTION: STBC 0x00688700 _$E294
// FUNCTION: STBC 0x00688710 _$E299
// FUNCTION: STBC 0x00688720 _$E298
// FUNCTION: STBC 0x00688730 _$E303
// FUNCTION: STBC 0x00688740 _$E302
// FUNCTION: STBC 0x00688750 _$E307
// FUNCTION: STBC 0x00688760 _$E306
// FUNCTION: STBC 0x00688770 _$E311
// FUNCTION: STBC 0x00688780 _$E310
// FUNCTION: STBC 0x00688790 _$E315
// FUNCTION: STBC 0x006887a0 _$E314
// FUNCTION: STBC 0x006887b0 _$E319
// FUNCTION: STBC 0x006887c0 _$E318
// FUNCTION: STBC 0x006887f0 _$E323
// FUNCTION: STBC 0x00688800 _$E322
// FUNCTION: STBC 0x00688830 _$E337
// FUNCTION: STBC 0x00688840 _$E336
// FUNCTION: STBC 0x00688850 _$E341
// FUNCTION: STBC 0x00688860 _$E340
// FUNCTION: STBC 0x00688870 _$E345
// FUNCTION: STBC 0x00688880 _$E344
// FUNCTION: STBC 0x00688890 _$E349
// FUNCTION: STBC 0x006888a0 _$E348
// FUNCTION: STBC 0x00688910 _$E353
// FUNCTION: STBC 0x00688920 _$E352
// FUNCTION: STBC 0x006889c0 _$E357
// FUNCTION: STBC 0x006889d0 _$E356
// FUNCTION: STBC 0x00688a00 _$E361
// FUNCTION: STBC 0x00688a10 _$E360
// FUNCTION: STBC 0x00688a20 _$E365
// FUNCTION: STBC 0x00688a30 _$E364
// FUNCTION: STBC 0x00688a60 _$E369
// FUNCTION: STBC 0x00688a70 _$E368
// FUNCTION: STBC 0x00688aa0 _$E373
// FUNCTION: STBC 0x00688ab0 _$E372
// FUNCTION: STBC 0x00688ac0 _$E377
// FUNCTION: STBC 0x00688ad0 _$E376
// FUNCTION: STBC 0x00688b20 _$E381
// FUNCTION: STBC 0x00688b30 _$E380
// FUNCTION: STBC 0x00688b40 _$E385
// FUNCTION: STBC 0x00688b50 _$E384
// FUNCTION: STBC 0x00688b60 _$E389
// FUNCTION: STBC 0x00688b70 _$E388
// FUNCTION: STBC 0x00688b80 _$E393
// FUNCTION: STBC 0x00688b90 _$E392
// FUNCTION: STBC 0x00688ba0 _$E397
// FUNCTION: STBC 0x00688bb0 _$E396
// FUNCTION: STBC 0x00688bc0 _$E401
// FUNCTION: STBC 0x00688bd0 _$E400
// FUNCTION: STBC 0x00688be0 _$E405
// FUNCTION: STBC 0x00688bf0 _$E404
// FUNCTION: STBC 0x00688c00 _$E409
// FUNCTION: STBC 0x00688c10 _$E408
// FUNCTION: STBC 0x00688c40 _$E413
// FUNCTION: STBC 0x00688c50 _$E412

/*
================
PositionOrientationProperty::Cast

The property as a PositionOrientationProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00688d00 ?Cast@PositionOrientationProperty@@SAPAV1@PAVTGModelProperty@@@Z
PositionOrientationProperty *PositionOrientationProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( POSITIONORIENTATIONPROPERTY_TYPE ) ) {
		return (PositionOrientationProperty *)property;
	}
	return NULL;
}

/*
================
PositionOrientationProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00688d30
PositionOrientationProperty *PositionOrientationProperty::Create( char *name ) {
	PositionOrientationProperty *property = new PositionOrientationProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
PositionOrientationProperty::PositionOrientationProperty
================
*/
// FUNCTION: STBC 0x00688d80
PositionOrientationProperty::PositionOrientationProperty( long id ) : TGModelProperty( id ) {
	forward = NiPoint3::UNIT_Y;
	right = NiPoint3::UNIT_X;
	up = NiPoint3::UNIT_Z;
	position = NiPoint3::ZERO;
	debugNode = NULL;
}

/*
================
PositionOrientationProperty::WriteScript

Writes "<variable> = App.PositionOrientationProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00688e80
void PositionOrientationProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.PositionOrientationProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
PositionOrientationProperty::~PositionOrientationProperty
================
*/
// FUNCTION: STBC 0x00688fb0
PositionOrientationProperty::~PositionOrientationProperty( void ) {
	if ( debugNode != NULL ) {
		debugNode->GetParent()->DetachChild( debugNode );
		debugNode = NULL;
	}
}

/*
================
PositionOrientationProperty::WriteInit

The orientation and position go through script TGPoint3s named after the
variable.
================
*/
// FUNCTION: STBC 0x00689040
void PositionOrientationProperty::WriteInit( TGStream *stream, char *variableName ) {
	char *line = new char[2048];

	sprintf( line, "%sForward = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sForward.SetXYZ(%f, %f, %f)\n", variableName, forward.x, forward.y, forward.z );
	stream->WriteCLine( line );
	sprintf( line, "%sUp = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sUp.SetXYZ(%f, %f, %f)\n", variableName, up.x, up.y, up.z );
	stream->WriteCLine( line );
	sprintf( line, "%sRight = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sRight.SetXYZ(%f, %f, %f)\n", variableName, right.x, right.y, right.z );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetOrientation(%sForward, %sUp, %sRight)\n", variableName, variableName, variableName, variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sPosition = App.TGPoint3()\n", variableName );
	stream->WriteCLine( line );
	sprintf( line, "%sPosition.SetXYZ(%f, %f, %f)\n", variableName, position.x, position.y, position.z );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPosition(%sPosition)\n", variableName, variableName );
	stream->WriteCLine( line );

	delete line;
}

/*
================
PositionOrientationProperty::VirtualSlot15

Shows the property's display model under the viewer's root node, building
it the first time, or hides it.
================
*/
// FUNCTION: STBC 0x006891e0
void PositionOrientationProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
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
	Unknown689410( viewer->GetRootNode() );
}

/*
================
PositionOrientationProperty::Copy
================
*/
// FUNCTION: STBC 0x00689350
bool PositionOrientationProperty::Copy( TGModelProperty *property ) {
	PositionOrientationProperty *positionOrientation = Cast( property );
	bool copied = TGModelProperty::Copy( property );
	if ( copied == true ) {
		SetOrientation( positionOrientation->GetForward(), positionOrientation->GetUp(), positionOrientation->GetRight() );
		SetPosition( positionOrientation->GetPosition() );
	}
	return copied;
}

/*
================
PositionOrientationProperty::Unknown689410

Builds the display model the first time: an unlit copy of the geometry
Unknown689530 makes, sized to a third of the root node's bound; later calls
resize it with Unknown689830.
================
*/
// FUNCTION: STBC 0x00689410
void PositionOrientationProperty::Unknown689410( NiNode *root ) {
	float size = root->GetWorldBound().GetRadius() * 0.33f;
	if ( debugNode->GetChildCount() == 0 ) {
		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		debugNode->AttachProperty( vertexColor );

		debugNode->AttachChild( Unknown689530( size ), false );
		debugNode->Update( 0.0f );
		debugNode->UpdateProperties();
	} else {
		Unknown689830( size );
		debugNode->Update( 0.0f );
	}
}

/*
================
PositionOrientationProperty::Unknown689530

The display geometry: three lines of the given length from the position along
the forward, up and right axes, in red, green and blue.
================
*/
// FUNCTION: STBC 0x00689530
// RULE-EXCEPTION: R35, the flag loop is braceless: retail's NiColorA call set needs own cost 565 or 566 (braced 573)
NiAVObject *PositionOrientationProperty::Unknown689530( float size ) {
	NiPoint3 *vertices = new NiPoint3[6];
	vertices[0] = position;
	vertices[1] = position + size * forward;
	vertices[2] = position;
	vertices[3] = position + size * up;
	vertices[4] = position;
	vertices[5] = position + size * right;

	NiColorA *colors = new NiColorA[6];
	colors[0] = NiColorA( 1.0f, 0.0f, 0.0f, 1.0f );
	colors[1] = colors[0];
	colors[2] = NiColorA( 0.0f, 1.0f, 0.0f, 1.0f );
	colors[3] = colors[2];
	colors[4] = NiColorA( 0.0f, 0.0f, 1.0f, 1.0f );
	colors[5] = colors[4];

	bool *flags = new bool[6];
	for ( int i = 0; i < 6; i++ )
		// MATCH-PENDING: R17, the parity is written !( i % 2 ) only for the inline budget (own cost 565); the family writes i % 2 == 0 (567), so the original's other 2 tuples are still unknown
		if ( !( i % 2 ) )
			flags[i] = true;
		else
			flags[i] = false;

	NiLines *lines = new NiLines( 6, vertices, colors, NULL, flags );
	NiBound &bound = lines->GetLinesData()->GetBound();
	bound.radius = size;
	bound.unknown10 = true;
	return lines;
}

/*
================
PositionOrientationProperty::Unknown689830

Moves the display geometry's three lines to the current position and axes at
the given length, and marks the geometry changed.
================
*/
// FUNCTION: STBC 0x00689830
// PARKED: PCH boundary: exact only with the unit's leading include block inside the precompiled block (/YX); the game units build without a precompiled header (accepted loss, user's ruling 2026-11-20; docs/c2-backend.md, "A/B over the tree")
void PositionOrientationProperty::Unknown689830( float size ) {
	NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 0 );

	NiPoint3 *vertices = lines->GetLinesData()->GetVertices();
	vertices[0] = position;
	vertices[1] = size * forward + position;
	vertices[2] = position;
	vertices[3] = size * up + position;
	vertices[4] = position;
	vertices[5] = size * right + position;

	NiBound &bound = lines->GetLinesData()->GetBound();
	bound.radius = size;
	bound.unknown10 = true;
	lines->GetLinesData()->IncRevisionID();
}

/*
================
PositionOrientationProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x00689a60
bool PositionOrientationProperty::WriteToStream( TGStream *stream ) {
	if ( !TGModelProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( forward.x );
	stream->WriteFloat( forward.y );
	stream->WriteFloat( forward.z );
	stream->WriteFloat( up.x );
	stream->WriteFloat( up.y );
	stream->WriteFloat( up.z );
	stream->WriteFloat( right.x );
	stream->WriteFloat( right.y );
	stream->WriteFloat( right.z );
	stream->WriteFloat( position.x );
	stream->WriteFloat( position.y );
	stream->WriteFloat( position.z );
	return true;
}

/*
================
PositionOrientationProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00689b10
bool PositionOrientationProperty::ReadFromStream( TGStream *stream ) {
	if ( !TGModelProperty::ReadFromStream( stream ) ) {
		return false;
	}
	forward.x = stream->ReadFloat();
	forward.y = stream->ReadFloat();
	forward.z = stream->ReadFloat();
	up.x = stream->ReadFloat();
	up.y = stream->ReadFloat();
	up.z = stream->ReadFloat();
	right.x = stream->ReadFloat();
	right.y = stream->ReadFloat();
	right.z = stream->ReadFloat();
	position.x = stream->ReadFloat();
	position.y = stream->ReadFloat();
	position.z = stream->ReadFloat();
	return true;
}
