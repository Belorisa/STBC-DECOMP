/*
===============================================================================

	CloakingSubsystem.cpp

===============================================================================
*/

#include <iostream>
#include "CloakingSubsystem.h"
#include "Game.h"
#include <stdlib.h>
#include "ShipClass.h"
#include "TGEvent.h"
#include "TGEventHandlerTable.h"
#include "TGEventManager.h"
#include "UtopiaModule.h"
#include "TGBoolEvent.h"
#include "TGSequence.h"
#include "TGTimer.h"
#include "TGTimerManager.h"
#include "TGSound.h"
#include "TGSoundManager.h"
#include "TGSoundRegion.h"
#include "SetClass.h"
#include "NiAlphaProperty.h"
#include "NiTextureModeProperty.h"
#include "NiVertexColorProperty.h"
#include "ShieldClass.h"
#include "TGStream.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00560370 _$E500
// FUNCTION: STBC 0x00560390 _$E499

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0055e160 _$E32
// FUNCTION: STBC 0x0055e170 _$E29
// FUNCTION: STBC 0x0055e180 _$E31
// FUNCTION: STBC 0x0055e190 _$E30
// FUNCTION: STBC 0x0055e1a0 _$E38
// FUNCTION: STBC 0x0055e1b0 _$E35
// FUNCTION: STBC 0x0055e1c0 _$E37
// FUNCTION: STBC 0x0055e1d0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0055e1e0 _$E78
// FUNCTION: STBC 0x0055e1f0 _$E77
// FUNCTION: STBC 0x0055e200 _$E297
// FUNCTION: STBC 0x0055e210 _$E296
// FUNCTION: STBC 0x0055e7e0 _$E89
// FUNCTION: STBC 0x0055e7f0 _$E88
// FUNCTION: STBC 0x0055edf0 _$E93
// FUNCTION: STBC 0x0055ee00 _$E92
// FUNCTION: STBC 0x0055f0f0 _$E97
// FUNCTION: STBC 0x0055f100 _$E96
// FUNCTION: STBC 0x0055f3c0 _$E101
// FUNCTION: STBC 0x0055f3d0 _$E100
// FUNCTION: STBC 0x0055f750 _$E216
// FUNCTION: STBC 0x0055f760 _$E215
// FUNCTION: STBC 0x0055fb70 _$E158
// FUNCTION: STBC 0x0055fb80 _$E157
// FUNCTION: STBC 0x0055fb90 _$E171
// FUNCTION: STBC 0x0055fba0 _$E170
// FUNCTION: STBC 0x0055fbb0 _$E221
// FUNCTION: STBC 0x0055fbc0 _$E220
// FUNCTION: STBC 0x0055fbd0 _$E302
// FUNCTION: STBC 0x0055fbe0 _$E301
// FUNCTION: STBC 0x0055fbf0 _$E243
// FUNCTION: STBC 0x0055fc00 _$E242
// FUNCTION: STBC 0x0055fc10 _$E306
// FUNCTION: STBC 0x0055fc20 _$E305
// FUNCTION: STBC 0x0055fc30 _$E313
// FUNCTION: STBC 0x0055fc40 _$E312
// FUNCTION: STBC 0x0055fc50 _$E347
// FUNCTION: STBC 0x0055fc60 _$E346
// FUNCTION: STBC 0x0055fc70 _$E351
// FUNCTION: STBC 0x0055fc80 _$E350
// FUNCTION: STBC 0x0055fc90 _$E120
// FUNCTION: STBC 0x0055fca0 _$E119
// FUNCTION: STBC 0x0055fcb0 _$E259
// FUNCTION: STBC 0x0055fcc0 _$E258
// FUNCTION: STBC 0x0055fcd0 _$E266
// FUNCTION: STBC 0x0055fce0 _$E265
// FUNCTION: STBC 0x0055fcf0 _$E270
// FUNCTION: STBC 0x0055fd00 _$E269
// FUNCTION: STBC 0x0055fd10 _$E274
// FUNCTION: STBC 0x0055fd20 _$E273
// FUNCTION: STBC 0x0055fd30 _$E278
// FUNCTION: STBC 0x0055fd40 _$E277
// FUNCTION: STBC 0x0055fd50 _$E284
// FUNCTION: STBC 0x0055fd60 _$E283
// FUNCTION: STBC 0x0055fd70 _$E225
// FUNCTION: STBC 0x0055fd80 _$E224
// FUNCTION: STBC 0x0055fd90 _$E229
// FUNCTION: STBC 0x0055fda0 _$E228
// FUNCTION: STBC 0x0055fdd0 _$E355
// FUNCTION: STBC 0x0055fde0 _$E354
// FUNCTION: STBC 0x0055fdf0 _$E359
// FUNCTION: STBC 0x0055fe00 _$E358
// FUNCTION: STBC 0x0055fe10 _$E363
// FUNCTION: STBC 0x0055fe20 _$E362
// FUNCTION: STBC 0x0055fe30 _$E367
// FUNCTION: STBC 0x0055fe40 _$E366
// FUNCTION: STBC 0x0055fe50 _$E371
// FUNCTION: STBC 0x0055fe60 _$E370
// FUNCTION: STBC 0x0055fe70 _$E375
// FUNCTION: STBC 0x0055fe80 _$E374
// FUNCTION: STBC 0x0055fe90 _$E379
// FUNCTION: STBC 0x0055fea0 _$E378
// FUNCTION: STBC 0x0055feb0 _$E383
// FUNCTION: STBC 0x0055fec0 _$E382
// FUNCTION: STBC 0x0055fed0 _$E125
// FUNCTION: STBC 0x0055fee0 _$E124
// FUNCTION: STBC 0x0055fef0 _$E387
// FUNCTION: STBC 0x0055ff00 _$E386
// FUNCTION: STBC 0x0055ff10 _$E130
// FUNCTION: STBC 0x0055ff20 _$E129
// FUNCTION: STBC 0x0055ff30 _$E391
// FUNCTION: STBC 0x0055ff40 _$E390
// FUNCTION: STBC 0x0055ff50 _$E395
// FUNCTION: STBC 0x0055ff60 _$E394
// FUNCTION: STBC 0x0055ff70 _$E399
// FUNCTION: STBC 0x0055ff80 _$E398
// FUNCTION: STBC 0x0055ff90 _$E403
// FUNCTION: STBC 0x0055ffa0 _$E402
// FUNCTION: STBC 0x0055ffb0 _$E135
// FUNCTION: STBC 0x0055ffc0 _$E134
// FUNCTION: STBC 0x0055ffd0 _$E407
// FUNCTION: STBC 0x0055ffe0 _$E406
// FUNCTION: STBC 0x0055fff0 _$E411
// FUNCTION: STBC 0x00560000 _$E410
// FUNCTION: STBC 0x00560010 _$E415
// FUNCTION: STBC 0x00560020 _$E414
// FUNCTION: STBC 0x00560030 _$E141
// FUNCTION: STBC 0x00560040 _$E140
// FUNCTION: STBC 0x00560050 _$E146
// FUNCTION: STBC 0x00560060 _$E145
// FUNCTION: STBC 0x00560070 _$E176
// FUNCTION: STBC 0x00560080 _$E175
// FUNCTION: STBC 0x00560090 _$E419
// FUNCTION: STBC 0x005600a0 _$E418
// FUNCTION: STBC 0x005600b0 _$E181
// FUNCTION: STBC 0x005600c0 _$E180
// FUNCTION: STBC 0x005600d0 _$E423
// FUNCTION: STBC 0x005600e0 _$E422
// FUNCTION: STBC 0x005600f0 _$E427
// FUNCTION: STBC 0x00560100 _$E426
// FUNCTION: STBC 0x00560110 _$E186
// FUNCTION: STBC 0x00560120 _$E185
// FUNCTION: STBC 0x00560130 _$E431
// FUNCTION: STBC 0x00560140 _$E430
// FUNCTION: STBC 0x00560150 _$E151
// FUNCTION: STBC 0x00560160 _$E150
// FUNCTION: STBC 0x00560170 _$E435
// FUNCTION: STBC 0x00560180 _$E434
// FUNCTION: STBC 0x00560190 _$E439
// FUNCTION: STBC 0x005601a0 _$E438
// FUNCTION: STBC 0x005601b0 _$E191
// FUNCTION: STBC 0x005601c0 _$E190
// FUNCTION: STBC 0x005601d0 _$E196
// FUNCTION: STBC 0x005601e0 _$E195
// FUNCTION: STBC 0x005601f0 _$E443
// FUNCTION: STBC 0x00560200 _$E442
// FUNCTION: STBC 0x00560210 _$E447
// FUNCTION: STBC 0x00560220 _$E446
// FUNCTION: STBC 0x00560230 _$E451
// FUNCTION: STBC 0x00560240 _$E450
// FUNCTION: STBC 0x00560250 _$E455
// FUNCTION: STBC 0x00560260 _$E454
// FUNCTION: STBC 0x00560270 _$E459
// FUNCTION: STBC 0x00560280 _$E458
// FUNCTION: STBC 0x00560290 _$E463
// FUNCTION: STBC 0x005602a0 _$E462
// FUNCTION: STBC 0x005602b0 _$E467
// FUNCTION: STBC 0x005602c0 _$E466
// FUNCTION: STBC 0x005602d0 _$E471
// FUNCTION: STBC 0x005602e0 _$E470
// FUNCTION: STBC 0x005602f0 _$E201
// FUNCTION: STBC 0x00560300 _$E200
// FUNCTION: STBC 0x00560310 _$E475
// FUNCTION: STBC 0x00560320 _$E474
// FUNCTION: STBC 0x00560330 _$E206
// FUNCTION: STBC 0x00560340 _$E205
// FUNCTION: STBC 0x00560350 _$E211
// FUNCTION: STBC 0x00560360 _$E210
// FUNCTION: STBC 0x005603a0 _$E479
// FUNCTION: STBC 0x005603b0 _$E478
// FUNCTION: STBC 0x005603c0 _$E484
// FUNCTION: STBC 0x005603d0 _$E483
// FUNCTION: STBC 0x005603e0 _$E327
// FUNCTION: STBC 0x005603f0 _$E326

