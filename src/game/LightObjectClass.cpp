/*
===============================================================================

	LightObjectClass.cpp

===============================================================================
*/

#include <iostream>
#include "LightObjectClass.h"
#include "SetClass.h"
#include "SetManager.h"
#include "NiLight.h"
#include "TGEventHandlerTable.h"
#include "NiAmbientLight.h"
#include "NiDirectionalLight.h"
#include "NiPointLight.h"
#include "NiSpotLight.h"
#include "TGStream.h"
#include "TGPoint3.h"
#include "LODModelManager.h"
#include "NiMath.h"
#include "NiBinaryVoxelData.h"
#include "NiBinaryVoxelExtraData.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x00434970 _$E459
// FUNCTION: STBC 0x00434990 _$E458

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x00434020 _$E32
// FUNCTION: STBC 0x00434030 _$E29
// FUNCTION: STBC 0x00434040 _$E31
// FUNCTION: STBC 0x00434050 _$E30
// FUNCTION: STBC 0x00434060 _$E38
// FUNCTION: STBC 0x00434070 _$E35
// FUNCTION: STBC 0x00434080 _$E37
// FUNCTION: STBC 0x00434090 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x004329c0 _$E252
// FUNCTION: STBC 0x004329d0 _$E251
// FUNCTION: STBC 0x004329e0 _$E256
// FUNCTION: STBC 0x004329f0 _$E255
// FUNCTION: STBC 0x004340a0 _$E66
// FUNCTION: STBC 0x004340b0 _$E65
// FUNCTION: STBC 0x004340c0 _$E260
// FUNCTION: STBC 0x004340d0 _$E259
// FUNCTION: STBC 0x004340e0 _$E77
// FUNCTION: STBC 0x004340f0 _$E76
// FUNCTION: STBC 0x00434100 _$E81
// FUNCTION: STBC 0x00434110 _$E80
// FUNCTION: STBC 0x00434120 _$E85
// FUNCTION: STBC 0x00434130 _$E84
// FUNCTION: STBC 0x00434140 _$E89
// FUNCTION: STBC 0x00434150 _$E88
// FUNCTION: STBC 0x00434160 _$E206
// FUNCTION: STBC 0x00434170 _$E205
// FUNCTION: STBC 0x004342c0 _$E148
// FUNCTION: STBC 0x004342d0 _$E147
// FUNCTION: STBC 0x004342e0 _$E161
// FUNCTION: STBC 0x004342f0 _$E160
// FUNCTION: STBC 0x00434300 _$E211
// FUNCTION: STBC 0x00434310 _$E210
// FUNCTION: STBC 0x00434330 _$E265
// FUNCTION: STBC 0x00434340 _$E264
// FUNCTION: STBC 0x00434350 _$E215
// FUNCTION: STBC 0x00434360 _$E214
// FUNCTION: STBC 0x00434370 _$E269
// FUNCTION: STBC 0x00434380 _$E268
// FUNCTION: STBC 0x00434390 _$E276
// FUNCTION: STBC 0x004343a0 _$E275
// FUNCTION: STBC 0x004343b0 _$E298
// FUNCTION: STBC 0x004343c0 _$E297
// FUNCTION: STBC 0x004343d0 _$E302
// FUNCTION: STBC 0x004343e0 _$E301
// FUNCTION: STBC 0x004343f0 _$E106
// FUNCTION: STBC 0x00434400 _$E105
// FUNCTION: STBC 0x00434450 _$E284
// FUNCTION: STBC 0x00434460 _$E283
// FUNCTION: STBC 0x00434470 _$E291
// FUNCTION: STBC 0x00434480 _$E290
// FUNCTION: STBC 0x00434490 _$E306
// FUNCTION: STBC 0x004344a0 _$E305
// FUNCTION: STBC 0x004344b0 _$E310
// FUNCTION: STBC 0x004344c0 _$E309
// FUNCTION: STBC 0x004344d0 _$E346
// FUNCTION: STBC 0x004344e0 _$E345
// FUNCTION: STBC 0x004344f0 _$E352
// FUNCTION: STBC 0x00434500 _$E351
// FUNCTION: STBC 0x00434510 _$E230
// FUNCTION: STBC 0x00434520 _$E229
// FUNCTION: STBC 0x00434530 _$E234
// FUNCTION: STBC 0x00434540 _$E233
// FUNCTION: STBC 0x00434550 _$E314
// FUNCTION: STBC 0x00434560 _$E313
// FUNCTION: STBC 0x00434570 _$E318
// FUNCTION: STBC 0x00434580 _$E317
// FUNCTION: STBC 0x00434590 _$E322
// FUNCTION: STBC 0x004345a0 _$E321
// FUNCTION: STBC 0x004345b0 _$E326
// FUNCTION: STBC 0x004345c0 _$E325
// FUNCTION: STBC 0x004345d0 _$E330
// FUNCTION: STBC 0x004345e0 _$E329
// FUNCTION: STBC 0x004345f0 _$E334
// FUNCTION: STBC 0x00434600 _$E333
// FUNCTION: STBC 0x00434610 _$E338
// FUNCTION: STBC 0x00434620 _$E337
// FUNCTION: STBC 0x00434630 _$E342
// FUNCTION: STBC 0x00434640 _$E341
// FUNCTION: STBC 0x00434650 _$E111
// FUNCTION: STBC 0x00434660 _$E110
// FUNCTION: STBC 0x00434670 _$E238
// FUNCTION: STBC 0x00434680 _$E237
// FUNCTION: STBC 0x00434690 _$E116
// FUNCTION: STBC 0x004346a0 _$E115
// FUNCTION: STBC 0x004346b0 _$E356
// FUNCTION: STBC 0x004346c0 _$E355
// FUNCTION: STBC 0x004346d0 _$E360
// FUNCTION: STBC 0x004346e0 _$E359
// FUNCTION: STBC 0x004346f0 _$E364
// FUNCTION: STBC 0x00434700 _$E363
// FUNCTION: STBC 0x00434710 _$E368
// FUNCTION: STBC 0x00434720 _$E367
// FUNCTION: STBC 0x00434730 _$E125
// FUNCTION: STBC 0x00434740 _$E124
// FUNCTION: STBC 0x00434750 _$E372
// FUNCTION: STBC 0x00434760 _$E371
// FUNCTION: STBC 0x00434770 _$E376
// FUNCTION: STBC 0x00434780 _$E375
// FUNCTION: STBC 0x00434790 _$E380
// FUNCTION: STBC 0x004347a0 _$E379
// FUNCTION: STBC 0x004347b0 _$E131
// FUNCTION: STBC 0x004347c0 _$E130
// FUNCTION: STBC 0x004347d0 _$E136
// FUNCTION: STBC 0x004347e0 _$E135
// FUNCTION: STBC 0x004347f0 _$E166
// FUNCTION: STBC 0x00434800 _$E165
// FUNCTION: STBC 0x00434900 _$E242
// FUNCTION: STBC 0x00434910 _$E241
// FUNCTION: STBC 0x00434930 _$E171
// FUNCTION: STBC 0x00434940 _$E170
// FUNCTION: STBC 0x00434950 _$E384
// FUNCTION: STBC 0x00434960 _$E383
// FUNCTION: STBC 0x004349a0 _$E388
// FUNCTION: STBC 0x004349b0 _$E387
// FUNCTION: STBC 0x004349c0 _$E176
// FUNCTION: STBC 0x004349d0 _$E175
// FUNCTION: STBC 0x004349e0 _$E246
// FUNCTION: STBC 0x004349f0 _$E245
// FUNCTION: STBC 0x00434a50 _$E141
// FUNCTION: STBC 0x00434a60 _$E140
// FUNCTION: STBC 0x00434a70 _$E392
// FUNCTION: STBC 0x00434a80 _$E391
// FUNCTION: STBC 0x00434a90 _$E396
// FUNCTION: STBC 0x00434aa0 _$E395
// FUNCTION: STBC 0x00434ab0 _$E181
// FUNCTION: STBC 0x00434ac0 _$E180
// FUNCTION: STBC 0x00434ad0 _$E186
// FUNCTION: STBC 0x00434ae0 _$E185
// FUNCTION: STBC 0x00434af0 _$E400
// FUNCTION: STBC 0x00434b00 _$E399
// FUNCTION: STBC 0x00434b10 _$E404
// FUNCTION: STBC 0x00434b20 _$E403
// FUNCTION: STBC 0x00434b30 _$E408
// FUNCTION: STBC 0x00434b40 _$E407
// FUNCTION: STBC 0x00434b50 _$E412
// FUNCTION: STBC 0x00434b60 _$E411
// FUNCTION: STBC 0x00434b70 _$E416
// FUNCTION: STBC 0x00434b80 _$E415
// FUNCTION: STBC 0x00434b90 _$E420
// FUNCTION: STBC 0x00434ba0 _$E419
// FUNCTION: STBC 0x00434bb0 _$E424
// FUNCTION: STBC 0x00434bc0 _$E423
// FUNCTION: STBC 0x00434bd0 _$E428
// FUNCTION: STBC 0x00434be0 _$E427
// FUNCTION: STBC 0x00434bf0 _$E191
// FUNCTION: STBC 0x00434c00 _$E190
// FUNCTION: STBC 0x00434c10 _$E432
// FUNCTION: STBC 0x00434c20 _$E431
// FUNCTION: STBC 0x00434c30 _$E196
// FUNCTION: STBC 0x00434c40 _$E195
// FUNCTION: STBC 0x00434c50 _$E201
// FUNCTION: STBC 0x00434c60 _$E200
// FUNCTION: STBC 0x00434c70 _$E436
// FUNCTION: STBC 0x00434c80 _$E435
// FUNCTION: STBC 0x00434c90 _$E441
// FUNCTION: STBC 0x00434ca0 _$E440
// FUNCTION: STBC 0x00434cb0 _$E445
// FUNCTION: STBC 0x00434cc0 _$E444

