/*
===============================================================================

	RadarScope.cpp

===============================================================================
*/

#include <iostream>
#include <math.h>
#include <string.h>

#include "RadarScope.h"
#include "RadarBlip.h"
#include "DamageableObject.h"
#include "Game.h"
#include "GraphicsModeInfo.h"
#include "NiAVObject.h"
#include "NiMath.h"
#include "NiMatrix3.h"
#include "NiPoint2.h"
#include "ObjectGroup.h"
#include "ObjectClass.h"
#include "PhysicsObjectClass.h"
#include "Planet.h"
#include "PyEmbed.h"
#include "SensorSubsystem.h"
#include "SetClass.h"
#include "ShipClass.h"
#include "ShipProperty.h"
#include "STColors.h"
#include "Torpedo.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGIcon.h"
#include "TGIconManager.h"
#include "TGIconPoly.h"
#include "TGUIModule.h"
#include "TGObjPtrEvent.h"
#include "TGPoint3.h"
#include "TGStream.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "UtopiaModule.h"
#include "WeaponHitEvent.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"
#include "TGModelUtils.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00545570 _$E531
// FUNCTION: STBC 0x00545590 _$E530

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00542f70 _$E32
// FUNCTION: STBC 0x00542f80 _$E29
// FUNCTION: STBC 0x00542f90 _$E31
// FUNCTION: STBC 0x00542fa0 _$E30
// FUNCTION: STBC 0x00542fb0 _$E38
// FUNCTION: STBC 0x00542fc0 _$E35
// FUNCTION: STBC 0x00542fd0 _$E37
// FUNCTION: STBC 0x00542fe0 _$E36

// The handler table's dynamic initialiser and exit-time destructor.

