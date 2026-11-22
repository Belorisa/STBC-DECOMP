/*
===============================================================================

	MapWindow.cpp

===============================================================================
*/

#include <iostream>
#include "MapWindow.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "TGMouseEvent.h"
#include "TGKeyboardEvent.h"
#include "TGStream.h"
#include "TGIcon.h"
#include "TacticalWindow.h"
#include "PyEmbed.h"
#include "NiNode.h"
#include "NiLines.h"
#include "NiTriShape.h"
#include "NiMath.h"
#include "NiMatrix3.h"
#include "TGPoint3.h"
#include <math.h>
#include "ImageManagerClass.h"
#include "NiZBufferProperty.h"
#include "NiAlphaProperty.h"
#include "NiVertexColorProperty.h"
#include "NiTextureModeProperty.h"
#include "NiTextureProperty.h"
#include "Nebula.h"
#include "NiTList.h"
#include "Game.h"
#include "Mission.h"
#include "Episode.h"
#include "ShipClass.h"
#include "SensorSubsystem.h"
#include "PlacementObject.h"
#include "ReticleManagerWindow.h"
#include "ReticleWindow.h"
#include "TGObjPtrEvent.h"
#include "TGTimerManager.h"
#include "TopWindow.h"
#include "SetClass.h"
#include "CameraObjectClass.h"
#include "NiCamera.h"
#include "SubtitleWindow.h"
#include "SetManager.h"
#include "TGTimer.h"
#include "TGEvent.h"
#include "UtopiaModule.h"
#include "GraphicsModeInfo.h"
#include "MultiplayerGame.h"
#include "ObjectGroup.h"
#include "STColors.h"
#include "TGLocalizationDatabase.h"
#include "TGLocalizationManager.h"
#include "TGParagraph.h"
#include "TGRect.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x004fe300 _$E32
// FUNCTION: STBC 0x004fe310 _$E29
// FUNCTION: STBC 0x004fe320 _$E31
// FUNCTION: STBC 0x004fe330 _$E30
// FUNCTION: STBC 0x004fe340 _$E38
// FUNCTION: STBC 0x004fe350 _$E35
// FUNCTION: STBC 0x004fe360 _$E37
// FUNCTION: STBC 0x004fe370 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004fe380 _$E57
// FUNCTION: STBC 0x004fe390 _$E56
// FUNCTION: STBC 0x004fe3a0 _$E61
// FUNCTION: STBC 0x004fe3b0 _$E60
// FUNCTION: STBC 0x004fe3c0 _$E72
// FUNCTION: STBC 0x004fe3d0 _$E71
// FUNCTION: STBC 0x004fe3e0 _$E76
// FUNCTION: STBC 0x004fe3f0 _$E75
// FUNCTION: STBC 0x004fe400 _$E80
// FUNCTION: STBC 0x004fe410 _$E79
// FUNCTION: STBC 0x004fe420 _$E84
// FUNCTION: STBC 0x004fe430 _$E83
// FUNCTION: STBC 0x004fe440 _$E208
// FUNCTION: STBC 0x004fe450 _$E207
// FUNCTION: STBC 0x004fe460 _$E150
// FUNCTION: STBC 0x004fe470 _$E149
// FUNCTION: STBC 0x004fe480 _$E163
// FUNCTION: STBC 0x004fe490 _$E162
// FUNCTION: STBC 0x004fe4a0 _$E213
// FUNCTION: STBC 0x004fe4b0 _$E212
// FUNCTION: STBC 0x004fe4c0 _$E218
// FUNCTION: STBC 0x004fe4d0 _$E217
// FUNCTION: STBC 0x004fe8b0 _$E222
// FUNCTION: STBC 0x004fe8c0 _$E221
// FUNCTION: STBC 0x004fede0 _$E226
// FUNCTION: STBC 0x004fedf0 _$E225
// FUNCTION: STBC 0x005008f0 _$E233
// FUNCTION: STBC 0x00500900 _$E232
// FUNCTION: STBC 0x00500910 _$E411
// FUNCTION: STBC 0x00500920 _$E410
// FUNCTION: STBC 0x00500930 _$E415
// FUNCTION: STBC 0x00500940 _$E414
// FUNCTION: STBC 0x00500950 _$E108
// FUNCTION: STBC 0x00500960 _$E107
// FUNCTION: STBC 0x00500b80 _$E274
// FUNCTION: STBC 0x00500b90 _$E273
// FUNCTION: STBC 0x00501360 _$E281
// FUNCTION: STBC 0x00501370 _$E280
// FUNCTION: STBC 0x005018b0 _$E293
// FUNCTION: STBC 0x005018c0 _$E292
// FUNCTION: STBC 0x005018d0 _$E297
// FUNCTION: STBC 0x005018e0 _$E296
// FUNCTION: STBC 0x00501980 _$E301
// FUNCTION: STBC 0x00501990 _$E300
// FUNCTION: STBC 0x00501aa0 _$E307
// FUNCTION: STBC 0x00501ab0 _$E306
// FUNCTION: STBC 0x00501ac0 _$E419
// FUNCTION: STBC 0x00501ad0 _$E418
// FUNCTION: STBC 0x00501ae0 _$E423
// FUNCTION: STBC 0x00501af0 _$E422
// FUNCTION: STBC 0x00501b00 _$E427
// FUNCTION: STBC 0x00501b10 _$E426
// FUNCTION: STBC 0x00501b20 _$E431
// FUNCTION: STBC 0x00501b30 _$E430
// FUNCTION: STBC 0x00501b40 _$E435
// FUNCTION: STBC 0x00501b50 _$E434
// FUNCTION: STBC 0x00501b60 _$E439
// FUNCTION: STBC 0x00501b70 _$E438
// FUNCTION: STBC 0x00501b80 _$E443
// FUNCTION: STBC 0x00501b90 _$E442
// FUNCTION: STBC 0x00501ba0 _$E447
// FUNCTION: STBC 0x00501bb0 _$E446
// FUNCTION: STBC 0x00501bc0 _$E451
// FUNCTION: STBC 0x00501bd0 _$E450
// FUNCTION: STBC 0x00501be0 _$E455
// FUNCTION: STBC 0x00501bf0 _$E454
// FUNCTION: STBC 0x00501c00 _$E113
// FUNCTION: STBC 0x00501c10 _$E112
// FUNCTION: STBC 0x00501c20 _$E459
// FUNCTION: STBC 0x00501c30 _$E458
// FUNCTION: STBC 0x00501c40 _$E118
// FUNCTION: STBC 0x00501c50 _$E117
// FUNCTION: STBC 0x00501c60 _$E463
// FUNCTION: STBC 0x00501c70 _$E462
// FUNCTION: STBC 0x00501c80 _$E467
// FUNCTION: STBC 0x00501c90 _$E466
// FUNCTION: STBC 0x00501ca0 _$E471
// FUNCTION: STBC 0x00501cb0 _$E470
// FUNCTION: STBC 0x00501cc0 _$E475
// FUNCTION: STBC 0x00501cd0 _$E474
// FUNCTION: STBC 0x00501ce0 _$E127
// FUNCTION: STBC 0x00501cf0 _$E126
// FUNCTION: STBC 0x00501d00 _$E285
// FUNCTION: STBC 0x00501d10 _$E284
// FUNCTION: STBC 0x00501d20 _$E289
// FUNCTION: STBC 0x00501d30 _$E288
// FUNCTION: STBC 0x00501d40 _$E479
// FUNCTION: STBC 0x00501d50 _$E478
// FUNCTION: STBC 0x00501d60 _$E133
// FUNCTION: STBC 0x00501d70 _$E132
// FUNCTION: STBC 0x00501d80 _$E138
// FUNCTION: STBC 0x00501d90 _$E137
// FUNCTION: STBC 0x00501da0 _$E168
// FUNCTION: STBC 0x00501db0 _$E167
// FUNCTION: STBC 0x00501dc0 _$E483
// FUNCTION: STBC 0x00501dd0 _$E482
// FUNCTION: STBC 0x00501de0 _$E173
// FUNCTION: STBC 0x00501df0 _$E172
// FUNCTION: STBC 0x00501e00 _$E487
// FUNCTION: STBC 0x00501e10 _$E486
// FUNCTION: STBC 0x00501e20 _$E491
// FUNCTION: STBC 0x00501e30 _$E490
// FUNCTION: STBC 0x00501e40 _$E178
// FUNCTION: STBC 0x00501e50 _$E177
// FUNCTION: STBC 0x00501e60 _$E495
// FUNCTION: STBC 0x00501e70 _$E494
// FUNCTION: STBC 0x00501e80 _$E143
// FUNCTION: STBC 0x00501e90 _$E142
// FUNCTION: STBC 0x00501ea0 _$E499
// FUNCTION: STBC 0x00501eb0 _$E498
// FUNCTION: STBC 0x00501ec0 _$E503
// FUNCTION: STBC 0x00501ed0 _$E502
// FUNCTION: STBC 0x00501ee0 _$E183
// FUNCTION: STBC 0x00501ef0 _$E182
// FUNCTION: STBC 0x00501f00 _$E188
// FUNCTION: STBC 0x00501f10 _$E187
// FUNCTION: STBC 0x00501f20 _$E507
// FUNCTION: STBC 0x00501f30 _$E506
// FUNCTION: STBC 0x00501f40 _$E511
// FUNCTION: STBC 0x00501f50 _$E510
// FUNCTION: STBC 0x00501f60 _$E515
// FUNCTION: STBC 0x00501f70 _$E514
// FUNCTION: STBC 0x00501f80 _$E519
// FUNCTION: STBC 0x00501f90 _$E518
// FUNCTION: STBC 0x00501fa0 _$E523
// FUNCTION: STBC 0x00501fb0 _$E522
// FUNCTION: STBC 0x00501fc0 _$E527
// FUNCTION: STBC 0x00501fd0 _$E526
// FUNCTION: STBC 0x00501fe0 _$E531
// FUNCTION: STBC 0x00501ff0 _$E530
// FUNCTION: STBC 0x00502000 _$E535
// FUNCTION: STBC 0x00502010 _$E534
// FUNCTION: STBC 0x00502020 _$E193
// FUNCTION: STBC 0x00502030 _$E192
// FUNCTION: STBC 0x00502040 _$E539
// FUNCTION: STBC 0x00502050 _$E538
// FUNCTION: STBC 0x00502060 _$E198
// FUNCTION: STBC 0x00502070 _$E197
// FUNCTION: STBC 0x00502080 _$E203
// FUNCTION: STBC 0x00502090 _$E202
// FUNCTION: STBC 0x00502280 _$E543
// FUNCTION: STBC 0x00502290 _$E542
// FUNCTION: STBC 0x005022a0 _$E548
// FUNCTION: STBC 0x005022b0 _$E547
// FUNCTION: STBC 0x005022c0 _$E552
// FUNCTION: STBC 0x005022d0 _$E551

