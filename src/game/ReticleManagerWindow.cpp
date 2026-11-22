/*
===============================================================================

	ReticleManagerWindow.cpp

===============================================================================
*/

#include "ReticleManagerWindow.h"
#include "TopWindow.h"
#include "MainWindow.h"
#include "ReticleWindow.h"
#include "ObjectClass.h"
#include "ShipClass.h"
#include "TGEvent.h"
#include "TGEventManager.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "PhysicsObjectClass.h"
#include "TGIconManager.h"
#include "UtopiaModule.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00514d60 _$E483
// FUNCTION: STBC 0x00514d80 _$E482

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00513d90 _$E33
// FUNCTION: STBC 0x00513da0 _$E32
// FUNCTION: STBC 0x00513db0 _$E37
// FUNCTION: STBC 0x00513dc0 _$E36
// FUNCTION: STBC 0x005144e0 _$E48
// FUNCTION: STBC 0x005144f0 _$E47
// FUNCTION: STBC 0x00514500 _$E52
// FUNCTION: STBC 0x00514510 _$E51
// FUNCTION: STBC 0x00514520 _$E56
// FUNCTION: STBC 0x00514530 _$E55
// FUNCTION: STBC 0x00514540 _$E60
// FUNCTION: STBC 0x00514550 _$E59
// FUNCTION: STBC 0x00514560 _$E184
// FUNCTION: STBC 0x00514570 _$E183
// FUNCTION: STBC 0x00514580 _$E126
// FUNCTION: STBC 0x00514590 _$E125
// FUNCTION: STBC 0x005145a0 _$E139
// FUNCTION: STBC 0x005145b0 _$E138
// FUNCTION: STBC 0x005145c0 _$E189
// FUNCTION: STBC 0x005145d0 _$E188
// FUNCTION: STBC 0x005145e0 _$E194
// FUNCTION: STBC 0x005145f0 _$E193
// FUNCTION: STBC 0x00514600 _$E198
// FUNCTION: STBC 0x00514610 _$E197
// FUNCTION: STBC 0x00514620 _$E202
// FUNCTION: STBC 0x00514630 _$E201
// FUNCTION: STBC 0x00514640 _$E209
// FUNCTION: STBC 0x00514650 _$E208
// FUNCTION: STBC 0x00514660 _$E319
// FUNCTION: STBC 0x00514670 _$E318
// FUNCTION: STBC 0x00514680 _$E323
// FUNCTION: STBC 0x00514690 _$E322
// FUNCTION: STBC 0x005146a0 _$E84
// FUNCTION: STBC 0x005146b0 _$E83
// FUNCTION: STBC 0x005146c0 _$E253
// FUNCTION: STBC 0x005146d0 _$E252
// FUNCTION: STBC 0x005146e0 _$E260
// FUNCTION: STBC 0x005146f0 _$E259
// FUNCTION: STBC 0x00514700 _$E264
// FUNCTION: STBC 0x00514710 _$E263
// FUNCTION: STBC 0x00514720 _$E268
// FUNCTION: STBC 0x00514730 _$E267
// FUNCTION: STBC 0x00514740 _$E272
// FUNCTION: STBC 0x00514750 _$E271
// FUNCTION: STBC 0x00514760 _$E278
// FUNCTION: STBC 0x00514770 _$E277
// FUNCTION: STBC 0x00514780 _$E327
// FUNCTION: STBC 0x00514790 _$E326
// FUNCTION: STBC 0x005147a0 _$E331
// FUNCTION: STBC 0x005147b0 _$E330
// FUNCTION: STBC 0x005147c0 _$E335
// FUNCTION: STBC 0x005147d0 _$E334
// FUNCTION: STBC 0x005147e0 _$E339
// FUNCTION: STBC 0x005147f0 _$E338
// FUNCTION: STBC 0x00514800 _$E343
// FUNCTION: STBC 0x00514810 _$E342
// FUNCTION: STBC 0x00514820 _$E347
// FUNCTION: STBC 0x00514830 _$E346
// FUNCTION: STBC 0x00514840 _$E351
// FUNCTION: STBC 0x00514850 _$E350
// FUNCTION: STBC 0x00514860 _$E355
// FUNCTION: STBC 0x00514870 _$E354
// FUNCTION: STBC 0x00514880 _$E359
// FUNCTION: STBC 0x00514890 _$E358
// FUNCTION: STBC 0x005148a0 _$E363
// FUNCTION: STBC 0x005148b0 _$E362
// FUNCTION: STBC 0x005148c0 _$E89
// FUNCTION: STBC 0x005148d0 _$E88
// FUNCTION: STBC 0x005148e0 _$E367
// FUNCTION: STBC 0x005148f0 _$E366
// FUNCTION: STBC 0x00514900 _$E94
// FUNCTION: STBC 0x00514910 _$E93
// FUNCTION: STBC 0x00514920 _$E371
// FUNCTION: STBC 0x00514930 _$E370
// FUNCTION: STBC 0x00514940 _$E375
// FUNCTION: STBC 0x00514950 _$E374
// FUNCTION: STBC 0x00514960 _$E379
// FUNCTION: STBC 0x00514970 _$E378
// FUNCTION: STBC 0x00514980 _$E383
// FUNCTION: STBC 0x00514990 _$E382
// FUNCTION: STBC 0x005149a0 _$E103
// FUNCTION: STBC 0x005149b0 _$E102
// FUNCTION: STBC 0x005149c0 _$E387
// FUNCTION: STBC 0x005149d0 _$E386
// FUNCTION: STBC 0x005149e0 _$E391
// FUNCTION: STBC 0x005149f0 _$E390
// FUNCTION: STBC 0x00514a00 _$E395
// FUNCTION: STBC 0x00514a10 _$E394
// FUNCTION: STBC 0x00514a20 _$E109
// FUNCTION: STBC 0x00514a30 _$E108
// FUNCTION: STBC 0x00514a40 _$E114
// FUNCTION: STBC 0x00514a50 _$E113
// FUNCTION: STBC 0x00514a60 _$E144
// FUNCTION: STBC 0x00514a70 _$E143
// FUNCTION: STBC 0x00514a80 _$E399
// FUNCTION: STBC 0x00514a90 _$E398
// FUNCTION: STBC 0x00514aa0 _$E149
// FUNCTION: STBC 0x00514ab0 _$E148
// FUNCTION: STBC 0x00514ac0 _$E403
// FUNCTION: STBC 0x00514ad0 _$E402
// FUNCTION: STBC 0x00514ae0 _$E407
// FUNCTION: STBC 0x00514af0 _$E406
// FUNCTION: STBC 0x00514b00 _$E154
// FUNCTION: STBC 0x00514b10 _$E153
// FUNCTION: STBC 0x00514b20 _$E411
// FUNCTION: STBC 0x00514b30 _$E410
// FUNCTION: STBC 0x00514b40 _$E119
// FUNCTION: STBC 0x00514b50 _$E118
// FUNCTION: STBC 0x00514b60 _$E415
// FUNCTION: STBC 0x00514b70 _$E414
// FUNCTION: STBC 0x00514b80 _$E419
// FUNCTION: STBC 0x00514b90 _$E418
// FUNCTION: STBC 0x00514ba0 _$E159
// FUNCTION: STBC 0x00514bb0 _$E158
// FUNCTION: STBC 0x00514bc0 _$E164
// FUNCTION: STBC 0x00514bd0 _$E163
// FUNCTION: STBC 0x00514be0 _$E423
// FUNCTION: STBC 0x00514bf0 _$E422
// FUNCTION: STBC 0x00514c00 _$E427
// FUNCTION: STBC 0x00514c10 _$E426
// FUNCTION: STBC 0x00514c20 _$E431
// FUNCTION: STBC 0x00514c30 _$E430
// FUNCTION: STBC 0x00514c40 _$E435
// FUNCTION: STBC 0x00514c50 _$E434
// FUNCTION: STBC 0x00514c60 _$E439
// FUNCTION: STBC 0x00514c70 _$E438
// FUNCTION: STBC 0x00514c80 _$E443
// FUNCTION: STBC 0x00514c90 _$E442
// FUNCTION: STBC 0x00514ca0 _$E447
// FUNCTION: STBC 0x00514cb0 _$E446
// FUNCTION: STBC 0x00514cc0 _$E451
// FUNCTION: STBC 0x00514cd0 _$E450
// FUNCTION: STBC 0x00514ce0 _$E169
// FUNCTION: STBC 0x00514cf0 _$E168
// FUNCTION: STBC 0x00514d00 _$E455
// FUNCTION: STBC 0x00514d10 _$E454
// FUNCTION: STBC 0x00514d20 _$E174
// FUNCTION: STBC 0x00514d30 _$E173
// FUNCTION: STBC 0x00514d40 _$E179
// FUNCTION: STBC 0x00514d50 _$E178
// FUNCTION: STBC 0x00514d90 _$E459
// FUNCTION: STBC 0x00514da0 _$E458
// FUNCTION: STBC 0x00514db0 _$E464
// FUNCTION: STBC 0x00514dc0 _$E463
// FUNCTION: STBC 0x00514dd0 _$E468
// FUNCTION: STBC 0x00514de0 _$E467

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00513d10 _$E303
// FUNCTION: STBC 0x00513d20 _$E300
// FUNCTION: STBC 0x00513d30 _$E302
// FUNCTION: STBC 0x00513d40 _$E301
// FUNCTION: STBC 0x00513d50 _$E309
// FUNCTION: STBC 0x00513d60 _$E306
// FUNCTION: STBC 0x00513d70 _$E308
// FUNCTION: STBC 0x00513d80 _$E307

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00513dd0 _$E474
// FUNCTION: STBC 0x00513de0 _$E470
// FUNCTION: STBC 0x00513df0 _$E473
// FUNCTION: STBC 0x00513e00 _$E471
// DATA: STBC 0x00987c80
TGEventHandlerTable ReticleManagerWindow::classHandlers;