// The handler table's dynamic initialiser and exit-time destructor.

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00543130 _$E517
// FUNCTION: STBC 0x00543140 _$E513
// FUNCTION: STBC 0x00543150 _$E516
// FUNCTION: STBC 0x00543160 _$E514

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00543180 _$E522
// FUNCTION: STBC 0x00543190 _$E519
// FUNCTION: STBC 0x005431a0 _$E521
// FUNCTION: STBC 0x005431b0 _$E520

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00542ff0 _$E57
// FUNCTION: STBC 0x00543000 _$E56
// FUNCTION: STBC 0x00543010 _$E61
// FUNCTION: STBC 0x00543020 _$E60
// FUNCTION: STBC 0x00543030 _$E72
// FUNCTION: STBC 0x00543040 _$E71
// FUNCTION: STBC 0x00543050 _$E76
// FUNCTION: STBC 0x00543060 _$E75
// FUNCTION: STBC 0x00543070 _$E80
// FUNCTION: STBC 0x00543080 _$E79
// FUNCTION: STBC 0x00543090 _$E84
// FUNCTION: STBC 0x005430a0 _$E83
// FUNCTION: STBC 0x005430b0 _$E208
// FUNCTION: STBC 0x005430c0 _$E207
// FUNCTION: STBC 0x005430d0 _$E150
// FUNCTION: STBC 0x005430e0 _$E149
// FUNCTION: STBC 0x005430f0 _$E163
// FUNCTION: STBC 0x00543100 _$E162
// FUNCTION: STBC 0x00543110 _$E213
// FUNCTION: STBC 0x00543120 _$E212
// FUNCTION: STBC 0x005435f0 _$E218
// FUNCTION: STBC 0x00543600 _$E217
// FUNCTION: STBC 0x00543890 _$E222
// FUNCTION: STBC 0x005438a0 _$E221
// FUNCTION: STBC 0x00544090 _$E226
// FUNCTION: STBC 0x005440a0 _$E225
// FUNCTION: STBC 0x005449e0 _$E233
// FUNCTION: STBC 0x005449f0 _$E232
// FUNCTION: STBC 0x00544b60 _$E362
// FUNCTION: STBC 0x00544b70 _$E361
// FUNCTION: STBC 0x00544c30 _$E366
// FUNCTION: STBC 0x00544c40 _$E365
// FUNCTION: STBC 0x00544cb0 _$E108
// FUNCTION: STBC 0x00544cc0 _$E107
// FUNCTION: STBC 0x00544cd0 _$E260
// FUNCTION: STBC 0x00544ce0 _$E259
// FUNCTION: STBC 0x00544cf0 _$E267
// FUNCTION: STBC 0x00544d00 _$E266
// FUNCTION: STBC 0x00544d10 _$E271
// FUNCTION: STBC 0x00544d20 _$E270
// FUNCTION: STBC 0x00544d30 _$E275
// FUNCTION: STBC 0x00544d40 _$E274
// FUNCTION: STBC 0x00544d50 _$E279
// FUNCTION: STBC 0x00544d60 _$E278
// FUNCTION: STBC 0x00544d70 _$E285
// FUNCTION: STBC 0x00544d80 _$E284
// FUNCTION: STBC 0x00544d90 _$E370
// FUNCTION: STBC 0x00544da0 _$E369
// FUNCTION: STBC 0x00544db0 _$E374
// FUNCTION: STBC 0x00544dc0 _$E373
// FUNCTION: STBC 0x00544dd0 _$E378
// FUNCTION: STBC 0x00544de0 _$E377
// FUNCTION: STBC 0x00544df0 _$E382
// FUNCTION: STBC 0x00544e00 _$E381
// FUNCTION: STBC 0x00544e10 _$E386
// FUNCTION: STBC 0x00544e20 _$E385
// FUNCTION: STBC 0x00544e30 _$E390
// FUNCTION: STBC 0x00544e40 _$E389
// FUNCTION: STBC 0x00544e50 _$E394
// FUNCTION: STBC 0x00544e60 _$E393
// FUNCTION: STBC 0x00544e70 _$E398
// FUNCTION: STBC 0x00544e80 _$E397
// FUNCTION: STBC 0x00544e90 _$E402
// FUNCTION: STBC 0x00544ea0 _$E401
// FUNCTION: STBC 0x00544eb0 _$E406
// FUNCTION: STBC 0x00544ec0 _$E405
// FUNCTION: STBC 0x00544ed0 _$E113
// FUNCTION: STBC 0x00544ee0 _$E112
// FUNCTION: STBC 0x00544ef0 _$E410
// FUNCTION: STBC 0x00544f00 _$E409
// FUNCTION: STBC 0x00544f10 _$E118
// FUNCTION: STBC 0x00544f20 _$E117
// FUNCTION: STBC 0x00544f30 _$E414
// FUNCTION: STBC 0x00544f40 _$E413
// FUNCTION: STBC 0x00544f50 _$E418
// FUNCTION: STBC 0x00544f60 _$E417
// FUNCTION: STBC 0x00544f70 _$E422
// FUNCTION: STBC 0x00544f80 _$E421
// FUNCTION: STBC 0x00544f90 _$E426
// FUNCTION: STBC 0x00544fa0 _$E425
// FUNCTION: STBC 0x00544fb0 _$E127
// FUNCTION: STBC 0x00544fc0 _$E126
// FUNCTION: STBC 0x00544fd0 _$E430
// FUNCTION: STBC 0x00544fe0 _$E429
// FUNCTION: STBC 0x00544ff0 _$E434
// FUNCTION: STBC 0x00545000 _$E433
// FUNCTION: STBC 0x00545010 _$E438
// FUNCTION: STBC 0x00545020 _$E437
// FUNCTION: STBC 0x00545030 _$E133
// FUNCTION: STBC 0x00545040 _$E132
// FUNCTION: STBC 0x00545050 _$E138
// FUNCTION: STBC 0x00545060 _$E137
// FUNCTION: STBC 0x00545070 _$E168
// FUNCTION: STBC 0x00545080 _$E167
// FUNCTION: STBC 0x00545090 _$E442
// FUNCTION: STBC 0x005450a0 _$E441
// FUNCTION: STBC 0x005450b0 _$E173
// FUNCTION: STBC 0x005450c0 _$E172
// FUNCTION: STBC 0x005450d0 _$E446
// FUNCTION: STBC 0x005450e0 _$E445
// FUNCTION: STBC 0x005450f0 _$E450
// FUNCTION: STBC 0x00545100 _$E449
// FUNCTION: STBC 0x00545110 _$E178
// FUNCTION: STBC 0x00545120 _$E177
// FUNCTION: STBC 0x00545130 _$E454
// FUNCTION: STBC 0x00545140 _$E453
// FUNCTION: STBC 0x00545150 _$E143
// FUNCTION: STBC 0x00545160 _$E142
// FUNCTION: STBC 0x00545170 _$E458
// FUNCTION: STBC 0x00545180 _$E457
// FUNCTION: STBC 0x00545190 _$E462
// FUNCTION: STBC 0x005451a0 _$E461
// FUNCTION: STBC 0x005451b0 _$E183
// FUNCTION: STBC 0x005451c0 _$E182
// FUNCTION: STBC 0x005451d0 _$E188
// FUNCTION: STBC 0x005451e0 _$E187
// FUNCTION: STBC 0x005451f0 _$E466
// FUNCTION: STBC 0x00545200 _$E465
// FUNCTION: STBC 0x00545210 _$E470
// FUNCTION: STBC 0x00545220 _$E469
// FUNCTION: STBC 0x00545230 _$E474
// FUNCTION: STBC 0x00545240 _$E473
// FUNCTION: STBC 0x00545250 _$E478
// FUNCTION: STBC 0x00545260 _$E477
// FUNCTION: STBC 0x00545270 _$E482
// FUNCTION: STBC 0x00545280 _$E481
// FUNCTION: STBC 0x00545290 _$E486
// FUNCTION: STBC 0x005452a0 _$E485
// FUNCTION: STBC 0x005452b0 _$E490
// FUNCTION: STBC 0x005452c0 _$E489
// FUNCTION: STBC 0x005452d0 _$E494
// FUNCTION: STBC 0x005452e0 _$E493
// FUNCTION: STBC 0x005452f0 _$E193
// FUNCTION: STBC 0x00545300 _$E192
// FUNCTION: STBC 0x00545310 _$E498
// FUNCTION: STBC 0x00545320 _$E497
// FUNCTION: STBC 0x00545330 _$E198
// FUNCTION: STBC 0x00545340 _$E197
// FUNCTION: STBC 0x00545350 _$E203
// FUNCTION: STBC 0x00545360 _$E202
// FUNCTION: STBC 0x005455a0 _$E502
// FUNCTION: STBC 0x005455b0 _$E501
// FUNCTION: STBC 0x005455c0 _$E507
// FUNCTION: STBC 0x005455d0 _$E506
// FUNCTION: STBC 0x005455e0 _$E511
// FUNCTION: STBC 0x005455f0 _$E510