// The handler table's dynamic initialiser, the registration of its exit-time destructor and the guarded destructor.
// FUNCTION: STBC 0x004fe4e0 _$E558
// FUNCTION: STBC 0x004fe4f0 _$E554
// FUNCTION: STBC 0x004fe500 _$E557
// FUNCTION: STBC 0x004fe510 _$E555
// DATA: STBC 0x00986fd8
TGEventHandlerTable MapWindow::classHandlers;

// The plain panel of the LCARS and System icon groups (invented name).
#define INTERFERENCE_PANEL_ICON		200

// DATA: STBC 0x0088e5f4
const float MapWindow::PERIODIC_UPDATE_INTERVAL = 1.0f;

/*
================
MapWindow::Cast

The object as a MapWindow; NULL when it is none.
================
*/
// FUNCTION: STBC 0x004fe530
MapWindow *MapWindow::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( MAPWINDOW_TYPE ) ) {
		return (MapWindow *)object;
	}
	return NULL;
}

/*
================
MapWindow::MapWindow

Hidden at first; follows group, set, navigation point and target changes.
Its first child is the "Sensor Interference" pane, a black panel with the
system art and a grey panel over it, hidden until the map is jammed.
================
*/
// FUNCTION: STBC 0x004fe560
MapWindow::MapWindow( void ) : MainWindow( MWT_TACTICAL_MAP, 1.0f, 1.0f ) {
	SetNotVisible();
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_GROUP_CHANGED, this, "MapWindow::GroupChangeHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_ENTERED_SET, this, "MapWindow::AddShipHandler" );
	g_kEventManager.AddBroadcastMethodHandler( ET_NAV_POINT_CHANGED, this, "MapWindow::NavPointChanged" );
	g_kEventManager.AddBroadcastMethodHandler( ET_TARGET_WAS_CHANGED, this, "MapWindow::TargetChangedHandler" );
	unknown64 = 0;

	char *lcarsGroup = GraphicsModeInfo::GetCurrentMode()->GetLcarsString();
	TGPane *interference = new TGPane( 1.0f, 1.0f );
	AddChild( interference );

	TGLocalizationDatabase *database = g_kLocalizationManager.Load( "data/TGL/Options.tgl", true );
	interference->AddChild( new TGParagraph( database->GetString( "Sensor Interference" ), 1.0f, &g_kInterfaceBorderColor, "Crillee", 15.0f, TGPF_READ_ONLY ) );
	g_kLocalizationManager.Unload( database );

	NiColorA black( 0.0f, 0.0f, 0.0f, 1.0f );
	interference->AddChild( new TGIcon( lcarsGroup, INTERFERENCE_PANEL_ICON, &black ) );

	TGIcon *icon = new TGIcon( "System", INTERFERENCE_PANEL_ICON );
	TGRect bounds( 0.0f, 0.0f, 1.0f, 1.0f );
	icon->SetBounds( &bounds );
	interference->AddChild( icon );

	NiColorA gray( 0.2f, 0.2f, 0.2f, 1.0f );
	icon = new TGIcon( lcarsGroup, INTERFERENCE_PANEL_ICON, &gray );
	icon->SetBounds( &bounds );
	interference->AddChild( icon );
	interference->SetNotVisible();

	unknown50 = 0.0f;
	unknown54 = 0.0f;
}