// DATA: STBC 0x00987cac
ReticleManagerWindow *ReticleManagerWindow::reticleManagerWindow;

/*
================
ReticleManagerWindow::ReticleManagerWindow

The one reticle manager: not zoomed, deleting the reticles of the ships
that are destroyed, and never given the focus.
================
*/
// FUNCTION: STBC 0x00513e20
ReticleManagerWindow::ReticleManagerWindow( void ) : TGPane( 1.0f, 1.0f ) {
	reticleManagerWindow = this;
	zoomed = false;
	frustumLeft = 0.0f;
	frustumRight = 0.0f;
	frustumTop = 0.0f;
	frustumBottom = 0.0f;
	viewportWidth = 1.0f;
	viewportHeight = 1.0f;
	savedCamera = NULL;
	g_kEventManager.AddBroadcastMethodHandler( ET_OBJECT_DESTROYED, this, "ReticleManagerWindow::ShipDestroyedHandler" );
	SetNoFocus();
	Unknown513F30();
}

/*
================
ReticleManagerWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00513f00
void ReticleManagerWindow::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&ReticleManagerWindow::ShipDestroyedHandler, "ReticleManagerWindow::ShipDestroyedHandler" );
}

/*
================
ReticleManagerWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00513f20
void ReticleManagerWindow::RegisterHandlers( void ) {
}

/*
================
ReticleManagerWindow::Unknown513F30
================
*/
// FUNCTION: STBC 0x00513f30
void ReticleManagerWindow::Unknown513F30( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) ) {
			((ReticleWindow *)child)->Unknown5152A0();
		}
	}
}

