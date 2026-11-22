/*
===============================================================================

	SubsystemProperty.cpp

===============================================================================
*/

#include <iostream>

#include "SubsystemProperty.h"
#include "TGStream.h"
#include "NiNode.h"
#include "DisplayModelExtraData.h"
#include "ModelPropertyViewer.h"
#include "NiVertexColorProperty.h"
#include "NiAlphaProperty.h"

#include <stdio.h>
#include "TGModelProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00691a20 _$E32
// FUNCTION: STBC 0x00691a30 _$E29
// FUNCTION: STBC 0x00691a40 _$E31
// FUNCTION: STBC 0x00691a50 _$E30
// FUNCTION: STBC 0x00691a60 _$E38
// FUNCTION: STBC 0x00691a70 _$E35
// FUNCTION: STBC 0x00691a80 _$E37
// FUNCTION: STBC 0x00691a90 _$E36
// FUNCTION: STBC 0x00691ea0 _$E434
// FUNCTION: STBC 0x00691ec0 _$E433

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00691aa0 _$E65
// FUNCTION: STBC 0x00691ab0 _$E64
// FUNCTION: STBC 0x00691ae0 _$E76
// FUNCTION: STBC 0x00691af0 _$E75
// FUNCTION: STBC 0x00691b00 _$E80
// FUNCTION: STBC 0x00691b10 _$E79
// FUNCTION: STBC 0x00691b20 _$E84
// FUNCTION: STBC 0x00691b30 _$E83
// FUNCTION: STBC 0x00691b40 _$E88
// FUNCTION: STBC 0x00691b50 _$E87
// FUNCTION: STBC 0x00691b60 _$E207
// FUNCTION: STBC 0x00691b70 _$E206
// FUNCTION: STBC 0x00691b80 _$E149
// FUNCTION: STBC 0x00691b90 _$E148
// FUNCTION: STBC 0x00691ba0 _$E162
// FUNCTION: STBC 0x00691bb0 _$E161
// FUNCTION: STBC 0x00691c00 _$E216
// FUNCTION: STBC 0x00691c10 _$E215
// FUNCTION: STBC 0x00691ca0 _$E107
// FUNCTION: STBC 0x00691cb0 _$E106
// FUNCTION: STBC 0x00691ef0 _$E112
// FUNCTION: STBC 0x00691f00 _$E111
// FUNCTION: STBC 0x00691f30 _$E117
// FUNCTION: STBC 0x00691f40 _$E116
// FUNCTION: STBC 0x00691fd0 _$E126
// FUNCTION: STBC 0x00691fe0 _$E125
// FUNCTION: STBC 0x00692050 _$E132
// FUNCTION: STBC 0x00692060 _$E131
// FUNCTION: STBC 0x00692070 _$E137
// FUNCTION: STBC 0x00692080 _$E136
// FUNCTION: STBC 0x00692090 _$E167
// FUNCTION: STBC 0x006920a0 _$E166
// FUNCTION: STBC 0x006920d0 _$E172
// FUNCTION: STBC 0x006920e0 _$E171
// FUNCTION: STBC 0x00692130 _$E177
// FUNCTION: STBC 0x00692140 _$E176
// FUNCTION: STBC 0x00692170 _$E142
// FUNCTION: STBC 0x00692180 _$E141
// FUNCTION: STBC 0x006921d0 _$E182
// FUNCTION: STBC 0x006921e0 _$E181
// FUNCTION: STBC 0x006921f0 _$E187
// FUNCTION: STBC 0x00692200 _$E186
// FUNCTION: STBC 0x00692310 _$E192
// FUNCTION: STBC 0x00692320 _$E191
// FUNCTION: STBC 0x00692350 _$E197
// FUNCTION: STBC 0x00692360 _$E196
// FUNCTION: STBC 0x00692370 _$E202
// FUNCTION: STBC 0x00692380 _$E201
// FUNCTION: STBC 0x00692390 _$E417
// FUNCTION: STBC 0x006923a0 _$E416
// FUNCTION: STBC 0x006923b0 _$E422
// FUNCTION: STBC 0x006923c0 _$E421
// FUNCTION: STBC 0x006923d0 _$E426
// FUNCTION: STBC 0x006923e0 _$E425
// FUNCTION: STBC 0x00691cc0 _$E245
// FUNCTION: STBC 0x00691cd0 _$E244
// FUNCTION: STBC 0x00691ce0 _$E252
// FUNCTION: STBC 0x00691cf0 _$E251
// FUNCTION: STBC 0x00691ff0 _$E345
// FUNCTION: STBC 0x00692000 _$E344
// FUNCTION: STBC 0x00692010 _$E349
// FUNCTION: STBC 0x00692020 _$E348
// FUNCTION: STBC 0x00691ac0 _$E221
// FUNCTION: STBC 0x00691ad0 _$E220
// FUNCTION: STBC 0x00691bc0 _$E212
// FUNCTION: STBC 0x00691bd0 _$E211
// FUNCTION: STBC 0x00691be0 _$E226
// FUNCTION: STBC 0x00691bf0 _$E225
// FUNCTION: STBC 0x00691c20 _$E230
// FUNCTION: STBC 0x00691c30 _$E229
// FUNCTION: STBC 0x00691c40 _$E237
// FUNCTION: STBC 0x00691c50 _$E236
// FUNCTION: STBC 0x00691c60 _$E259
// FUNCTION: STBC 0x00691c70 _$E258
// FUNCTION: STBC 0x00691c80 _$E263
// FUNCTION: STBC 0x00691c90 _$E262
// FUNCTION: STBC 0x00691d00 _$E275
// FUNCTION: STBC 0x00691d10 _$E274
// FUNCTION: STBC 0x00691d20 _$E279
// FUNCTION: STBC 0x00691d30 _$E278
// FUNCTION: STBC 0x00691d40 _$E319
// FUNCTION: STBC 0x00691d50 _$E318
// FUNCTION: STBC 0x00691d60 _$E325
// FUNCTION: STBC 0x00691d70 _$E324
// FUNCTION: STBC 0x00691d80 _$E267
// FUNCTION: STBC 0x00691d90 _$E266
// FUNCTION: STBC 0x00691da0 _$E271
// FUNCTION: STBC 0x00691db0 _$E270
// FUNCTION: STBC 0x00691dc0 _$E283
// FUNCTION: STBC 0x00691dd0 _$E282
// FUNCTION: STBC 0x00691de0 _$E287
// FUNCTION: STBC 0x00691df0 _$E286
// FUNCTION: STBC 0x00691e00 _$E291
// FUNCTION: STBC 0x00691e10 _$E290
// FUNCTION: STBC 0x00691e20 _$E295
// FUNCTION: STBC 0x00691e30 _$E294
// FUNCTION: STBC 0x00691e40 _$E299
// FUNCTION: STBC 0x00691e50 _$E298
// FUNCTION: STBC 0x00691e60 _$E303
// FUNCTION: STBC 0x00691e70 _$E302
// FUNCTION: STBC 0x00691e80 _$E307
// FUNCTION: STBC 0x00691e90 _$E306
// FUNCTION: STBC 0x00691ed0 _$E311
// FUNCTION: STBC 0x00691ee0 _$E310
// FUNCTION: STBC 0x00691f10 _$E315
// FUNCTION: STBC 0x00691f20 _$E314
// FUNCTION: STBC 0x00691f50 _$E329
// FUNCTION: STBC 0x00691f60 _$E328
// FUNCTION: STBC 0x00691f70 _$E333
// FUNCTION: STBC 0x00691f80 _$E332
// FUNCTION: STBC 0x00691f90 _$E337
// FUNCTION: STBC 0x00691fa0 _$E336
// FUNCTION: STBC 0x00691fb0 _$E341
// FUNCTION: STBC 0x00691fc0 _$E340
// FUNCTION: STBC 0x00692030 _$E353
// FUNCTION: STBC 0x00692040 _$E352
// FUNCTION: STBC 0x006920b0 _$E357
// FUNCTION: STBC 0x006920c0 _$E356
// FUNCTION: STBC 0x006920f0 _$E361
// FUNCTION: STBC 0x00692100 _$E360
// FUNCTION: STBC 0x00692110 _$E365
// FUNCTION: STBC 0x00692120 _$E364
// FUNCTION: STBC 0x00692150 _$E369
// FUNCTION: STBC 0x00692160 _$E368
// FUNCTION: STBC 0x00692190 _$E373
// FUNCTION: STBC 0x006921a0 _$E372
// FUNCTION: STBC 0x006921b0 _$E377
// FUNCTION: STBC 0x006921c0 _$E376
// FUNCTION: STBC 0x00692210 _$E381
// FUNCTION: STBC 0x00692220 _$E380
// FUNCTION: STBC 0x00692230 _$E385
// FUNCTION: STBC 0x00692240 _$E384
// FUNCTION: STBC 0x00692250 _$E389
// FUNCTION: STBC 0x00692260 _$E388
// FUNCTION: STBC 0x00692270 _$E393
// FUNCTION: STBC 0x00692280 _$E392
// FUNCTION: STBC 0x00692290 _$E397
// FUNCTION: STBC 0x006922a0 _$E396
// FUNCTION: STBC 0x006922b0 _$E401
// FUNCTION: STBC 0x006922c0 _$E400
// FUNCTION: STBC 0x006922d0 _$E405
// FUNCTION: STBC 0x006922e0 _$E404
// FUNCTION: STBC 0x006922f0 _$E409
// FUNCTION: STBC 0x00692300 _$E408
// FUNCTION: STBC 0x00692330 _$E413
// FUNCTION: STBC 0x00692340 _$E412