/*
================
MapWindow::GotFocus

Entering the map: the subtitles move to the map's place, the player's
camera takes its map mode, the visible objects get reticles and the
periodic update starts.
================
*/
// FUNCTION: STBC 0x004fe8d0
void MapWindow::GotFocus( bool layout ) {
	TGUIObject::GotFocus( false );

	SubtitleWindow *subtitles = (SubtitleWindow *)TopWindow::GetTopWindow()->FindMainWindow( MWT_SUBTITLE );
	MoveToFront( subtitles, false );
	subtitles->SetPositionForMode( 4, false );

	if ( Game::GetCurrentGame() != NULL ) {
		SetClass *set = Game::GetCurrentGame()->GetPlayerSet();
		if ( set != NULL ) {
			g_kSetManager.MakeRenderedSet( set );
			PyEmbed::CallFunction( "Camera", "PlayerCameraAsMap", "", NULL, "()" );
			PyErr_Print();
			AddReticles();
			NiCamera *camera = set->GetActiveNiCamera();
			if ( camera != NULL ) {
				camera->SetLODAdjust( 0.01f );
			}
		}
	}
	ReticleManagerWindow::GetReticleManagerWindow()->Unknown514350();

	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( this );
	event->SetEventType( ET_MAP_WINDOW_PERIODIC_UPDATE );
	TGTimer *timer = new TGTimer;
	timer->SetTimerStart( g_kUtopiaModule.GetGameTime() );
	timer->SetDelay( PERIODIC_UPDATE_INTERVAL );
	timer->SetDuration( TIMER_FOREVER );
	timer->SetEvent( event );
	g_kTimerManager.AddTimer( timer );
	unknown64 = timer->GetObjID();

	Update( g_kUtopiaModule.GetGameTime() );
	SetVisible( layout );
}

/*
================
MapWindow::LostFocus

Leaving the map: the player's camera leaves the map mode, the target
reticles and orientation graphics go, the periodic update stops and the
window goes behind the others.
================
*/
// FUNCTION: STBC 0x004fea50
void MapWindow::LostFocus( bool layout ) {
	TGUIObject::LostFocus( false );

	SetClass *set = Game::GetCurrentGame()->GetPlayerSet();
	if ( set != NULL ) {
		CameraObjectClass *camera = set->GetActiveCamera();
		if ( camera != NULL ) {
			camera->PopCameraMode( NULL );
			camera->GetNiCamera()->SetLODAdjust( 1.0f );
		}
		ReticleManagerWindow::GetReticleManagerWindow()->RemoveAllReticles();
	}
	RemovePlayerOrientationGraphics();
	if ( unknown64 != 0 ) {
		g_kTimerManager.DeleteTimer( unknown64 );
	}
	SetNotVisible( layout );
	TopWindow::GetTopWindow()->MoveToBack( this );
}

/*
================
MapWindow::Initialize

The map mode's Python handlers set the window up.
================
*/
// FUNCTION: STBC 0x004feae0
void MapWindow::Initialize( void ) {
	PyObject *window = VirtualSlot08();
	PyEmbed::CallFunction( "MapModeInterfaceHandlers", "Initialize", "", NULL, "(O)", window );
	Py_DECREF( window );
}

/*
================
MapWindow::Update

Turns the space camera, places the player's orientation graphics and,
while the first child pane is shown, steps its first icon every tenth of a
second.
================
*/
// FUNCTION: STBC 0x004feb20
void MapWindow::Update( float time ) {
	TacticalWindow::SpaceCameraRotation();
	UpdatePlayerOrientationGraphics();

	TGPane *pane = (TGPane *)GetFirstChild();
	if ( pane != NULL && pane->IsCompletelyVisible() ) {
		if ( unknown50 > 0.0f ) {
			unknown54 += time - unknown50;
		}
		unknown50 = time;
		if ( unknown54 > 0.1f ) {
			TGIcon *icon = (TGIcon *)pane->GetNthChild( 2 );
			if ( icon != NULL ) {
				int iconNum = icon->GetIconNum() + 1;
				if ( iconNum > 202 ) {
					iconNum = 200;
				}
				icon->SetIconNum( iconNum );
			}
			unknown54 = 0.0f;
		}
	}
}

