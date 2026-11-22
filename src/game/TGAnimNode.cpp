/*
===============================================================================

	TGAnimNode.cpp

===============================================================================
*/

#include <iostream>
#include <string.h>

#include "TGAnimNode.h"
#include "TGAnimBlender.h"
#include "TGAnimationManagerClass.h"
#include "UtopiaModule.h"
#include "NiKeyframeController.h"
#include "NiVisController.h"
#include "ModelHelpers.h"
#include "NiVertexColorProperty.h"
#include "NiMaterialProperty.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x006c59d0 _$E488
// FUNCTION: STBC 0x006c59f0 _$E487

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x006c5890 _$E32
// FUNCTION: STBC 0x006c58a0 _$E29
// FUNCTION: STBC 0x006c58b0 _$E31
// FUNCTION: STBC 0x006c58c0 _$E30
// FUNCTION: STBC 0x006c58d0 _$E38
// FUNCTION: STBC 0x006c58e0 _$E35
// FUNCTION: STBC 0x006c58f0 _$E37
// FUNCTION: STBC 0x006c5900 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x006c5910 _$E45
// FUNCTION: STBC 0x006c5920 _$E44
// FUNCTION: STBC 0x006c5930 _$E56
// FUNCTION: STBC 0x006c5940 _$E55
// FUNCTION: STBC 0x006c5950 _$E60
// FUNCTION: STBC 0x006c5960 _$E59
// FUNCTION: STBC 0x006c5970 _$E64
// FUNCTION: STBC 0x006c5980 _$E63
// FUNCTION: STBC 0x006c5990 _$E68
// FUNCTION: STBC 0x006c59a0 _$E67
// FUNCTION: STBC 0x006c59b0 _$E192
// FUNCTION: STBC 0x006c59c0 _$E191
// FUNCTION: STBC 0x006c5a00 _$E197
// FUNCTION: STBC 0x006c5a10 _$E196
// FUNCTION: STBC 0x006c5a20 _$E201
// FUNCTION: STBC 0x006c5a30 _$E200
// FUNCTION: STBC 0x006c5a80 _$E234
// FUNCTION: STBC 0x006c5a90 _$E233
// FUNCTION: STBC 0x006c5aa0 _$E216
// FUNCTION: STBC 0x006c5ab0 _$E215
// FUNCTION: STBC 0x006c5ac0 _$E225
// FUNCTION: STBC 0x006c5ad0 _$E224
// FUNCTION: STBC 0x006c5ae0 _$E238
// FUNCTION: STBC 0x006c5af0 _$E237
// FUNCTION: STBC 0x006c5b00 _$E246
// FUNCTION: STBC 0x006c5b10 _$E245
// FUNCTION: STBC 0x006c5b20 _$E134
// FUNCTION: STBC 0x006c5b30 _$E133
// FUNCTION: STBC 0x006c5b40 _$E147
// FUNCTION: STBC 0x006c5b50 _$E146
// FUNCTION: STBC 0x006c5b60 _$E251
// FUNCTION: STBC 0x006c5b70 _$E250
// FUNCTION: STBC 0x006c5b80 _$E255
// FUNCTION: STBC 0x006c5b90 _$E254
// FUNCTION: STBC 0x006c5ba0 _$E262
// FUNCTION: STBC 0x006c5bb0 _$E261
// FUNCTION: STBC 0x006c5bc0 _$E307
// FUNCTION: STBC 0x006c5bd0 _$E306
// FUNCTION: STBC 0x006c5be0 _$E311
// FUNCTION: STBC 0x006c5bf0 _$E310
// FUNCTION: STBC 0x006c5c00 _$E92
// FUNCTION: STBC 0x006c5c10 _$E91
// FUNCTION: STBC 0x006c5c20 _$E293
// FUNCTION: STBC 0x006c5c30 _$E292
// FUNCTION: STBC 0x006c5c40 _$E300
// FUNCTION: STBC 0x006c5c50 _$E299
// FUNCTION: STBC 0x006c5c60 _$E323
// FUNCTION: STBC 0x006c5c70 _$E322
// FUNCTION: STBC 0x006c5c80 _$E327
// FUNCTION: STBC 0x006c5c90 _$E326
// FUNCTION: STBC 0x006c5ca0 _$E367
// FUNCTION: STBC 0x006c5cb0 _$E366
// FUNCTION: STBC 0x006c5cc0 _$E373
// FUNCTION: STBC 0x006c5cd0 _$E372
// FUNCTION: STBC 0x006c5ce0 _$E315
// FUNCTION: STBC 0x006c5cf0 _$E314
// FUNCTION: STBC 0x006c5d00 _$E319
// FUNCTION: STBC 0x006c5d10 _$E318
// FUNCTION: STBC 0x006c5d20 _$E331
// FUNCTION: STBC 0x006c5d30 _$E330
// FUNCTION: STBC 0x006c5d40 _$E335
// FUNCTION: STBC 0x006c5d50 _$E334
// FUNCTION: STBC 0x006c5d60 _$E339
// FUNCTION: STBC 0x006c5d70 _$E338
// FUNCTION: STBC 0x006c5d80 _$E343
// FUNCTION: STBC 0x006c5d90 _$E342
// FUNCTION: STBC 0x006c5da0 _$E347
// FUNCTION: STBC 0x006c5db0 _$E346
// FUNCTION: STBC 0x006c5dc0 _$E351
// FUNCTION: STBC 0x006c5dd0 _$E350
// FUNCTION: STBC 0x006c5de0 _$E355
// FUNCTION: STBC 0x006c5df0 _$E354
// FUNCTION: STBC 0x006c5e00 _$E359
// FUNCTION: STBC 0x006c5e10 _$E358
// FUNCTION: STBC 0x006c5e20 _$E97
// FUNCTION: STBC 0x006c5e30 _$E96
// FUNCTION: STBC 0x006c5e40 _$E363
// FUNCTION: STBC 0x006c5e50 _$E362
// FUNCTION: STBC 0x006c5e60 _$E102
// FUNCTION: STBC 0x006c5e70 _$E101
// FUNCTION: STBC 0x006c5e80 _$E377
// FUNCTION: STBC 0x006c5e90 _$E376
// FUNCTION: STBC 0x006c5ea0 _$E381
// FUNCTION: STBC 0x006c5eb0 _$E380
// FUNCTION: STBC 0x006c5ec0 _$E385
// FUNCTION: STBC 0x006c5ed0 _$E384
// FUNCTION: STBC 0x006c5ee0 _$E389
// FUNCTION: STBC 0x006c5ef0 _$E388
// FUNCTION: STBC 0x006c5f00 _$E111
// FUNCTION: STBC 0x006c5f10 _$E110
// FUNCTION: STBC 0x006c5f20 _$E393
// FUNCTION: STBC 0x006c5f30 _$E392
// FUNCTION: STBC 0x006c5f40 _$E397
// FUNCTION: STBC 0x006c5f50 _$E396
// FUNCTION: STBC 0x006c5f60 _$E401
// FUNCTION: STBC 0x006c5f70 _$E400
// FUNCTION: STBC 0x006c5f80 _$E117
// FUNCTION: STBC 0x006c5f90 _$E116
// FUNCTION: STBC 0x006c5fa0 _$E122
// FUNCTION: STBC 0x006c5fb0 _$E121
// FUNCTION: STBC 0x006c5fc0 _$E152
// FUNCTION: STBC 0x006c5fd0 _$E151
// FUNCTION: STBC 0x006c5fe0 _$E405
// FUNCTION: STBC 0x006c5ff0 _$E404
// FUNCTION: STBC 0x006c6000 _$E157
// FUNCTION: STBC 0x006c6010 _$E156
// FUNCTION: STBC 0x006c6020 _$E409
// FUNCTION: STBC 0x006c6030 _$E408
// FUNCTION: STBC 0x006c6040 _$E413
// FUNCTION: STBC 0x006c6050 _$E412
// FUNCTION: STBC 0x006c6060 _$E162
// FUNCTION: STBC 0x006c6070 _$E161
// FUNCTION: STBC 0x006c6080 _$E417
// FUNCTION: STBC 0x006c6090 _$E416
// FUNCTION: STBC 0x006c60a0 _$E127
// FUNCTION: STBC 0x006c60b0 _$E126
// FUNCTION: STBC 0x006c60c0 _$E421
// FUNCTION: STBC 0x006c60d0 _$E420
// FUNCTION: STBC 0x006c60e0 _$E425
// FUNCTION: STBC 0x006c60f0 _$E424
// FUNCTION: STBC 0x006c6100 _$E167
// FUNCTION: STBC 0x006c6110 _$E166
// FUNCTION: STBC 0x006c6120 _$E172
// FUNCTION: STBC 0x006c6130 _$E171
// FUNCTION: STBC 0x006c6140 _$E429
// FUNCTION: STBC 0x006c6150 _$E428
// FUNCTION: STBC 0x006c6160 _$E433
// FUNCTION: STBC 0x006c6170 _$E432
// FUNCTION: STBC 0x006c6180 _$E437
// FUNCTION: STBC 0x006c6190 _$E436
// FUNCTION: STBC 0x006c61a0 _$E441
// FUNCTION: STBC 0x006c61b0 _$E440
// FUNCTION: STBC 0x006c61c0 _$E445
// FUNCTION: STBC 0x006c61d0 _$E444
// FUNCTION: STBC 0x006c61e0 _$E449
// FUNCTION: STBC 0x006c61f0 _$E448
// FUNCTION: STBC 0x006c6200 _$E453
// FUNCTION: STBC 0x006c6210 _$E452
// FUNCTION: STBC 0x006c6220 _$E457
// FUNCTION: STBC 0x006c6230 _$E456
// FUNCTION: STBC 0x006c6240 _$E177
// FUNCTION: STBC 0x006c6250 _$E176
// FUNCTION: STBC 0x006c6260 _$E461
// FUNCTION: STBC 0x006c6270 _$E460
// FUNCTION: STBC 0x006c6280 _$E182
// FUNCTION: STBC 0x006c6290 _$E181
// FUNCTION: STBC 0x006c62a0 _$E187
// FUNCTION: STBC 0x006c62b0 _$E186
// FUNCTION: STBC 0x006c62c0 _$E274
// FUNCTION: STBC 0x006c62d0 _$E273
// FUNCTION: STBC 0x006c62e0 _$E278
// FUNCTION: STBC 0x006c62f0 _$E277
// FUNCTION: STBC 0x006c6300 _$E282
// FUNCTION: STBC 0x006c6310 _$E281

