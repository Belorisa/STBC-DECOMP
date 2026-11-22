/*
===============================================================================

	ReticleWindow.cpp

	The target reticle: four corner icons around the object it follows, and
	an arrow that points to the object while it is off the screen.

===============================================================================
*/

#include "ReticleWindow.h"
#include "MainWindow.h"
#include "NiCamera.h"
#include "ObjectClass.h"
#include "PyEmbed.h"
#include "ReticleManagerWindow.h"
#include "Game.h"
#include "ShipClass.h"
#include "ShipSubsystem.h"
#include "TGModelUtils.h"
#include "SetManager.h"
#include "TGEventHandlerTable.h"
#include "TGIcon.h"
#include "TGIconManager.h"
#include "TGStream.h"
#include "TopWindow.h"
#include <iostream>
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00516890 _$E507
// FUNCTION: STBC 0x005168b0 _$E506

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x00514e70 _$E33
// FUNCTION: STBC 0x00514e80 _$E32
// FUNCTION: STBC 0x00514e90 _$E37
// FUNCTION: STBC 0x00514ea0 _$E36
// FUNCTION: STBC 0x00515b80 _$E48
// FUNCTION: STBC 0x00515b90 _$E47
// FUNCTION: STBC 0x00515ba0 _$E52
// FUNCTION: STBC 0x00515bb0 _$E51
// FUNCTION: STBC 0x00515d30 _$E56
// FUNCTION: STBC 0x00515d40 _$E55
// FUNCTION: STBC 0x00515f50 _$E60
// FUNCTION: STBC 0x00515f60 _$E59
// FUNCTION: STBC 0x00516090 _$E184
// FUNCTION: STBC 0x005160a0 _$E183
// FUNCTION: STBC 0x005160b0 _$E126
// FUNCTION: STBC 0x005160c0 _$E125
// FUNCTION: STBC 0x005160d0 _$E139
// FUNCTION: STBC 0x005160e0 _$E138
// FUNCTION: STBC 0x005160f0 _$E189
// FUNCTION: STBC 0x00516100 _$E188
// FUNCTION: STBC 0x00516110 _$E194
// FUNCTION: STBC 0x00516120 _$E193
// FUNCTION: STBC 0x00516130 _$E198
// FUNCTION: STBC 0x00516140 _$E197
// FUNCTION: STBC 0x00516150 _$E202
// FUNCTION: STBC 0x00516160 _$E201
// FUNCTION: STBC 0x00516170 _$E209
// FUNCTION: STBC 0x00516180 _$E208
// FUNCTION: STBC 0x00516190 _$E343
// FUNCTION: STBC 0x005161a0 _$E342
// FUNCTION: STBC 0x005161b0 _$E347
// FUNCTION: STBC 0x005161c0 _$E346
// FUNCTION: STBC 0x005161d0 _$E84
// FUNCTION: STBC 0x005161e0 _$E83
// FUNCTION: STBC 0x005161f0 _$E278
// FUNCTION: STBC 0x00516200 _$E277
// FUNCTION: STBC 0x00516210 _$E285
// FUNCTION: STBC 0x00516220 _$E284
// FUNCTION: STBC 0x00516230 _$E289
// FUNCTION: STBC 0x00516240 _$E288
// FUNCTION: STBC 0x00516250 _$E293
// FUNCTION: STBC 0x00516260 _$E292
// FUNCTION: STBC 0x00516270 _$E297
// FUNCTION: STBC 0x00516280 _$E296
// FUNCTION: STBC 0x00516290 _$E303
// FUNCTION: STBC 0x005162a0 _$E302
// FUNCTION: STBC 0x005162b0 _$E351
// FUNCTION: STBC 0x005162c0 _$E350
// FUNCTION: STBC 0x005162d0 _$E355
// FUNCTION: STBC 0x005162e0 _$E354
// FUNCTION: STBC 0x005162f0 _$E359
// FUNCTION: STBC 0x00516300 _$E358
// FUNCTION: STBC 0x00516310 _$E363
// FUNCTION: STBC 0x00516320 _$E362
// FUNCTION: STBC 0x00516330 _$E367
// FUNCTION: STBC 0x00516340 _$E366
// FUNCTION: STBC 0x00516350 _$E371
// FUNCTION: STBC 0x00516360 _$E370
// FUNCTION: STBC 0x00516370 _$E375
// FUNCTION: STBC 0x00516380 _$E374
// FUNCTION: STBC 0x00516390 _$E379
// FUNCTION: STBC 0x005163a0 _$E378
// FUNCTION: STBC 0x005163b0 _$E383
// FUNCTION: STBC 0x005163c0 _$E382
// FUNCTION: STBC 0x005163d0 _$E387
// FUNCTION: STBC 0x005163e0 _$E386
// FUNCTION: STBC 0x005163f0 _$E89
// FUNCTION: STBC 0x00516400 _$E88
// FUNCTION: STBC 0x00516410 _$E391
// FUNCTION: STBC 0x00516420 _$E390
// FUNCTION: STBC 0x00516430 _$E94
// FUNCTION: STBC 0x00516440 _$E93
// FUNCTION: STBC 0x00516450 _$E395
// FUNCTION: STBC 0x00516460 _$E394
// FUNCTION: STBC 0x00516470 _$E399
// FUNCTION: STBC 0x00516480 _$E398
// FUNCTION: STBC 0x00516490 _$E403
// FUNCTION: STBC 0x005164a0 _$E402
// FUNCTION: STBC 0x005164b0 _$E407
// FUNCTION: STBC 0x005164c0 _$E406
// FUNCTION: STBC 0x005164d0 _$E103
// FUNCTION: STBC 0x005164e0 _$E102
// FUNCTION: STBC 0x005164f0 _$E411
// FUNCTION: STBC 0x00516500 _$E410
// FUNCTION: STBC 0x00516510 _$E415
// FUNCTION: STBC 0x00516520 _$E414
// FUNCTION: STBC 0x00516530 _$E419
// FUNCTION: STBC 0x00516540 _$E418
// FUNCTION: STBC 0x00516550 _$E109
// FUNCTION: STBC 0x00516560 _$E108
// FUNCTION: STBC 0x00516570 _$E114
// FUNCTION: STBC 0x00516580 _$E113
// FUNCTION: STBC 0x00516590 _$E144
// FUNCTION: STBC 0x005165a0 _$E143
// FUNCTION: STBC 0x005165b0 _$E423
// FUNCTION: STBC 0x005165c0 _$E422
// FUNCTION: STBC 0x005165d0 _$E149
// FUNCTION: STBC 0x005165e0 _$E148
// FUNCTION: STBC 0x005165f0 _$E427
// FUNCTION: STBC 0x00516600 _$E426
// FUNCTION: STBC 0x00516610 _$E431
// FUNCTION: STBC 0x00516620 _$E430
// FUNCTION: STBC 0x00516630 _$E154
// FUNCTION: STBC 0x00516640 _$E153
// FUNCTION: STBC 0x00516650 _$E435
// FUNCTION: STBC 0x00516660 _$E434
// FUNCTION: STBC 0x00516670 _$E119
// FUNCTION: STBC 0x00516680 _$E118
// FUNCTION: STBC 0x00516690 _$E439
// FUNCTION: STBC 0x005166a0 _$E438
// FUNCTION: STBC 0x005166b0 _$E443
// FUNCTION: STBC 0x005166c0 _$E442
// FUNCTION: STBC 0x005166d0 _$E159
// FUNCTION: STBC 0x005166e0 _$E158
// FUNCTION: STBC 0x005166f0 _$E164
// FUNCTION: STBC 0x00516700 _$E163
// FUNCTION: STBC 0x00516710 _$E447
// FUNCTION: STBC 0x00516720 _$E446
// FUNCTION: STBC 0x00516730 _$E451
// FUNCTION: STBC 0x00516740 _$E450
// FUNCTION: STBC 0x00516750 _$E455
// FUNCTION: STBC 0x00516760 _$E454
// FUNCTION: STBC 0x00516770 _$E459
// FUNCTION: STBC 0x00516780 _$E458
// FUNCTION: STBC 0x00516790 _$E463
// FUNCTION: STBC 0x005167a0 _$E462
// FUNCTION: STBC 0x005167b0 _$E467
// FUNCTION: STBC 0x005167c0 _$E466
// FUNCTION: STBC 0x005167d0 _$E471
// FUNCTION: STBC 0x005167e0 _$E470
// FUNCTION: STBC 0x005167f0 _$E475
// FUNCTION: STBC 0x00516800 _$E474
// FUNCTION: STBC 0x00516810 _$E169
// FUNCTION: STBC 0x00516820 _$E168
// FUNCTION: STBC 0x00516830 _$E479
// FUNCTION: STBC 0x00516840 _$E478
// FUNCTION: STBC 0x00516850 _$E174
// FUNCTION: STBC 0x00516860 _$E173
// FUNCTION: STBC 0x00516870 _$E179
// FUNCTION: STBC 0x00516880 _$E178
// FUNCTION: STBC 0x005168c0 _$E483
// FUNCTION: STBC 0x005168d0 _$E482
// FUNCTION: STBC 0x005168e0 _$E488
// FUNCTION: STBC 0x005168f0 _$E487
// FUNCTION: STBC 0x00516900 _$E492
// FUNCTION: STBC 0x00516910 _$E491