/*
================
MapWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x004febc0
void MapWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MapWindow::GroupChangeHandler, "MapWindow::GroupChangeHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MapWindow::AddShipHandler, "MapWindow::AddShipHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MapWindow::NavPointChanged, "MapWindow::NavPointChanged" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MapWindow::PeriodicUpdateHandler, "MapWindow::PeriodicUpdateHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&MapWindow::TargetChangedHandler, "MapWindow::TargetChangedHandler" );
}

/*
================
MapWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x004fec30
void MapWindow::RegisterHandlers( void ) {
	AddPythonFuncHandlerForClass( &classHandlers, ET_MOUSE, "MapModeInterfaceHandlers.HandleMouse" );
	AddPythonFuncHandlerForClass( &classHandlers, ET_KEYBOARD, "MapModeInterfaceHandlers.HandleKeyboard" );
	AddMethodHandlerForClass( &classHandlers, ET_MAP_WINDOW_PERIODIC_UPDATE, "MapWindow::PeriodicUpdateHandler" );
}

/*
================
MapWindow::AddReticle
================
*/
// FUNCTION: STBC 0x004fec70
void MapWindow::AddReticle( ObjectClass *object ) {
	ReticleManagerWindow *manager = ReticleManagerWindow::GetReticleManagerWindow();
	if ( manager->FindReticle( object ) != NULL ) {
		return;
	}
	NamedReticleWindow *reticle = new NamedReticleWindow;
	reticle->SetTargetID( object->GetObjID() );
	manager->AddChild( reticle );
	reticle->SetColor( object->GetAffiliationColor() );
}

/*
================
MapWindow::AddReticles
================
*/
// FUNCTION: STBC 0x004fecf0
void MapWindow::AddReticles( void ) {
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	SetClass *set = player->GetContainingSet();
	if ( set == NULL ) {
		return;
	}
	SensorSubsystem *sensors = player->GetSensorSubsystem();
	if ( sensors == NULL ) {
		return;
	}

	int count;
	int i;
	ShipClass **ships = ShipClass::GetClassObjects( set, &count );
	for ( i = 0; i < count; i++ ) {
		ShipClass *ship = ships[i];
		if ( ship != player->GetTarget() && ( ship == player || ( ship != NULL && sensors->IsObjectVisible( ship ) == true && ship->IsTargetable() == true ) ) ) {
			AddReticle( ship );
		}
	}

	PlacementObject **placements = PlacementObject::GetClassObjects( set, &count );
	for ( i = 0; i < count; i++ ) {
		if ( placements[i]->IsNavPoint() ) {
			AddReticle( placements[i] );
		}
	}
}

/*
================
MapWindow::Unknown4FEE00

Shows the "Sensor Interference" pane: the text centred, the grey panel
behind it with a margin of an eighth of its height.
================
*/
// FUNCTION: STBC 0x004fee00
// PARKED: floats copied through the FPU: retail copies the text's height and width into their locals with fld/fstp, this build with integer moves (TGUIObject::GetHeight/GetWidth, a NiPoint2 and operand orders tried); the rest matches
void MapWindow::Unknown4FEE00( void ) {
	TGPane *interference = (TGPane *)GetFirstChild();
	if ( interference == NULL || interference->IsVisible() ) {
		return;
	}
	interference->SetVisible();
	TGParagraph *text = (TGParagraph *)interference->GetFirstChild();
	TGUIObject *panel = interference->GetNthChild( 1 );
	text->Layout();
	float height = text->GetHeight();
	float width = text->GetWidth();
	float x = ( 1.0f - width ) * 0.5f;
	float y = ( 1.0f - height ) * 0.5f;
	float margin = height * 0.125f;
	text->SetPosition( x, y );
	TGRect bounds( x - margin, y, margin + margin + width, margin + height );
	panel->SetBounds( &bounds );
}

/*
================
MapWindow::Unknown4FEF30
================
*/
// FUNCTION: STBC 0x004fef30
void MapWindow::Unknown4FEF30( void ) {
	TGUIObject *pane = GetFirstChild();
	if ( pane != NULL && pane->IsVisible() ) {
		pane->SetNotVisible( true );
	}
}

/*
================
MapWindow::UpdatePlayerOrientationGraphics

With no player the graphics go. Otherwise they are built when missing; in a
nebula that is too dense at the graphics the sensor interference pane shows,
and elsewhere the reticles follow the ships of the set.
================
*/
// FUNCTION: STBC 0x004fef50 ?UpdatePlayerOrientationGraphics@MapWindow@@QAEXXZ
void MapWindow::UpdatePlayerOrientationGraphics( void ) {
	Game *game = Game::GetCurrentGame();
	ShipClass *player;
	if ( game == NULL || ( player = game->GetPlayer() ) == NULL ) {
		RemovePlayerOrientationGraphics();
		return;
	}
	if ( unknown58 == NULL ) {
		Unknown4FF0F0( player );
	}
	SetClass *set = player->GetContainingSet();
	if ( set == NULL ) {
		return;
	}
	// Inside a nebula the graphics show only while the nebula's VirtualSlot71
	// for their bound is above zero.
	bool visible = true;
	if ( set->GetNebula() != NULL ) {
		unknown58->Update( 0.0f );
		visible = set->GetNebula()->VirtualSlot71( &unknown58->GetWorldBound() ) > 0.0f;
	}
	if ( visible ) {
		Unknown4FEF30();
		int count;
		ObjectClass **ships = ObjectClass::GetClassObjects( set, &count );
		Unknown500BA0( player, player->GetTarget(), count, ships );
	} else {
		Unknown4FEE00();
	}
}

/*
================
MapWindow::RemovePlayerOrientationGraphics

Detaches the player's orientation graphics from the scene and drops them.
================
*/
// FUNCTION: STBC 0x004ff020
void MapWindow::RemovePlayerOrientationGraphics( void ) {
	if ( unknown58 != NULL ) {
		NiNode *parent = unknown58->GetParent();
		if ( parent != NULL ) {
			parent->DetachChild( unknown58 );
		}
		unknown58 = NULL;
	}
	if ( unknown60 != NULL ) {
		NiNode *parent = unknown60->GetParent();
		if ( parent != NULL ) {
			parent->DetachChild( unknown60 );
		}
		unknown60 = NULL;
	}
	Unknown501380();
}