/*
================
LightObjectClass::AddToSet

Adds the light to the set's list of its class type, and to those of its
base classes.
================
*/
// FUNCTION: STBC 0x00432a50
void LightObjectClass::AddToSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Add( this, LIGHTOBJECTCLASS_TYPE );
		BaseObjectClass::AddToSet( set );
	}
}

/*
================
LightObjectClass::RemoveFromSet
================
*/
// FUNCTION: STBC 0x00432a80
void LightObjectClass::RemoveFromSet( SetClass *set ) {
	if ( set != NULL ) {
		set->objectTable.Remove( this, LIGHTOBJECTCLASS_TYPE );
		BaseObjectClass::RemoveFromSet( set );
	}
}

/*
================
LightObjectClass::GetClassObjects

The set's light objects and their number; NULL and 0 when there are none.
================
*/
// FUNCTION: STBC 0x00432ab0
LightObjectClass **LightObjectClass::GetClassObjects( SetClass *set, int *count ) {
	if ( set != NULL ) {
		return (LightObjectClass **)set->objectTable.GetObjects( LIGHTOBJECTCLASS_TYPE, count );
	}
	*count = 0;
	return NULL;
}

/*
================
LightObjectClass::GetNumClassObjects

The number of the set's lights.
================
*/
// FUNCTION: STBC 0x00432ae0 ?GetNumClassObjects@LightObjectClass@@SAHPAVSetClass@@@Z
int LightObjectClass::GetNumClassObjects( SetClass *set ) {
	if ( set != NULL ) {
		return set->objectTable.GetNumObjects( LIGHTOBJECTCLASS_TYPE );
	}
	return 0;
}