// The reticle's icons keep the clamp mode 3 (invented name).
#define RETICLE_ICON_CLAMP			3

// Where the tactical view's empty area begins when the script can't say
// (invented names).
#define DEFAULT_EMPTY_AREA_LEFT		0.25625f
#define DEFAULT_EMPTY_AREA_BOTTOM	0.72f

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00514df0 _$E327
// FUNCTION: STBC 0x00514e00 _$E324
// FUNCTION: STBC 0x00514e10 _$E326
// FUNCTION: STBC 0x00514e20 _$E325
// FUNCTION: STBC 0x00514e30 _$E333
// FUNCTION: STBC 0x00514e40 _$E330
// FUNCTION: STBC 0x00514e50 _$E332
// FUNCTION: STBC 0x00514e60 _$E331

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x00514eb0 _$E498
// FUNCTION: STBC 0x00514ec0 _$E494
// FUNCTION: STBC 0x00514ed0 _$E497
// FUNCTION: STBC 0x00514ee0 _$E495
// DATA: STBC 0x00987db8
TGEventHandlerTable ReticleWindow::classHandlers;

/*
================
ReticleWindow::ReticleWindow

The four corner icons of the "ReticleTextures" group, and the target arrow
in a pane of its own; hidden until it has an object.
================
*/
// FUNCTION: STBC 0x00514f00
ReticleWindow::ReticleWindow( void ) : TGPane( 1.0f, 1.0f ) {
	// MATCH-PENDING: R17, the window's width and height are named in locals, and read again before the last corner, only so their loads precede each corner's reload (the scheduler's tie on input order, tools/c2order.py); no evidence shows the locals
	targetID = 0;
	g_kIconManager.GetIconGroup( "ReticleTextures" );

	corners[0] = new TGIcon( "ReticleTextures", 0 );
	corners[0]->SetClamp( RETICLE_ICON_CLAMP );
	AddChild( corners[0], 0.0f, 0.0f, false );

	corners[1] = new TGIcon( "ReticleTextures", 1 );
	corners[1]->SetClamp( RETICLE_ICON_CLAMP );
	float width = GetWidth();
	AddChild( corners[1], width - corners[1]->GetWidth(), 0.0f, false );

	corners[2] = new TGIcon( "ReticleTextures", 2 );
	corners[2]->SetClamp( RETICLE_ICON_CLAMP );
	float height = GetHeight();
	AddChild( corners[2], 0.0f, height - corners[2]->GetHeight(), false );

	corners[3] = new TGIcon( "ReticleTextures", 3 );
	corners[3]->SetClamp( RETICLE_ICON_CLAMP );
	width = GetWidth();
	height = GetHeight();
	AddChild( corners[3], width - corners[3]->GetWidth(), height - corners[3]->GetHeight(), false );

	targetArrow = new TGIcon( "TargetArrow", 0 );
	TGPane *arrowPane = new TGPane( 1.0f, 1.0f );
	arrowPane->AddChild( targetArrow );
	AddChild( arrowPane );
	Unknown515F70();

	targetArrow->SetNotVisible();
	SetNotVisible();
	SetNoFocus();
	Unknown5152A0();
}

