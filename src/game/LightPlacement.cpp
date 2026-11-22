/*
===============================================================================

	LightPlacement.cpp

===============================================================================
*/

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "LightPlacement.h"
#include "LightObjectClass.h"
#include "SetClass.h"
#include "TGEventHandlerTable.h"
#include "TGStream.h"
#include "NiAmbientLight.h"
#include "NiDirectionalLight.h"
#include "NiPointLight.h"
#include "NiSpotLight.h"
#include "NiMain.h"
#include "NiListener.h"
#include "NiSoundSystem.h"
#include "NiSource.h"

// The locale-id pair: std::ctype<unsigned short>::id's guard byte and its exit-time registration.
// FUNCTION: STBC 0x0049ae70 _$E455
// FUNCTION: STBC 0x0049ae90 _$E454

// The standard streams' initialisers and exit-time destructors (<iostream>).
// FUNCTION: STBC 0x0049a830 _$E32
// FUNCTION: STBC 0x0049a840 _$E29
// FUNCTION: STBC 0x0049a850 _$E31
// FUNCTION: STBC 0x0049a860 _$E30
// FUNCTION: STBC 0x0049a870 _$E38
// FUNCTION: STBC 0x0049a880 _$E35
// FUNCTION: STBC 0x0049a890 _$E37
// FUNCTION: STBC 0x0049a8a0 _$E36