// The standard streams' initialisers and exit-time destructors (<iostream>).

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0055e230 _$E490
// FUNCTION: STBC 0x0055e240 _$E486
// FUNCTION: STBC 0x0055e250 _$E489
// FUNCTION: STBC 0x0055e260 _$E487

// DATA: STBC 0x008e4e1c
float CloakingSubsystem::cloakTime = 5.0f;

// DATA: STBC 0x008e4e20
float CloakingSubsystem::shieldDelay = 1.0f;

// DATA: STBC 0x0098b850
TGEventHandlerTable CloakingSubsystem::classHandlers;

/*
================
CloakingSubsystem::GetProperty
================
*/
// FUNCTION: STBC 0x0055e220
CloakingSubsystemProperty *CloakingSubsystem::GetProperty( void ) {
	return (CloakingSubsystemProperty *)property;
}

/*
================
CloakingSubsystem::Cast

The object as a CloakingSubsystem; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0055e280
CloakingSubsystem *CloakingSubsystem::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( CLOAKINGSUBSYSTEM_TYPE ) ) {
		return (CloakingSubsystem *)object;
	}
	return NULL;
}

/*
================
CloakingSubsystem::CloakingSubsystem

A device that starts decloaked. One created locally, rather than for an
object a packet describes, starts off.
================
*/
// FUNCTION: STBC 0x0055e2b0
CloakingSubsystem::CloakingSubsystem( long id ) : PoweredSubsystem( id ) {
	cloaked = false;
	tryingToCloak = false;
	cloakState = 0;
	unknownB4 = 0.0f;
	if ( g_kUtopiaModule.IsMultiplayer() ) {
		if ( !g_kUtopiaModule.IsProcessingPackets() && id == 0 ) {
			TurnOff();
		}
	} else if ( id == 0 ) {
		TurnOff();
	}
	powerMode = POWER_MODE_STEAL_FROM_RESERVE;
}