/*
================
ReticleManagerWindow::VirtualSlot82
================
*/
// FUNCTION: STBC 0x00513f70
void ReticleManagerWindow::VirtualSlot82( float time ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) ) {
			((ReticleWindow *)child)->VirtualSlot82( time );
		}
	}
}

/*
================
ReticleManagerWindow::RemoveReticles
================
*/
// FUNCTION: STBC 0x00513fb0
void ReticleManagerWindow::RemoveReticles( long id ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) && !child->IsTypeOf( PLAYERRETICLEWINDOW_TYPE ) && ((ReticleWindow *)child)->GetTargetID() == id ) {
			DeleteChild( child );
		}
	}
}

/*
================
ReticleManagerWindow::RemoveAllReticles
================
*/
// FUNCTION: STBC 0x00514010
void ReticleManagerWindow::RemoveAllReticles( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) && !child->IsTypeOf( PLAYERRETICLEWINDOW_TYPE ) ) {
			DeleteChild( child );
		}
	}
}

/*
================
ReticleManagerWindow::ShipDestroyedHandler
================
*/
// FUNCTION: STBC 0x00514060
void ReticleManagerWindow::ShipDestroyedHandler( TGEvent *event ) {
	// BUG: the destination is tested for NULL only after its IsTypeOf call
	if ( !event->GetDestination()->IsTypeOf( SHIPCLASS_TYPE ) || event->GetDestination() == NULL ) {
		return;
	}
	long id = event->GetDestination()->GetObjID();
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) && !child->IsTypeOf( PLAYERRETICLEWINDOW_TYPE ) && ((ReticleWindow *)child)->GetTargetID() == id ) {
			DeleteChild( child );
		}
	}
}