// The icon of the ShipIcons group for a ship that is not known (invented name).
#define DEFAULT_SHIP_ICON		0x65

// The icons of the LCARS group a blip shows (invented names): an object
// without a direction (a torpedo, an asteroid or a planet), a ship, and the
// bracket on a ship that hit the player.
#define BLIP_ICON_DOT			0x1A4
#define BLIP_ICON_SHIP			0x19A
#define BLIP_ICON_BRACKET		0x1AE

// The farthest range the scope shows while its range is limited (invented name).
#define MAX_LIMITED_RANGE		170.0f

// The ship icon's share of the scope's middle, as a factor of its size, and
// the distance out to which the blips spread over three quarters of the
// scope's radius (invented names).
#define SHIP_ICON_MARGIN		1.15f
#define RADAR_NEAR_DISTANCE		229.0f

// DATA: STBC 0x008e3e44
float RadarScope::unknown8E3E44 = 3.0f;

// How long a bracket stays on a ship that hit the player, in seconds (invented name).
// DATA: STBC 0x008e3e48
static const float BRACKET_TIME = 3.0f;

// DATA: STBC 0x0098a7f0
TGEventHandlerTable RadarBlip::classHandlers;

// DATA: STBC 0x0098a7c0
TGEventHandlerTable RadarScope::classHandlers;

/*
================
RadarBlip::RadarBlip
================
*/
// FUNCTION: STBC 0x005431d0
RadarBlip::RadarBlip( char *groupName, int iconNum, long shipID ) : TGIcon( groupName, iconNum ) {
	this->shipID = shipID;
}

/*
================
RadarBlip::GetObjType
================
*/
// FUNCTION: STBC 0x00543200
int RadarBlip::GetObjType( void ) {
	return RADARBLIP_TYPE;
}

/*
================
RadarBlip::IsTypeOf
================
*/
// FUNCTION: STBC 0x00543210
bool RadarBlip::IsTypeOf( int type ) {
	if ( type == RADARBLIP_TYPE ) {
		return true;
	}
	return TGIcon::IsTypeOf( type );
}

/*
================
RadarBlip::GetClassName
================
*/
// FUNCTION: STBC 0x00543270
char *RadarBlip::GetClassName( void ) {
	return "RadarBlip";
}

/*
================
RadarBlip::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00543280
char *RadarBlip::GetSwigTypeName( void ) {
	return "_p_RadarBlip";
}

/*
================
RadarBlip::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00543290
char *RadarBlip::GetPythonClassName( void ) {
	return "RadarBlipPtr";
}

/*
================
RadarBlip::GetClassHandlers
================
*/
// FUNCTION: STBC 0x005432a0
TGEventHandlerTable *RadarBlip::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
RadarBlip::~RadarBlip
================
*/
// FUNCTION: STBC 0x005432e0
RadarBlip::~RadarBlip( void ) {
}

/*
================
RadarBlip::Layout
================
*/
// FUNCTION: STBC 0x005432f0
void RadarBlip::Layout( void ) {
	SizeToArtwork( false );
	TGIcon::Layout();
	UpdateDirection();
}