/*
================
CloakingSubsystem::GetObjType
================
*/
// FUNCTION: STBC 0x0055e350
int CloakingSubsystem::GetObjType( void ) {
	return CLOAKINGSUBSYSTEM_TYPE;
}

/*
================
CloakingSubsystem::GetClassName
================
*/
// FUNCTION: STBC 0x0055e3c0
char *CloakingSubsystem::GetClassName( void ) {
	return "CloakingSubsystem";
}

/*
================
CloakingSubsystem::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x0055e3d0
char *CloakingSubsystem::GetSwigTypeName( void ) {
	return "_p_CloakingSubsystem";
}

/*
================
CloakingSubsystem::GetPythonClassName
================
*/
// FUNCTION: STBC 0x0055e3e0
char *CloakingSubsystem::GetPythonClassName( void ) {
	return "CloakingSubsystemPtr";
}

/*
================
CloakingSubsystem::GetClassHandlers
================
*/
// FUNCTION: STBC 0x0055e3f0
TGEventHandlerTable *CloakingSubsystem::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
CloakingSubsystem::~CloakingSubsystem
================
*/
// FUNCTION: STBC 0x0055e430
// FUNCTION: STBC 0x0055e400 ??_GCloakingSubsystem@@UAEPAXI@Z
CloakingSubsystem::~CloakingSubsystem( void ) {
}

/*
================
CloakingSubsystem::RegisterHandlerNames
================
*/
// FUNCTION: STBC 0x0055e4a0
void CloakingSubsystem::RegisterHandlerNames( void ) {
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CloakingSubsystem::StartCloakingHandler, "CloakingSubsystem::StartCloakingHandler" );
	g_kEventManager.RegisterHandlerMethod( (TGHandlerMethod)&CloakingSubsystem::StopCloakingHandler, "CloakingSubsystem::StopCloakingHandler" );
}