// The object's initialiser and exit-time destructor.
// FUNCTION: STBC 0x006c48f0 _$E480
// FUNCTION: STBC 0x006c4900 _$E476
// FUNCTION: STBC 0x006c4920 _$E479
// FUNCTION: STBC 0x006c4930 _$E477
// DATA: STBC 0x9977B8 ?rtti@TGAnimNode@@2VNiRTTI@@B
const NiRTTI TGAnimNode::rtti( "TGAnimNode", &NiNode::rtti );

/*
================
TGAnimNode::FindBlender

Removes the idle blenders that have outlived their time on the way.
================
*/
// FUNCTION: STBC 0x006c4950
TGAnimBlender *TGAnimNode::FindBlender( bool exclusive ) {
	TGAnimBlender *found = NULL;
	NiTimeController *controller = GetControllers();
	while ( controller ) {
		NiTimeController *next = controller->GetNext();
		TGAnimBlender *blender = NiDynamicCast( TGAnimBlender, controller );
		if ( blender && ( !blender->playing || exclusive ) ) {
			if ( found == NULL ) {
				found = blender;
				blender->startTime = g_kUtopiaModule.GetGameTime();
			} else {
				if ( exclusive ) {
					blender->Stop();
				}
				if ( g_kUtopiaModule.GetGameTime() - blender->startTime > blender->GetUnknown48() ) {
					RemoveController( controller );
				}
			}
		}
		controller = next;
	}
	if ( found == NULL ) {
		found = new TGAnimBlender;
		found->SetTarget( this );
	}
	return found;
}