/*
================
MapWindow::Unknown4FF0F0

Builds the player's orientation graphics from four meshes: the horizontal
and the vertical ring around the player (a triangular tube of 32 segments
each, shaded by the segment's angle), the ticks across the horizontal ring
(longer every second, fourth and eighth segment) and a grey disc inside it
that is blended. The node holding them hangs under the player's node.
================
*/
// FUNCTION: STBC 0x004ff0f0 ?Unknown4FF0F0@MapWindow@@QAEXPAVShipClass@@@Z
// DRAFT: the ring geometry in NiPoint3 operators (retail's out-of-line NiPoint3 constructors sit inside them); inline budget: retail expands the tick textures' NiPoint2::ZERO copies, the NiZBufferProperty constructor and both AttachProperty calls at the end, where this build has run out
void MapWindow::Unknown4FF0F0( ShipClass *player ) {
	NiPoint3 *ringVertices = new NiPoint3[96];
	NiPoint3 *ring2Vertices = new NiPoint3[96];
	NiColorA *ringColors = new NiColorA[192];
	NiColorA *ring2Colors = new NiColorA[192];
	NiPoint2 *ringTextures = new NiPoint2[96];
	NiPoint2 *ring2Textures = new NiPoint2[96];
	unsigned short *ringTriangles = new unsigned short[576];
	unsigned short *ring2Triangles = new unsigned short[576];
	NiPoint3 *tickVertices = new NiPoint3[128];
	NiColorA *tickColors = new NiColorA[256];
	NiPoint2 *tickTextures = new NiPoint2[128];
	unsigned short *tickTriangles = new unsigned short[288];
	NiPoint3 *discVertices = new NiPoint3[33];
	NiColorA *discColors = new NiColorA[33];
	NiPoint2 *discTextures = new NiPoint2[33];
	unsigned short *discTriangles = new unsigned short[192];

	// The disc's centre.
	discVertices[0] = NiPoint3::ZERO;
	discColors[0] = NiColorA( 0.375f, 0.375f, 0.375f, 0.5f );
	discTextures[0] = NiPoint2::ZERO;

	for ( int i = 0; i < 32; i++ ) {
		float angle = i * RING_STEP * NI_TWO_PI;
		float sine = sin( angle );

		// The point of each ring: the horizontal one between forward and right, the vertical one between forward and up.
		NiPoint3 flat = ( cos( angle ) * TGPoint3::GetModelForward() + sine * TGPoint3::GetModelRight() ) * STALK_RANGE;
		NiPoint3 upright = ( cos( angle ) * TGPoint3::GetModelForward() + sine * TGPoint3::GetModelUp() ) * STALK_RANGE;

		NiPoint3 flatDirection = flat;
		flatDirection.Unitize();
		NiPoint3 up = TGPoint3::GetModelUp();
		NiPoint3 uprightDirection = upright;
		uprightDirection.Unitize();
		NiPoint3 right = TGPoint3::GetModelRight();

		// The tube's section is a triangle: one corner above the point, two below it to either side.
		NiPoint3 flatUp = up * STALK_WIDTH;
		NiPoint3 flatDrop = -up * sin( NI_PI * ( 1.0f / 6.0f ) ) * STALK_WIDTH;
		NiPoint3 flatSide = flatDirection * cos( NI_PI * ( 1.0f / 6.0f ) ) * STALK_WIDTH;
		NiPoint3 uprightUp = uprightDirection * STALK_WIDTH;
		NiPoint3 uprightDrop = -uprightDirection * sin( NI_PI * ( 1.0f / 6.0f ) ) * STALK_WIDTH;
		NiPoint3 uprightSide = cos( NI_PI * ( 1.0f / 6.0f ) ) * right * STALK_WIDTH;

		ringVertices[i * 3] = flat + flatUp;
		ringVertices[i * 3 + 1] = flat + flatDrop + flatSide;
		ringVertices[i * 3 + 2] = flat + flatDrop - flatSide;

		ring2Vertices[i * 3] = upright + uprightUp;
		ring2Vertices[i * 3 + 1] = upright + uprightDrop + uprightSide;
		ring2Vertices[i * 3 + 2] = upright + uprightDrop - uprightSide;

		// The ticks reach further in at every second, fourth and eighth segment.
		float tick = 0.1f;
		if ( i % 8 == 0 ) {
			tick = 0.5f;
		} else if ( i % 4 == 0 ) {
			tick = 0.3f;
		} else if ( i % 2 == 0 ) {
			tick = 0.175f;
		}
		NiMatrix3 turn;
		turn.MakeRotation( angle, TGPoint3::GetModelUp() );
		NiPoint3 across = turn * TGPoint3::GetModelRight();
		tickVertices[i * 4] = flat * ( 1.0f - tick ) - flatDirection;
		tickVertices[i * 4 + 1] = flat + flatUp;
		tickVertices[i * 4 + 2] = flat + flatDrop - flatSide + across * STALK_WIDTH;
		tickVertices[i * 4 + 3] = flat + flatDrop - flatSide - across * STALK_WIDTH;
		discVertices[i + 1] = flat;

		// The colours brighten with the cosine of the segment.
		float shade = ( (float)cos( i * TWO_PI * RING_STEP ) + 1.0f ) * 0.5f;
		NiColorA color( shade, shade, 1.0f, 1.0f );
		ringColors[i * 3] = color;
		ringColors[i * 3 + 1] = color;
		ringColors[i * 3 + 2] = color;
		ring2Colors[i * 3] = color;
		ring2Colors[i * 3 + 1] = color;
		ring2Colors[i * 3 + 2] = color;
		tickColors[i * 4] = NiColorA( 0.5f, 0.5f, 0.5f, 1.0f );
		tickColors[i * 4 + 1] = color;
		tickColors[i * 4 + 2] = color;
		tickColors[i * 4 + 3] = color;
		ringColors[96 + i * 3] = color;
		ringColors[96 + i * 3 + 1] = color;
		ringColors[96 + i * 3 + 2] = color;
		ring2Colors[96 + i * 3] = color;
		ring2Colors[96 + i * 3 + 1] = color;
		ring2Colors[96 + i * 3 + 2] = color;
		tickColors[128 + i * 4] = NiColorA( 0.5f, 0.5f, 0.5f, 1.0f );
		tickColors[128 + i * 4 + 1] = color;
		tickColors[128 + i * 4 + 2] = color;
		tickColors[128 + i * 4 + 3] = color;
		discColors[i + 1] = NiColorA( 0.375f, 0.375f, 0.375f, 0.5f );
		ringTextures[i * 3].x = 0.0f;
		ringTextures[i * 3].y = 0.0f;
		ringTextures[i * 3 + 1].x = 0.0f;
		ringTextures[i * 3 + 1].y = 0.0f;
		ringTextures[i * 3 + 2].x = 0.0f;
		ringTextures[i * 3 + 2].y = 0.0f;
		ring2Textures[i * 3].x = 0.0f;
		ring2Textures[i * 3].y = 0.0f;
		ring2Textures[i * 3 + 1].x = 0.0f;
		ring2Textures[i * 3 + 1].y = 0.0f;
		ring2Textures[i * 3 + 2].x = 0.0f;
		ring2Textures[i * 3 + 2].y = 0.0f;
		tickTextures[i * 4] = NiPoint2::ZERO;
		tickTextures[i * 4 + 1] = NiPoint2::ZERO;
		tickTextures[i * 4 + 2] = NiPoint2::ZERO;
		tickTextures[i * 4 + 3] = NiPoint2::ZERO;
		discTextures[i + 1] = NiPoint2::ZERO;

		// Each segment of a ring is joined to the next one's corners.
		int base = i * 3;
		unsigned short current0 = base % 96;
		unsigned short current1 = ( base + 1 ) % 96;
		unsigned short current2 = ( base + 2 ) % 96;
		unsigned short next0 = ( base + 3 ) % 96;
		unsigned short next1 = ( base + 4 ) % 96;
		unsigned short next2 = ( base + 5 ) % 96;
		unsigned short *triangles = ringTriangles + i * 18;
		triangles[0] = next0;
		triangles[1] = next1;
		triangles[2] = current0;
		triangles[3] = next1;
		triangles[4] = current1;
		triangles[5] = current0;
		triangles[6] = next1;
		triangles[7] = current2;
		triangles[8] = current1;
		triangles[9] = next1;
		triangles[10] = next2;
		triangles[11] = current2;
		triangles[12] = next2;
		triangles[13] = next0;
		triangles[14] = current2;
		triangles[15] = next0;
		triangles[16] = current0;
		triangles[17] = current2;
		for ( int k = 0; k < 18; k++ ) {
			ring2Triangles[i * 18 + k] = ringTriangles[i * 18 + k];
		}

		unsigned short corner = i * 4;
		unsigned short *tickTriangle = tickTriangles + i * 9;
		tickTriangle[0] = corner;
		tickTriangle[1] = corner + 2;
		tickTriangle[2] = corner + 1;
		tickTriangle[3] = corner;
		tickTriangle[4] = corner + 3;
		tickTriangle[5] = corner + 2;
		tickTriangle[6] = corner;
		tickTriangle[7] = corner + 1;
		tickTriangle[8] = corner + 3;

		// The disc is a fan, drawn from both sides.
		unsigned short nextEdge = ( i + 1 ) % 32 + 1;
		unsigned short *fan = discTriangles + i * 6;
		fan[0] = 0;
		fan[1] = nextEdge;
		fan[2] = i + 1;
		fan[3] = 0;
		fan[4] = i + 1;
		fan[5] = nextEdge;
	}

	NiTriShape *ring = new NiTriShape( 96, ringVertices, NULL, ringColors, ringTextures, 192, ringTriangles );
	NiTriShape *ring2 = new NiTriShape( 96, ring2Vertices, NULL, ring2Colors, ring2Textures, 192, ring2Triangles );
	NiTriShape *ticks = new NiTriShape( 128, tickVertices, NULL, tickColors, tickTextures, 96, tickTriangles );
	NiTriShape *disc = new NiTriShape( 33, discVertices, NULL, discColors, discTextures, 64, discTriangles );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( false );
	disc->AttachProperty( zBuffer );
	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( true );
	disc->AttachProperty( alpha );

	unknown58 = new NiNode;
	unknown58->AttachChild( ring, false );
	unknown58->AttachChild( ring2, false );
	unknown58->AttachChild( ticks, false );
	unknown58->AttachChild( disc, false );
	Unknown500990( unknown58 );
	unknown58->UpdateProperties();
	unknown58->Update( 0.0f );
	player->GetNode()->AttachChild( unknown58, false );
}