/*
================
RadarBlip::UpdateDirection

Turns the poly's four vertices about its centre so that the icon points
the way the ship heads, seen from the player's ship. The vertical axis is
stretched to square pixels for the turn and back after it.
================
*/
// FUNCTION: STBC 0x00543310 ?UpdateDirection@RadarBlip@@IAEXXZ
// DRAFT: the call set matches (NiPoint3::Length out of line in Unitize, NiPoint3::operator*( float ) out of line, as retail); off: retail reads the local vector's x and y from memory for the dot and the length where this build keeps a copy in registers (and a frame slot for it). The heading needs a cast to NiPoint3 because TGPoint3 * NiMatrix3 is ambiguous with TGMatrix3's converting constructor. Retail converts the height and width each with fild qword and fdivp; this build divides with fidiv
void RadarBlip::UpdateDirection( void ) {
	if ( iconPoly == NULL || iconNum == BLIP_ICON_DOT ) {
		return;
	}
	ShipClass *player = Game::GetCurrentPlayer();
	PhysicsObjectClass *ship = PhysicsObjectClass::GetObjectByID( NULL, shipID );
	if ( ship == NULL || player == NULL ) {
		return;
	}

	TGPoint3 forward = ship->GetWorldForward();
	forward.Unitize();
	NiPoint3 local = TGModelUtils::WorldToLocalVector( player->GetNode(), forward );
	NiPoint2 direction( local.x, local.y );
	float angle = -NiACos( NiPoint2::UNIT_Y * direction / direction.Length() );
	if ( local.x > 0.0f ) {
		angle = -angle;
	}
	NiMatrix3 rotation;
	rotation.MakeZRotation( angle );

	NiPoint3 *vertices = iconPoly->GetVertices();
	GraphicsModeInfo *mode = GraphicsModeInfo::currentMode;
	float centerX = ( vertices[3].x - vertices[0].x ) * 0.5f + vertices[0].x;
	float centerY = ( vertices[1].y - vertices[0].y ) * 0.5f + vertices[0].y;
	for ( int i = 0; i < 4; i++ ) {
		NiPoint3 &vertex = vertices[i];
		vertex.x -= centerX;
		vertex.y -= centerY;
		vertex.y /= (float)(unsigned int)mode->GetHeight() / (unsigned int)mode->GetWidth();
		vertex = rotation * vertex;
		vertex.y *= (float)(unsigned int)mode->GetHeight() / (unsigned int)mode->GetWidth();
		vertex.x += centerX;
		vertex.y += centerY;
	}
	// the same revision bump tells the renderer the vertices changed
	iconPoly->TexturesChanged();
}

/*
================
RadarBlip::WriteToStream
================
*/
// FUNCTION: STBC 0x00543610
bool RadarBlip::WriteToStream( TGStream *stream ) {
	if ( !TGIcon::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( shipID );
	return true;
}

/*
================
RadarBlip::ReadFromStream
================
*/
// FUNCTION: STBC 0x00543640
bool RadarBlip::ReadFromStream( TGStream *stream ) {
	if ( !TGIcon::ReadFromStream( stream ) ) {
		return false;
	}
	shipID = stream->ReadID();
	return true;
}

/*
================
RadarScope::RadarScope

Listens to the group changes, the objects entering and leaving sets and the
player changing, then has the script build the children.
================
*/
// FUNCTION: STBC 0x00543670 ??0RadarScope@@QAE@MM@Z
RadarScope::RadarScope( float width, float height ) : TGPane( 0 ), brackets( 2 ) {
	limitRange = true;
	playerID = 0;
	Resize( width, height, true );
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_GROUP_CHANGED, this, "RadarScope::GroupChangeHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "RadarScope::SetObjectUpdate" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TORPEDO_ENTERED_SET, this, "RadarScope::SetObjectUpdate" );
	g_kEventManager.AddBroadcastMethodHandler( ET_EXITED_SET, this, "RadarScope::SetObjectUpdate" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TORPEDO_EXITED_SET, this, "RadarScope::SetObjectUpdate" );
	g_kEventManager.AddBroadcastMethodHandler( ET_SET_PLAYER, this, "RadarScope::SetPlayerHandler" );
	Rebuild();
}

/*
================
RadarScope::~RadarScope
================
*/
// FUNCTION: STBC 0x005438b0 ??1RadarScope@@UAE@XZ
RadarScope::~RadarScope( void ) {
	targetBracket = NULL;
	brackets.RemoveAll();
	g_kEventManager.RemoveBroadcastHandler( ET_OBJECT_GROUP_CHANGED, this, "RadarScope::GroupChangeHandler" );
	// BUG: nothing adds this handler, and the ones for the torpedo set events are never removed
	g_kEventManager.RemoveBroadcastHandler( ET_RADAR_DISPLAY_OBJECT_UPDATE, this, "RadarScope::UpdateBlips" );
	g_kEventManager.RemoveBroadcastHandler( ET_ENTERED_SET, this, "RadarScope::SetObjectUpdate" );
	g_kEventManager.RemoveBroadcastHandler( ET_EXITED_SET, this, "RadarScope::SetObjectUpdate" );
	g_kEventManager.RemoveBroadcastHandler( ET_SET_PLAYER, this, "RadarScope::SetPlayerHandler" );
}

/*
================
RadarScope::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00543a30
void RadarScope::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RadarScope::GroupChangeHandler, "RadarScope::GroupChangeHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RadarScope::SetObjectUpdate, "RadarScope::SetObjectUpdate" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RadarScope::SetPlayerHandler, "RadarScope::SetPlayerHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RadarScope::PlayerDamagedHandler, "RadarScope::PlayerDamagedHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&RadarScope::BracketExpireHandler, "RadarScope::BracketExpireHandler" );
}

/*
================
RadarScope::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00543aa0
void RadarScope::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_RADAR_DISPLAY_BRACKET_EXPIRE, "RadarScope::BracketExpireHandler" );
}

/*
================
RadarScope::Rebuild
================
*/
// FUNCTION: STBC 0x00543ac0
void RadarScope::Rebuild( void ) {
	KillChildren();
	brackets.RemoveAll();
	PyObject *self = VirtualSlot08();
	PyEmbed::CallFunction( "Tactical.Interface.RadarScope", "Create", NULL, NULL, "(O)", self );
	Py_DECREF( self );
}

/*
================
RadarScope::CreateShipIcon

An icon of the player's ship in the ShipIcons group; a ship icon of number
DEFAULT_SHIP_ICON when there is no player.
================
*/
// FUNCTION: STBC 0x00543b40
TGIcon *RadarScope::CreateShipIcon( void ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player != NULL ) {
		return new TGIcon( "ShipIcons", player->GetShipProperty()->GetSpecies(), NULL );
	}
	return new TGIcon( "ShipIcons", DEFAULT_SHIP_ICON, NULL );
}