/*
================
CloakingSubsystem::RegisterHandlers
================
*/
// FUNCTION: STBC 0x0055e4d0
void CloakingSubsystem::RegisterHandlers( void ) {
	AddMethodHandlerForClass( &classHandlers, ET_START_CLOAKING, "CloakingSubsystem::StartCloakingHandler" );
	AddMethodHandlerForClass( &classHandlers, ET_STOP_CLOAKING, "CloakingSubsystem::StopCloakingHandler" );
}

/*
================
CloakingSubsystem::Update

A cloak or decloak in progress advances by the time, as a fraction of the
cloak time, and completes at either end. A device that is on then starts the
cloak it is trying to make, ends the one it no longer wants, or gives up a
cloak whose power has dropped.
================
*/
// FUNCTION: STBC 0x0055e500
void CloakingSubsystem::Update( float deltaTime ) {
	PoweredSubsystem::Update( deltaTime );
	float fraction;
	switch ( cloakState ) {
	case 2:
		unknownB4 += deltaTime;
		fraction = unknownB4 / cloakTime;
		if ( fraction >= 1.0f ) {
			fraction = 1.0f;
			Unknown55F6D0();
		}
		Unknown55E640( fraction );
		break;
	case 5:
		unknownB4 -= deltaTime;
		fraction = unknownB4 / cloakTime;
		if ( fraction <= 0.0f ) {
			fraction = 0.0f;
			Unknown55F7F0();
		}
		Unknown55E640( fraction );
		break;
	}
	if ( !IsOn() ) {
		// BUG: redundant: the subsystem was already updated above
		PoweredSubsystem::Update( deltaTime );
		return;
	}
	bool notCloaking = !( cloakState == 1 || cloakState == 2 || cloakState == 3 );
	bool notDecloaking = !( cloakState == 4 || cloakState == 5 || cloakState == 0 );
	if ( tryingToCloak == true && notCloaking ) {
		Unknown55F110( true );
		return;
	}
	if ( tryingToCloak || !notDecloaking ) {
		if ( cloakState == CLOAK_STATE_CLOAKED && powerPercentage < 0.8f ) {
			StopCloaking();
		} else {
			return;
		}
	}
	Unknown55F110( false );
}

/*
================
CloakingSubsystem::Unknown55E640

The cloak is part-way done: the player's ambient light follows the fraction,
and the ship's tree fades by what is left.
================
*/
// FUNCTION: STBC 0x0055e640
void CloakingSubsystem::Unknown55E640( float fraction ) {
	if ( fraction >= 0.0f && fraction <= 1.0f ) {
		ShipClass *ship = parentShip;
		if ( ship == Game::GetCurrentPlayer() ) {
			ambientLight->SetDimmer( fraction * ( 1.0f / 3.0f ) );
		}
		Unknown55EE10( 1.0f - fraction, ship->GetNiObject() );
	}
}

/*
================
CloakingSubsystem::Unknown55E6B0

Makes the ship's tree cloakable. The player's ship, whose model stays
visible, is also lit by an ambient light of its own that fades in with the
cloak.
================
*/
// FUNCTION: STBC 0x0055e6b0
void CloakingSubsystem::Unknown55E6B0( void ) {
	ShipClass *ship = parentShip;
	ship->Unknown593270();
	NiAVObject *object = ship->GetNiObject();
	Unknown55E840( object );
	object->UpdateProperties();
	if ( ship == Game::GetCurrentPlayer() ) {
		if ( ambientLight == NULL ) {
			ambientLight = new NiAmbientLight;
		}
		ambientLight->SetDimmer( 0.0f );
		ambientLight->AttachAffectedNode( NiDynamicCast( NiNode, object ) );
		object->UpdateEffects();
	}
}

/*
================
CloakingSubsystem::Unknown55E800

The decloak's end: the ship's tree gives up what the cloak did to it, and
the ambient light lets go of the nodes it lit.
================
*/
// FUNCTION: STBC 0x0055e800
void CloakingSubsystem::Unknown55E800( void ) {
	ShipClass *ship = parentShip;
	ship->Unknown593550();
	if ( ambientLight != NULL ) {
		ambientLight->DetachAllAffectedNodes();
	}
	ship->GetNiObject()->UpdateProperties();
	ship->MarkGeometryChanged( NULL );
}