/*
================
LightObjectClass::GetObjectByID

The light object with the given ID in the set, or among every object when
the set is NULL; NULL when it is none or not a light object.
================
*/
// FUNCTION: STBC 0x00432b00
LightObjectClass *LightObjectClass::GetObjectByID( SetClass *set, long id ) {
	if ( set == NULL ) {
		TGObject *object;
		if ( TGObject::objectTable->GetAt( id, object ) && object->IsTypeOf( LIGHTOBJECTCLASS_TYPE ) ) {
			return (LightObjectClass *)object;
		}
		return NULL;
	}
	return (LightObjectClass *)set->GetClassObject( LIGHTOBJECTCLASS_TYPE, id );
}

/*
================
LightObjectClass::GetObject

The light with the name in the set, or in the last set that has one when the
set is NULL; NULL when there is none.
================
*/
// FUNCTION: STBC 0x00432b70 ?GetObject@LightObjectClass@@SAPAV1@PAVSetClass@@PAD@Z
LightObjectClass *LightObjectClass::GetObject( SetClass *set, char *name_ ) {
	if ( set == NULL ) {
		int count = g_kSetManager.GetNumSets();
		SetClass **sets = g_kSetManager.GetSets();
		BaseObjectClass *object = NULL;
		for ( int i = count - 1; i >= 0 && object == NULL; i-- ) {
			object = sets[i]->GetClassObject( LIGHTOBJECTCLASS_TYPE, name_ );
		}
		return (LightObjectClass *)object;
	}
	return (LightObjectClass *)set->GetClassObject( LIGHTOBJECTCLASS_TYPE, name_ );
}

