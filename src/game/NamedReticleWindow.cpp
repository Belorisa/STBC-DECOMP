/*
===============================================================================

	NamedReticleWindow.cpp

	The reticle with a name: the target reticles the map window adds.

===============================================================================
*/

#include "ReticleWindow.h"
#include "BaseObjectClass.h"
#include "GraphicsModeInfo.h"
#include "ObjectGroup.h"
#include "SensorSubsystem.h"
#include "STColors.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIcon.h"
#include "TGIconManager.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGObjPtrEvent.h"
#include "TGStream.h"
#include "Game.h"
#include "ShipClass.h"
#include "HullClass.h"
#include "ReticleManagerWindow.h"
#include "SetManager.h"
#include "TGParagraph.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00511410 _$E498
// FUNCTION: STBC 0x00511430 _$E497

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0050fdf0 _$E33
// FUNCTION: STBC 0x0050fe00 _$E32
// FUNCTION: STBC 0x0050fe10 _$E37
// FUNCTION: STBC 0x0050fe20 _$E36
// FUNCTION: STBC 0x0050fe30 _$E48
// FUNCTION: STBC 0x0050fe40 _$E47
// FUNCTION: STBC 0x005102f0 _$E52
// FUNCTION: STBC 0x00510300 _$E51
// FUNCTION: STBC 0x00510830 _$E56
// FUNCTION: STBC 0x00510840 _$E55
// FUNCTION: STBC 0x00510850 _$E60
// FUNCTION: STBC 0x00510860 _$E59
// FUNCTION: STBC 0x00510b40 _$E184
// FUNCTION: STBC 0x00510b50 _$E183
// FUNCTION: STBC 0x00510c30 _$E126
// FUNCTION: STBC 0x00510c40 _$E125
// FUNCTION: STBC 0x00510c50 _$E139
// FUNCTION: STBC 0x00510c60 _$E138
// FUNCTION: STBC 0x00510c70 _$E189
// FUNCTION: STBC 0x00510c80 _$E188
// FUNCTION: STBC 0x00510c90 _$E194
// FUNCTION: STBC 0x00510ca0 _$E193
// FUNCTION: STBC 0x00510cb0 _$E198
// FUNCTION: STBC 0x00510cc0 _$E197
// FUNCTION: STBC 0x00510cd0 _$E202
// FUNCTION: STBC 0x00510ce0 _$E201
// FUNCTION: STBC 0x00510cf0 _$E209
// FUNCTION: STBC 0x00510d00 _$E208
// FUNCTION: STBC 0x00510d10 _$E334
// FUNCTION: STBC 0x00510d20 _$E333
// FUNCTION: STBC 0x00510d30 _$E338
// FUNCTION: STBC 0x00510d40 _$E337
// FUNCTION: STBC 0x00510d50 _$E84
// FUNCTION: STBC 0x00510d60 _$E83
// FUNCTION: STBC 0x00510d70 _$E271
// FUNCTION: STBC 0x00510d80 _$E270
// FUNCTION: STBC 0x00510d90 _$E278
// FUNCTION: STBC 0x00510da0 _$E277
// FUNCTION: STBC 0x00510db0 _$E282
// FUNCTION: STBC 0x00510dc0 _$E281
// FUNCTION: STBC 0x00510dd0 _$E286
// FUNCTION: STBC 0x00510de0 _$E285
// FUNCTION: STBC 0x00510df0 _$E290
// FUNCTION: STBC 0x00510e00 _$E289
// FUNCTION: STBC 0x00510e10 _$E296
// FUNCTION: STBC 0x00510e20 _$E295
// FUNCTION: STBC 0x00510e30 _$E342
// FUNCTION: STBC 0x00510e40 _$E341
// FUNCTION: STBC 0x00510e50 _$E346
// FUNCTION: STBC 0x00510e60 _$E345
// FUNCTION: STBC 0x00510e70 _$E350
// FUNCTION: STBC 0x00510e80 _$E349
// FUNCTION: STBC 0x00510e90 _$E354
// FUNCTION: STBC 0x00510ea0 _$E353
// FUNCTION: STBC 0x00510eb0 _$E358
// FUNCTION: STBC 0x00510ec0 _$E357
// FUNCTION: STBC 0x00510ed0 _$E362
// FUNCTION: STBC 0x00510ee0 _$E361
// FUNCTION: STBC 0x00510ef0 _$E366
// FUNCTION: STBC 0x00510f00 _$E365
// FUNCTION: STBC 0x00510f10 _$E370
// FUNCTION: STBC 0x00510f20 _$E369
// FUNCTION: STBC 0x00510f30 _$E374
// FUNCTION: STBC 0x00510f40 _$E373
// FUNCTION: STBC 0x00510f50 _$E378
// FUNCTION: STBC 0x00510f60 _$E377
// FUNCTION: STBC 0x00510f70 _$E89
// FUNCTION: STBC 0x00510f80 _$E88
// FUNCTION: STBC 0x00510f90 _$E382
// FUNCTION: STBC 0x00510fa0 _$E381
// FUNCTION: STBC 0x00510fb0 _$E94
// FUNCTION: STBC 0x00510fc0 _$E93
// FUNCTION: STBC 0x00510fd0 _$E386
// FUNCTION: STBC 0x00510fe0 _$E385
// FUNCTION: STBC 0x00510ff0 _$E390
// FUNCTION: STBC 0x00511000 _$E389
// FUNCTION: STBC 0x00511010 _$E394
// FUNCTION: STBC 0x00511020 _$E393
// FUNCTION: STBC 0x00511030 _$E398
// FUNCTION: STBC 0x00511040 _$E397
// FUNCTION: STBC 0x00511050 _$E103
// FUNCTION: STBC 0x00511060 _$E102
// FUNCTION: STBC 0x00511070 _$E402
// FUNCTION: STBC 0x00511080 _$E401
// FUNCTION: STBC 0x00511090 _$E406
// FUNCTION: STBC 0x005110a0 _$E405
// FUNCTION: STBC 0x005110b0 _$E410
// FUNCTION: STBC 0x005110c0 _$E409
// FUNCTION: STBC 0x005110d0 _$E109
// FUNCTION: STBC 0x005110e0 _$E108
// FUNCTION: STBC 0x005110f0 _$E114
// FUNCTION: STBC 0x00511100 _$E113
// FUNCTION: STBC 0x00511110 _$E144
// FUNCTION: STBC 0x00511120 _$E143
// FUNCTION: STBC 0x00511130 _$E414
// FUNCTION: STBC 0x00511140 _$E413
// FUNCTION: STBC 0x00511150 _$E149
// FUNCTION: STBC 0x00511160 _$E148
// FUNCTION: STBC 0x00511170 _$E418
// FUNCTION: STBC 0x00511180 _$E417
// FUNCTION: STBC 0x00511190 _$E422
// FUNCTION: STBC 0x005111a0 _$E421
// FUNCTION: STBC 0x005111b0 _$E154
// FUNCTION: STBC 0x005111c0 _$E153
// FUNCTION: STBC 0x005111d0 _$E426
// FUNCTION: STBC 0x005111e0 _$E425
// FUNCTION: STBC 0x005111f0 _$E119
// FUNCTION: STBC 0x00511200 _$E118
// FUNCTION: STBC 0x00511210 _$E430
// FUNCTION: STBC 0x00511220 _$E429
// FUNCTION: STBC 0x00511230 _$E434
// FUNCTION: STBC 0x00511240 _$E433
// FUNCTION: STBC 0x00511250 _$E159
// FUNCTION: STBC 0x00511260 _$E158
// FUNCTION: STBC 0x00511270 _$E164
// FUNCTION: STBC 0x00511280 _$E163
// FUNCTION: STBC 0x00511290 _$E438
// FUNCTION: STBC 0x005112a0 _$E437
// FUNCTION: STBC 0x005112b0 _$E442
// FUNCTION: STBC 0x005112c0 _$E441
// FUNCTION: STBC 0x005112d0 _$E446
// FUNCTION: STBC 0x005112e0 _$E445
// FUNCTION: STBC 0x005112f0 _$E450
// FUNCTION: STBC 0x00511300 _$E449
// FUNCTION: STBC 0x00511310 _$E454
// FUNCTION: STBC 0x00511320 _$E453
// FUNCTION: STBC 0x00511330 _$E458
// FUNCTION: STBC 0x00511340 _$E457
// FUNCTION: STBC 0x00511350 _$E462
// FUNCTION: STBC 0x00511360 _$E461
// FUNCTION: STBC 0x00511370 _$E466
// FUNCTION: STBC 0x00511380 _$E465
// FUNCTION: STBC 0x00511390 _$E169
// FUNCTION: STBC 0x005113a0 _$E168
// FUNCTION: STBC 0x005113b0 _$E470
// FUNCTION: STBC 0x005113c0 _$E469
// FUNCTION: STBC 0x005113d0 _$E174
// FUNCTION: STBC 0x005113e0 _$E173
// FUNCTION: STBC 0x005113f0 _$E179
// FUNCTION: STBC 0x00511400 _$E178
// FUNCTION: STBC 0x00511470 _$E474
// FUNCTION: STBC 0x00511480 _$E473
// FUNCTION: STBC 0x00511490 _$E479
// FUNCTION: STBC 0x005114a0 _$E478
// FUNCTION: STBC 0x005114b0 _$E483
// FUNCTION: STBC 0x005114c0 _$E482