/*
================
CloakingSubsystem::Unknown55E840

Makes the tree fade-able: every triangle shape gets white vertex colours
when it has none, alpha blending, a texture mode and a vertex colour
property that takes the colours into the lighting.
================
*/
// FUNCTION: STBC 0x0055e840
// DRAFT: inline budget: retail expands one more IsKindOf (33.1) and SetField and calls one more NiProperty constructor and NiPointer copy (0x42FD00) out of line; no single own-cost or site edit gives that set
void CloakingSubsystem::Unknown55E840( NiObject *object ) {
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiGeometryData *data = shape->GetShapeData();
		unsigned short count = data->GetVertexCount();
		if ( data->GetColors() == NULL ) {
			NiColorA *colors = new NiColorA[count];
			unsigned int textureSets = data->GetTextureSets();
			data->SetData( count, data->GetVertices(), data->GetNormals(), colors, data->GetTextures() );
			data->SetTextureSets( textureSets );
			for ( unsigned short i = 0; i < count; i++ ) {
				colors[i] = NiColorA::WHITE;
			}
			NiAlphaProperty *newAlpha = new NiAlphaProperty;
			newAlpha->SetAlphaBlending( true );
			shape->AttachProperty( newAlpha );
		}
		NiAlphaProperty *alpha = NiDynamicCast( NiAlphaProperty, shape->GetProperty( NiAlphaProperty::ALPHA ) );
		if ( alpha == NULL ) {
			alpha = new NiAlphaProperty;
			shape->AttachProperty( alpha );
		}
		alpha->SetAlphaBlending( true );

		NiTextureModeProperty *textureMode = NiDynamicCast( NiTextureModeProperty, shape->GetProperty( NiTextureModeProperty::TEXTURE_MODE ) );
		if ( textureMode == NULL ) {
			textureMode = new NiTextureModeProperty;
			shape->AttachProperty( textureMode );
		}
		textureMode->SetApplyMode( NiTextureModeProperty::APPLY_MODULATE_VERTEX );

		NiVertexColorProperty *vertexColor = NiDynamicCast( NiVertexColorProperty, shape->GetProperty( NiVertexColorProperty::VERTEX_COLOR ) );
		if ( vertexColor == NULL ) {
			vertexColor = new NiVertexColorProperty;
			shape->AttachProperty( vertexColor );
		}
		vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_AMB_DIFF );
		vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );
	}
	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			Unknown55E840( node->GetAt( i ) );
		}
	}
}

/*
================
CloakingSubsystem::Unknown55EE10

Sets the alpha of every vertex colour in the tree for the fade: flickering
while the cloak is under way. The player's ship is kept visible by a
floor.
================
*/
// FUNCTION: STBC 0x0055ee10
// DRAFT: inline budget: retail calls NiObject::IsKindOf (0x450530) out of line for the shape test and keeps the alpha on the x87 stack through each loop; this build expands the test inline
void CloakingSubsystem::Unknown55EE10( float fraction, NiObject *object ) {
	if ( object == NULL ) {
		return;
	}
	NiTriShape *shape = NiDynamicCast( NiTriShape, object );
	if ( shape != NULL ) {
		NiGeometryData *data = shape->GetShapeData();
		unsigned short count = data->GetVertexCount();
		NiColorA *colors = data->GetColors();
		if ( colors != NULL ) {
			float alpha = fraction;
			if ( parentShip == Game::GetCurrentPlayer() ) {
				for ( unsigned int i = 0; i < count; i++ ) {
					if ( cloakState == 1 || cloakState == 2 || cloaked == true ) {
						if ( fraction <= 0.4f ) {
							alpha = rand() * ( 0.4f / RAND_MAX );
						} else {
							alpha = ( fraction - 0.4f ) + rand() * ( 0.4f / RAND_MAX );
						}
						alpha = alpha * fraction;
					}
					if ( cloakState == 4 || cloakState == 5 ) {
						alpha = ( ( fraction * 1.4f - 0.4f ) + rand() * ( 0.4f / RAND_MAX ) ) * fraction;
					}
					float value = alpha + 0.3f;
					if ( value > 1.0f ) {
						value = 1.0f;
					} else if ( value < 0.0f ) {
						value = 0.0f;
					}
					colors[i].a = value;
				}
			} else {
				for ( unsigned int i = 0; i < count; i++ ) {
					if ( cloakState == 1 || cloakState == 2 || cloaked == true ) {
						if ( fraction <= 0.4f ) {
							alpha = rand() * ( 0.4f / RAND_MAX );
						} else {
							alpha = ( fraction - 0.4f ) + rand() * ( 0.4f / RAND_MAX );
						}
						alpha = alpha * fraction;
					}
					if ( cloakState == 4 || cloakState == 5 ) {
						float value = ( ( fraction * 1.4f - 0.4f ) + rand() * ( 0.4f / RAND_MAX ) ) * fraction;
						alpha = 1.0f;
						if ( value <= 1.0f ) {
							alpha = value;
							if ( value < 0.0f ) {
								alpha = 0.0f;
							}
						}
					}
					colors[i].a = alpha;
				}
			}
			shape->GetShapeData()->IncRevisionID();
		}
	}
	NiNode *node = NiDynamicCast( NiNode, object );
	if ( node != NULL ) {
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			Unknown55EE10( fraction, node->GetAt( i ) );
		}
	}
}