// The NetImmerse headers' class registrations (NiRegisterStream, NiStreamMacros.h).
// FUNCTION: STBC 0x0049a8b0 _$E66
// FUNCTION: STBC 0x0049a8c0 _$E65
// FUNCTION: STBC 0x0049a8d0 _$E256
// FUNCTION: STBC 0x0049a8e0 _$E255
// FUNCTION: STBC 0x0049a8f0 _$E77
// FUNCTION: STBC 0x0049a900 _$E76
// FUNCTION: STBC 0x0049a910 _$E81
// FUNCTION: STBC 0x0049a920 _$E80
// FUNCTION: STBC 0x0049a930 _$E85
// FUNCTION: STBC 0x0049a940 _$E84
// FUNCTION: STBC 0x0049a950 _$E89
// FUNCTION: STBC 0x0049a960 _$E88
// FUNCTION: STBC 0x0049a970 _$E206
// FUNCTION: STBC 0x0049a980 _$E205
// FUNCTION: STBC 0x0049a990 _$E148
// FUNCTION: STBC 0x0049a9a0 _$E147
// FUNCTION: STBC 0x0049a9b0 _$E161
// FUNCTION: STBC 0x0049a9c0 _$E160
// FUNCTION: STBC 0x0049a9d0 _$E211
// FUNCTION: STBC 0x0049a9e0 _$E210
// FUNCTION: STBC 0x0049a9f0 _$E261
// FUNCTION: STBC 0x0049aa00 _$E260
// FUNCTION: STBC 0x0049aa10 _$E215
// FUNCTION: STBC 0x0049aa20 _$E214
// FUNCTION: STBC 0x0049aa30 _$E265
// FUNCTION: STBC 0x0049aa40 _$E264
// FUNCTION: STBC 0x0049aa50 _$E272
// FUNCTION: STBC 0x0049aa60 _$E271
// FUNCTION: STBC 0x0049aa70 _$E294
// FUNCTION: STBC 0x0049aa80 _$E293
// FUNCTION: STBC 0x0049aa90 _$E298
// FUNCTION: STBC 0x0049aaa0 _$E297
// FUNCTION: STBC 0x0049aab0 _$E106
// FUNCTION: STBC 0x0049aac0 _$E105
// FUNCTION: STBC 0x0049aad0 _$E280
// FUNCTION: STBC 0x0049aae0 _$E279
// FUNCTION: STBC 0x0049aaf0 _$E287
// FUNCTION: STBC 0x0049ab00 _$E286
// FUNCTION: STBC 0x0049ab10 _$E302
// FUNCTION: STBC 0x0049ab20 _$E301
// FUNCTION: STBC 0x0049ab30 _$E306
// FUNCTION: STBC 0x0049ab40 _$E305
// FUNCTION: STBC 0x0049ab50 _$E342
// FUNCTION: STBC 0x0049ab60 _$E341
// FUNCTION: STBC 0x0049ab70 _$E348
// FUNCTION: STBC 0x0049ab80 _$E347
// FUNCTION: STBC 0x0049ab90 _$E236
// FUNCTION: STBC 0x0049aba0 _$E235
// FUNCTION: STBC 0x0049abb0 _$E240
// FUNCTION: STBC 0x0049abc0 _$E239
// FUNCTION: STBC 0x0049abd0 _$E310
// FUNCTION: STBC 0x0049abe0 _$E309
// FUNCTION: STBC 0x0049abf0 _$E314
// FUNCTION: STBC 0x0049ac00 _$E313
// FUNCTION: STBC 0x0049ac10 _$E318
// FUNCTION: STBC 0x0049ac20 _$E317
// FUNCTION: STBC 0x0049ac30 _$E322
// FUNCTION: STBC 0x0049ac40 _$E321
// FUNCTION: STBC 0x0049ac50 _$E326
// FUNCTION: STBC 0x0049ac60 _$E325
// FUNCTION: STBC 0x0049ac70 _$E330
// FUNCTION: STBC 0x0049ac80 _$E329
// FUNCTION: STBC 0x0049ac90 _$E334
// FUNCTION: STBC 0x0049aca0 _$E333
// FUNCTION: STBC 0x0049acb0 _$E338
// FUNCTION: STBC 0x0049acc0 _$E337
// FUNCTION: STBC 0x0049acd0 _$E111
// FUNCTION: STBC 0x0049ace0 _$E110
// FUNCTION: STBC 0x0049acf0 _$E244
// FUNCTION: STBC 0x0049ad00 _$E243
// FUNCTION: STBC 0x0049ad10 _$E116
// FUNCTION: STBC 0x0049ad20 _$E115
// FUNCTION: STBC 0x0049ad30 _$E352
// FUNCTION: STBC 0x0049ad40 _$E351
// FUNCTION: STBC 0x0049ad50 _$E356
// FUNCTION: STBC 0x0049ad60 _$E355
// FUNCTION: STBC 0x0049ad70 _$E360
// FUNCTION: STBC 0x0049ad80 _$E359
// FUNCTION: STBC 0x0049ad90 _$E364
// FUNCTION: STBC 0x0049ada0 _$E363
// FUNCTION: STBC 0x0049adb0 _$E125
// FUNCTION: STBC 0x0049adc0 _$E124
// FUNCTION: STBC 0x0049add0 _$E368
// FUNCTION: STBC 0x0049ade0 _$E367
// FUNCTION: STBC 0x0049adf0 _$E372
// FUNCTION: STBC 0x0049ae00 _$E371
// FUNCTION: STBC 0x0049ae10 _$E376
// FUNCTION: STBC 0x0049ae20 _$E375
// FUNCTION: STBC 0x0049ae30 _$E131
// FUNCTION: STBC 0x0049ae40 _$E130
// FUNCTION: STBC 0x0049ae50 _$E136
// FUNCTION: STBC 0x0049ae60 _$E135
// FUNCTION: STBC 0x0049aea0 _$E166
// FUNCTION: STBC 0x0049aeb0 _$E165
// FUNCTION: STBC 0x0049aec0 _$E248
// FUNCTION: STBC 0x0049aed0 _$E247
// FUNCTION: STBC 0x0049aee0 _$E171
// FUNCTION: STBC 0x0049aef0 _$E170
// FUNCTION: STBC 0x0049af00 _$E380
// FUNCTION: STBC 0x0049af10 _$E379
// FUNCTION: STBC 0x0049af20 _$E384
// FUNCTION: STBC 0x0049af30 _$E383
// FUNCTION: STBC 0x0049af40 _$E176
// FUNCTION: STBC 0x0049af50 _$E175
// FUNCTION: STBC 0x0049af60 _$E252
// FUNCTION: STBC 0x0049af70 _$E251
// FUNCTION: STBC 0x0049af80 _$E141
// FUNCTION: STBC 0x0049af90 _$E140
// FUNCTION: STBC 0x0049afa0 _$E388
// FUNCTION: STBC 0x0049afb0 _$E387
// FUNCTION: STBC 0x0049afc0 _$E392
// FUNCTION: STBC 0x0049afd0 _$E391
// FUNCTION: STBC 0x0049afe0 _$E181
// FUNCTION: STBC 0x0049aff0 _$E180
// FUNCTION: STBC 0x0049b000 _$E186
// FUNCTION: STBC 0x0049b010 _$E185
// FUNCTION: STBC 0x0049b020 _$E396
// FUNCTION: STBC 0x0049b030 _$E395
// FUNCTION: STBC 0x0049b040 _$E400
// FUNCTION: STBC 0x0049b050 _$E399
// FUNCTION: STBC 0x0049b060 _$E404
// FUNCTION: STBC 0x0049b070 _$E403
// FUNCTION: STBC 0x0049b080 _$E408
// FUNCTION: STBC 0x0049b090 _$E407
// FUNCTION: STBC 0x0049b0a0 _$E412
// FUNCTION: STBC 0x0049b0b0 _$E411
// FUNCTION: STBC 0x0049b0c0 _$E416
// FUNCTION: STBC 0x0049b0d0 _$E415
// FUNCTION: STBC 0x0049b0e0 _$E420
// FUNCTION: STBC 0x0049b0f0 _$E419
// FUNCTION: STBC 0x0049b100 _$E424
// FUNCTION: STBC 0x0049b110 _$E423
// FUNCTION: STBC 0x0049b120 _$E191
// FUNCTION: STBC 0x0049b130 _$E190
// FUNCTION: STBC 0x0049b140 _$E428
// FUNCTION: STBC 0x0049b150 _$E427
// FUNCTION: STBC 0x0049b160 _$E196
// FUNCTION: STBC 0x0049b170 _$E195
// FUNCTION: STBC 0x0049b180 _$E201
// FUNCTION: STBC 0x0049b190 _$E200
// FUNCTION: STBC 0x0049b1a0 _$E432
// FUNCTION: STBC 0x0049b1b0 _$E431
// FUNCTION: STBC 0x0049b1c0 _$E437
// FUNCTION: STBC 0x0049b1d0 _$E436
// FUNCTION: STBC 0x0049b1e0 _$E441
// FUNCTION: STBC 0x0049b1f0 _$E440