/*
================
LightObjectClass::Cast

The object as a LightObjectClass; NULL when it is none.
================
*/
// FUNCTION: STBC 0x00432bd0 ?Cast@LightObjectClass@@SAPAV1@PAVTGObject@@@Z
LightObjectClass *LightObjectClass::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( LIGHTOBJECTCLASS_TYPE ) ) {
		return (LightObjectClass *)object;
	}
	return NULL;
}

/*
================
LightObjectClass::LightObjectClass
================
*/
// FUNCTION: STBC 0x00432c00 ??0LightObjectClass@@QAE@J@Z
LightObjectClass::LightObjectClass( long id ) : BaseObjectClass( id ), illuminatedObjects( 2 ) {
}

// FUNCTION: STBC 0x00432a00 _$E451
// FUNCTION: STBC 0x00432a10 _$E447
// FUNCTION: STBC 0x00432a20 _$E450
// FUNCTION: STBC 0x00432a30 _$E448
// DATA: STBC 0x0097f4c0
TGEventHandlerTable LightObjectClass::classHandlers;

/*
================
LightObjectClass::GetObjType
================
*/
// FUNCTION: STBC 0x00432c40
int LightObjectClass::GetObjType( void ) {
	return LIGHTOBJECTCLASS_TYPE;
}

/*
================
LightObjectClass::GetClassName
================
*/
// FUNCTION: STBC 0x00432ca0
char *LightObjectClass::GetClassName( void ) {
	return "LightObjectClass";
}

/*
================
LightObjectClass::GetSwigTypeName
================
*/
// FUNCTION: STBC 0x00432cb0
char *LightObjectClass::GetSwigTypeName( void ) {
	return "_p_LightObjectClass";
}

/*
================
LightObjectClass::GetPythonClassName
================
*/
// FUNCTION: STBC 0x00432cc0
char *LightObjectClass::GetPythonClassName( void ) {
	return "LightObjectClassPtr";
}

/*
================
LightObjectClass::GetClassHandlers
================
*/
// FUNCTION: STBC 0x00432cd0
TGEventHandlerTable *LightObjectClass::GetClassHandlers( void ) {
	return &classHandlers;
}

/*
================
LightObjectClass::~LightObjectClass
================
*/
// FUNCTION: STBC 0x00432d10
LightObjectClass::~LightObjectClass( void ) {
}

/*
================
LightObjectClass::Create

Wraps the NetImmerse light in a light object named after it.
================
*/
// FUNCTION: STBC 0x00432d60 ?Create@LightObjectClass@@SAPAV1@PAVNiLight@@@Z
LightObjectClass *LightObjectClass::Create( NiLight *light ) {
	LightObjectClass *object = new LightObjectClass;
	object->SetNiObject( light );
	object->SetName( const_cast<char *>( light->GetName() ) );
	return object;
}