/*
================
ReticleWindow::GetObjType
================
*/
// FUNCTION: STBC 0x005151a0
int ReticleWindow::GetObjType( void ) {
	return RETICLEWINDOW_TYPE;
}

/*
================
ReticleWindow::GetClassName
================
*/
// FUNCTION: STBC 0x00515210
char *ReticleWindow::GetClassName( void ) {
	return "ReticleWindow";
}

/*
================
ReticleWindow::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00515220
char *ReticleWindow::GetSwigTypeName( void ) {
	return "_p_ReticleWindow";
}

/*
================
ReticleWindow::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00515230
char *ReticleWindow::GetPythonClassName( void ) {
	return "ReticleWindowPtr";
}

/*
================
ReticleWindow::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00515240
TGEventHandlerTable *ReticleWindow::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
ReticleWindow::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x00515280
void ReticleWindow::RegisterHandlerNames( void ) {
}

/*
================
ReticleWindow::RegisterHandlers
================
*/
// FUNCTION: STBC 0x00515290
void ReticleWindow::RegisterHandlers( void ) {
}

/*
================
ReticleWindow::Unknown5152A0
================
*/
// FUNCTION: STBC 0x005152a0
void ReticleWindow::Unknown5152A0( void ) {
}

/*
================
ReticleWindow::VirtualSlot82

Hides the reticle when its object is gone, or is outside the rendered set
while the map is not zoomed; places it otherwise.
================
*/
// FUNCTION: STBC 0x005152b0
void ReticleWindow::VirtualSlot82( float time ) {
	if ( targetID == 0 ) {
		return;
	}
	ObjectClass *object = ObjectClass::GetObjectByID( NULL, targetID );
	if ( object == NULL ) {
		SetNotVisible();
		return;
	}
	if ( !ReticleManagerWindow::GetReticleManagerWindow()->IsZoomed() && object->GetContainingSet() != g_kSetManager.GetRenderedSet() ) {
		SetNotVisible();
		return;
	}
	Unknown515310( object );
}