/*
================
RadarScope::GetRange
================
*/
// FUNCTION: STBC 0x00543bb0
float RadarScope::GetRange( float range ) {
	float scopeRange;
	if ( range != 0.0f ) {
		scopeRange = range;
	} else {
		ShipClass *player = Game::GetCurrentPlayer();
		if ( player == NULL ) {
			return 0.0f;
		}
		SensorSubsystem *sensors = player->GetSensorSubsystem();
		if ( sensors == NULL ) {
			return 0.0f;
		}
		scopeRange = sensors->GetSensorRange();
	}
	if ( limitRange && MAX_LIMITED_RANGE < scopeRange ) {
		scopeRange = MAX_LIMITED_RANGE;
	}
	return scopeRange;
}

/*
================
RadarScope::GetBlipColor
================
*/
// FUNCTION: STBC 0x00543c20
NiColorA RadarScope::GetBlipColor( ObjectClass *object ) {
	if ( IsIncomingTorpedo( object ) ) {
		return g_kSTRadarIncomingTorpColor;
	}
	return *object->GetAffiliationColor();
}

/*
================
RadarScope::FindBracketBlip
================
*/
// FUNCTION: STBC 0x00543c90
RadarBlip *RadarScope::FindBracketBlip( ObjectClass *object ) {
	NiTListIterator pos = brackets.GetHeadPos();
	long objectID = object->GetObjID();
	while ( pos ) {
		RadarBracket *bracket = brackets.GetNext( pos );
		if ( bracket != NULL && bracket->blip != NULL && bracket->blip->GetShipID() == objectID ) {
			return bracket->blip;
		}
	}
	return NULL;
}

/*
================
RadarScope::IsIncomingTorpedo
================
*/
// FUNCTION: STBC 0x00543cd0
bool RadarScope::IsIncomingTorpedo( ObjectClass *object ) {
	if ( object->IsTypeOf( TORPEDO_TYPE ) ) {
		Torpedo *torpedo = Torpedo::Cast( object );
		ShipClass *player = Game::GetCurrentPlayer();
		if ( player != NULL && torpedo->GetTargetID() == player->GetObjID() ) {
			return true;
		}
	}
	return false;
}

/*
================
RadarScope::GroupChangeHandler

The blips take the colours of their objects' new groups.
================
*/
// FUNCTION: STBC 0x00543d20
void RadarScope::GroupChangeHandler( TGEvent *event ) {
	Game *game = Game::GetCurrentGame();
	if ( game == NULL ) {
		return;
	}
	SetClass *set = game->GetPlayerSet();
	TGPane *blipPane = TGPane::Cast( GetNthChild( BLIP_PANE ) );
	for ( int i = 0; i < blipPane->GetNumChildren(); i++ ) {
		RadarBlip *blip = (RadarBlip *)blipPane->GetNthChild( i );
		ObjectClass *object = ObjectClass::GetObjectByID( set, blip->GetShipID() );
		if ( object != NULL ) {
			// VC6 takes the address of the returned colour (a nonstandard extension)
			blip->SetColor( &GetBlipColor( object ) );
		}
	}
}