/*
================
SubsystemProperty::Cast

The property as a SubsystemProperty; NULL when it is none.
================
*/
// FUNCTION: STBC 0x006923f0 ?Cast@SubsystemProperty@@SAPAV1@PAVTGModelProperty@@@Z
SubsystemProperty *SubsystemProperty::Cast( TGModelProperty *property ) {
	if ( property != NULL && property->IsTypeOf( SUBSYSTEMPROPERTY_TYPE ) ) {
		return (SubsystemProperty *)property;
	}
	return NULL;
}

/*
================
SubsystemProperty::Create

A new property of the name.
================
*/
// FUNCTION: STBC 0x00692420
SubsystemProperty *SubsystemProperty::Create( char *name ) {
	SubsystemProperty *property = new SubsystemProperty;
	property->SetName( TGString( name ) );
	return property;
}

/*
================
SubsystemProperty::SubsystemProperty
================
*/
// FUNCTION: STBC 0x00692470
SubsystemProperty::SubsystemProperty( long id ) : TGModelProperty( id ) {
	maxCondition = 200.0f;
	critical = false;
	targetable = true;
	primary = true;
	position = NiPoint3::ZERO;
	position2D = NiPoint2::ZERO;
	debugNode = NULL;
	repairComplexity = 1.0f;
	disabledPercentage = 0.5f;
	radius = 0.25f;
}