// The icons of the reticle's bars in the LCARS icon group (invented names).
#define RETICLE_BAR_ICON		200
#define RETICLE_FRAME_ICON		0x78

// The bars' height in pixels (invented name).
#define RETICLE_BAR_PIXELS		3.0f

// The name and bars' layout, in pixels (invented names).
#define RETICLE_TEXT_GAP		3.0f	// between the arrow and the name
#define RETICLE_BAR_OFFSET		73.0f	// the bars' left edge from the reticle's, left of the arrow
#define RETICLE_BAR_WIDTH		70.0f
#define RETICLE_BAR_GAP			5.0f	// between the reticle and the bars above it
#define RETICLE_NAME_RAISE		7.0f

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0050fd80 _$E315
// FUNCTION: STBC 0x0050fda0 _$E316
// FUNCTION: STBC 0x0050fd90 _$E317
// FUNCTION: STBC 0x0050fd70 _$E318
// FUNCTION: STBC 0x0050fdc0 _$E321
// FUNCTION: STBC 0x0050fde0 _$E322
// FUNCTION: STBC 0x0050fdd0 _$E323
// FUNCTION: STBC 0x0050fdb0 _$E324

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0050fe60 _$E485
// FUNCTION: STBC 0x0050fe80 _$E486
// FUNCTION: STBC 0x0050fe70 _$E488
// FUNCTION: STBC 0x0050fe50 _$E489
// DATA: STBC 0x00987968
TGEventHandlerTable NamedReticleWindow::classHandlers;