/*
================
ReticleWindow::Unknown515310

Places the reticle on the object: the four corners around it, sized by the
object's bound and the zoom, while it is on the screen (or the map is
zoomed); the target arrow at the screen's edge, pointing at it, otherwise.
The player's targeted subsystem, when there is one, is the point aimed at.
================
*/
// FUNCTION: STBC 0x00515310
// DRAFT: 65.6; frame 0x74 against retail's 0x60: retail shares slots (the world scale in the object argument's, the aspect in the frustum height's) and the x87 order of the corner and size sums differs
void ReticleWindow::Unknown515310( ObjectClass *object ) {
	SetVisible();
	NiCamera *camera = g_kIconManager.GetCamera();

	NiPoint3 location = object->GetWorldLocation();
	if ( Game::GetCurrentGame() != NULL && Game::GetCurrentGame()->GetPlayer() != NULL ) {
		ShipSubsystem *subsystem = Game::GetCurrentGame()->GetPlayer()->GetTargetSubsystem();
		if ( subsystem != NULL ) {
			location = TGModelUtils::LocalToWorldPoint( object->GetNode(), subsystem->GetDamagePoint() );
		}
	}

	float x, y;
	Unknown515BC0( camera, location, x, y );

	ReticleManagerWindow *manager = ReticleManagerWindow::GetReticleManagerWindow();
	NiRect<float> port;
	float frustumLeft, frustumBottom, frustumWidth, frustumHeight;
	if ( manager->IsZoomed() ) {
		frustumLeft = manager->GetFrustumLeft();
		frustumBottom = manager->GetFrustumBottom();
		port = manager->GetViewport();
		frustumWidth = manager->GetFrustumRight() + ( port.right - port.left ) + frustumLeft;
		frustumHeight = manager->GetFrustumTop() + ( port.top - port.bottom ) + frustumBottom;
		x = ( ( ( x - 0.5f ) * ( frustumHeight / frustumWidth ) + 0.5f ) * frustumWidth - frustumLeft ) / ( port.right - port.left );
		y = ( frustumHeight * y - frustumBottom ) / ( port.top - port.bottom );
	} else {
		frustumWidth = 1.0f;
		frustumHeight = 1.0f;
	}

	if ( !manager->IsZoomed() && ( x < 0.0f || x > 1.0f || y < 0.0f || y > 1.0f ) ) {
		// Off the screen: the arrow at the edge, pointing at the object.
		if ( !targetArrow->IsVisible() ) {
			targetArrow->SetVisible();
			corners[0]->SetNotVisible();
			corners[1]->SetNotVisible();
			corners[2]->SetNotVisible();
			corners[3]->SetNotVisible();
		}
		float arrowX = x < 0.0f ? 0.0f : ( x > 1.0f ? 1.0f : x );
		float arrowY = y < 0.0f ? 1.0f : ( y > 1.0f ? 0.0f : 1.0f - y );

		TGPane *pane = targetArrow->GetParent();
		float paneWidth = pane->GetWidth();
		float paneHeight = pane->GetHeight();
		float paneLeft = pane->GetLeft();
		float paneTop = pane->GetTop();
		if ( arrowX < paneLeft ) {
			arrowX = paneLeft;
		}
		if ( arrowY < paneTop ) {
			arrowY = paneTop;
		}
		if ( arrowX > paneLeft + paneWidth ) {
			arrowX = paneLeft + paneWidth;
		}
		if ( arrowY > paneTop + paneHeight ) {
			arrowY = paneTop + paneHeight;
		}
		arrowX -= paneLeft;
		arrowY -= paneTop;

		// The arrow's icon: 0 to 2 along the top, 3 and 4 down the sides, 5 to 7 along the bottom.
		int icon;
		if ( arrowY <= 0.0f ) {
			if ( arrowX / paneWidth < 0.25f ) {
				icon = 0;
			} else if ( arrowX / paneWidth <= 0.75f ) {
				icon = 1;
			} else {
				icon = 2;
			}
		} else if ( arrowY >= paneHeight ) {
			if ( arrowX / paneWidth < 0.25f ) {
				icon = 5;
			} else if ( arrowX / paneWidth <= 0.75f ) {
				icon = 6;
			} else {
				icon = 7;
			}
		} else if ( arrowX <= 0.0f ) {
			if ( arrowY / paneHeight < 0.25 ) {
				icon = 0;
			} else if ( arrowY / paneHeight <= 0.75 ) {
				icon = 3;
			} else {
				icon = 5;
			}
		} else if ( arrowX >= paneWidth && arrowY / paneHeight >= 0.25 ) {
			if ( arrowY / paneHeight <= 0.75 ) {
				icon = 4;
			} else {
				icon = 7;
			}
		} else {
			icon = 2;
		}
		if ( targetArrow->GetIconNum() != icon ) {
			targetArrow->SetIconNum( icon, false );
			targetArrow->SizeToArtwork( true );
		}

		if ( targetArrow->GetWidth() + arrowX > paneWidth ) {
			arrowX = paneWidth - targetArrow->GetWidth();
		}
		if ( targetArrow->GetHeight() + arrowY > paneHeight ) {
			arrowY = paneHeight - targetArrow->GetHeight();
		}
		unknown6C.left = paneLeft + arrowX;
		unknown6C.top = paneTop + arrowY;
		unknown6C.width = targetArrow->GetWidth();
		unknown6C.height = targetArrow->GetHeight();
		targetArrow->SetPosition( arrowX, arrowY );
		return;
	}

	// On the screen: the four corners around the object's bound.
	if ( targetArrow->IsVisible() ) {
		targetArrow->SetNotVisible();
		corners[0]->SetVisible();
		corners[1]->SetVisible();
		corners[2]->SetVisible();
		corners[3]->SetVisible();
	}
	float radius = object->GetWorldBound().GetRadius();
	NiPoint3 right = camera->GetWorldRightVector() * radius;
	NiPoint3 up = camera->GetWorldUpVector() * radius;
	NiPoint3 corner = location + right + up;
	float cornerX, cornerY;
	Unknown515BC0( camera, corner, cornerX, cornerY );
	if ( manager->IsZoomed() ) {
		cornerX = ( cornerX * frustumWidth - frustumLeft ) / ( port.right - port.left );
		cornerY = ( cornerY * frustumHeight - frustumBottom ) / ( port.top - port.bottom );
	}
	float width = cornerX - x;
	float height = cornerY - y;
	float aspect = g_kIconManager.GetScreenHeight() / g_kIconManager.GetScreenWidth();
	if ( manager->IsZoomed() ) {
		aspect = ( port.right - port.left ) * aspect / ( port.top - port.bottom );
	}
	corners[0]->SizeToArtwork( true );
	corners[1]->SizeToArtwork( true );
	corners[2]->SizeToArtwork( true );
	corners[3]->SizeToArtwork( true );

	// No wider than a quarter of the view, no narrower than two corners.
	if ( manager->IsZoomed() ) {
		float limit = frustumWidth / ( port.right - port.left ) * 0.25f;
		height /= aspect;
		if ( width > limit ) {
			height = limit / width * height;
			width = limit;
		} else if ( width < corners[0]->GetWidth() * 2.0f ) {
			width = corners[0]->GetWidth() * 2.0f;
			height = corners[0]->GetHeight() * 8.0f;
		}
	} else if ( width > 0.25f ) {
		width = 0.25f;
		height = 0.25f / aspect;
	} else if ( width < corners[0]->GetWidth() * 2.0f ) {
		width = corners[0]->GetWidth() * 2.0f;
		height = corners[0]->GetHeight() * 8.0f;
	}

	unknown6C.width = width;
	unknown6C.height = height;
	float left = x - width * 0.5f;
	float top = ( 1.0f - y ) - height * 0.5f;
	unknown6C.left = left;
	unknown6C.top = top;
	corners[0]->SetPosition( left, top );
	corners[1]->SetPosition( unknown6C.width + unknown6C.left - corners[1]->GetWidth(), unknown6C.top );
	corners[2]->SetPosition( unknown6C.left, unknown6C.height + unknown6C.top - corners[2]->GetHeight() );
	corners[3]->SetPosition( unknown6C.width + unknown6C.left - corners[3]->GetWidth(), unknown6C.height + unknown6C.top - corners[3]->GetHeight() );
}