/*
================
TGAnimNode::TGAnimNode
================
*/
// FUNCTION: STBC 0x006c4a70
TGAnimNode::TGAnimNode( void ) {
	rootNode = NULL;
	rootTranslate = NiPoint3::ZERO;
	rootRotate = NiMatrix3::IDENTITY;
	parentTranslate = NiPoint3::ZERO;
	parentRotate = NiMatrix3::IDENTITY;
	blendTime = 0.34f;
}

/*
================
TGAnimNode::~TGAnimNode
================
*/
// FUNCTION: STBC 0x006c4b30
TGAnimNode::~TGAnimNode() {
}

/*
================
TGAnimNode::CreateClone
================
*/
// FUNCTION: STBC 0x006c4b40
NiObject *TGAnimNode::CreateClone() {
	TGAnimNode *clone = new TGAnimNode;
	CopyMembers( clone );
	return clone;
}

/*
================
TGAnimNode::CopyMembers
================
*/
// FUNCTION: STBC 0x006c4b90
void TGAnimNode::CopyMembers( TGAnimNode *dest ) {
	NiNode::CopyMembers( dest );
	dest->Copy( this );
}

/*
================
TGAnimNode::Copy

Takes the source's rest transforms and finds the node of the source's
root node's name under this one.
================
*/
// FUNCTION: STBC 0x006c4bb0 ?Copy@TGAnimNode@@QAEXPAV1@@Z
void TGAnimNode::Copy( TGAnimNode *source ) {
	rootTranslate = source->rootTranslate;
	rootRotate = source->rootRotate;
	parentTranslate = source->parentTranslate;
	parentRotate = source->parentRotate;
	if ( source->rootNode ) {
		const char *name = source->rootNode->GetName();
		rootNode = NiDynamicCast( NiNode, GetObjectByName( name ) );
		if ( rootNode ) {
			rootNode->Update( 0.0f );
			rootTranslate = rootNode->GetTranslate();
			rootRotate = rootNode->GetRotate();
			parentTranslate = rootNode->GetParent()->GetTranslate();
			parentRotate = rootNode->GetParent()->GetRotate();
		}
	} else {
		rootNode = NULL;
	}
}