// The handler table's dynamic initialiser and exit-time destructor.
// FUNCTION: STBC 0x0049a100 _$E447
// FUNCTION: STBC 0x0049a110 _$E443
// FUNCTION: STBC 0x0049a120 _$E446
// FUNCTION: STBC 0x0049a130 _$E444
// DATA: STBC 0x00983400
TGEventHandlerTable LightPlacement::classHandlers;

/*
================
LightPlacement::Cast

The object as a LightPlacement; NULL when it is none.
================
*/
// FUNCTION: STBC 0x0049a150
LightPlacement *LightPlacement::Cast( TGObject *object ) {
	if ( object != NULL && object->IsTypeOf( LIGHTPLACEMENT_TYPE ) ) {
		return (LightPlacement *)object;
	}
	return NULL;
}

/*
================
LightPlacement::LightPlacement
================
*/
// FUNCTION: STBC 0x0049a180
LightPlacement::LightPlacement( char *name, char *setName, char *modelName ) :
	PlacementObject( name, setName, modelName ),
	lightID( 0 ) {
}

/*
================
LightPlacement::~LightPlacement

Takes the light the placement made out of its set.
================
*/
// FUNCTION: STBC 0x0049a1b0
LightPlacement::~LightPlacement( void ) {
	SetClass *set = GetContainingSet();
	if ( set != NULL || ( set = FindContainingSet() ) != NULL ) {
		set->DeleteLightByID( lightID );
		lightID = 0;
	}
}