/*
================
RadarScope::SetPlayerHandler

Moves the weapon hit handler to the new player's ship and rebuilds the
scope.
================
*/
// FUNCTION: STBC 0x00543dc0
void RadarScope::SetPlayerHandler( TGObjPtrEvent *event ) {
	ObjectClass *oldPlayer = ObjectClass::GetObjectByID( NULL, playerID );
	if ( oldPlayer != NULL ) {
		g_kEventManager.RemoveBroadcastHandler( ET_WEAPON_HIT, this, "RadarScope::PlayerDamagedHandler", oldPlayer );
	}
	ShipClass *player = ShipClass::Cast( event->GetObjPtr() );
	if ( player != NULL ) {
		playerID = player->GetObjID();
		g_kEventManager.AddBroadcastMethodHandler( ET_WEAPON_HIT, this, "RadarScope::PlayerDamagedHandler", player );
	} else {
		playerID = 0;
	}
	Rebuild();
}

/*
================
RadarScope::PlayerDamagedHandler

Puts a bracket on the ship that hit the player, or keeps the one it has,
for another BRACKET_TIME seconds.
================
*/
// FUNCTION: STBC 0x00543e50
void RadarScope::PlayerDamagedHandler( WeaponHitEvent *event ) {
	ObjectClass *attacker = event->GetFiringObject();
	if ( attacker == NULL ) {
		return;
	}
	long attackerID = attacker->GetObjID();
	RadarBlip *blip = NULL;
	NiTListIterator pos = brackets.GetHeadPos();
	while ( pos ) {
		RadarBracket *bracket = brackets.GetNext( pos );
		if ( bracket != NULL && bracket->blip != NULL && bracket->blip->GetShipID() == attackerID ) {
			g_kTimerManager.DeleteTimer( bracket->timerID );
			brackets.Remove( bracket );
			blip = bracket->blip;
			delete bracket;
			break;
		}
	}

	TGObjPtrEvent *expire = new TGObjPtrEvent;
	expire->SetEventType( ET_RADAR_DISPLAY_BRACKET_EXPIRE );
	expire->SetSource( this );
	expire->SetDestination( this );
	expire->SetObjPtr( attacker );
	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + BRACKET_TIME );
	timer->SetEvent( expire );
	g_kTimerManager.AddTimer( timer );

	if ( blip == NULL ) {
		GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
		blip = new RadarBlip( mode->GetLcarsString(), BLIP_ICON_BRACKET, attackerID );
		blip->SetColor( &g_kSTRadarIncomingTorpColor );
		TGPane::Cast( GetNthChild( BRACKET_PANE ) )->AddChild( blip );
		blip->SetNotVisible( false );
	}
	RadarBracket *bracket = new RadarBracket;
	bracket->timerID = timer->GetObjID();
	bracket->blip = blip;
	bracket->eventID = timer->GetEvent()->GetObjID();
	brackets.AddTail( bracket );
}

/*
================
RadarScope::BracketExpireHandler

Takes the bracket down from the ship of the event.
================
*/
// FUNCTION: STBC 0x005440b0
void RadarScope::BracketExpireHandler( TGObjPtrEvent *event ) {
	ShipClass *ship = ShipClass::Cast( event->GetObjPtr() );
	if ( ship == NULL ) {
		CallNextHandler( event );
		return;
	}
	long shipID = ship->GetObjID();
	NiTListIterator pos = brackets.GetHeadPos();
	while ( pos ) {
		RadarBracket *bracket = brackets.GetNext( pos );
		if ( bracket != NULL && bracket->blip != NULL && bracket->blip->GetShipID() == shipID ) {
			bracket->blip->GetParent()->DeleteChild( bracket->blip, false );
			brackets.Remove( bracket );
			delete bracket;
			break;
		}
	}
	CallNextHandler( event );
}