/*
================
LightObjectClass::CreateNiPointLight

A point light of the colour, named, with a quarter of it for ambient and
white specular; a non-negative attenuation sets the linear attenuation to
2 over it. Without glow maps the dimmer is doubled, up to 1.
================
*/
// FUNCTION: STBC 0x00432dc0 ?CreateNiPointLight@LightObjectClass@@SAPAVNiLight@@MMMMMPAD@Z
// DRAFT: retail copies the colour through an out-of-line NiColor copy (0x4342A0) twice for the quarter-colour ambient; ours expands it
NiLight *LightObjectClass::CreateNiPointLight( float red, float green, float blue, float dimmer, float linearAttenuation,
											   char *name ) {
	if ( !g_kLODModelManager.AreGlowMapsEnabled() ) {
		dimmer = dimmer + dimmer;
		if ( dimmer > 1.0f ) {
			dimmer = 1.0f;
		}
	}
	NiPointLight *light = new NiPointLight;
	NiColor color( red, green, blue );
	light->SetAmbientColor( color * 0.25f );
	light->SetDiffuseColor( NiColor( red, green, blue ) );
	light->SetSpecularColor( NiColor( 1.0f, 1.0f, 1.0f ) );
	light->SetDimmer( dimmer );
	if ( linearAttenuation >= 0.0f ) {
		light->SetLinearAttenuation( 2.0f / linearAttenuation );
	}
	light->SetName( name );
	return light;
}

/*
================
LightObjectClass::CreateNiSpotLight

A spot light of the colour, named, pointing the direction, with a quarter
of the colour for ambient and white specular; the spot angle is half the
angle. Without glow maps the dimmer is doubled, up to 1.
================
*/
// FUNCTION: STBC 0x00432f90 ?CreateNiSpotLight@LightObjectClass@@SAPAVNiLight@@MMMMMMMMPAD@Z
// DRAFT: retail calls UnitCross, NiMatrix3::operator= and the NiColor copy (0x4342A0) out of line; ours expands them
NiLight *LightObjectClass::CreateNiSpotLight( float red, float green, float blue, float dimmer, float directionX, float directionY,
											  float directionZ, float angle, char *name ) {
	if ( !g_kLODModelManager.AreGlowMapsEnabled() ) {
		dimmer = dimmer + dimmer;
		if ( dimmer > 1.0f ) {
			dimmer = 1.0f;
		}
	}
	NiSpotLight *light = new NiSpotLight;
	NiPoint3 direction( directionX, directionY, directionZ );
	NiPoint3 axis = direction.UnitCross( NiPoint3( 1.0f, 0.0f, 0.0f ) );
	float turn = NiACos( NiPoint3( 1.0f, 0.0f, 0.0f ).Dot( direction ) );
	if ( axis == NiPoint3::ZERO ) {
		axis = NiPoint3( 0.0f, 1.0f, 0.0f );
	}
	NiMatrix3 rotation;
	rotation.MakeRotation( turn, axis );
	light->SetRotate( rotation );
	light->SetSpotAngle( angle * 0.5f );
	NiColor color( red, green, blue );
	light->SetAmbientColor( color * 0.25f );
	light->SetDiffuseColor( NiColor( red, green, blue ) );
	light->SetSpecularColor( NiColor( 1.0f, 1.0f, 1.0f ) );
	light->SetDimmer( dimmer );
	light->SetName( name );
	return light;
}

/*
================
LightObjectClass::CreateNiAmbientLight

An ambient light of the colour, named. Without glow maps the dimmer is
doubled, up to 1.
================
*/
// FUNCTION: STBC 0x00433220 ?CreateNiAmbientLight@LightObjectClass@@SAPAVNiLight@@MMMMPAD@Z
NiLight *LightObjectClass::CreateNiAmbientLight( float red, float green, float blue, float dimmer, char *name ) {
	if ( !g_kLODModelManager.AreGlowMapsEnabled() ) {
		dimmer = dimmer + dimmer;
		if ( dimmer > 1.0f ) {
			dimmer = 1.0f;
		}
	}
	NiAmbientLight *light = new NiAmbientLight;
	light->SetAmbientColor( NiColor( red, green, blue ) );
	light->SetDimmer( dimmer );
	light->SetName( name );
	return light;
}