/*
================
ReticleWindow::Unknown515BC0

Projects a world point through the camera into its view port, without the
frustum test of NiCamera::WorldPtToScreenPt; leaves x and y alone when the
point is behind the eye.
================
*/
// FUNCTION: STBC 0x00515bc0
void ReticleWindow::Unknown515BC0( NiCamera *camera, const NiPoint3 &point, float &x, float &y ) {
	float ptX = point.x;
	float ptY = point.y;
	float ptZ = point.z;
	const float *m = camera->GetWorldToCameraMatrix();
	double w = ptX * m[12] + ptY * m[13] + ptZ * m[14] + m[15];
	if ( w > 1e-5f ) {
		double invW = 1.0 / w;
		x = ptX * m[0] + ptY * m[1] + ptZ * m[2] + m[3];
		y = ptX * m[4] + ptY * m[5] + ptZ * m[6] + m[7];
		x *= invW;
		y *= invW;
		x = ( camera->GetViewPort().right - camera->GetViewPort().left ) * x * 0.5f;
		y = ( camera->GetViewPort().top - camera->GetViewPort().bottom ) * y * 0.5f;
		x += ( camera->GetViewPort().left + camera->GetViewPort().right ) * 0.5f;
		y += ( camera->GetViewPort().bottom + camera->GetViewPort().top ) * 0.5f;
	}
}