/*
================
MapWindow::Unknown500990

White-textured, depth-tested meshes whose vertex colours are shown as they
are.
================
*/
// FUNCTION: STBC 0x00500990
// PARKED: EH states (structure, decided outside this function): retail treats NiProperty::SetBit as non-throwing (no state inside the inline NiZBufferProperty constructor) and SetField as throwing (state 4 inside NiTextureModeProperty's); without /YX this build treats both as non-throwing; the mix follows what the unit compiled before (Unknown4FF0F0 calls NiAlphaProperty's constructor out of line at 0x500970 and never SetField in retail)
void MapWindow::Unknown500990( NiAVObject *object ) {
	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	object->AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( true );
	object->AttachProperty( zBuffer );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( "data/Textures/White.tga", true, true ) );
	object->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	object->AttachProperty( textureMode );
}

/*
================
MapWindow::Unknown500BA0

Builds a stalk for each of the ships that the player can target and that
stands within STALK_RANGE of the player's plane projection, and hangs the
line graphic they make under the set's scene. Nothing to show drops the
old graphic.
================
*/
// FUNCTION: STBC 0x00500ba0 ?Unknown500BA0@MapWindow@@QAEXPAVShipClass@@PAVObjectClass@@HPAPAV3@@Z
// DRAFT: inline budget: retail expands the dropped point's NiPoint3 constructor and calls flat's (one site fewer after it); the range test as a sum of squares into a local and the widths written at both arguments give 95.5 but expand the list's RemoveAll at the end, losing its out-of-line copy (0x5020E0), so this stays; see the card
void MapWindow::Unknown500BA0( ShipClass *player, ObjectClass *target, int count, ObjectClass **ships ) {
	SetClass *set = player->GetContainingSet();
	if ( player == NULL || count < 1 || set == NULL ) {
		Unknown501380();
		return;
	}
	if ( unknown5C != NULL ) {
		Unknown501380();
	}

	NiTList<MapStalk *> stalks( 2 );
	ShipClass *playerShip = ShipClass::Cast( player );
	for ( int i = 0; i < count; i++ ) {
		ObjectClass *object = ships[i];
		if ( object != player && ( playerShip == NULL || playerShip->CanTargetObject( object ) ) ) {
			NiPoint3 position = object->GetWorldLocation();
			NiPoint3 offset = position - player->GetWorldLocation();
			float height = offset.Dot( player->GetWorldUp() );
			NiPoint3 base = position - height * player->GetWorldUp();
			NiPoint3 flat( base - player->GetWorldLocation() );
			if ( flat.SqrLength() <= STALK_RANGE * STALK_RANGE ) {
				NiColorA color = *object->GetAffiliationColor();
				NiColorA tipColor = color;
				float width = STALK_WIDTH * 0.75f;
				stalks.AddTail( new MapStalk( base, position, tipColor, color, width, width ) );
			}
		}
	}

	unknown5C = Unknown500F50( &stalks );
	if ( unknown5C != NULL ) {
		unknown5C->UpdateProperties();
		unknown5C->Update( 0.0f );
		set->GetSceneRoot()->AttachChild( unknown5C, false );
	}
}