/*
================
RadarScope::Update

Puts a blip in the blip pane for each object of the player's set the scope
shows: the ships the sensors see, torpedoes, asteroids and planets. A blip
lies the way the object lies from the player's ship, at a distance that
grows with the object's up to RADAR_NEAR_DISTANCE and then closes in on the
scope's rim; its colour fades with the object's height. The target bracket
and an object's bracket blip follow the object's blip. Blips left over from
the last update are deleted.
================
*/
// FUNCTION: STBC 0x00544200 ?Update@RadarScope@@QAEXXZ
// DRAFT: structure in place; off: the dot test (retail lays the blip body out twice, once for the torpedo and asteroid tests and once for the planet test, where this build materialises the flag), the blip bookkeeping order, the PixelAlignValue stores (retail moves the screen size through the FPU) and register allocation across the loop
void RadarScope::Update( void ) {
	TGIcon *shipIcon = TGIcon::Cast( GetNthChild( SHIP_ICON ) );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	SensorSubsystem *sensors = player->GetSensorSubsystem();
	SetClass *set = player->GetContainingSet();
	ObjectClass *target = player->GetTarget();
	TGPane *blipPane = TGPane::Cast( GetNthChild( BLIP_PANE ) );
	GraphicsModeInfo *mode = GraphicsModeInfo::GetCurrentMode();
	targetBracket->SetNotVisible( false );
	if ( mode == NULL || set == NULL ) {
		return;
	}
	char *groupName = mode->GetLcarsString();
	if ( groupName == NULL ) {
		return;
	}
	float centerX = TGUIModule::PixelAlignValue( GetWidth() * 0.5f );
	float centerY = TGUIModule::PixelAlignValue( GetHeight() * 0.5f, false );
	float sensorRange = sensors->GetSensorRange();

	int numBlips = 0;
	ObjectClass *object = set->GetFirstObject();
	ObjectClass *first = object;
	while ( object != NULL ) {
		if ( object == player ) {
			object = set->GetNextObject( object->GetObjID() );
			if ( object == first ) {
				break;
			}
			continue;
		}
		RadarBlip *blip = NULL;
		if ( object->IsTypeOf( SHIPCLASS_TYPE ) ) {
			if ( sensors->IsObjectVisible( object ) ) {
				int icon = BLIP_ICON_SHIP;
				ShipClass *ship = ShipClass::Cast( object );
				if ( ship != NULL && ship->GetLODModelName() != NULL && strstr( ship->GetLODModelName(), "Asteroid" ) != NULL ) {
					icon = BLIP_ICON_DOT;
				}
				blip = (RadarBlip *)blipPane->GetNthChild( numBlips );
				if ( blip != NULL ) {
					blip->SetShipID( object->GetObjID() );
					blip->SetIconNum( icon, false );
				} else {
					blip = new RadarBlip( groupName, icon, object->GetObjID() );
					blipPane->AddChild( blip, 0.0f, 0.0f, false );
				}
				numBlips++;
			}
		} else {
			bool isDot = object->IsTypeOf( TORPEDO_TYPE );
			if ( !isDot ) {
				if ( DamageableObject::Cast( object ) != NULL ) {
					DamageableObject *damageable = DamageableObject::Cast( object );
					isDot = damageable->GetLODModelName() != NULL && strstr( damageable->GetLODModelName(), "Asteroid" ) != NULL;
				} else {
					isDot = Planet::Cast( object ) != NULL;
				}
			}
			if ( isDot ) {
			blip = (RadarBlip *)blipPane->GetNthChild( numBlips );
			numBlips++;
			if ( blip == NULL ) {
				blip = new RadarBlip( groupName, BLIP_ICON_DOT, object->GetObjID() );
				blipPane->AddChild( blip, 0.0f, 0.0f, false );
			} else {
				blip->SetShipID( object->GetObjID() );
				blip->SetIconNum( BLIP_ICON_DOT, false );
			}
			}
		}

		if ( blip != NULL ) {
			// the object in the player's frame, with x to the right and y ahead
			TGPoint3 location = object->GetWorldLocation();
			NiPoint3 local = TGModelUtils::WorldToLocalPoint( player->GetNode(), location );
			NiPoint2 direction( local.x, -local.y );
			direction.Unitize();

			float rimX = ( GetWidth() - shipIcon->GetWidth() * SHIP_ICON_MARGIN ) * 0.5f;
			float rimY = ( GetHeight() - shipIcon->GetHeight() * SHIP_ICON_MARGIN ) * 0.5f;
			float range = GetRange( 0.0f );
			float distance = local.Length();
			float offsetX;
			float offsetY;
			if ( distance > RADAR_NEAR_DISTANCE ) {
				float fraction = ( 1.0f - ( range + 1.0f ) / distance ) * 0.25f + 0.75f;
				offsetX = fraction * rimX;
				offsetY = fraction * rimY;
			} else {
				offsetX = distance * rimX * ( 0.75f / RADAR_NEAR_DISTANCE );
				offsetY = distance * rimY * ( 0.75f / RADAR_NEAR_DISTANCE );
			}
			float x = ( shipIcon->GetWidth() * SHIP_ICON_MARGIN * 0.5f + offsetX ) * direction.x + centerX;
			float y = ( shipIcon->GetHeight() * SHIP_ICON_MARGIN * 0.5f + offsetY ) * direction.y + centerY;
			blip->SetPosition( x - blip->GetWidth() * 0.5f, y - blip->GetHeight() * 0.5f, false );

			float height = (float)fabs( local.z / sensorRange );
			if ( height > 1.0f ) {
				height = 1.0f;
			}
			NiColorA color = GetBlipColor( object ) * ( 1.0f - height * 0.4f );
			blip->SetColor( &color );
			blip->Layout();

			if ( target != NULL && object == target ) {
				targetBracket->SetPosition( x - targetBracket->GetWidth() * 0.5f, y - targetBracket->GetHeight() * 0.5f, false );
				( (RadarBlip *)targetBracket )->SetShipID( target->GetObjID() );
				targetBracket->SetVisible( false );
				targetBracket->Layout();
			}
			RadarBlip *bracketBlip = FindBracketBlip( object );
			if ( bracketBlip != NULL ) {
				bracketBlip->SetPosition( x - bracketBlip->GetWidth() * 0.5f, y - bracketBlip->GetHeight() * 0.5f, false );
				bracketBlip->SetVisible( false );
				bracketBlip->Layout();
			}
		}
		object = set->GetNextObject( object->GetObjID() );
		if ( object == first ) {
			break;
		}
	}

	// BUG: deleting a child moves the next one down to i, so every other
	// left-over blip survives
	for ( int i = numBlips; i < blipPane->GetNumChildren(); i++ ) {
		TGUIObject *child = blipPane->GetNthChild( i );
		if ( child != NULL ) {
			blipPane->DeleteChild( child, false );
		}
	}
}