/*
================
ReticleWindow::SetColor
================
*/
// FUNCTION: STBC 0x00515cf0
void ReticleWindow::SetColor( NiColorA *color ) {
	corners[0]->SetColor( color );
	corners[1]->SetColor( color );
	corners[2]->SetColor( color );
	corners[3]->SetColor( color );
	targetArrow->SetColor( color );
}

/*
================
ReticleWindow::SetTargetID

Follows the object while it exists; hides the reticle otherwise.
================
*/
// FUNCTION: STBC 0x00515d50
void ReticleWindow::SetTargetID( long id ) {
	if ( ObjectClass::GetObjectByID( NULL, id ) != NULL ) {
		targetID = id;
	} else {
		targetID = 0;
		SetNotVisible();
	}
}

/*
================
ReticleWindow::VirtualSlot84
================
*/
// FUNCTION: STBC 0x00515d90
void ReticleWindow::VirtualSlot84( void ) {
}

/*
================
ReticleWindow::WriteToStream
================
*/
// FUNCTION: STBC 0x00515da0
bool ReticleWindow::WriteToStream( TGStream *stream ) {
	if ( !TGPane::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( targetID );
	for ( int i = 0; i < 4; i++ ) {
		stream->WriteID( GetSavedID( corners[i] ) );
	}
	stream->WriteFloat( unknown64 );
	stream->WriteFloat( unknown68 );
	stream->WriteID( GetSavedID( targetArrow ) );
	return true;
}

/*
================
ReticleWindow::ReadFromStream
================
*/
// FUNCTION: STBC 0x00515e30
bool ReticleWindow::ReadFromStream( TGStream *stream ) {
	if ( !TGPane::ReadFromStream( stream ) ) {
		return false;
	}
	targetID = stream->ReadID();
	for ( int i = 0; i < 4; i++ ) {
		corners[i] = (TGIcon *)stream->ReadID();
	}
	unknown64 = stream->ReadFloat();
	unknown68 = stream->ReadFloat();
	targetArrow = (TGIcon *)stream->ReadID();
	return true;
}

/*
================
ReticleWindow::FixUpPointers
================
*/
// FUNCTION: STBC 0x00515ea0
bool ReticleWindow::FixUpPointers( void ) {
	if ( !TGPane::FixUpPointers() ) {
		return false;
	}
	for ( int i = 0; i < 4; i++ ) {
		if ( corners[i] != NULL && GetTGObjectPtr( (long)corners[i] ) == NULL ) {
			return false;
		}
	}
	if ( targetArrow != NULL && GetTGObjectPtr( (long)targetArrow ) == NULL ) {
		return false;
	}
	return true;
}

/*
================
ReticleWindow::PostFixUp
================
*/
// FUNCTION: STBC 0x00515f00
void ReticleWindow::PostFixUp( void ) {
	TGPane::PostFixUp();
	for ( int i = 0; i < 4; i++ ) {
		corners[i] = (TGIcon *)GetSavedObject( (long)corners[i] );
	}
	targetArrow = (TGIcon *)GetSavedObject( (long)targetArrow );
}

/*
================
ReticleWindow::Unknown515F70

While the tactical window has the focus the target arrow's pane covers
the tactical view's empty area, which the Tactical script gives; it covers
the whole screen otherwise.
================
*/
// FUNCTION: STBC 0x00515f70
void ReticleWindow::Unknown515F70( void ) {
	if ( targetArrow == NULL ) {
		return;
	}
	TGPane *arrowPane = targetArrow->GetParent();
	MainWindow *tactical = TopWindow::GetTopWindow()->FindMainWindow( MWT_TACTICAL );
	if ( tactical != NULL && tactical->HasFocus() ) {
		float left = GetTacticalFloat( "GetEmptyAreaLeft", DEFAULT_EMPTY_AREA_LEFT );
		float bottom = GetTacticalFloat( "GetEmptyAreaBottom", DEFAULT_EMPTY_AREA_BOTTOM );
		arrowPane->Resize( 1.0f - left, bottom, false );
		arrowPane->SetPosition( left, 0.0f, false );
		arrowPane->Layout();
	} else {
		arrowPane->Resize( 1.0f, 1.0f, false );
		arrowPane->SetPosition( 0.0f, 0.0f, false );
		arrowPane->Layout();
	}
}