/*
================
MapWindow::Unknown500F50

One line segment for each stalk, from its base to its tip, the vertex
colours of its two ends in white-textured, unlit, depth-tested lines.
The list is emptied.
================
*/
// FUNCTION: STBC 0x00500f50 ?Unknown500F50@MapWindow@@SAPAVNiLines@@PAV?$NiTList@PAUMapStalk@@@@@Z
// PARKED: frame slots: the demo's copy of this code (stbcdemo 0x500BF0, same 1036 bytes) differs from retail only in two locals' frame slots and one register pick, so retail's slot order flips between builds; still open besides: retail ORs 3 into the z-buffer property's flags where this calls SetBit twice, and the vertex-flag stores walk a pointer stepped by 2
NiLines *MapWindow::Unknown500F50( NiTList<MapStalk *> *stalks ) {
	int count = stalks->GetSize();
	if ( count == 0 ) {
		return NULL;
	}
	int vertexCount = count * 2;
	NiPoint3 *vertices = new NiPoint3[vertexCount];
	NiColorA *colors = new NiColorA[vertexCount];
	NiPoint2 *textures = new NiPoint2[vertexCount];
	bool *flags = new bool[vertexCount];
	int vertex = 0;
	while ( stalks->GetSize() != 0 ) {
		// BUG: the stalk is never freed.
		MapStalk *stalk = stalks->RemoveHead();
		vertices[vertex] = stalk->base;
		vertices[vertex + 1] = stalk->tip;
		colors[vertex] = stalk->baseColor;
		colors[vertex + 1] = stalk->tipColor;
		flags[vertex] = true;
		flags[vertex + 1] = false;
		vertex += 2;
	}

	NiLines *lines = new NiLines( vertexCount, vertices, colors, textures, flags );

	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_EMISSIVE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E );
	lines->AttachProperty( vertexColor );

	NiZBufferProperty *zBuffer = new NiZBufferProperty;
	zBuffer->SetZBufferTest( true );
	zBuffer->SetZBufferWrite( true );
	lines->AttachProperty( zBuffer );

	NiAlphaProperty *alpha = new NiAlphaProperty;
	alpha->SetAlphaBlending( false );
	alpha->SetNoSorter( true );
	lines->AttachProperty( alpha );

	NiTextureProperty *texture = new NiTextureProperty;
	texture->SetImage( g_kImageManager.GetImage( "data/Textures/White.tga", true, true ) );
	lines->AttachProperty( texture );

	NiTextureModeProperty *textureMode = new NiTextureModeProperty;
	textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE );
	lines->AttachProperty( textureMode );
	return lines;
}

/*
================
MapWindow::Unknown501380
================
*/
// FUNCTION: STBC 0x00501380
void MapWindow::Unknown501380( void ) {
	if ( unknown5C != NULL ) {
		NiNode *parent = unknown5C->GetParent();
		if ( parent != NULL ) {
			parent->DetachChild( unknown5C );
		}
		unknown5C = NULL;
	}
}

/*
================
MapWindow::GroupChangeHandler

When one of the mission's affiliation groups changes, the reticles take the
colours of their ships again.
================
*/
// FUNCTION: STBC 0x005013f0 ?GroupChangeHandler@MapWindow@@QAEXPAVTGEvent@@@Z
// PARKED: 93.9, complete; scheduling: retail stores the NULL of the second and third group pointer after pushing their arguments and saves this after the player set test; declaration forms of the three pointers change nothing
void MapWindow::GroupChangeHandler( TGEvent *event ) {
	Game *game = Game::GetCurrentGame();
	if ( game != NULL ) {
		SetClass *playerSet = game->GetPlayerSet();
		if ( playerSet != NULL ) {
			Mission *mission = game->GetCurrentEpisode()->GetCurrentMission();
			void *friendlyGroup = NULL;
			PyEmbed::Unknown6F98E0( mission->GetFriendlyGroup(), &friendlyGroup, "ObjectGroup" );
			void *enemyGroup = NULL;
			PyEmbed::Unknown6F98E0( mission->GetEnemyGroup(), &enemyGroup, "ObjectGroup" );
			void *neutralGroup = NULL;
			PyEmbed::Unknown6F98E0( mission->GetNeutralGroup(), &neutralGroup, "ObjectGroup" );
			if ( event->GetDestination() == friendlyGroup || event->GetDestination() == enemyGroup || event->GetDestination() == neutralGroup ) {
				ReticleManagerWindow *manager = ReticleManagerWindow::GetReticleManagerWindow();
				// BUG: the walk asks each reticle for its next sibling instead of the manager
				for ( ReticleWindow *reticle = static_cast<ReticleWindow *>( manager->GetFirstChild() ); reticle != NULL; reticle = static_cast<ReticleWindow *>( reticle->GetNextChild( reticle ) ) ) {
					ShipClass *ship = ShipClass::GetObjectByID( playerSet, reticle->GetTargetID() );
					if ( ship != NULL ) {
						NiColorA color = *ship->GetAffiliationColor();
						reticle->SetColor( &color );
					}
				}
			}
		}
	}
	CallNextHandler( event );
}