/*
================
SubsystemProperty::WriteScript

Writes "<variable> = App.SubsystemProperty_Create("<name>")", the variable being the
name without its spaces, then the lines of WriteInit.
================
*/
// FUNCTION: STBC 0x00692550
void SubsystemProperty::WriteScript( TGStream *stream ) {
	char *propertyName = name.Unknown6F56F0();
	TGString variable( propertyName );
	int space;
	while ( ( space = variable.FindC( " " ) ) != -1 ) {
		variable.Unknown6F55C0( space, 1 );
	}
	char *variableName = variable.Unknown6F56F0();

	stream->WriteCLine( variableName );
	stream->WriteCLine( " = App.SubsystemProperty_Create(" );
	stream->WriteCLine( "\"" );
	stream->WriteCLine( propertyName );
	stream->WriteCLine( "\")\n\n" );
	WriteInit( stream, variableName );

	delete[] variableName;
	delete[] propertyName;
}

/*
================
SubsystemProperty::~SubsystemProperty

Detaches the debug node from its parent.
================
*/
// FUNCTION: STBC 0x00692680
SubsystemProperty::~SubsystemProperty( void ) {
	if ( debugNode != NULL ) {
		debugNode->GetParent()->DetachChild( debugNode );
		debugNode = NULL;
	}
}