/*
================
LightObjectClass::CreateNiDirectionalLight

A directional light of the colour, named, turned from the x axis to the
direction, with black ambient and specular. Without glow maps the dimmer
is doubled, up to 1.
================
*/
// FUNCTION: STBC 0x004332f0 ?CreateNiDirectionalLight@LightObjectClass@@SAPAVNiLight@@MMMMMMMPAD@Z
// DRAFT: retail calls UnitCross, Dot and NiACos out of line (inline budget); ours expands them and folds the x axis
NiLight *LightObjectClass::CreateNiDirectionalLight( float red, float green, float blue, float dimmer, float directionX,
													 float directionY, float directionZ, char *name ) {
	if ( !g_kLODModelManager.AreGlowMapsEnabled() ) {
		dimmer = dimmer + dimmer;
		if ( dimmer > 1.0f ) {
			dimmer = 1.0f;
		}
	}
	NiDirectionalLight *light = new NiDirectionalLight;
	NiPoint3 direction( directionX, directionY, directionZ );
	NiPoint3 xAxis( 1.0f, 0.0f, 0.0f );
	NiPoint3 axis = direction.UnitCross( xAxis );
	float turn = NiACos( xAxis.Dot( direction ) );
	if ( axis == NiPoint3::ZERO ) {
		axis = NiPoint3( 0.0f, 1.0f, 0.0f );
	}
	NiMatrix3 rotation;
	rotation.MakeRotation( turn, axis );
	light->SetRotate( rotation );
	light->SetAmbientColor( NiColor::BLACK );
	light->SetDiffuseColor( NiColor( red, green, blue ) );
	light->SetSpecularColor( NiColor::BLACK );
	light->SetDimmer( dimmer );
	light->SetName( name );
	return light;
}

/*
================
LightObjectClass::Update
================
*/
// FUNCTION: STBC 0x00433510
void LightObjectClass::Update( float time ) {
	BaseObjectClass::Update( time );
}

/*
================
LightObjectClass::GetNiLight
================
*/
// FUNCTION: STBC 0x00433520 ?GetNiLight@LightObjectClass@@QAEPAVNiLight@@XZ
NiLight *LightObjectClass::GetNiLight( void ) {
	return NiDynamicCast( NiLight, niObject );
}

/*
================
LightObjectClass::SetIntensity

Scales the light's colours by the intensity.
================
*/
// FUNCTION: STBC 0x00433560 ?SetIntensity@LightObjectClass@@QAEXM@Z
void LightObjectClass::SetIntensity( float intensity ) {
	NiLight *light = GetNiLight();
	if ( light != NULL ) {
		light->SetDimmer( intensity );
	}
}

/*
================
LightObjectClass::GetIntensity

The light's dimmer; 0 without a light.
================
*/
// FUNCTION: STBC 0x00433590 ?GetIntensity@LightObjectClass@@QAEMXZ
float LightObjectClass::GetIntensity( void ) {
	NiLight *light = GetNiLight();
	if ( light != NULL ) {
		return light->GetDimmer();
	}
	return 0.0f;
}

/*
================
LightObjectClass::AddIlluminatedObject

Lets the light shine on the object's node alone: the light stops shining on
the set's whole scene.
================
*/
// FUNCTION: STBC 0x004335b0
void LightObjectClass::AddIlluminatedObject( BaseObjectClass *object ) {
	NiLight *light = GetNiLight();
	if ( light != NULL && object != NULL ) {
		if ( containingSet != NULL ) {
			light->DetachAffectedNode( containingSet->sceneRoot );
			containingSet->sceneChanged = true;
			light->AttachAffectedNode( object->GetNode() );
		} else {
			light->AttachAffectedNode( object->GetNode() );
			object->GetNode()->UpdateEffects();
		}
		illuminatedObjects.AddTail( object->GetObjID() );
	}
}

/*
================
LightObjectClass::RemoveIlluminatedObject
================
*/
// FUNCTION: STBC 0x00433680
void LightObjectClass::RemoveIlluminatedObject( BaseObjectClass *object ) {
	NiLight *light = GetNiLight();
	if ( light != NULL && object != NULL ) {
		light->DetachAffectedNode( object->GetNode() );
		if ( containingSet != NULL ) {
			containingSet->sceneChanged = true;
		} else {
			object->GetNode()->UpdateEffects();
		}
		illuminatedObjects.Remove( object->GetObjID() );
	}
}