/*
================
NamedReticleWindow::NamedReticleWindow

The name, two half-transparent bars in the full and the empty condition
colours, and the frame, in a pane in front of the reticle; the reticle
follows ship identification, group and name changes.
================
*/
// FUNCTION: STBC 0x0050fea0
NamedReticleWindow::NamedReticleWindow( void ) {
	unknown7C = new TGPane( 1.0f, 1.0f );
	unknown7C->AddChild( new TGParagraph( "", 1.0f, NULL, "", 0.0f, TGPF_READ_ONLY ), 0.0f, 0.0f, false );

	float pixelHeight = 1.0f / g_kIconManager.GetScreenHeight();
	char *lcarsGroup = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();

	NiColorA color = g_pConditionColors[19];
	color.a = 0.5f;
	TGIcon *bar = new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color );
	bar->Resize( 0.0f, pixelHeight * RETICLE_BAR_PIXELS, false );
	unknown7C->AddChild( bar, 0.0f, 0.0f, false );

	color = g_pConditionColors[1];
	color.a = 0.5f;
	bar = new TGIcon( lcarsGroup, RETICLE_BAR_ICON, &color );
	bar->Resize( 0.0f, pixelHeight * RETICLE_BAR_PIXELS, false );
	unknown7C->AddChild( bar, 0.0f, 0.0f, false );

	unknown7C->AddChild( new TGIcon( lcarsGroup, RETICLE_FRAME_ICON ) );
	PrependChild( unknown7C );

	g_kEventManager.AddBroadcastMethodHandler( ET_SENSORS_SHIP_IDENTIFIED, this, "NamedReticleWindow::HandleShipIdentified" );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_GROUP_CHANGED, this, "NamedReticleWindow::HandleObjectGroupChanged" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NAME_CHANGE, this, "NamedReticleWindow::HandleNameChange" );
}

/*
================
NamedReticleWindow::GetObjType
================
*/
// FUNCTION: STBC 0x005101c0
int NamedReticleWindow::GetObjType( void ) {
	return NAMEDRETICLEWINDOW_TYPE;
}