/*
================
LightPlacement::ConfigAmbientLight

Sets the light's colour and dimmer when it is an ambient light already;
otherwise replaces it with a new ambient light named after the placement.
================
*/
// FUNCTION: STBC 0x0049a220
void LightPlacement::ConfigAmbientLight( float red, float green, float blue, float dimmer ) {
	LightObjectClass *light = GetLight();
	if ( light != NULL ) {
		NiLight *niLight = light->GetNiLight();
		if ( niLight != NULL && niLight->IsKindOf( &NiAmbientLight::rtti ) ) {
			niLight->SetAmbientColor( NiColor( red, green, blue ) );
			niLight->SetDimmer( dimmer );
			return;
		}
		if ( light->GetContainingSet() != NULL ) {
			light->GetContainingSet()->DeleteLightByID( light->GetObjID() );
		} else {
			delete light;
		}
	}

	char *lightName = new char[strlen( GetName() ) + strlen( "Light" ) + 1];
	strcpy( lightName, GetName() );
	strcat( lightName, "Light" );
	light = LightObjectClass::Create( LightObjectClass::CreateNiAmbientLight( red, green, blue, dimmer, lightName ) );
	lightID = light->GetObjID();
	SetClass *set = FindContainingSet();
	if ( !set->AddLightToSet( light, lightName ) ) {
		delete light;
		lightID = 0;
	}
	delete [] lightName;
}

/*
================
LightPlacement::ConfigDirectionalLight

Sets the light's colour and dimmer when it is a directional light already;
otherwise replaces it with a new directional light named after the
placement, pointing along the model's forward axis.
================
*/
// FUNCTION: STBC 0x0049a380
void LightPlacement::ConfigDirectionalLight( float red, float green, float blue, float dimmer ) {
	LightObjectClass *light = GetLight();
	if ( light != NULL ) {
		NiLight *niLight = light->GetNiLight();
		if ( niLight != NULL && niLight->IsKindOf( &NiDirectionalLight::rtti ) ) {
			niLight->SetDiffuseColor( NiColor( red, green, blue ) );
			niLight->SetDimmer( dimmer );
			return;
		}
		if ( light->GetContainingSet() != NULL ) {
			light->GetContainingSet()->DeleteLightByID( light->GetObjID() );
		} else {
			delete light;
		}
	}

	char *lightName = new char[strlen( GetName() ) + strlen( "Light" ) + 1];
	strcpy( lightName, GetName() );
	strcat( lightName, "Light" );
	NiPoint3 forward = TGPoint3::GetModelForward();
	light = LightObjectClass::Create( LightObjectClass::CreateNiDirectionalLight( red, green, blue, dimmer, forward.x, forward.y, forward.z,
																				 lightName ) );
	lightID = light->GetObjID();
	SetClass *set = FindContainingSet();
	if ( !set->AddLightToSet( light, lightName ) ) {
		delete light;
		lightID = 0;
	}
	delete [] lightName;
}

/*
================
LightPlacement::ConfigPointLight

The call does nothing.
================
*/
// FUNCTION: STBC 0x0049a510
void LightPlacement::ConfigPointLight( float, float, float, float, float ) {
}

/*
================
LightPlacement::ConfigSpotLight

The call does nothing.
================
*/
// FUNCTION: STBC 0x0049a520
void LightPlacement::ConfigSpotLight( float, float, float, float, float, float, float, float ) {
}

/*
================
LightPlacement::GetLightType

The kind of the light the placement made; LIGHT_TYPE_NONE when it has none.
================
*/
// FUNCTION: STBC 0x0049a530
int LightPlacement::GetLightType( void ) {
	int type = LIGHT_TYPE_NONE;
	LightObjectClass *light = GetLight();
	if ( light != NULL ) {
		NiLight *niLight = light->GetNiLight();
		if ( niLight != NULL ) {
			if ( niLight->IsKindOf( &NiAmbientLight::rtti ) ) {
				type = LIGHT_TYPE_AMBIENT;
			} else if ( niLight->IsKindOf( &NiDirectionalLight::rtti ) ) {
				type = LIGHT_TYPE_DIRECTIONAL;
			} else if ( niLight->IsKindOf( &NiPointLight::rtti ) ) {
				type = LIGHT_TYPE_POINT;
			} else if ( niLight->IsKindOf( &NiSpotLight::rtti ) ) {
				type = LIGHT_TYPE_SPOT;
			}
		}
	}
	return type;
}