/*
================
CloakingSubsystem::SetParentShip
================
*/
// FUNCTION: STBC 0x0055f0d0
void CloakingSubsystem::SetParentShip( ShipClass *ship ) {
	PoweredSubsystem::SetParentShip( ship );
	powerWanted = 0.0f;
}

/*
================
CloakingSubsystem::Unknown55F110

Starts the cloak (true) or the decloak (false). A cloak that can start is a
sequence: the shields go off, and after the shield delay the ship is told
the cloak is beginning. False when the device is disabled. A decloak tells
the ship at once.
================
*/
// FUNCTION: STBC 0x0055f110
// DRAFT: register allocation: same instructions; retail keeps the ship in ebx, this in ebp and the shield sequence in edi, and spills the main sequence
bool CloakingSubsystem::Unknown55F110( bool cloak ) {
	ShipClass *ship = parentShip;
	if ( cloak == true ) {
		if ( IsDisabled() == true ) {
			return false;
		}
		TGSequence *shieldsOff = NULL;
		TGSequence *sequence = new TGSequence;
		if ( ship->GetShields() != NULL ) {
			shieldsOff = new TGSequence;
			TGBoolEvent *shieldEvent = new TGBoolEvent;
			shieldEvent->SetBool( false );
			shieldEvent->SetEventType( ET_SHIELDS_SET_STATE );
			shieldEvent->SetSource( this );
			shieldEvent->SetDestination( parentShip->GetShields() );
			shieldsOff->AddCompletedEvent( shieldEvent );
			sequence->AddAction( shieldsOff );
		}
		TGSequence *begin = new TGSequence;
		TGEvent *event = new TGEvent;
		event->SetSource( this );
		event->SetDestination( parentShip );
		event->SetEventType( ET_CLOAK_BEGINNING );
		begin->AddCompletedEvent( event );
		if ( shieldsOff != NULL ) {
			sequence->AddAction( begin, shieldsOff, shieldDelay );
		} else {
			sequence->AddAction( begin );
		}
		sequence->Play();
		if ( cloakState != 5 ) {
			unknownB4 = 0.0f;
		}
		cloakState = 2;
		Unknown55F660();
		return true;
	}
	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( ship );
	event->SetEventType( ET_DECLOAK_BEGINNING );
	g_kEventManager.AddEvent( event );
	if ( cloakState != 2 ) {
		unknownB4 = cloakTime;
	}
	cloakState = 5;
	Unknown55F770();
	return true;
}

/*
================
CloakingSubsystem::StartCloaking

Switches the device on; the ship tries to cloak.
================
*/
// FUNCTION: STBC 0x0055f360
void CloakingSubsystem::StartCloaking( void ) {
	TurnOn();
	tryingToCloak = true;
}

/*
================
CloakingSubsystem::StopCloaking

A device that is cloaking or cloaked announces the change; either way it
is switched off.
================
*/
// FUNCTION: STBC 0x0055f380
void CloakingSubsystem::StopCloaking( void ) {
	if ( cloakState == 1 || cloakState == 2 || cloaked == true ) {
		Unknown55F110( false );
	}
	tryingToCloak = false;
	TurnOff();
}

/*
================
CloakingSubsystem::InstantCloak

A device that is not cloaked switches on and cloaks at once: the ship is
hidden, except for the player's, and the shields go off.
================
*/
// FUNCTION: STBC 0x0055f3e0
void CloakingSubsystem::InstantCloak( void ) {
	ShipClass *ship = parentShip;
	if ( cloaked != true ) {
		StartCloaking();
		Unknown55E6B0();
		if ( ship != Game::GetCurrentPlayer() ) {
			ship->GetNiObject()->SetAppCulled( true );
		}
		TGSequence *sequence = new TGSequence;
		if ( parentShip->GetShields() != NULL ) {
			TGSequence *shieldsOff = new TGSequence;
			TGBoolEvent *shieldEvent = new TGBoolEvent;
			shieldEvent->SetBool( false );
			shieldEvent->SetEventType( ET_SHIELDS_SET_STATE );
			shieldEvent->SetSource( this );
			shieldEvent->SetDestination( parentShip->GetShields() );
			shieldsOff->AddCompletedEvent( shieldEvent );
			sequence->AddAction( shieldsOff );
		}
		sequence->Play();
		TGEvent *event = new TGEvent;
		event->SetSource( this );
		event->SetDestination( parentShip );
		event->SetEventType( ET_CLOAK_COMPLETED );
		g_kEventManager.AddEvent( event );
		cloakState = CLOAK_STATE_CLOAKED;
		cloaked = true;
	}
}