/*
================
NamedReticleWindow::GetClassName
================
*/
// FUNCTION: STBC 0x00510230
char *NamedReticleWindow::GetClassName( void ) {
	return "NamedReticleWindow";
}

/*
================
NamedReticleWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00510240
char *NamedReticleWindow::GetSwigTypeName( void ) {
	return "_p_NamedReticleWindow";
}

/*
================
NamedReticleWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00510250
char *NamedReticleWindow::GetPythonClassName( void ) {
	return "NamedReticleWindowPtr";
}

/*
================
NamedReticleWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00510260
TGEventHandlerTable *NamedReticleWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
NamedReticleWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00510310
void NamedReticleWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&NamedReticleWindow::HandleShipIdentified, "NamedReticleWindow::HandleShipIdentified" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&NamedReticleWindow::HandleObjectGroupChanged, "NamedReticleWindow::HandleObjectGroupChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&NamedReticleWindow::HandleNameChange, "NamedReticleWindow::HandleNameChange" );
}

/*
================
NamedReticleWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00510350
void NamedReticleWindow::RegisterHandlers( void ) {
}

/*
================
NamedReticleWindow::VirtualSlot82

Hides the reticle when its object is gone, or is outside the rendered set
while the map is not zoomed; places it otherwise.
================
*/
// FUNCTION: STBC 0x00510360
void NamedReticleWindow::VirtualSlot82( float time ) {
	if ( targetID == 0 ) {
		return;
	}
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, targetID );
	if ( object == NULL ) {
		targetID = 0;
		SetNotVisible();
		return;
	}
	if ( !ReticleManagerWindow::GetReticleManagerWindow()->IsZoomed() && object->GetContainingSet() != g_kSetManager.GetRenderedSet() ) {
		SetNotVisible();
		return;
	}
	Unknown5103D0( object );
}

/*
================
NamedReticleWindow::Unknown5103D0

Places the reticle on the object, then its name and bars.
================
*/
// FUNCTION: STBC 0x005103d0
void NamedReticleWindow::Unknown5103D0( ObjectClass *object ) {
	Unknown515310( object );
	Unknown5103F0( object );
}