/*
================
TGAnimNode::UseAnimation
================
*/
// FUNCTION: STBC 0x006c4d20 ?UseAnimation@TGAnimNode@@QAEXPAD_NMMP6AXJ@ZJ@Z
void TGAnimNode::UseAnimation( char *animation, bool unknown, float start, float end, TGAnimCallbackPtr callback, long id ) {
	TGAnimBlender *blender = FindBlender( unknown );
	if ( blender ) {
		blender->Init();
		blender->SetBlendTime( blendTime );
		blender->SetAnimationName( animation );
		// BUG: the end time is not kept; the blender's Start plays to the end
		blender->blend = false;
		blender->exclusive = unknown;
		blender->rootNode = rootNode;
		blender->SetCallback( callback, id );
		blender->Start( start );
	}
}

/*
================
TGAnimNode::SetExclusiveAnimation

An animation that moves the root node starts from the parent's place: the
root's offset from its rest transform is folded into the parent first.
Otherwise the parent's transform is folded into the root, and the parent
reset.
================
*/
// FUNCTION: STBC 0x006c4d90 ?SetExclusiveAnimation@TGAnimNode@@QAEXPADM_NP6AXJ@ZJ@Z
// PARKED: inline budget: with the blender's root node through GetRootNode() the sum's constructor goes out of line as retail; root->SetTranslate( NiPoint3::ZERO )'s operator= needs one more inline site after it (share 57 against cost 57), which retail calls out of line
void TGAnimNode::SetExclusiveAnimation( char *animation, float time, bool unknown, TGAnimCallbackPtr callback, long id ) {
	NiNode *root = rootNode;
	if ( root ) {
		if ( g_kAnimationManager.GetAnimationType( animation ) ) {
			NiPoint3 translate = root->GetTranslate();
			NiNode *parent = root->GetParent();
			NiMatrix3 rotate = root->GetRotate();
			rotate = rootRotate.TransposeTimes( rotate );
			NiPoint3 parentTranslate = parent->GetTranslate();
			NiMatrix3 parentRotate = parent->GetRotate();
			translate = parentTranslate + parentRotate * translate;
			rotate = parentRotate * rotate;
			parent->SetRotate( rotate );
			parent->SetTranslate( translate );
			root->SetRotate( rootRotate );
			root->SetTranslate( NiPoint3::ZERO );
		} else {
			NiNode *parent = root->GetParent();
			NiPoint3 parentTranslate = parent->GetTranslate();
			NiMatrix3 parentRotate = parent->GetRotate();
			NiPoint3 translate = root->GetTranslate();
			NiMatrix3 rotate = root->GetRotate();
			translate = parentTranslate + translate * parentRotate;
			rotate = parentRotate * rotate;
			root->SetRotate( rotate );
			root->SetTranslate( translate );
			parent->SetRotate( NiMatrix3::IDENTITY );
			parent->SetTranslate( NiPoint3::ZERO );
		}
	}

	TGAnimBlender *blender = FindBlender( true );
	if ( blender ) {
		blender->Init();
		blender->SetBlendTime( blendTime );
		blender->SetAnimationName( animation );
		blender->unknown55 = unknown;
		blender->rootNode = GetRootNode();
		blender->SetCallback( callback, id );
		blender->Start( time );
	}
}