/*
================
SubsystemProperty::Copy
================
*/
// FUNCTION: STBC 0x00692710
bool SubsystemProperty::Copy( TGModelProperty *property ) {
	SubsystemProperty *subsystem = Cast( property );
	bool copied = TGModelProperty::Copy( property );
	if ( copied == true ) {
		SetMaxCondition( subsystem->GetMaxCondition() );
		SetCritical( subsystem->IsCritical() );
		SetTargetable( subsystem->IsTargetable() );
		SetPrimary( subsystem->IsPrimary() );
		SetPosition( subsystem->GetPosition() );
		SetPosition2D( subsystem->GetPosition2D() );
		SetRepairComplexity( subsystem->GetRepairComplexity() );
		SetDisabledPercentage( subsystem->GetDisabledPercentage() );
		SetRadius( subsystem->GetRadius() );
	}
	return copied;
}

/*
================
SubsystemProperty::WriteInit
================
*/
// FUNCTION: STBC 0x006927e0
void SubsystemProperty::WriteInit( TGStream *stream, char *variableName ) {
	char *line = new char[2048];
	NiPoint3 position = GetPosition();
	NiPoint2 position2D = GetPosition2D();

	sprintf( line, "%s.SetMaxCondition(%f)\n", variableName, GetMaxCondition() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetCritical(%d)\n", variableName, IsCritical() == true );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetTargetable(%d)\n", variableName, IsTargetable() == true );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPrimary(%d)\n", variableName, IsPrimary() == true );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPosition(%f, %f, %f)\n", variableName, position.x, position.y, position.z );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetPosition2D(%f, %f)\n", variableName, position2D.x, position2D.y );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetRepairComplexity(%f)\n", variableName, GetRepairComplexity() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetDisabledPercentage(%f)\n", variableName, GetDisabledPercentage() );
	stream->WriteCLine( line );
	sprintf( line, "%s.SetRadius(%f)\n", variableName, GetRadius() );
	stream->WriteCLine( line );

	delete line;
}

/*
================
SubsystemProperty::VirtualSlot15

Shows the subsystem's display model under the viewer's root node, building
it the first time, or hides it.
================
*/
// FUNCTION: STBC 0x006929a0
void SubsystemProperty::VirtualSlot15( bool show, ModelPropertyViewer *viewer, NiNode *parent ) {
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
	Unknown692B10( viewer->GetRootNode() );
}

/*
================
SubsystemProperty::Unknown692B10

Builds the display model the first time: an unlit, opaque copy of the
geometry Unknown692CC0 makes for the subsystem's radius; later calls resize
it with Unknown6930B0. The root node goes unused.
================
*/
// FUNCTION: STBC 0x00692b10
// PARKED: retail copies the radius through the FPU (the open subsystem-group FPU-copy wall)
void SubsystemProperty::Unknown692B10( NiNode *root ) {
	float radius = GetRadius();
	if ( debugNode->GetChildCount() == 0 ) {
		NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
		debugNode->AttachProperty( vertexColor );

		NiAlphaProperty *alpha = new NiAlphaProperty;
		alpha->SetAlphaBlending( false );
		debugNode->AttachProperty( alpha );

		debugNode->AttachChild( Unknown692CC0( radius ), false );
		debugNode->Update( 0.0f );
		debugNode->UpdateProperties();
	} else {
		Unknown6930B0( radius );
		debugNode->Update( 0.0f );
	}
}