/*
================
MapWindow::AddShipHandler

A ship the player sees gets a reticle while the map is up.
================
*/
// FUNCTION: STBC 0x00501510
void MapWindow::AddShipHandler( TGEvent *event ) {
	if ( !IsWindowActive() ) {
		CallNextHandler( event );
		return;
	}
	ObjectClass *ship = (ObjectClass *)event->GetDestination();
	if ( !ship->IsTypeOf( SHIPCLASS_TYPE ) ) {
		return;
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || player->GetSensorSubsystem() == NULL ) {
		return;
	}
	if ( ship == player || ( player->GetContainingSet() != NULL && player->GetSensorSubsystem()->IsObjectVisible( ship ) == true && ship->IsTargetable() == true ) ) {
		AddReticle( ship );
	}
}

/*
================
MapWindow::NavPointChanged

A nav point of the player's set gets a reticle while it is shown and loses
it otherwise.
================
*/
// FUNCTION: STBC 0x00501590
void MapWindow::NavPointChanged( TGEvent *event ) {
	if ( !IsWindowActive() ) {
		CallNextHandler( event );
		return;
	}
	PlacementObject *placement = (PlacementObject *)event->GetDestination();
	Game *game = Game::GetCurrentGame();
	if ( game == NULL ) {
		return;
	}
	SetClass *set = game->GetPlayerSet();
	if ( set == NULL || PlacementObject::GetObjectByID( set, placement->GetObjID() ) != placement ) {
		return;
	}
	if ( placement->IsNavPoint() ) {
		AddReticle( placement );
	} else {
		ReticleManagerWindow::GetReticleManagerWindow()->RemoveReticles( placement->GetObjID() );
	}
}

/*
================
MapWindow::PeriodicUpdateHandler

Once a second while the map is up: a reticle whose ship the player's sensors
lost goes away, a ship they gained gets one, and the map is laid out again.
================
*/
// FUNCTION: STBC 0x00501610 ?PeriodicUpdateHandler@MapWindow@@QAEXPAVTGEvent@@@Z
// DRAFT: register: retail keeps a zero in esi for the NULL tests (re-zeroed at the reticle loop top, pushed as GetObjectByID's NULL), the loop count in ebp and player in ebx; this, sensors and the count list slots swapped
void MapWindow::PeriodicUpdateHandler( TGEvent *event ) {
	if ( !IsWindowActive() ) {
		CallNextHandler( event );
		return;
	}
	ReticleManagerWindow *manager = ReticleManagerWindow::GetReticleManagerWindow();
	TGUIObject *reticle = manager->GetFirstChild();
	NiTList<ShipClass *> reticleShips( 2 );
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL ) {
		return;
	}
	SetClass *set = player->GetContainingSet();
	SensorSubsystem *sensors = player->GetSensorSubsystem();
	if ( sensors == NULL ) {
		return;
	}
	for ( ; reticle != NULL; reticle = manager->GetNextChild( reticle ) ) {
		ShipClass *ship = ShipClass::Cast( ObjectClass::GetObjectByID( NULL, ( (ReticleWindow *)reticle )->GetTargetID() ) );
		if ( ship != NULL ) {
			reticleShips.AddTail( ship );
			if ( ship != player && ( !sensors->IsObjectVisible( ship ) || !ship->IsTargetable() ) ) {
				manager->RemoveReticles( ship->GetObjID() );
			}
		}
	}

	int count;
	ShipClass **ships = ShipClass::GetClassObjects( set, &count );
	for ( int i = 0; i < count; i++ ) {
		ShipClass *ship = ships[i];
		if ( reticleShips.FindPos( ship ) == NULL ) {
			if ( ship == player ) {
				AddReticle( ship );
			} else if ( sensors->IsObjectVisible( ship ) == true && ships[i]->IsTargetable() == true && ships[i] != player->GetTarget() ) {
				AddReticle( ships[i] );
			}
		}
	}
	Layout();
	CallNextHandler( event );
}

/*
================
MapWindow::TargetChangedHandler

The player's new target gets a reticle, and the old one's goes.
================
*/
// FUNCTION: STBC 0x005018f0
void MapWindow::TargetChangedHandler( TGEvent *event ) {
	if ( !IsWindowActive() ) {
		CallNextHandler( event );
		return;
	}
	ShipClass *player = Game::GetCurrentPlayer();
	if ( player == NULL || event->GetDestination() != player || !event->IsTypeOf( TGOBJPTREVENT_TYPE ) ) {
		return;
	}
	ObjectClass *target = player->GetTarget();
	ObjectClass *oldTarget = (ObjectClass *)( (TGObjPtrEvent *)event )->GetObjPtr();
	if ( oldTarget == target ) {
		return;
	}
	if ( oldTarget != NULL ) {
		AddReticle( oldTarget );
	}
	if ( target != NULL && ReticleManagerWindow::GetReticleManagerWindow() != NULL ) {
		ReticleManagerWindow::GetReticleManagerWindow()->RemoveReticles( target->GetObjID() );
	}
}

/*
================
MapWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x005019a0
bool MapWindow::WriteToStream( TGStream *stream ) {
	if ( !MainWindow::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( unknown64 );
	stream->WriteFloat( unknown50 );
	stream->WriteFloat( unknown54 );
	return true;
}

/*
================
MapWindow::ReadFromStream

The graphics are rebuilt rather than loaded.
================
*/
// FUNCTION: STBC 0x005019f0
bool MapWindow::ReadFromStream( TGStream *stream ) {
	if ( !MainWindow::ReadFromStream( stream ) ) {
		return false;
	}
	unknown58 = NULL;
	unknown5C = NULL;
	unknown60 = NULL;
	unknown64 = stream->ReadID();
	unknown50 = stream->ReadFloat();
	unknown54 = stream->ReadFloat();
	return true;
}

// DATA: STBC 0x0088e5e4
const float MapWindow::TWO_PI = 6.2831855f;

// DATA: STBC 0x0088e5ec
const float MapWindow::STALK_RANGE = 250.0f;

// DATA: STBC 0x0088e5f0
const float MapWindow::STALK_WIDTH = 1.25f;

// DATA: STBC 0x0088e5f8
const float MapWindow::RING_STEP = 0.03125f;