/*
================
NamedReticleWindow::Unknown5103F0

Lays the name, the condition bars and the frame out around the reticle.
While the target arrow shows, the name goes beside the arrow (to the right
for the arrow's icons 0, 3 and 5, to the left otherwise, and lower for a
ship at the bottom edge) with the bars under it. Otherwise the bars go
centred above the reticle and the name above them; without a ship, the
name alone sits above the reticle. The frame covers the name.
================
*/
// FUNCTION: STBC 0x005103f0
// PARKED: the hull's condition is copied through fld/fstp as retail's (GetConditionPercentage's copy level), but the hull pointer and the rectangle loads take other registers (edx/eax swapped); in the unarrowed branch the empty bar's x adds a bar x kept in the name height's slot (-0xC) where ours keeps it in -0x8; the reticle rectangle is read through its fields, as retail's evaluation order needs
void NamedReticleWindow::Unknown5103F0( ObjectClass *object ) {
	TGParagraph *name = (TGParagraph *)unknown7C->GetNthChild( 0 );
	TGIcon *fullBar = (TGIcon *)unknown7C->GetNthChild( 1 );
	TGIcon *emptyBar = (TGIcon *)unknown7C->GetNthChild( 2 );
	TGIcon *frame = (TGIcon *)unknown7C->GetNthChild( 3 );
	float pixelHeight = 1.0f / g_kIconManager.GetScreenHeight();
	float nameWidth = name->GetWidth();
	float nameHeight = name->GetHeight();

	bool isShip;
	float condition;
	if ( object->IsTypeOf( SHIPCLASS_TYPE ) ) {
		isShip = true;
		condition = ShipClass::Cast( object )->GetHull()->GetConditionPercentage();
	} else {
		isShip = false;
		condition = 0.0f;
	}

	if ( targetArrow->IsVisible() ) {
		float pixelWidth = 1.0f / g_kIconManager.GetScreenWidth();
		float pixelHeight = 1.0f / g_kIconManager.GetScreenHeight();
		float nameX;
		float nameY;
		float barX;
		switch ( targetArrow->GetIconNum() ) {
		case 0:
		case 3:
			barX = nameX = pixelWidth * RETICLE_TEXT_GAP + unknown6C.width + unknown6C.left;
			nameY = unknown6C.top;
			break;
		case 5:
			barX = nameX = pixelWidth * RETICLE_TEXT_GAP + unknown6C.width + unknown6C.left;
			nameY = unknown6C.top;
			if ( isShip ) {
				nameY -= pixelHeight * RETICLE_NAME_RAISE;
			}
			break;
		case 1:
		case 2:
		case 4:
			nameX = unknown6C.left - nameWidth - pixelWidth * RETICLE_TEXT_GAP;
			nameY = unknown6C.top;
			barX = unknown6C.left - pixelWidth * RETICLE_BAR_OFFSET;
			break;
		case 6:
		case 7:
			nameX = unknown6C.left - nameWidth - pixelWidth * RETICLE_TEXT_GAP;
			nameY = unknown6C.top;
			barX = unknown6C.left - pixelWidth * RETICLE_BAR_OFFSET;
			if ( isShip ) {
				nameY -= pixelHeight * RETICLE_NAME_RAISE;
			}
			break;
		}
		if ( isShip ) {
			float barWidth = pixelWidth * RETICLE_BAR_WIDTH;
			float fullWidth = barWidth * condition;
			fullBar->SetVisible();
			float barHeight = pixelHeight * RETICLE_BAR_PIXELS;
			fullBar->Resize( fullWidth, barHeight );
			float barY = pixelHeight * 2.0f + nameY + nameHeight;
			fullBar->SetPosition( barX, barY );
			emptyBar->SetVisible();
			emptyBar->Resize( barWidth - fullWidth, barHeight );
			emptyBar->SetPosition( barX, barY );
		}
		name->SetPosition( nameX, nameY );
		name->Layout();
	} else if ( isShip ) {
		float barWidth = 1.0f / g_kIconManager.GetScreenWidth() * RETICLE_BAR_WIDTH;
		float barX = ( unknown6C.width - barWidth ) * 0.5f;
		float fullWidth = barWidth * condition;
		fullBar->SetVisible();
		float barHeight = pixelHeight * RETICLE_BAR_PIXELS;
		fullBar->Resize( fullWidth, barHeight );
		float barGap = pixelHeight * RETICLE_BAR_GAP;
		fullBar->SetPosition( barX + unknown6C.left, unknown6C.top - barGap );
		emptyBar->SetVisible();
		emptyBar->Resize( barWidth - fullWidth, barHeight );
		emptyBar->SetPosition( fullWidth + barX + unknown6C.left, unknown6C.top - barGap );
		name->SetPosition( ( unknown6C.width - nameWidth ) * 0.5f + unknown6C.left, unknown6C.top - nameHeight - pixelHeight * RETICLE_NAME_RAISE );
	} else {
		name->SetPosition( ( unknown6C.width - nameWidth ) * 0.5f + unknown6C.left, unknown6C.top - nameHeight );
		name->RecalcBounds();
		fullBar->SetNotVisible();
		emptyBar->SetNotVisible();
	}
	frame->SetPosition( name->GetLeft(), name->GetTop(), false );
	frame->Resize( name->GetWidth(), name->GetHeight() );
}

/*
================
NamedReticleWindow::SetTargetID

Follows the object, and shows its name, or "Unknown" while the player's
sensors have not identified it.
================
*/
// FUNCTION: STBC 0x00510870
void NamedReticleWindow::SetTargetID( long id ) {
	TGParagraph *name = (TGParagraph *)unknown7C->GetNthChild( 0 );
	ObjectClass *oldTarget = ObjectClass::GetObjectByID( NULL, id );
	if ( oldTarget != NULL ) {
		g_kEventManager.RemoveBroadcastHandler( ET_SENSORS_SHIP_IDENTIFIED, this, "NamedReticleWindow::HandleShipIdentified", oldTarget );
	}
	ReticleWindow::SetTargetID( id );

	ObjectClass *object = ObjectClass::GetObjectByID( NULL, id );
	if ( object != NULL ) {
		VirtualSlot85();

		bool known = true;
		if ( Game::GetCurrentGame() != NULL ) {
			ShipClass *player = ShipClass::Cast( Game::GetCurrentGame()->GetPlayer() );
			if ( player != NULL && player != object && player->GetSensorSubsystem() != NULL ) {
				known = player->GetSensorSubsystem()->IsObjectKnown( object );
			}
		}
		TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Ships.tgl", true );
		if ( known == true ) {
			name->SetString( &object->GetDisplayName(), false );
		} else {
			TGString *unknownName = database->GetString( "Unknown" );
			name->SetString( unknownName, false );
		}
		name->RecalcBounds( true );
		g_kLocalizationManager.Unload( database );
	}
}