/*
================
SubsystemProperty::Unknown692CC0

The display geometry: six white lines of the given length from the position,
along each axis both ways.
================
*/
// FUNCTION: STBC 0x00692cc0
NiAVObject *SubsystemProperty::Unknown692CC0( float radius ) {
	NiPoint3 *vertices = new NiPoint3[12];
	vertices[0] = position;
	vertices[1] = NiPoint3::UNIT_Y * radius + position;
	vertices[2] = position;
	vertices[3] = NiPoint3::UNIT_Z * radius + position;
	vertices[4] = position;
	vertices[5] = NiPoint3::UNIT_X * radius + position;
	vertices[6] = position;
	vertices[7] = position - NiPoint3::UNIT_Y * radius;
	vertices[8] = position;
	vertices[9] = position - NiPoint3::UNIT_Z * radius;
	vertices[10] = position;
	vertices[11] = position - NiPoint3::UNIT_X * radius;

	NiColorA *colors = new NiColorA[12];
	int i;
	for ( i = 0; i < 12; i++ ) {
		colors[i] = NiColorA( 1.0f, 1.0f, 1.0f, 1.0f );
	}

	// room for 24 flags; the lines use 12
	bool *flags = new bool[24];
	for ( i = 0; i < 12; i++ ) {
		if ( i % 2 == 0 ) {
			flags[i] = true;
		} else {
			flags[i] = false;
		}
	}

	NiLines *lines = new NiLines( 12, vertices, colors, NULL, flags );
	NiBound &bound = lines->GetLinesData()->GetBound();
	bound.radius = 2.0f;
	bound.unknown10 = true;
	return lines;
}

/*
================
SubsystemProperty::Unknown6930B0

Moves the six lines of Unknown692CC0 to the present position at the given
length.
================
*/
// FUNCTION: STBC 0x006930b0
void SubsystemProperty::Unknown6930B0( float radius ) {
	NiPointer<NiLines> lines = (NiLines *)debugNode->GetAt( 0 );
	NiPoint3 *vertices = lines->GetLinesData()->GetVertices();
	vertices[0] = position;
	vertices[1] = NiPoint3::UNIT_Y * radius + position;
	vertices[2] = position;
	vertices[3] = NiPoint3::UNIT_Z * radius + position;
	vertices[4] = position;
	vertices[5] = NiPoint3::UNIT_X * radius + position;
	vertices[6] = position;
	vertices[7] = position - NiPoint3::UNIT_Y * radius;
	vertices[8] = position;
	vertices[9] = position - NiPoint3::UNIT_Z * radius;
	vertices[10] = position;
	vertices[11] = position - NiPoint3::UNIT_X * radius;
	NiBound &bound = lines->GetLinesData()->GetBound();
	bound.radius = radius;
	bound.unknown10 = true;
	lines->GetLinesData()->IncRevisionID();
}

/*
================
SubsystemProperty::WriteToStream
================
*/
// FUNCTION: STBC 0x006933b0
bool SubsystemProperty::WriteToStream( TGStream *stream ) {
	if ( !TGModelProperty::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteFloat( maxCondition );
	stream->WriteBool( critical );
	stream->WriteBool( targetable );
	stream->WriteBool( primary );
	stream->WriteFloat( position.x );
	stream->WriteFloat( position.y );
	stream->WriteFloat( position.z );
	stream->WriteFloat( position2D.x );
	stream->WriteFloat( position2D.y );
	stream->WriteFloat( repairComplexity );
	stream->WriteFloat( disabledPercentage );
	stream->WriteFloat( radius );
	return true;
}

/*
================
SubsystemProperty::ReadFromStream
================
*/
// FUNCTION: STBC 0x00693460
bool SubsystemProperty::ReadFromStream( TGStream *stream ) {
	if ( !TGModelProperty::ReadFromStream( stream ) ) {
		return false;
	}
	maxCondition = stream->ReadFloat();
	critical = stream->ReadBool();
	targetable = stream->ReadBool();
	primary = stream->ReadBool();
	position.x = stream->ReadFloat();
	position.y = stream->ReadFloat();
	position.z = stream->ReadFloat();
	position2D.x = stream->ReadFloat();
	position2D.y = stream->ReadFloat();
	repairComplexity = stream->ReadFloat();
	disabledPercentage = stream->ReadFloat();
	radius = stream->ReadFloat();
	return true;
}