/*
================
ReticleManagerWindow::SetZoom

Lays the reticles out for the view screen's camera: the icon manager draws
through that camera with the screen size scaled to the camera's port, and
RestoreZoom gives the saved camera and size back. The view screen's camera
object's own NiCamera is passed but not used.
================
*/
// FUNCTION: STBC 0x005140e0
void ReticleManagerWindow::SetZoom( NiCamera *sourceCamera, NiCamera *camera, float left, float right, float top, float bottom, const NiRect<float> &port ) {
	VirtualSlot60();
	savedCamera = g_kIconManager.GetCamera();
	g_kIconManager.SetCamera( camera );
	viewportWidth = port.right - port.left;
	viewportHeight = port.top - port.bottom;
	savedScreenWidth = g_kIconManager.GetScreenWidth();
	savedScreenHeight = g_kIconManager.GetScreenHeight();
	g_kIconManager.SetScreenSize( savedScreenWidth * viewportWidth, savedScreenHeight * viewportHeight );
	frustumLeft = left;
	frustumRight = right;
	frustumTop = top;
	frustumBottom = bottom;
	viewport = port;
	zoomed = true;
	VirtualSlot82( g_kUtopiaModule.GetGameTime() );
	Layout();
}

/*
================
ReticleManagerWindow::RestoreZoom
================
*/
// FUNCTION: STBC 0x00514260
void ReticleManagerWindow::RestoreZoom( void ) {
	VirtualSlot60();
	g_kIconManager.SetCamera( savedCamera );
	savedCamera = NULL;
	g_kIconManager.SetScreenWidth( savedScreenWidth );
	g_kIconManager.SetScreenHeight( savedScreenHeight );
	frustumLeft = 0.0f;
	frustumRight = 0.0f;
	frustumTop = 0.0f;
	frustumBottom = 0.0f;
	viewportWidth = 1.0f;
	viewportHeight = 1.0f;
	zoomed = false;
	Unknown514310();
	VirtualSlot82( 0.0f );
}

/*
================
ReticleManagerWindow::Unknown514310
================
*/
// FUNCTION: STBC 0x00514310
void ReticleManagerWindow::Unknown514310( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) ) {
			((ReticleWindow *)child)->VirtualSlot84();
		}
	}
}

/*
================
ReticleManagerWindow::Unknown514350
================
*/
// FUNCTION: STBC 0x00514350
void ReticleManagerWindow::Unknown514350( void ) {
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) ) {
			((ReticleWindow *)child)->Unknown515F70();
		}
	}
}

/*
================
ReticleManagerWindow::FindReticle
================
*/
// FUNCTION: STBC 0x00514390
ReticleWindow *ReticleManagerWindow::FindReticle( ObjectClass *object ) {
	if ( object == NULL ) {
		return NULL;
	}
	NiTListIterator pos = children.GetHeadPos();
	while ( pos != NULL ) {
		TGUIObject *child = children.GetNext( pos );
		if ( child->IsTypeOf( RETICLEWINDOW_TYPE ) && !child->IsTypeOf( PLAYERRETICLEWINDOW_TYPE ) && ((ReticleWindow *)child)->GetTargetID() == object->GetObjID() ) {
			return (ReticleWindow *)child;
		}
	}
	return NULL;
}

/*
================
ReticleManagerWindow::UpdateVisibility

Hidden in a cutscene, while the options window is shown, and while the
cinematic window has the focus; shown otherwise.
================
*/
// FUNCTION: STBC 0x005143f0
void ReticleManagerWindow::UpdateVisibility( void ) {
	TopWindow *topWindow = TopWindow::GetTopWindow();
	if ( topWindow != NULL ) {
		if ( topWindow->IsCutsceneMode() ) {
			SetNotVisible( true );
			return;
		}
		MainWindow *options = topWindow->FindMainWindow( MWT_OPTIONS );
		if ( options != NULL && options->IsVisible() ) {
			SetNotVisible( true );
			return;
		}
		MainWindow *cinematic = topWindow->FindMainWindow( MWT_CINEMATIC );
		if ( cinematic != NULL && cinematic == (MainWindow *)topWindow->GetFocus() ) {
			SetNotVisible( true );
			return;
		}
		SetVisible( true );
	}
}

/*
================
ReticleManagerWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x00514480
bool ReticleManagerWindow::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteBool( zoomed );
	return true;
}

/*
================
ReticleManagerWindow::ReadFromStream

The loaded manager is the one.
================
*/
// FUNCTION: STBC 0x005144b0
bool ReticleManagerWindow::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	zoomed = stream->ReadBool();
	reticleManagerWindow = this;
	return true;
}