/*
================
TGAnimNode::SetNonExclusiveAnimation
================
*/
// FUNCTION: STBC 0x006c5070 ?SetNonExclusiveAnimation@TGAnimNode@@QAEXPADM_NP6AXJ@ZJ@Z
// PARKED: inline budget: retail calls NiPoint3's constructor (0x414540) and operator= (0x414560) out of line in the root-moving branch; this build expands both by a margin of 1 and 0 units: one more inline site after root->SetTranslate( NiPoint3::ZERO ) flips both
void TGAnimNode::SetNonExclusiveAnimation( char *animation, float time, bool unknown, TGAnimCallbackPtr callback, long id ) {
	NiNode *root = rootNode;
	if ( root ) {
		int type = g_kAnimationManager.GetAnimationType( animation );
		if ( type == TGAnimationManagerClass::SAT_SPECIAL ) {
			NiPoint3 translate = root->GetTranslate();
			NiNode *parent = root->GetParent();
			NiMatrix3 rotate = root->GetRotate();
			rotate = rootRotate.TransposeTimes( rotate );
			NiPoint3 parentTranslate = parent->GetTranslate();
			NiMatrix3 parentRotate = parent->GetRotate();
			translate = parentTranslate + parentRotate * translate;
			rotate = parentRotate * rotate;
			parent->SetRotate( rotate );
			parent->SetTranslate( translate );
			root->SetRotate( rootRotate );
			root->SetTranslate( NiPoint3::ZERO );
		} else if ( type == TGAnimationManagerClass::SAT_MOVES_PELVIS ) {
			NiNode *parent = root->GetParent();
			NiPoint3 parentTranslate = parent->GetTranslate();
			NiMatrix3 parentRotate = parent->GetRotate();
			NiPoint3 translate = root->GetTranslate();
			NiMatrix3 rotate = root->GetRotate();
			translate = parentTranslate + translate * parentRotate;
			rotate = parentRotate * rotate;
			root->SetRotate( rotate );
			root->SetTranslate( translate );
			parent->SetRotate( NiMatrix3::IDENTITY );
			parent->SetTranslate( NiPoint3::ZERO );
		}
	}

	TGAnimBlender *blender = FindBlender( false );
	if ( blender ) {
		blender->Init();
		blender->SetBlendTime( blendTime );
		blender->SetAnimationName( animation );
		blender->unknown55 = unknown;
		blender->exclusive = false;
		blender->rootNode = GetRootNode();
		blender->SetCallback( callback, id );
		blender->Start( time );
	}
}

/*
================
TGAnimNode::StopNonExclusiveAnimation

Stops the first playing blender of the animation.
================
*/
// FUNCTION: STBC 0x006c5350 ?StopNonExclusiveAnimation@TGAnimNode@@QAEXPAD@Z
void TGAnimNode::StopNonExclusiveAnimation( char *animation ) {
	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		TGAnimBlender *blender = NiDynamicCast( TGAnimBlender, controller );
		if ( blender && blender->playing && strcmp( blender->animationName, animation ) == 0 ) {
			blender->Stop();
			return;
		}
	}
}

/*
================
TGAnimNode::SetExclusiveAnimationUseDefault
================
*/
// FUNCTION: STBC 0x006c53f0 ?SetExclusiveAnimationUseDefault@TGAnimNode@@QAEXPADM_NP6AXJ@ZJ@Z
void TGAnimNode::SetExclusiveAnimationUseDefault( char *animation, float time, bool unknown, TGAnimCallbackPtr callback, long id ) {
	TGAnimBlender *blender = FindBlender( true );
	if ( blender ) {
		blender->Init();
		blender->SetBlendTime( blendTime );
		blender->SetAnimationName( animation );
		blender->unknown55 = unknown;
		blender->useDefault = true;
		blender->rootNode = rootNode;
		blender->SetCallback( callback, id );
		blender->Start( time );
	}
}

/*
================
TGAnimNode::UseAnimationPosition

Poses the node at the animation's position: the root's parent is reset,
the animation applied, and the root's resulting transform kept as its rest
transform.
================
*/
// FUNCTION: STBC 0x006c5460 ?UseAnimationPosition@TGAnimNode@@QAEXPAD@Z
void TGAnimNode::UseAnimationPosition( char *animation ) {
	TGAnimBlender *blender = FindBlender( true );
	if ( blender ) {
		blender->SetAnimationName( animation );
		blender->rootNode = rootNode;
		if ( rootNode ) {
			NiNode *parent = rootNode->GetParent();
			parent->SetRotate( NiMatrix3::IDENTITY );
			parent->SetTranslate( NiPoint3::ZERO );
		}
		g_kAnimationManager.PositionUsingAnimation( blender, animation );
		blender->Stop();
		Update( 0.0f );
		if ( rootNode ) {
			rootRotate = rootNode->GetRotate();
			rootTranslate = rootNode->GetTranslate();
		}
	}
}