/*
================
CloakingSubsystem::InstantDecloak

A cloaked device stops cloaking and the ship is shown again, except for
the player's, whose model was never hidden. The ship is told that it has
decloaked.
================
*/
// FUNCTION: STBC 0x0055f560
void CloakingSubsystem::InstantDecloak( void ) {
	if ( cloaked ) {
		StopCloaking();
		ShipClass *ship = parentShip;
		cloaked = false;
		if ( ship != Game::GetCurrentPlayer() ) {
			ship->GetNiObject()->SetAppCulled( false );
		}
		TGEvent *event = new TGEvent;
		event->SetSource( this );
		event->SetDestination( parentShip );
		event->SetEventType( ET_DECLOAK_COMPLETED );
		g_kEventManager.AddEvent( event );
	}
}

/*
================
CloakingSubsystem::Unknown55F5F0

While the ship is cloaked, cloaking or decloaking, makes its tree cloakable
again and reapplies the fade for the cloak's progress
(WarpEngineSubsystem::TransitionToState calls it).
================
*/
// FUNCTION: STBC 0x0055f5f0
void CloakingSubsystem::Unknown55F5F0( void ) {
	if ( cloaked || IsCloaking() || IsDecloaking() ) {
		Unknown55E6B0();
		float fraction = unknownB4 / cloakTime;
		if ( fraction >= 1.0f ) {
			fraction = 1.0f;
		}
		Unknown55E640( fraction );
	}
}

/*
================
CloakingSubsystem::Unknown55F660

The cloak starts: the ship is made cloakable and the cloak sound plays
from its node in the region of its set.
================
*/
// FUNCTION: STBC 0x0055f660
void CloakingSubsystem::Unknown55F660( void ) {
	Unknown55E6B0();
	cloakState = 2;
	SetClass *set = parentShip->GetContainingSet();
	if ( set != NULL ) {
		TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
		if ( region != NULL ) {
			TGSound *sound = g_kSoundManager.GetSound( "Cloak" );
			if ( sound != NULL ) {
				sound->AttachToNode( parentShip->GetNode() );
				region->AddSound( sound );
				sound->Play( true );
			}
		}
	}
}

/*
================
CloakingSubsystem::Unknown55F6D0

The cloak is complete: the ship is told, and its model is hidden, except for
the player's.
================
*/
// FUNCTION: STBC 0x0055f6d0
void CloakingSubsystem::Unknown55F6D0( void ) {
	ShipClass *ship = parentShip;
	cloakState = CLOAK_STATE_CLOAKED;
	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( ship );
	event->SetEventType( ET_CLOAK_COMPLETED );
	g_kEventManager.AddEvent( event );
	if ( ship != Game::GetCurrentPlayer() ) {
		ship->GetNiObject()->SetAppCulled( true );
	}
	cloaked = true;
}

/*
================
CloakingSubsystem::Unknown55F770

The decloak starts: the ship's model shows again, except for the player's,
and the uncloak sound plays from its node.
================
*/
// FUNCTION: STBC 0x0055f770
void CloakingSubsystem::Unknown55F770( void ) {
	ShipClass *ship = parentShip;
	cloaked = false;
	if ( ship != Game::GetCurrentPlayer() ) {
		ship->GetNiObject()->SetAppCulled( false );
	}
	cloakState = 5;
	SetClass *set = ship->GetContainingSet();
	if ( set != NULL ) {
		TGSoundRegion *region = TGSoundRegion::GetRegion( set->GetName() );
		if ( region != NULL ) {
			TGSound *sound = g_kSoundManager.GetSound( "Uncloak" );
			if ( sound != NULL ) {
				sound->AttachToNode( parentShip->GetNode() );
				region->AddSound( sound );
				sound->Play( true );
			}
		}
	}
}