/*
================
LightPlacement::Update

Moves the light with the placement.
================
*/
// FUNCTION: STBC 0x0049a5e0
void LightPlacement::Update( float time ) {
	PlacementObject::Update( time );
	LightObjectClass *light = GetLight();
	if ( light != NULL ) {
		light->SetMatrixRotation( GetRotation() );
		light->SetTranslate( GetTranslate() );
	}
}

/*
================
LightPlacement::GetLight
================
*/
// FUNCTION: STBC 0x0049a630
LightObjectClass *LightPlacement::GetLight( void ) {
	SetClass *set = GetContainingSet();
	if ( set == NULL ) {
		set = FindContainingSet();
	}
	if ( set != NULL ) {
		return LightObjectClass::GetObjectByID( set, lightID );
	}
	return NULL;
}

/*
================
LightPlacement::VirtualSlot68

Writes the line of the saved script that creates the placement.
================
*/
// FUNCTION: STBC 0x0049a660
bool LightPlacement::VirtualSlot68( FILE *file, char *setName ) {
	fprintf( file, "\t# Light position \"%s\"\n", GetName() );
	if ( modelName != NULL ) {
		fprintf( file, "\tkThis = App.LightPlacement_Create(\"%s\", sSetName, \"%s\")\n", GetName(), modelName );
	} else {
		fprintf( file, "\tkThis = App.LightPlacement_Create(\"%s\", sSetName, None)\n", GetName() );
	}
	return true;
}

/*
================
LightPlacement::VirtualSlot69

Writes PlacementObject's lines, then the line that configures the light.
PlacementObject's result is not checked.
================
*/
// FUNCTION: STBC 0x0049a6c0
// PARKED: x87 order: retail loads the three colour floats and the dimmer before converting any of them for fprintf; this build loads and converts each argument right to left (a copied NiColor local scored lower)
bool LightPlacement::VirtualSlot69( FILE *file ) {
	PlacementObject::VirtualSlot69( file );
	LightObjectClass *light = GetLight();
	if ( light != NULL ) {
		NiLight *niLight = light->GetNiLight();
		if ( niLight != NULL ) {
			float dimmer = niLight->GetDimmer();
			if ( niLight->IsKindOf( &NiAmbientLight::rtti ) ) {
				fprintf( file, "\tkThis.ConfigAmbientLight(%f, %f, %f, %f)\n", niLight->GetAmbientColor().r,
						 niLight->GetAmbientColor().g, niLight->GetAmbientColor().b, dimmer );
				return true;
			}
			if ( niLight->IsKindOf( &NiDirectionalLight::rtti ) ) {
				fprintf( file, "\tkThis.ConfigDirectionalLight(%f, %f, %f, %f)\n", niLight->GetDiffuseColor().r,
						 niLight->GetDiffuseColor().g, niLight->GetDiffuseColor().b, dimmer );
			}
		}
	}
	return true;
}

/*
================
LightPlacement::WriteToStream
================
*/
// FUNCTION: STBC 0x0049a7d0
bool LightPlacement::WriteToStream( TGStream *stream ) {
	if ( !PlacementObject::WriteToStream( stream ) ) {
		return false;
	}
	stream->WriteID( lightID );
	return true;
}

/*
================
LightPlacement::ReadFromStream
================
*/
// FUNCTION: STBC 0x0049a800
bool LightPlacement::ReadFromStream( TGStream *stream ) {
	if ( !PlacementObject::ReadFromStream( stream ) ) {
		return false;
	}
	lightID = stream->ReadID();
	return true;
}