/*
================
TGAnimNode::Unknown6C5520

Gives the node its own vertex colour property (no vertex colours, lit
emissive-ambient-diffuse) and a white material in place of the ones its
model came with; ShipClass::VirtualSlot66 calls it on the animated node
under each child of the ship's node.
================
*/
// FUNCTION: STBC 0x006c5520
// PARKED: complete (calls, stores and their order agree); retail keeps this in ebx and the property list address in esi across both AttachProperty calls, this build keeps this in edi and the list address in a frame slot: register/frame wall, as CharacterClass::Unknown668000
void TGAnimNode::Unknown6C5520( void ) {
	RemovePropertyFromTree( this, NiVertexColorProperty::VERTEX_COLOR );
	NiVertexColorProperty *vertexColor = new NiVertexColorProperty;
	vertexColor->SetSourceMode( NiVertexColorProperty::SOURCE_IGNORE );
	vertexColor->SetLightingMode( NiVertexColorProperty::LIGHTING_E_A_D );
	AttachProperty( vertexColor );

	RemovePropertyFromTree( this, NiMaterialProperty::MATERIAL );
	NiMaterialProperty *material = new NiMaterialProperty;
	material->SetAmbientColor( NiColor::WHITE );
	material->SetDiffuseColor( NiColor::WHITE );
	material->SetEmittance( NiColor::BLACK );
	material->SetSpecularColor( NiColor::WHITE );
	AttachProperty( material );
}

/*
================
TGAnimNode::Unknown6C5680
================
*/
// FUNCTION: STBC 0x006c5680
void TGAnimNode::Unknown6C5680( void ) {
	Unknown6C5690( this );
}

/*
================
TGAnimNode::Unknown6C5690

Drops the keys of the keyframe controllers of the object and of the
objects under it.
================
*/
// FUNCTION: STBC 0x006c5690
void TGAnimNode::Unknown6C5690( NiAVObject *object ) {
	NiKeyframeController *keyframeController = FindKeyframeController( object );
	if ( keyframeController ) {
		keyframeController->ClearData();
	}
	// BUG: the visibility controller is looked up and never used
	FindVisController( object );

	if ( object && object->IsKindOf( &NiNode::rtti ) ) {
		NiNode *node = (NiNode *)object;
		for ( unsigned int i = 0; i < node->GetArrayCount(); i++ ) {
			NiAVObject *child = node->GetAt( i );
			if ( child && child->IsKindOf( &NiAVObject::rtti ) ) {
				Unknown6C5690( child );
			}
		}
	}
}

/*
================
TGAnimNode::IsAnimate

True while one of the node's blenders is playing.
================
*/
// FUNCTION: STBC 0x006c5760 ?IsAnimate@TGAnimNode@@QAE_NXZ
bool TGAnimNode::IsAnimate( void ) {
	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		TGAnimBlender *blender = NiDynamicCast( TGAnimBlender, controller );
		if ( blender && blender->playing ) {
			return true;
		}
	}
	return false;
}

/*
================
TGAnimNode::Stop

Stops every blender of the node's.
================
*/
// FUNCTION: STBC 0x006c57b0 ?Stop@TGAnimNode@@QAEXXZ
void TGAnimNode::Stop( void ) {
	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		TGAnimBlender *blender = NiDynamicCast( TGAnimBlender, controller );
		if ( blender ) {
			blender->Stop();
		}
	}
}

/*
================
TGAnimNode::StopAnimation

Stops every blender of the animation, playing or not (invented name;
nothing in retail calls it).
================
*/
// FUNCTION: STBC 0x006c5800
void TGAnimNode::StopAnimation( char *animation ) {
	for ( NiTimeController *controller = GetControllers(); controller; controller = controller->GetNext() ) {
		TGAnimBlender *blender = NiDynamicCast( TGAnimBlender, controller );
		if ( blender && strcmp( blender->animationName, animation ) == 0 ) {
			blender->Stop();
		}
	}
}