/*
================
CloakingSubsystem::Unknown55F7F0

The decloak is complete: the ship is told. A ship with shields has them
switched back on after the shield delay, and the shields are asked for power
if they have none wanted.
================
*/
// FUNCTION: STBC 0x0055f7f0
void CloakingSubsystem::Unknown55F7F0( void ) {
	cloakState = 0;
	TGEvent *event = new TGEvent;
	event->SetSource( this );
	event->SetDestination( parentShip );
	event->SetEventType( ET_DECLOAK_COMPLETED );
	g_kEventManager.AddEvent( event );

	ShieldClass *shields = parentShip->GetShields();
	if ( shields != NULL ) {
		TGBoolEvent *shieldEvent = new TGBoolEvent;
		shieldEvent->SetSource( this );
		shieldEvent->SetDestination( shields );
		shieldEvent->SetBool( true );
		shieldEvent->SetEventType( ET_SHIELDS_SET_STATE );
		TGTimer *timer = new TGTimer;
		timer->SetTimerStart( g_kUtopiaModule.GetGameTime() + shieldDelay );
		timer->SetEvent( shieldEvent );
		g_kTimerManager.AddTimer( timer );
		if ( shields->GetPowerPercentageWanted() <= 0.0f ) {
			shields->SetPowerPercentageWanted( 1.0f );
		}
	}
	Unknown55E800();
}

/*
================
CloakingSubsystem::TurnOff

Switching off a device that is cloaked, or trying to, stops the cloaking.
================
*/
// FUNCTION: STBC 0x0055f930
void CloakingSubsystem::TurnOff( void ) {
	if ( IsOn() ) {
		PoweredSubsystem::TurnOff();
		if ( cloakState == CLOAK_STATE_CLOAKED || tryingToCloak == true ) {
			StopCloaking();
			Unknown55F110( false );
		}
	}
}

/*
================
CloakingSubsystem::VirtualSlot26

After PoweredSubsystem's state, whether the subsystem is trying to cloak.
================
*/
// FUNCTION: STBC 0x0055f970
void CloakingSubsystem::VirtualSlot26( TGStream *stream ) {
	PoweredSubsystem::VirtualSlot26( stream );

	stream->WriteBool( tryingToCloak );
	stream->GetPos();
}

/*
================
CloakingSubsystem::VirtualSlot27

Reads what VirtualSlot26 writes.
================
*/
// FUNCTION: STBC 0x0055f9a0
void CloakingSubsystem::VirtualSlot27( TGStream *stream ) {
	PoweredSubsystem::VirtualSlot27( stream );

	tryingToCloak = stream->ReadBool();
	stream->GetPos();
}

/*
================
CloakingSubsystem::StartCloakingHandler

The ship tries to cloak. The change is not announced again while it is
made.
================
*/
// FUNCTION: STBC 0x0055f9d0
void CloakingSubsystem::StartCloakingHandler( TGEvent *event ) {
	unknownB8 = false;
	tryingToCloak = true;
	Unknown55F110( true );
	unknownB8 = true;
	CallNextHandler( event );
}

/*
================
CloakingSubsystem::StopCloakingHandler

The ship stops trying to cloak. The change is not announced again while
it is made.
================
*/
// FUNCTION: STBC 0x0055fa00
void CloakingSubsystem::StopCloakingHandler( TGEvent *event ) {
	unknownB8 = false;
	tryingToCloak = false;
	Unknown55F110( false );
	unknownB8 = true;
	CallNextHandler( event );
}

/*
================
CloakingSubsystem::WriteToStream
================
*/
// FUNCTION: STBC 0x0055fa30
bool CloakingSubsystem::WriteToStream( TGStream *stream ) {
	if ( !PoweredSubsystem::WriteToStream( stream ) ) {
		return false;
	}

	stream->WriteBool( cloaked );
	stream->WriteBool( tryingToCloak );
	stream->WriteLong( cloakState );
	stream->WriteFloat( unknownB4 );
	stream->WriteBool( unknownB8 );
	return true;
}

/*
================
CloakingSubsystem::ReadFromStream
================
*/
// FUNCTION: STBC 0x0055faa0
bool CloakingSubsystem::ReadFromStream( TGStream *stream ) {
	if ( !PoweredSubsystem::ReadFromStream( stream ) ) {
		return false;
	}

	cloaked = stream->ReadBool();
	tryingToCloak = stream->ReadBool();
	cloakState = stream->ReadLong();
	unknownB4 = stream->ReadFloat();
	unknownB8 = stream->ReadBool();
	return true;
}

/*
================
CloakingSubsystem::VirtualSlot34

A cloaked device is cloaked again at once; one caught cloaking or
decloaking is snapped to the start of that transition, keeping
unknownB4.
================
*/
// FUNCTION: STBC 0x0055fb00
void CloakingSubsystem::VirtualSlot34( void ) {
	if ( cloaked ) {
		cloaked = false;
		InstantCloak();
		return;
	}

	float savedB4 = unknownB4;
	if ( cloakState == 2 ) {
		InstantDecloak();
		StartCloaking();
	} else if ( cloakState == 5 ) {
		InstantCloak();
		StopCloaking();
	}
	unknownB4 = savedB4;
}