/*
================
LightObjectClass::IlluminateEntireSet

Lets the light shine on the whole scene and the backdrops of the set.
================
*/
// FUNCTION: STBC 0x004337c0
void LightObjectClass::IlluminateEntireSet( void ) {
	if ( containingSet != NULL ) {
		NiLight *light = GetNiLight();
		if ( light != NULL ) {
			light->DetachAllAffectedNodes();
			light->AttachAffectedNode( containingSet->sceneRoot );
			light->AttachAffectedNode( containingSet->backdropRoot );
			containingSet->sceneChanged = true;
		}
	}
}

/*
================
LightObjectClass::UnilluminateEntireSet

Stops the light shining on anything.
================
*/
// FUNCTION: STBC 0x00433810
void LightObjectClass::UnilluminateEntireSet( void ) {
	NiLight *light = GetNiLight();
	if ( light != NULL ) {
		light->DetachAllAffectedNodes();
		if ( containingSet != NULL ) {
			containingSet->sceneChanged = true;
		}
	}
}

/*
================
LightObjectClass::SetContainingSet

A light in a set shines on the set's scene and backdrops.
================
*/
// FUNCTION: STBC 0x00433840
void LightObjectClass::SetContainingSet( SetClass *set ) {
	BaseObjectClass::SetContainingSet( set );
	if ( set != NULL ) {
		NiLight *light = GetNiLight();
		if ( light != NULL ) {
			light->AttachAffectedNode( set->sceneRoot );
			light->AttachAffectedNode( set->backdropRoot );
		}
	}
}

/*
================
LightObjectClass::WriteToStream

The colours and the dimmer of the light, the position and attenuation of
a point light, the direction and cone of a spot light, a code for the kind
of light, then the IDs of the objects it shines on alone. The dimmer is
written halved when there are no glow maps. A directional light's direction
is not written.
================
*/
// FUNCTION: STBC 0x00433880
bool LightObjectClass::WriteToStream( TGStream *stream ) {
	if ( !BaseObjectClass::WriteToStream( stream ) ) {
		return false;
	}
	NiLight *light = GetNiLight();
	stream->WriteBool( light != NULL );
	if ( light == NULL ) {
		return true;
	}
	stream->WriteFloat( light->GetAmbientColor().r );
	stream->WriteFloat( light->GetAmbientColor().g );
	stream->WriteFloat( light->GetAmbientColor().b );
	stream->WriteFloat( light->GetDiffuseColor().r );
	stream->WriteFloat( light->GetDiffuseColor().g );
	stream->WriteFloat( light->GetDiffuseColor().b );
	stream->WriteFloat( light->GetSpecularColor().r );
	stream->WriteFloat( light->GetSpecularColor().g );
	stream->WriteFloat( light->GetSpecularColor().b );
	float dimmer = light->GetDimmer();
	if ( !g_kLODModelManager.AreGlowMapsEnabled() ) {
		dimmer = dimmer * 0.5f;
	}
	stream->WriteFloat( dimmer );
	if ( light->IsKindOf( &NiPointLight::rtti ) ) {
		if ( light->IsKindOf( &NiSpotLight::rtti ) ) {
			stream->WriteChar( 1 );
		} else {
			stream->WriteChar( 0 );
		}
		NiPoint3 position = light->GetWorldTranslate();
		stream->WriteFloat( position.x );
		stream->WriteFloat( position.y );
		stream->WriteFloat( position.z );
		stream->WriteFloat( ( (NiPointLight *)light )->GetConstantAttenuation() );
		stream->WriteFloat( ( (NiPointLight *)light )->GetLinearAttenuation() );
		stream->WriteFloat( ( (NiPointLight *)light )->GetQuadraticAttenuation() );
		if ( light->IsKindOf( &NiSpotLight::rtti ) ) {
			NiPoint3 direction = ( (NiSpotLight *)light )->GetWorldDirection();
			stream->WriteFloat( direction.x );
			stream->WriteFloat( direction.y );
			stream->WriteFloat( direction.z );
			stream->WriteFloat( ( (NiSpotLight *)light )->GetSpotAngle() );
			stream->WriteFloat( ( (NiSpotLight *)light )->GetSpotExponent() );
		}
	}
	if ( light->IsKindOf( &NiAmbientLight::rtti ) ) {
		stream->WriteChar( 3 );
	}
	if ( light->IsKindOf( &NiDirectionalLight::rtti ) ) {
		stream->WriteChar( 2 );
	}
	stream->WriteLong( illuminatedObjects.GetSize() );
	NiTListIterator pos = illuminatedObjects.GetHeadPos();
	while ( pos != NULL ) {
		stream->WriteID( illuminatedObjects.GetNext( pos ) );
	}
	return true;
}