/*
================
NamedReticleWindow::VirtualSlot84

Passes the call to the reticles among the children, and lays the name out.
================
*/
// FUNCTION: STBC 0x00510980
void NamedReticleWindow::VirtualSlot84( void ) {
	TGParagraph *name = (TGParagraph *)unknown7C->GetNthChild( 0 );
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) ) {
			( (ReticleWindow *)child )->VirtualSlot84();
		}
	}
	name->RecalcBounds( false );
}

/*
================
NamedReticleWindow::HandleShipIdentified

The object the event names was identified: shows its name when it is the
reticle's.
================
*/
// FUNCTION: STBC 0x005109e0
void NamedReticleWindow::HandleShipIdentified( TGObjPtrEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetObjPtr() );
	ObjectClass *target = ObjectClass::GetObjectByID( NULL, targetID );
	if ( object == target && object != NULL ) {
		TGParagraph *name = TGParagraph::Cast( unknown7C->GetNthChild( 0 ) );
		if ( name != NULL ) {
			name->SetString( &target->GetDisplayName(), false );
			name->RecalcBounds( true );
		}
	}
}

/*
================
NamedReticleWindow::HandleNameChange

The event's destination changed its name: shows the new one when it is the
reticle's object.
================
*/
// FUNCTION: STBC 0x00510a50
void NamedReticleWindow::HandleNameChange( TGEvent *event ) {
	ObjectClass *object = ObjectClass::Cast( event->GetDestination() );
	ObjectClass *target = ObjectClass::GetObjectByID( NULL, targetID );
	if ( object == target && object != NULL ) {
		TGParagraph *name = TGParagraph::Cast( unknown7C->GetNthChild( 0 ) );
		if ( name != NULL ) {
			name->SetString( &target->GetDisplayName(), false );
			name->RecalcBounds( true );
		}
	}
}

/*
================
NamedReticleWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x00510ac0
bool NamedReticleWindow::WriteToStream( TGStream *stream ) {
	if ( !ReticleWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( unknown7C != NULL ? unknown7C->GetObjID() : 0 );
	return true;
}

/*
================
NamedReticleWindow::ReadFromStream
================
*/
// FUNCTION: STBC 0x00510b10
bool NamedReticleWindow::ReadFromStream( TGStream *stream ) {
	if ( !ReticleWindow::ReadFromStream( stream ) ) {
		return false;
	}
	unknown7C = (TGPane *)stream->ReadID();
	return true;
}

/*
================
NamedReticleWindow::FixUpPointers
================
*/
// FUNCTION: STBC 0x00510b60
bool NamedReticleWindow::FixUpPointers( void ) {
	if ( !ReticleWindow::FixUpPointers() ) {
		return false;
	}
	if ( unknown7C != NULL && GetTGObjectPtr( (long)unknown7C ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
NamedReticleWindow::PostFixUp
================
*/
// FUNCTION: STBC 0x00510b90
void NamedReticleWindow::PostFixUp( void ) {
	ReticleWindow::PostFixUp();
	unknown7C = ( unknown7C != NULL ) ? (TGPane *)GetTGObjectPtr( (long)unknown7C ) : NULL;
}

/*
================
NamedReticleWindow::VirtualSlot85

Colours the name with the object's affiliation.
================
*/
// FUNCTION: STBC 0x00510bc0
void NamedReticleWindow::VirtualSlot85( void ) {
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, targetID );
	if ( object != NULL ) {
		TGParagraph *name = (TGParagraph *)unknown7C->GetNthChild( 0 );
		NiColorA color = *object->GetAffiliationColor();
		name->SetColor( &color );
	}
}

/*
================
NamedReticleWindow::HandleObjectGroupChanged

The object's group changed: the affiliation colour again.
================
*/
// FUNCTION: STBC 0x00510c20
void NamedReticleWindow::HandleObjectGroupChanged( TGEvent *event ) {
	VirtualSlot85();
}