/*
================
RadarScope::SetObjectUpdate

The player's ship leaving its set takes the blips and the brackets down.
================
*/
// FUNCTION: STBC 0x00544a00
void RadarScope::SetObjectUpdate( TGEvent *event ) {
	int type = event->GetEventType();
	if ( event->GetDestination()->IsTypeOf( PHYSICSOBJECTCLASS_TYPE ) ) {
		TGEventHandlerObject *object = event->GetDestination();
		ShipClass *player = Game::GetCurrentPlayer();
		if ( player != NULL && ( type == ET_EXITED_SET || type == ET_TORPEDO_EXITED_SET ) && object == player ) {
			ClearBlips();
			ClearBrackets();
		}
	}
	CallNextHandler( event );
}

/*
================
RadarScope::ClearBlips
================
*/
// FUNCTION: STBC 0x00544a60
void RadarScope::ClearBlips( void ) {
	TGPane *blipPane = TGPane::Cast( GetNthChild( BLIP_PANE ) );
	// BUG: redundant: the bracket pane and the icon group are looked up and never used
	TGPane *bracketPane = TGPane::Cast( GetNthChild( BRACKET_PANE ) );
	blipPane->KillChildren();
	char *groupName = GraphicsModeInfo::currentMode->GetLcarsString();
}

/*
================
RadarScope::ClearBrackets
================
*/
// FUNCTION: STBC 0x00544ab0
void RadarScope::ClearBrackets( void ) {
	NiTListIterator pos = brackets.GetHeadPos();
	while ( pos ) {
		RadarBracket *bracket = brackets.GetNext( pos );
		if ( bracket != NULL ) {
			bracket->blip->GetParent()->RemoveChild( bracket->blip, false );
			if ( !g_kTimerManager.DeleteTimer( bracket->timerID ) ) {
				g_kEventManager.RemoveQueuedEvent( bracket->eventID );
			}
			delete bracket;
		}
	}
	brackets.RemoveAll();
	TGPane *bracketPane = TGPane::Cast( GetNthChild( BRACKET_PANE ) );
	if ( bracketPane != NULL ) {
		bracketPane->KillChildren();
	}
}

/*
================
RadarScope::WriteToStream

The pane, then the target bracket's ID, the range limit, the player's ID
and the saved float.
================
*/
// FUNCTION: STBC 0x00544b80 ?WriteToStream@RadarScope@@UAE_NPAVTGStream@@@Z
bool RadarScope::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( TGObject::GetSavedID( targetBracket ) );
	stream->WriteBool( limitRange );
	stream->WriteID( playerID );
	stream->WriteFloat( unknown8E3E44 );
	return true;
}

/*
================
RadarScope::ReadFromStream
================
*/
// FUNCTION: STBC 0x00544be0 ?ReadFromStream@RadarScope@@UAE_NPAVTGStream@@@Z
bool RadarScope::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	targetBracket = (TGUIObject *)stream->ReadID();
	limitRange = stream->ReadBool();
	playerID = stream->ReadID();
	unknown8E3E44 = stream->ReadFloat();
	return true;
}

/*
================
RadarScope::FixUpPointers

The saved target bracket ID must name an object that still exists.
================
*/
// FUNCTION: STBC 0x00544c50 ?FixUpPointers@RadarScope@@UAE_NXZ
bool RadarScope::FixUpPointers( void ) {
	if ( !TGPane::FixUpPointers() ) {
		return false;
	}
	if ( targetBracket != NULL && TGObject::GetTGObjectPtr( (long)targetBracket ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
RadarScope::PostFixUp

The target bracket pointer is the object of the saved ID.
================
*/
// FUNCTION: STBC 0x00544c80 ?PostFixUp@RadarScope@@UAEXXZ
void RadarScope::PostFixUp( void ) {
	TGPane::PostFixUp();
	targetBracket = (TGUIObject *)TGObject::GetSavedObject( (long)targetBracket );
}