/*
================
LightObjectClass::ReadFromStream

Makes the light the stream describes and lists the IDs of the objects it
shines on alone. A directional light is made with the diffuse colour, the
others with the ambient one. The specular colour, the position and the
attenuations (but the linear one of a point light) are read and dropped;
the factories derive them again.
================
*/
// FUNCTION: STBC 0x00433b40
// DRAFT: frame now retail's; left: argument push order and loads in the spot and point cases (the creation calls' operands read in another order), and the count loop's layout
bool LightObjectClass::ReadFromStream( TGStream *stream ) {
	if ( !BaseObjectClass::ReadFromStream( stream ) ) {
		return false;
	}
	if ( !stream->ReadBool() ) {
		return true;
	}
	NiLight *light = NULL;
	NiColor ambient;
	ambient.r = stream->ReadFloat();
	ambient.g = stream->ReadFloat();
	ambient.b = stream->ReadFloat();
	NiColor diffuse;
	diffuse.r = stream->ReadFloat();
	diffuse.g = stream->ReadFloat();
	diffuse.b = stream->ReadFloat();
	stream->ReadFloat();
	stream->ReadFloat();
	stream->ReadFloat();
	float dimmer = stream->ReadFloat();
	if ( !g_kLODModelManager.AreGlowMapsEnabled() ) {
		dimmer = dimmer + dimmer;
	}
	char kind = stream->ReadChar();
	switch ( kind ) {
	case 0:
	case 1: {
		stream->ReadFloat();
		stream->ReadFloat();
		stream->ReadFloat();
		stream->ReadFloat();
		float linearAttenuation = stream->ReadFloat();
		stream->ReadFloat();
		if ( kind == 0 ) {
			light = CreateNiPointLight( ambient.r, ambient.g, ambient.b, dimmer, linearAttenuation, name );
		} else {
			NiPoint3 spotDirection;
			spotDirection.x = stream->ReadFloat();
			spotDirection.y = stream->ReadFloat();
			spotDirection.z = stream->ReadFloat();
			float angle = stream->ReadFloat();
			stream->ReadFloat();
			light = CreateNiSpotLight( ambient.r, ambient.g, ambient.b, dimmer, spotDirection.x, spotDirection.y, spotDirection.z, angle, name );
		}
		break;
	}
	case 3:
		light = CreateNiAmbientLight( ambient.r, ambient.g, ambient.b, dimmer, name );
		break;
	case 2: {
		NiPoint3 direction = TGPoint3::GetModelForward();
		light = CreateNiDirectionalLight( diffuse.r, diffuse.g, diffuse.b, dimmer, direction.x, direction.y, direction.z, name );
		break;
	}
	}
	SetNiObject( light );
	long count = stream->ReadLong();
	for ( long i = 0; i < count; i++ ) {
		illuminatedObjects.AddTail( stream->ReadID() );
	}
	return true;
}

/*
================
LightObjectClass::VirtualSlot50

Lights the objects of the list afresh, now that they may exist: the list
holds the IDs ReadFromStream loaded.
================
*/
// FUNCTION: STBC 0x00433e90
void LightObjectClass::VirtualSlot50( void ) {
	if ( illuminatedObjects.GetSize() > 0 ) {
		UnilluminateEntireSet();
		NiTListIterator pos = illuminatedObjects.GetHeadPos();
		NiTList<long> ids( illuminatedObjects.GetSize() );
		while ( pos != NULL ) {
			ids.AddTail( illuminatedObjects.GetNext( pos ) );
		}
		illuminatedObjects.RemoveAll();
		pos = ids.GetHeadPos();
		while ( pos != NULL ) {
			BaseObjectClass *object = BaseObjectClass::GetObjectByID( NULL, ids.GetNext( pos ) );
			if ( object != NULL ) {
				AddIlluminatedObject( object );
			}
		}
	}
}

